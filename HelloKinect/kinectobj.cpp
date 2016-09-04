#include <QDebug>
#include "kinectobj.h"

KinectObj::KinectObj(QObject *parent) : QObject(parent)
{
    mpKinect = nullptr;
    mpColorFrameReader = nullptr;
    mhColorFrameArrived = NULL;
    mpColorRGBX = nullptr;
    mpColorRGBX = new RGBQUAD[IMAGE_WIDTH * IMAGE_HEIGHT];
    mBufferSize = IMAGE_WIDTH * IMAGE_HEIGHT * sizeof(RGBQUAD);
    pMutex = new QMutex();

    this->initKinect();
}

KinectObj::~KinectObj()
{
    if (mhColorFrameArrived != NULL && mpColorFrameReader != nullptr)
    {
        mpColorFrameReader->UnsubscribeFrameArrived(mhColorFrameArrived);
        mhColorFrameArrived = 0;
    }

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

void KinectObj::myImageCleanupHandler(void *info)
{
    delete[] info;
}

HRESULT KinectObj::initKinect()
{
    IColorFrameSource* pColorFrameSource = nullptr;
    HRESULT hr = ::GetDefaultKinectSensor(&mpKinect);

    if (SUCCEEDED(hr))
    {
        hr = mpKinect->Open();
        qDebug() << "open Kinect Success";
    }

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

HRESULT KinectObj::checkColorFrame()
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
    if (mBufferImage.count() > 60)
    {
        mBufferImage.pop_front();
    }
    pMutex->unlock();
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
