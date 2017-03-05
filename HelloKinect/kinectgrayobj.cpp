#include "kinectgrayobj.h"
#include <QDebug>

KinectGrayObj::KinectGrayObj(IKinectSensor* pKinect, QObject *parent) : KinectObj(pKinect, parent)
{
    mpFraredFrameReader = nullptr;
    mhFraredFrameArrived = NULL;
    mpColorRGBX = nullptr;
    mpColorRGBX = new RGBQUAD[IMAGE_WIDTH * IMAGE_HEIGHT];
    mBufferSize = IMAGE_WIDTH * IMAGE_HEIGHT * sizeof(RGBQUAD);

    this->initKinect();
}

KinectGrayObj::~KinectGrayObj()
{
    if (mhFraredFrameArrived != NULL && mpFraredFrameReader != nullptr)
    {
        mpFraredFrameReader->UnsubscribeFrameArrived(this->mhFraredFrameArrived);
        this->mhFraredFrameArrived = NULL;
    }

    SafeRelease(this->mpFraredFrameReader);
}

HRESULT KinectGrayObj::initKinect()
{
    IInfraredFrameSource* pInfraredFrameSource = nullptr;

    HRESULT hr = mpKinect != nullptr ? S_OK : S_FALSE;

    if (SUCCEEDED(hr))
    {
        hr = mpKinect->get_InfraredFrameSource(&pInfraredFrameSource);
    }

    if (SUCCEEDED(hr))
    {
        if (this->mpFraredFrameReader == nullptr)
        {
            hr = pInfraredFrameSource->OpenReader(&this->mpFraredFrameReader);
        }
        else
        {
            qDebug() << "成员变量mpFraredFrameReader已经存在";
        }
    }

    if (SUCCEEDED(hr))
    {
        if (this->mhFraredFrameArrived == NULL)
        {
            hr = this->mpFraredFrameReader->SubscribeFrameArrived(&this->mhFraredFrameArrived);
        }
        else
        {
            qDebug() << "成员变量mhFraredFrameArrived已经存在";
        }
    }

    SafeRelease(pInfraredFrameSource);

    return hr;
}

HRESULT KinectGrayObj::checkFrame()
{
    if (this->mpFraredFrameReader == nullptr)
    {
        return S_FALSE;
    }

    // 红外临帧事件参数
       IInfraredFrameArrivedEventArgs* pArgs = nullptr;
       // 红外帧引用
       IInfraredFrameReference* pIRFrameRef = nullptr;
       // 红外帧
       IInfraredFrame* pInfraredFrame = nullptr;
       // 帧描述
       IFrameDescription* pFrameDescription = nullptr;
       // 红外帧宽度数据
       int width = 0;
       // 红外帧高度数据
       int height = 0;
       // 帧缓存大小
       UINT nBufferSize = 0;
       // 帧缓存
       UINT16 *pBuffer = nullptr;

       HRESULT hr = this->mpFraredFrameReader->GetFrameArrivedEventData(this->mhFraredFrameArrived, &pArgs);

       // 获取引用
       if (SUCCEEDED(hr)) {
           hr = pArgs->get_FrameReference(&pIRFrameRef);
       }
       // 获取红外帧
       if (SUCCEEDED(hr)) {
           hr = pIRFrameRef->AcquireFrame(&pInfraredFrame);
       }
       // 获取帧描述
       if (SUCCEEDED(hr)) {
           hr = pInfraredFrame->get_FrameDescription(&pFrameDescription);
       }
       // 获取帧宽度
       if (SUCCEEDED(hr)) {
          hr = pFrameDescription->get_Width(&width);
       }
       // 获取帧高度
       if (SUCCEEDED(hr)) {
          hr = pFrameDescription->get_Height(&height);
       }
       // 获取红外数据
       if (SUCCEEDED(hr)) {
           hr = pInfraredFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
       }

       if (SUCCEEDED(hr)){
           uchar* pData= new uchar[nBufferSize * 4];
           ZeroMemory(pData, nBufferSize * 4);

           for (uint i = 0; i < nBufferSize; ++i)
           {
               pData[i * 4] = pData[i * 4 + 1] = pData[i * 4 + 2] = static_cast<BYTE>(pBuffer[i] >> 7);
           }

           QImage* pImage = new QImage(pData, width, height, QImage::Format_RGBX8888, KinectObj::myImageCleanupHandler);
           this->addQImage(pImage);
       }

       // 安全释放
       SafeRelease(pFrameDescription);
       SafeRelease(pInfraredFrame);
       SafeRelease(pIRFrameRef);
       SafeRelease(pArgs);

       if (SUCCEEDED(hr))
       {
           qDebug() << "红外扫描成功";
       }
       else
       {
           qDebug() << "红外扫描失败";
       }

       return hr;
}

WAITABLE_HANDLE KinectGrayObj::getWaitableHandle()
{
    return this->mhFraredFrameArrived;
}

bool KinectGrayObj::update()
{
    return false;
}
