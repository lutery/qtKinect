#include "kinectfactory.h"
#include <kinectobj.h>
#include <QDebug>

KinectFactory* KinectFactory::pSelf = nullptr;

KinectFactory::KinectFactory(QObject *parent) : QObject(parent)
{
    mColorObj = nullptr;
    mhrReady = S_FALSE;
    mOpened = false;
}

KinectFactory::~KinectFactory()
{
//    if (mColorObj != nullptr)
//    {
//        delete mColorObj;
//        mColorObj = nullptr;
//    }
    mColorObj = nullptr;
}

KinectFactory* KinectFactory::Instance()
{
    if (pSelf == nullptr)
    {
        pSelf = new KinectFactory();
        pSelf->initKinect();
    }

    return pSelf;
}

HRESULT KinectFactory::initKinect()
{
    if (S_FALSE == mhrReady)
    {
        mhrReady = ::GetDefaultKinectSensor(&mpKinect);
    }

    if (SUCCEEDED(mhrReady) && !mOpened)
    {
        mhrReady = mpKinect->Open();

        if (SUCCEEDED(mhrReady))
        {
            mOpened = true;
             qDebug() << "open Kinect Success";
        }
    }

    return mhrReady;
}

void KinectFactory::closeKinect()
{
    if (mpKinect != nullptr)
    {
        mpKinect->Close();
        mpKinect = nullptr;
        mOpened = false;
        mhrReady = S_FALSE;
        qDebug() << "close Kinect Success";
    }
}

KinectFrameProtocol* KinectFactory::getColorFrame()
{
    if (SUCCEEDED(this->initKinect()) && mColorObj == nullptr)
    {
        mColorObj = std::shared_ptr<KinectObj>(new KinectObj(mpKinect));
    }

    return mColorObj.get();
}

void KinectFactory::closeColorFrame()
{
//    this->closeFrameObj(&this->);
    this->mColorObj = nullptr;
}

void KinectFactory::closeFrameObj(KinectFrameProtocol** pFrameObj)
{
    if ((*pFrameObj) != nullptr)
    {
        delete *pFrameObj;
        *pFrameObj = nullptr;
    }
}
