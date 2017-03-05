#include "kinectbodyindex.h"
#include <QDebug>

KinectBodyIndex::KinectBodyIndex(IKinectSensor* pKinect, QObject *parent) : KinectObj(pKinect, parent)
{
    this->mpBodyIndexFrameReader = nullptr;
    this->mhBodyIndexFrameArrived = NULL;
    this->mpColorRGBX = nullptr;
    this->mpColorRGBX = new RGBQUAD[IMAGE_WIDTH * IMAGE_HEIGHT];
    this->mBufferSize = IMAGE_WIDTH * IMAGE_HEIGHT * sizeof(RGBQUAD);
    this->pMutex = new QMutex;

    this->initKinect();
}

KinectBodyIndex::~KinectBodyIndex()
{
    if (this->mhBodyIndexFrameArrived != NULL && this->mpBodyIndexFrameReader != nullptr)
    {
        this->mpBodyIndexFrameReader->UnsubscribeFrameArrived(this->mhBodyIndexFrameArrived);
        this->mhBodyIndexFrameArrived = NULL;
    }

    SafeRelease(this->mpBodyIndexFrameReader);
}

HRESULT KinectBodyIndex::initKinect()
{
    IBodyIndexFrameSource* pBodyIndexFrameSource = nullptr;

    HRESULT hr = mpKinect != nullptr ? S_OK : S_FALSE;

    if (SUCCEEDED(hr))
    {
        hr = mpKinect->get_BodyIndexFrameSource(&pBodyIndexFrameSource);
    }

    if (SUCCEEDED(hr))
    {
        if (this->mpBodyIndexFrameReader != nullptr)
        {
            qDebug() << "成员变量mpBodyIndexFrameReader已经存在";
        }
        else
        {
            hr = pBodyIndexFrameSource->OpenReader(&this->mpBodyIndexFrameReader);
        }
    }

    if (SUCCEEDED(hr))
    {
        if (this->mhBodyIndexFrameArrived != NULL)
        {
            qDebug() << "成员变量mhBodyIndexFrameArrived已经存在";
        }
        else
        {
            hr = this->mpBodyIndexFrameReader->SubscribeFrameArrived(&this->mhBodyIndexFrameArrived);
        }
    }

    SafeRelease(pBodyIndexFrameSource);

    return hr;
}

HRESULT KinectBodyIndex::checkFrame()
{
    if (this->mpBodyIndexFrameReader == nullptr)
    {
        return S_FALSE;
    }

    // 玩家索引临帧事件参数
       IBodyIndexFrameArrivedEventArgs* pArgs = nullptr;
       // 玩家索引帧引用
       IBodyIndexFrameReference* pCFrameRef = nullptr;
       // 玩家索引帧
       IBodyIndexFrame* pBodyIndexFrame = nullptr;
       // 帧描述
       IFrameDescription* pFrameDescription = nullptr;
       // 玩家索引帧宽度数据
       int width = 0;
       // 玩家索引帧高度数据
       int height = 0;
       // 索引缓存大小
       UINT nBufferSize = 0;
       // 索引缓存
       BYTE *pBuffer = nullptr;

       HRESULT hr = this->mpBodyIndexFrameReader->GetFrameArrivedEventData(this->mhBodyIndexFrameArrived, &pArgs);

       if (SUCCEEDED(hr))
       {
           hr = pArgs->get_FrameReference(&pCFrameRef);
       }

       if (SUCCEEDED(hr))
       {
           hr = pCFrameRef->AcquireFrame(&pBodyIndexFrame);
       }

       if (SUCCEEDED(hr))
       {
           hr = pBodyIndexFrame->get_FrameDescription(&pFrameDescription);
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
           hr = pBodyIndexFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
       }

       if (SUCCEEDED(hr))
       {
            uchar* pData = new uchar[nBufferSize * 4];

            for (uint i = 0; i < nBufferSize; ++i)
            {
                pData[i * 4 + 0] = pBuffer[i] & 0x01 ? 0x00 : 0xff;
                pData[i * 4 + 1] = pBuffer[i] & 0x02 ? 0x00 : 0xff;
                pData[i * 4 + 2] = pBuffer[i] & 0x04 ? 0x00 : 0xff;
                pData[i * 4 + 3] = 0xff;
            }

            QImage* pImage = new QImage(pData, width, height, QImage::Format_RGBX8888, KinectBodyIndex::myImageCleanupHandler);

            this->addQImage(pImage);
       }

       SafeRelease(pFrameDescription);
       SafeRelease(pBodyIndexFrame);
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

WAITABLE_HANDLE KinectBodyIndex::getWaitableHandle()
{
    return this->mhBodyIndexFrameArrived;
}

bool KinectBodyIndex::update()
{
    return false;
}
