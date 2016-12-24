#include "KinectGreenScreenInBl.h"

KinectGreenScreenInBl::KinectGreenScreenInBl(IKinectSensor* pKinect, QObject *parent) : KinectObj(pKinect, parent)
{
    this->mpMultiSourceFrameReader = nullptr;
    this->mhMultiSourceFrameArrived = NULL;
    this->mpColorRGBX = nullptr;
    this->mpColorRGBX = new RGBQUAD[IMAGE_WIDTH * IMAGE_HEIGHT];
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
}

HRESULT KinectGreenScreenInBl::initKinect()
{
    HRESULT hr = mpKinect != nullptr ? S_OK : S_FALSE;

    if (SUCCEEDED(hr))
    {
        hr = mpKinect->OpenMultiSourceFrameReader()
    }
}

HRESULT KinectGreenScreenInBl::checkFrame()
{

}

WAITABLE_HANDLE KinectGreenScreenInBl::getWaitableHandle()
{

}
