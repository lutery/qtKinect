#include "kinectfactory.h"
#include <kinectobj.h>
#include <kinectdepthobj.h>
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
    mColorObj = nullptr;
}

/**
 * @brief KinectFactory::Instance 返回Kinect实例对象指针
 * @return KinectFactory实例对象指针
 */
KinectFactory* KinectFactory::Instance()
{
    if (pSelf == nullptr)
    {
        pSelf = new KinectFactory();
        pSelf->initKinect();
    }

    return pSelf;
}

/**
 * @brief KinectFactory::initKinect 初始化Kinect设备对象
 * @return 返回S_FALSE表示初始化失败，返回S_OK表示初始化成功
 */
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

/**
 * @brief KinectFactory::closeKinect 关闭Kinect对象
 */
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

    SafeRelease(mpKinect);
}

/**
 * @brief KinectFactory::getColorFrame 获取颜色帧
 * @return 返回颜色帧对象
 */
KinectFrameProtocol* KinectFactory::getColorFrame()
{
    if (SUCCEEDED(this->initKinect()) && mColorObj == nullptr)
    {
        mColorObj = std::shared_ptr<KinectObj>(new KinectObj(mpKinect));
    }

    return mColorObj.get();
}

KinectFrameProtocol* KinectFactory::getDepthFrame()
{
    if (SUCCEEDED(this->initKinect()) && mpDepthObj == nullptr)
    {
        mpDepthObj = std::shared_ptr<KinectDepthObj>(new KinectDepthObj(mpKinect));
    }

    return mpDepthObj.get();
}

/**
 * @brief KinectFactory::closeColorFrame 关闭颜色帧对象
 */
void KinectFactory::closeColorFrame()
{
    this->mColorObj = nullptr;
}

void KinectFactory::closeDepthFrame()
{
    this->mpDepthObj = nullptr;
}

/**
 * @brief KinectFactory::closeFrameObj 关闭帧对象通用函数，可以用协议实现，也可以用模板实现
 * @param pFrameObj 返回帧对象
 */
void KinectFactory::closeFrameObj(KinectFrameProtocol** pFrameObj)
{
    if ((*pFrameObj) != nullptr)
    {
        delete *pFrameObj;
        *pFrameObj = nullptr;
    }
}
