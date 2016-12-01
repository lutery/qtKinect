/*********************************************************
* Copyright: chenghui
* Author: chenghui
* Date: 20160925
* Description:KinectObj实现从Kinect获取彩色帧的功能
*
**********************************************************/
#include <QDebug>
#include "kinectobj.h"

/**
 * @brief KinectObj::KinectObj 构造函数
 * @param parent 传入父类，当父类释放的时候，该类也会被释放
 */
KinectObj::KinectObj(IKinectSensor* pKinect, QObject *parent) : QObject(parent)
{
    mpKinect = pKinect;
    mpColorFrameReader = nullptr;
    mhColorFrameArrived = NULL;
    mpColorRGBX = nullptr;
    mpColorRGBX = new RGBQUAD[IMAGE_WIDTH * IMAGE_HEIGHT];
    mBufferSize = IMAGE_WIDTH * IMAGE_HEIGHT * sizeof(RGBQUAD);
    pMutex = new QMutex();

    this->initKinect();
}

/**
 * @brief KinectObj::~KinectObj 析构函数，释放资源
 */
KinectObj::~KinectObj()
{
    // 释放帧生成事件
    if (mhColorFrameArrived != NULL && mpColorFrameReader != nullptr)
    {
        mpColorFrameReader->UnsubscribeFrameArrived(mhColorFrameArrived);
        mhColorFrameArrived = 0;
    }

    // 释放彩色帧读取对象
    SafeRelease(mpColorFrameReader);

    if (mpKinect != nullptr)
    {
        mpKinect->Close();
        qDebug() << "close Kinect Success";
    }

    SafeRelease(mpKinect);

    if (mpColorRGBX != nullptr)
    {
        delete[] mpColorRGBX;
    }

    if (mBufferImage.count() != 0)
    {
        mBufferImage.clear();
    }

    if (pMutex != nullptr)
    {
        pMutex->lock();
        pMutex->unlock();

        delete pMutex;
        pMutex = nullptr;
    }
}

/**
 * @brief KinectObj::myImageCleanupHandler QImage内存释放函数
 * @param info 传入相关的信息，这里传入的就是内存地址，只要把这个释放了就可以释放内存
 */
void KinectObj::myImageCleanupHandler(void *info)
{
    delete[] info;
}

/**
 * @brief KinectObj::initKinect 初始化Kinect设备
 * @return 返回S_OK表示初始化成功，返回S_FALSE表示初始化失败
 */
HRESULT KinectObj::initKinect()
{
    IColorFrameSource* pColorFrameSource = nullptr;

    HRESULT hr = mpKinect != nullptr ? S_OK : S_FALSE;

    if (SUCCEEDED(hr))
    {
        hr = mpKinect->get_ColorFrameSource(&pColorFrameSource);
    }

    if (SUCCEEDED(hr))
    {
        if (mpColorFrameReader != nullptr)
        {
            qDebug() << "成员变量mpColorFrameReader已经存在";
        }

        hr = pColorFrameSource->OpenReader(&mpColorFrameReader);
    }

    if (SUCCEEDED(hr))
    {
        if (mhColorFrameArrived)
        {
            qDebug() << "成员变量mhColorFrameArrived已经存在";
        }

        hr = mpColorFrameReader->SubscribeFrameArrived(&mhColorFrameArrived);
    }

    return hr;
}

/**
 * @brief KinectObj::checkColorFrame 获取每帧，必须要等到帧生成事件响应，否则多次获取空帧将会出现异常
 * @return  返回S_OK表示初始化成功,返回S_FALSE表示初始化失败
 */
HRESULT KinectObj::checkFrame()
{
    if (mpColorFrameReader == nullptr)
    {
        return S_FALSE;
    }

    IColorFrameArrivedEventArgs* pArgs = nullptr;
    IColorFrameReference* pCFrameRef = nullptr;
    IColorFrame* pColorFrame = nullptr;
    IFrameDescription* pFrameDescription = nullptr;
    int width = 0;
    int height = 0;
    ColorImageFormat imageFormat = ColorImageFormat_None;
    UINT nBufferSize = 0;
    RGBQUAD* pBuffer = nullptr;

    HRESULT hr = mpColorFrameReader->GetFrameArrivedEventData(mhColorFrameArrived, &pArgs);

    if (SUCCEEDED(hr))
    {
        hr = pArgs->get_FrameReference(&pCFrameRef);
    }

    if (SUCCEEDED(hr))
    {
        hr = pCFrameRef->AcquireFrame(&pColorFrame);
    }

    if (SUCCEEDED(hr))
    {
        hr = pColorFrame->get_FrameDescription(&pFrameDescription);
    }

    if (SUCCEEDED(hr))
    {
        hr = pFrameDescription->get_Width(&width);
    }

    if (SUCCEEDED(hr))
    {
        hr = pFrameDescription->get_Height(&height);
    }

    if (SUCCEEDED(hr))
    {
        hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
    }

    if (SUCCEEDED(hr))
    {
        if (imageFormat == ColorImageFormat_Rgba)
        {
            hr = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&pBuffer));
        }
        else if (mpColorRGBX != nullptr)
        {
            pBuffer = mpColorRGBX;
            nBufferSize = mBufferSize;
            hr = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(mpColorRGBX), ColorImageFormat_Rgba);
        }
    }

    if (SUCCEEDED(hr))
    {
        //这里将像素数据拷贝到显示
        uchar* pData = new uchar[nBufferSize];
        memcpy_s(pData, nBufferSize * sizeof(uchar), pBuffer, nBufferSize * sizeof(BYTE));
        QImage* pImage = new QImage(pData, width, height, QImage::Format_RGBX8888, KinectObj::myImageCleanupHandler, pData);
        this->addQImage(pImage);
    }

    SafeRelease(pFrameDescription);
    SafeRelease(pColorFrame);
    SafeRelease(pCFrameRef);
    SafeRelease(pArgs);

    if (SUCCEEDED(hr))
    {
        qDebug() << "成功";
    }
    else
    {
        qDebug() << "失败";
    }

    return hr;
}

void KinectObj::addQImage(QImage *image)
{
    pMutex->lock();
    mBufferImage.append(std::shared_ptr<QImage>(image));
    //这里有一个未解之谜，如果仅仅只是绘制在主DIALOG主对话框中得化
    //那么即使这里填写60，也不会有延时
    if (mBufferImage.count() > MAX_FRAME_COUNT)
    {
        mBufferImage.pop_front();
    }
    qDebug() << "BufferImage Size is " << mBufferImage.size();
    pMutex->unlock();
}

WAITABLE_HANDLE KinectObj::getWaitableHandle()
{
    return this->mhColorFrameArrived;
}

std::shared_ptr<QImage> KinectObj::getQImage()
{
    if (mBufferImage.count() <= 0)
    {
        return nullptr;
    }

    pMutex->lock();
    auto pImage = mBufferImage.first();
    mBufferImage.pop_front();
    pMutex->unlock();

    return pImage;
}
