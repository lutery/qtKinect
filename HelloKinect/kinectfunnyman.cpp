#include "kinectfunnyman.h"
#include <QPainter>
#include <QDebug>

KinectFunnyMan::KinectFunnyMan(IKinectSensor* pKinect, QObject *parent) : KinectObj(pKinect, parent)
{
    mpColorRGBX = nullptr;
    mpColorRGBX = new RGBQUAD[IMAGE_WIDTH * IMAGE_HEIGHT];
    mBufferSize = IMAGE_WIDTH * IMAGE_HEIGHT * sizeof(RGBQUAD);
//    mhFrameArrived = NULL;
    this->mpFunnyMan = new QImage(":/kinect/Kinect/res/funnyman.jpg");

    this->initKinect();
}

KinectFunnyMan::~KinectFunnyMan()
{
    SafeRelease(this->mpColorFrameReader);
    SafeRelease(this->mpCoordinateMapper);
    SafeRelease(this->mpBodyFrameReader);

    if (mpColorRGBX != nullptr)
    {
        delete[] mpColorRGBX;
        mpColorRGBX = nullptr;
    }

    if (mpTempImage != nullptr)
    {
        delete mpTempImage;
        mpTempImage = nullptr;
    }
}

HRESULT KinectFunnyMan::initKinect()
{
    IColorFrameSource* pColorFrameSource = nullptr;
    IBodyFrameSource* pBodyFrameSource = nullptr;

    HRESULT hr = this->mpKinect->get_ColorFrameSource(&pColorFrameSource);

    if (SUCCEEDED(hr))
    {
        hr = pColorFrameSource->OpenReader(&this->mpColorFrameReader);
    }

    if (SUCCEEDED(hr))
    {
        hr = mpKinect->get_BodyFrameSource(&pBodyFrameSource);
    }

    if (SUCCEEDED(hr))
    {
        hr = pBodyFrameSource->OpenReader(&this->mpBodyFrameReader);
    }

    if (SUCCEEDED(hr))
    {
        hr = this->mpKinect->get_CoordinateMapper(&this->mpCoordinateMapper);
    }

    SafeRelease(pBodyFrameSource);
    SafeRelease(pColorFrameSource);

    return hr;
}

void KinectFunnyMan::checkColorFrame()
{
    IColorFrame* pColorFrame = nullptr;
    IFrameDescription* pColorFrameDescription = nullptr;
    int width = 0;
    int height = 0;
    ColorImageFormat imageFormat = ColorImageFormat_None;
    UINT nBufferSize = 0;
    RGBQUAD* pBuffer = nullptr;

    HRESULT hr = this->mpColorFrameReader->AcquireLatestFrame(&pColorFrame);

    if (SUCCEEDED(hr))
    {
        hr = pColorFrame->get_FrameDescription(&pColorFrameDescription);
    }

    if (SUCCEEDED(hr))
    {
        hr = pColorFrameDescription->get_Width(&width);
    }

    if (SUCCEEDED(hr))
    {
        hr = pColorFrameDescription->get_Height(&height);
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
        else
        {
            pBuffer = mpColorRGBX;
            nBufferSize = mBufferSize;
            hr = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(mpColorRGBX), ColorImageFormat_Rgba);
        }
    }

    if (SUCCEEDED(hr))
    {
        uchar* pData = new uchar[nBufferSize];
        memcpy_s(pData, nBufferSize * sizeof(uchar), pBuffer, nBufferSize * sizeof(BYTE));
        QImage* pImage = new QImage(pData, width, height, QImage::Format_RGBX8888, KinectObj::myImageCleanupHandler, pData);
//        this->addQImage(pImage);
        mpTempImage = pImage;
    }

    SafeRelease(pColorFrame);
    SafeRelease(pColorFrameDescription);
}

void KinectFunnyMan::checkBodyFrame()
{
    IBodyFrame* pBodyFrame = nullptr;
    IBody* ppBody[BODY_COUNT] = {0};

    HRESULT hr = this->mpBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

    if (SUCCEEDED(hr))
    {
        hr = pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, ppBody);
    }

    if (SUCCEEDED(hr))
    {
        for (int i = 0; i < BODY_COUNT; ++i)
        {
            BOOLEAN bTracked = false;
            hr = ppBody[i]->get_IsTracked(&bTracked);
            if (!ppBody[i] || !bTracked)
            {
                continue;
            }

            IBody* pNowBody = ppBody[i];
            Joint joints[JointType_Count];
            hr = pNowBody->GetJoints(JointType_Count, joints);

            if (SUCCEEDED(hr))
            {
                ColorSpacePoint depthPoint = { 0.f, 0.f };
                this->mpCoordinateMapper->MapCameraPointToColorSpace(joints[JointType_Head].Position, &depthPoint);

                mFunnyPosX = depthPoint.X + 2.f * static_cast<float>((rand() % 4) - 2);
                mFunnyPosY = depthPoint.Y + 2.f * static_cast<float>((rand() % 4) - 2);
                qDebug() << "Funny Pos X is " << mFunnyPosX;

                mFunnyZoom = 1.f / joints[JointType_Head].Position.Z;
            }
        }
    }
}

void KinectFunnyMan::renderImage()
{
    if (this->mpTempImage != nullptr)
    {
        this->mpRenderTarget = std::shared_ptr<QPainter>(new QPainter(this->mpTempImage));
        this->mpRenderTarget->drawImage(this->mFunnyPosX, this->mFunnyPosY, *mpFunnyMan);
        this->mpRenderTarget = nullptr;
        this->addQImage(this->mpTempImage);
        this->mpTempImage = nullptr;
        this->mbReady = true;
    }
    else
    {
        this->mbReady = false;
    }
}

HRESULT KinectFunnyMan::checkFrame()
{
    return S_FALSE;
}

WAITABLE_HANDLE KinectFunnyMan::getWaitableHandle()
{
    return NULL;
}

bool KinectFunnyMan::update()
{
    this->checkColorFrame();
    this->checkBodyFrame();
    this->renderImage();

    return this->mbReady;
}
