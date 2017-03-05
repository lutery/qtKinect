/*********************************************
 * Copyright:chenghui
 * Author: chenghui
 * Date: 20161204
 * Description: KinectDepthObj Kinect颜色帧对象
 *********************************************/

#include "kinectdepthobj.h"
#include <QDebug>

/**
 * @brief KinectDepthObj::KinectDepthObj
 * @param pKinect
 * @param parent
 */
KinectDepthObj::KinectDepthObj(IKinectSensor* pKinect, QObject *parent) : KinectObj(pKinect, parent)
{
//    mpKinect = pKinect;
    mpDepthFrameReader = nullptr;
    mhDepthFrameArrived = NULL;
    mpColorRGBX = nullptr;
    mpColorRGBX = new RGBQUAD[IMAGE_WIDTH * IMAGE_HEIGHT];
    mBufferSize = IMAGE_WIDTH * IMAGE_HEIGHT * sizeof(RGBQUAD);
    pMutex = new QMutex;

    this->initKinect();
}

/**
 * @brief KinectDepthObj::~KinectDepthObj
 */
KinectDepthObj::~KinectDepthObj()
{
    if (mhDepthFrameArrived != NULL && mpDepthFrameReader != nullptr)
    {
        mpDepthFrameReader->UnsubscribeFrameArrived(mhDepthFrameArrived);
        mhDepthFrameArrived = 0;
    }

    SafeRelease(mpDepthFrameReader);

//    if (mpColorRGBX != nullptr)
//    {
//        delete[] mpColorRGBX;
//    }

//    if (mBufferImage.count() != 0)
//    {
//        mBufferImage.clear();
//    }

//    if (pMutex != nullptr)
//    {
//        pMutex->lock();
//        pMutex->unlock();

//        delete pMutex;
//        pMutex = nullptr;
//    }
}
/**
 * @brief KinectDepthObj::myImageCleanupHandler
 * @param info
 */
//void KinectDepthObj::myImageCleanupHandler(void *info)
//{
//    delete[] info;
//}

HRESULT KinectDepthObj::initKinect()
{
    IDepthFrameSource* pDepthFrameSource = nullptr;

    HRESULT hr = mpKinect != nullptr ? S_OK : S_FALSE;

    if (SUCCEEDED(hr))
    {
        hr = mpKinect->get_DepthFrameSource(&pDepthFrameSource);
    }

    if (SUCCEEDED(hr))
    {
        if (mpDepthFrameReader != nullptr)
        {
            qDebug() << "成员变量mpDepthFrameReader已经存在";
        }

        hr = pDepthFrameSource->OpenReader(&mpDepthFrameReader);
    }

    if (SUCCEEDED(hr))
    {
        if (mhDepthFrameArrived)
        {
            qDebug() << "成员变量mhColorFrameArrived已经存在";
        }

        hr = mpDepthFrameReader->SubscribeFrameArrived(&mhDepthFrameArrived);
    }

    SafeRelease(pDepthFrameSource);

    return hr;
}

HRESULT KinectDepthObj::checkFrame()
{
    if (mpDepthFrameReader == nullptr)
    {
        return S_FALSE;
    }

    IDepthFrameArrivedEventArgs* pArgs = nullptr;
    IDepthFrameReference* pCFrameRef = nullptr;
    IDepthFrame* pDepthFrame = nullptr;
    IFrameDescription* pFrameDescription = nullptr;
    int width = 0;
    int height = 0;

    USHORT depth_min_reliable_distance = 0;
    USHORT depth_max_reliable_distance = 0;
    UINT nBufferSize = 0;
    UINT16* pBuffer = nullptr;

    HRESULT hr = mpDepthFrameReader->GetFrameArrivedEventData(mhDepthFrameArrived, &pArgs);

    if (SUCCEEDED(hr))
    {
        hr = pArgs->get_FrameReference(&pCFrameRef);
    }

    if (SUCCEEDED(hr))
    {
        hr = pCFrameRef->AcquireFrame(&pDepthFrame);
    }

    if (SUCCEEDED(hr))
    {
        hr = pDepthFrame->get_FrameDescription(&pFrameDescription);
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
        hr = pDepthFrame->get_DepthMinReliableDistance(&depth_min_reliable_distance);
    }

    if (SUCCEEDED(hr))
    {
        hr = pDepthFrame->get_DepthMaxReliableDistance(&depth_max_reliable_distance);
    }

    if (SUCCEEDED(hr))
    {
        hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
    }

    if (SUCCEEDED(hr))
    {
        uchar* pData = new uchar[nBufferSize * 4];
        ZeroMemory(pData, nBufferSize * 4);

        for (uint i = 0; i < nBufferSize; ++i)
        {
            if (!pBuffer[i])
            {
                pData[i * 4] = 0xff;
            }
            else if (pBuffer[i] < depth_min_reliable_distance)
            {
                pData[i * 4 + 1] = pBuffer[i] & 0x7f + 0x80;
            }
            else if (pBuffer[i] > depth_max_reliable_distance)
            {
                pData[i * 4 + 2] = pBuffer[i] & 0x7f + 0x80;
            }
            else
            {
                pData[i * 4] = pBuffer[i] & 0xff;
                pData[i * 4 + 1] = pData[i * 4];
                pData[i * 4 + 2] = pData[i * 4];
            }
        }

        QImage* pImage = new QImage(pData, width, height, QImage::Format_RGBX8888, KinectDepthObj::myImageCleanupHandler, pData);
        this->addQImage(pImage);
    }

    SafeRelease(pFrameDescription);
    SafeRelease(pDepthFrame);
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

//void KinectDepthObj::addQImage(QImage *image)
//{
//    pMutex->lock();
//    mBufferImage.append(std::shared_ptr<QImage>(image));

//    if (mBufferImage.count() > MAX_FRAME_COUNT)
//    {
//        mBufferImage.pop_front();
//    }

//    pMutex->unlock();
//}

//std::shared_ptr<QImage> KinectDepthObj::getQImage()
//{
//    if (mBufferImage.count() <= 0)
//    {
//        return nullptr;
//    }

//    pMutex->lock();
//    auto pImage = mBufferImage.first();
//    mBufferImage.pop_front();
//    pMutex->unlock();

//    return pImage;
//}

WAITABLE_HANDLE KinectDepthObj::getWaitableHandle()
{
    return this->mhDepthFrameArrived;
}

bool KinectDepthObj::update()
{
    return false;
}
