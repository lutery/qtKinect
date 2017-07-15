#include "kinectfactory.h"
#include <kinectcolorobj.h>
#include <kinectdepthobj.h>
#include <kinectgrayobj.h>
#include <kinectbodyindex.h>
#include <kinectgreenscreeninbl.h>
#include <kinectbone.h>
#include <kinectfunnyman.h>
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
    closeColorFrame();
    closeDepthFrame();
    closeFraredFrame();
    closeGreenScreenFrame();
    closeBoneFrame();
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
        mColorObj = std::shared_ptr<KinectColorObj>(new KinectColorObj(mpKinect));
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

KinectFrameProtocol *KinectFactory::getFraredFrame()
{
    if (SUCCEEDED(this->initKinect()) && this->mpFraredObj == nullptr)
    {
        this->mpFraredObj = std::shared_ptr<KinectGrayObj>(new KinectGrayObj(mpKinect));
    }

    return this->mpFraredObj.get();
}

KinectFrameProtocol* KinectFactory::getBodyIndexFrame()
{
    if (SUCCEEDED(this->initKinect()) && this->mpBodyIndexObj == nullptr)
    {
        this->mpBodyIndexObj = std::shared_ptr<KinectBodyIndex>(new KinectBodyIndex(mpKinect));
    }

    return this->mpBodyIndexObj.get();
}

KinectFrameProtocol *KinectFactory::getGreenScreenFrame()
{
    if (SUCCEEDED(this->initKinect()) && this->mpGreenScreenObj == nullptr)
    {
        this->mpGreenScreenObj = std::shared_ptr<KinectGreenScreenInBl>(new KinectGreenScreenInBl(mpKinect));
    }

    return this->mpGreenScreenObj.get();
}

KinectFrameProtocol *KinectFactory::getBoneFrame()
{
    if (SUCCEEDED(this->initKinect()) && this->mpBoneObj == nullptr)
    {
        this->mpBoneObj = std::shared_ptr<KinectBone>(new KinectBone(mpKinect));
    }

    return this->mpBoneObj.get();
}

KinectFrameProtocol *KinectFactory::getFunnyManFrame()
{
    if (SUCCEEDED(this->initKinect()) && this->mpFunnyManObj == nullptr)
    {
        this->mpFunnyManObj = std::shared_ptr<KinectFunnyMan>(new KinectFunnyMan(mpKinect));
    }

    return this->mpFunnyManObj.get();
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

void KinectFactory::closeFraredFrame()
{
    this->mpFraredObj = nullptr;
}

void KinectFactory::closeGreenScreenFrame()
{
    this->mpGreenScreenObj = nullptr;
}

void KinectFactory::closeBoneFrame()
{
    this->mpBoneObj = nullptr;
}

void KinectFactory::closeFunnyManFrame()
{
    this->mpFunnyManObj = nullptr;
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
