#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QPainter>
#include "kinectobj.h"
#include "kinectthread.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
//    mpKinect = nullptr;
//    mpColorFrameReader = nullptr;
//    mhColorFrameArrived = NULL;
//    mpColorRGBX = nullptr;
//    mpColorRGBX = new RGBQUAD[IMAGE_WIDTH * IMAGE_HEIGHT];
//    nBufferSize = IMAGE_WIDTH * IMAGE_HEIGHT * sizeof(RGBQUAD);

//    this->initKinect();
    this->setFixedWidth(1920);
    this->setFixedHeight(1080);

    pKinectObj = new KinectObj();

    pKinectThread = new KinectThread();
    pKinectThread->pKinectObj = pKinectObj;

    connect(pKinectThread, SIGNAL(update()), this, SLOT(updateUI()));

    pKinectThread->start();
}

Dialog::~Dialog()
{
    delete ui;

    if (pKinectThread != nullptr)
    {
        if (pKinectThread->isRunning())
        {
            pKinectThread->exit();
        }

        delete[] pKinectThread;
    }

    if (pKinectObj != nullptr)
    {
        delete pKinectObj;
    }

//    if (mhColorFrameArrived != NULL && mpColorFrameReader != nullptr)
//    {
//        mpColorFrameReader->UnsubscribeFrameArrived(mhColorFrameArrived);
//        mhColorFrameArrived = 0;
//    }

//    SafeRelease(mpColorFrameReader);

//    if (mpKinect != nullptr)
//    {
//        mpKinect->Close();
//        qDebug() << "close Kinect Success";
//    }

//    SafeRelease(mpKinect);

//    if (mpColorRGBX != nullptr)
//    {
//        delete[] mpColorRGBX;
    //    }
}

void Dialog::updateUI()
{
    this->update();
}

void Dialog::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);

    QPainter painter(this);
    auto pImage = pKinectObj->getQImage();
    if (pImage != nullptr)
    {
        painter.drawImage(0, 0, *pImage);
    }
}

//HRESULT Dialog::initKinect()
//{
//    IColorFrameSource* pColorFrameSource = nullptr;
//    HRESULT hr = ::GetDefaultKinectSensor(&mpKinect);

//    if (SUCCEEDED(hr))
//    {
//        hr = mpKinect->Open();
//        qDebug() << "open Kinect Success";
//    }

//    if (SUCCEEDED(hr))
//    {
//        hr = mpKinect->get_ColorFrameSource(&pColorFrameSource);
//    }

//    if (SUCCEEDED(hr))
//    {
//        if (mpColorFrameReader != nullptr)
//        {
//            qDebug() << "成员变量mpColorFrameReader已经存在";
//        }

//        hr = pColorFrameSource->OpenReader(&mpColorFrameReader);
//    }

//    if (SUCCEEDED(hr))
//    {
//        if (mhColorFrameArrived)
//        {
//            qDebug() << "成员变量mhColorFrameArrived已经存在";
//        }

//        mpColorFrameReader->SubscribeFrameArrived(&mhColorFrameArrived);
//    }

//    return S_OK;
//}

//void Dialog::checkColorFrame()
//{
//    if (mpColorFrameReader != nullptr)
//    {
//        return;
//    }

//    IColorFrameArrivedEventArgs* pArgs = nullptr;
//    IColorFrameReference* pCFrameRef = nullptr;
//    IColorFrame* pColorFrame = nullptr;
//    IFrameDescription* pFrameDescription = nullptr;
//    int width = 0;
//    int height = 0;
//    ColorImageFormat imageFormat = ColorImageFormat_None;
//    UINT nBufferSize = 0;
//    RGBQUAD* pBuffer = nullptr;

//    HRESULT hr = mpColorFrameReader->GetFrameArrivedEventData(mhColorFrameArrived, &pArgs);

//    if (SUCCEEDED(hr))
//    {
//        hr = pArgs->get_FrameReference(&pCFrameRef);
//    }

//    if (SUCCEEDED(hr))
//    {
//        hr = pCFrameRef->AcquireFrame(&pColorFrame);
//    }

//    if (SUCCEEDED(hr))
//    {
//        hr = pColorFrame->get_FrameDescription(&pFrameDescription);
//    }

//    if (SUCCEEDED(hr))
//    {
//        hr = pFrameDescription->get_Width(&width);
//    }

//    if (SUCCEEDED(hr))
//    {
//        hr = pFrameDescription->get_Height(&height);
//    }

//    if (SUCCEEDED(hr))
//    {
//        hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
//    }

//    if (SUCCEEDED(hr))
//    {
//        if (imageFormat == ColorImageFormat_Rgba)
//        {
//            hr = pColorFrame->AccessRawUnderlyingBuffer(&nBufferSize, reinterpret_cast<BYTE**>(&pBuffer));
//        }
//        else if (mpColorRGBX != nullptr)
//        {
//            hr = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(pBuffer), ColorImageFormat_Rgba);
//        }
//    }

//    if (SUCCEEDED(hr))
//    {
//        //这里将像素数据拷贝到显示
//    }

//    SafeRelease(pFrameDescription);
//    SafeRelease(pColorFrame);
//    SafeRelease(pCFrameRef);
//    SafeRelease(pArgs);

//    if (SUCCEEDED(hr))
//    {
//        qDebug() << "成功";
//    }
//    else
//    {
//        qDebug() << "失败";
//    }
//}
