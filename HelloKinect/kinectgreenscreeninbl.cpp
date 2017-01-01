#include "KinectGreenScreenInBl.h"
#include <QDebug>

KinectGreenScreenInBl::KinectGreenScreenInBl(IKinectSensor* pKinect, QObject *parent) : KinectObj(pKinect, parent)
{
    this->mpMultiSourceFrameReader = nullptr;
    this->mhMultiSourceFrameArrived = NULL;
    this->mpColorRGBX = nullptr;
    this->mpColorRGBX = new RGBQUAD[IMAGE_WIDTH * IMAGE_HEIGHT];
    this->mpColorCoordinates = new ColorSpacePoint[IMAGE_D_WIDTH * IMAGE_D_HEIGHT];
    this->mBufferSize = IMAGE_WIDTH * IMAGE_HEIGHT * sizeof(RGBQUAD);
    this->pMutex = new QMutex;

    this->initKinect();
}

KinectGreenScreenInBl::~KinectGreenScreenInBl()
{
    if (this->mhMultiSourceFrameArrived && this->mpMultiSourceFrameReader)
    {
        this->mpMultiSourceFrameReader->UnsubscribeMultiSourceFrameArrived(this->mhMultiSourceFrameArrived);
        this->mhMultiSourceFrameArrived = NULL;
    }

    SafeRelease(this->mpMultiSourceFrameReader);

    if (this->mhCoordinateMapperChanged && this->mpColorCoordinates)
    {
        this->mpCoordinateMapper->UnsubscribeCoordinateMappingChanged(this->mhCoordinateMapperChanged);
        this->mhCoordinateMapperChanged = NULL;
    }

    SafeRelease(this->mpCoordinateMapper);

    if (this->mpColorCoordinates)
    {
        delete[] this->mpColorCoordinates;
        this->mpColorCoordinates = nullptr;
    }

    if (this->mpColorRGBX)
    {
        delete[] this->mpColorRGBX;
        this->mpColorRGBX = nullptr;
    }
}

HRESULT KinectGreenScreenInBl::initKinect()
{
    HRESULT hr = mpKinect != nullptr ? S_OK : S_FALSE;

    if (SUCCEEDED(hr))
    {
        hr = mpKinect->OpenMultiSourceFrameReader(
                    FrameSourceTypes::FrameSourceTypes_Color |
                    FrameSourceTypes::FrameSourceTypes_BodyIndex |
                    FrameSourceTypes::FrameSourceTypes_Depth,
                    &this->mpMultiSourceFrameReader
                    );
    }

    if (SUCCEEDED(hr))
    {
        if (this->mhMultiSourceFrameArrived)
        {
            qDebug() << "mhMultiSource Frame Arrived Exist";
        }
        else
        {
            this->mpMultiSourceFrameReader->SubscribeMultiSourceFrameArrived(&this->mhMultiSourceFrameArrived);
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = this->mpKinect->get_CoordinateMapper(&this->mpCoordinateMapper);
    }

    if (SUCCEEDED(hr))
    {
        hr = this->mpCoordinateMapper->SubscribeCoordinateMappingChanged(&this->mhCoordinateMapperChanged);
    }

    return hr;
}

HRESULT KinectGreenScreenInBl::checkFrame()
{
    if (this->mpMultiSourceFrameReader == nullptr)
    {
        return S_FALSE;
    }

    // 复源临帧事件参数
    IMultiSourceFrameArrivedEventArgs* pArgs = nullptr;
    // 复源帧引用
    IMultiSourceFrameReference* pMSFrameRef = nullptr;
    // 复源帧
    IMultiSourceFrame* pMultiSourceFrame = nullptr;
    // 彩色帧
    IColorFrame* pColorFrame = nullptr;
    // 帧描述
    IFrameDescription* pColorFrameDescription = nullptr;
    // 彩色帧宽度数据
    int width_color = 0;
    // 彩色帧高度数据
    int height_color = 0;
    // 帧格式
    ColorImageFormat imageFormat = ColorImageFormat_None;
    // 彩色帧缓存大小
    UINT nColorBufferSize = 0;
    // 彩色帧缓存
    RGBQUAD *pColorBuffer = nullptr;

    HRESULT hr = this->mpMultiSourceFrameReader->GetMultiSourceFrameArrivedEventData(this->mhMultiSourceFrameArrived, &pArgs);

    if (SUCCEEDED(hr))
    {
        hr = pArgs->get_FrameReference(&pMSFrameRef);
    }

    if (SUCCEEDED(hr))
    {
        hr = pMSFrameRef->AcquireFrame(&pMultiSourceFrame);
    }

    if (SUCCEEDED(hr))
    {
        IColorFrameReference* pColorFrameReference = nullptr;
        hr = pMultiSourceFrame->get_ColorFrameReference(&pColorFrameReference);
        if (SUCCEEDED(hr))
        {
            hr = pColorFrameReference->AcquireFrame(&pColorFrame);
        }
        SafeRelease(pColorFrameReference);
    }

    if (SUCCEEDED(hr))
    {
        hr = pColorFrame->get_FrameDescription(&pColorFrameDescription);
    }

    if (SUCCEEDED(hr))
    {
        hr = pColorFrameDescription->get_Width(&width_color);
    }

    if (SUCCEEDED(hr))
    {
        hr = pColorFrameDescription->get_Height(&height_color);
    }

    if (SUCCEEDED(hr))
    {
        hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
    }

    if (SUCCEEDED(hr))
    {
        if (imageFormat == ColorImageFormat_Rgba)
        {
            hr = pColorFrame->AccessRawUnderlyingBuffer(&nColorBufferSize, reinterpret_cast<BYTE**>(&pColorBuffer));
        }
        else
        {
            nColorBufferSize = this->IMAGE_WIDTH * this->IMAGE_HEIGHT * sizeof(RGBQUAD);
            pColorBuffer = this->mpColorRGBX;
            hr = pColorFrame->CopyConvertedFrameDataToArray(nColorBufferSize, reinterpret_cast<BYTE*>(pColorBuffer), ColorImageFormat_Rgba);
        }
    }


    // 深度帧
    IDepthFrame* pDepthFrame = nullptr;
    // 深度帧缓存区大小
    UINT32 nDepthFrameBufferSize = 0;
    // 深度帧缓冲区位置
    UINT16* pDepthBuffer = nullptr;
    // 深度帧宽度
    int nDepthFrameWidth = 0;
    // 深度帧高度
    int nDepthFrameHeight = 0;
    // 深度帧描述
    IFrameDescription* pDepthFrameDescription = nullptr;

    if (SUCCEEDED(hr))
    {
        IDepthFrameReference* pDepthFrameReference = nullptr;
        hr = pMultiSourceFrame->get_DepthFrameReference(&pDepthFrameReference);
        if (SUCCEEDED(hr))
        {
            hr = pDepthFrameReference->AcquireFrame(&pDepthFrame);
        }
        SafeRelease(pDepthFrameReference);
    }

    if (SUCCEEDED(hr))
    {
        hr = pDepthFrame->get_FrameDescription(&pDepthFrameDescription);
    }

    if (SUCCEEDED(hr))
    {
        hr = pDepthFrameDescription->get_Width(&nDepthFrameWidth);
    }

    if (SUCCEEDED(hr))
    {
        hr = pDepthFrameDescription->get_Height(&nDepthFrameHeight);
    }

    if (SUCCEEDED(hr))
    {
        hr = pDepthFrame->AccessUnderlyingBuffer(&nDepthFrameBufferSize, &pDepthBuffer);
    }


    // 玩家索引帧
    IBodyIndexFrame* pBodyIndexFrame = nullptr;
    // 索引缓存区大小
    UINT32 nBodyIndexSize = 0;
    // 索引缓冲区位置
    BYTE* pIndexBuffer = nullptr;

    if (SUCCEEDED(hr))
    {
        IBodyIndexFrameReference* pBodyIndexFrameReference = nullptr;
        hr = pMultiSourceFrame->get_BodyIndexFrameReference(&pBodyIndexFrameReference);
        if (SUCCEEDED(hr))
        {
            hr = pBodyIndexFrameReference->AcquireFrame(&pBodyIndexFrame);
        }
        SafeRelease(pBodyIndexFrameReference);
    }

    if (SUCCEEDED(hr))
    {
        hr = pBodyIndexFrame->AccessUnderlyingBuffer(&nBodyIndexSize, &pIndexBuffer);
    }

    if (SUCCEEDED(hr))
    {
        hr = this->mpCoordinateMapper->MapDepthFrameToColorSpace(nDepthFrameWidth * nDepthFrameHeight, (UINT16*)pDepthBuffer, nDepthFrameWidth * nDepthFrameHeight, this->mpColorCoordinates);
    }

    if (SUCCEEDED(hr))
    {
        this->mBufferSize = nColorBufferSize;
        uchar* pData = new uchar[nColorBufferSize];
        memset(pData, 0, sizeof(uchar) * nColorBufferSize);
        int x, y;
        for (UINT i = 0; i < nDepthFrameBufferSize; ++i)
        {
            ColorSpacePoint colorPoint = this->mpColorCoordinates[i];
            x = static_cast<int>(floor(colorPoint.X + .5f));
            y = static_cast<int>(floor(colorPoint.Y + .5f));
            if (pIndexBuffer[i] == 0xff || x >= width_color || y >= height_color || x < 0 || y < 0)
            {

            }
            else
            {
                pData[y * width_color * 4 + x * 4 + 0] = pColorBuffer[y * width_color + x].rgbBlue;
                pData[y * width_color * 4 + x * 4 + 1] = pColorBuffer[y * width_color + x].rgbGreen;
                pData[y * width_color * 4 + x * 4 + 2] = pColorBuffer[y * width_color + x].rgbRed;
                pData[y * width_color * 4 + x * 4 + 3] = pColorBuffer[y * width_color + x].rgbReserved;
            }
        }

        QImage* pImage = new QImage(pData, width_color, height_color, QImage::Format_RGBX8888, KinectObj::myImageCleanupHandler, pData);
        this->addQImage(pImage);
    }

    SafeRelease(pDepthFrameDescription);
    SafeRelease(pDepthFrame);
    SafeRelease(pBodyIndexFrame);
    SafeRelease(pColorFrame);
    SafeRelease(pMultiSourceFrame);
    SafeRelease(pMSFrameRef);
    SafeRelease(pArgs);

    return hr;
}

WAITABLE_HANDLE KinectGreenScreenInBl::getWaitableHandle()
{
    return this->mhMultiSourceFrameArrived;
}
