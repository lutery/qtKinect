#include "kinectitem.h"
#include "kinectrender.h"
#include <QQuickWindow>
#include "kinectthread.h"
#include "kinectfactory.h"
#include <QDebug>

KinectItem::KinectItem()
{
    mpRender = nullptr;
    connect(this, &QQuickItem::windowChanged, this, &KinectItem::handleWindowChanged);

    mpKinectThread = new KinectThread();
//    connect(mpKinectThread, SIGNAL(update(std::shared_ptr<QImage>)), this, SLOT(sltQImage(std::shared_ptr<QImage>)));
//    connect(mpKinectThread, SIGNAL(update()), this, SLOT(sltTest()));
    connect(mpKinectThread, SIGNAL(update()), this, SLOT(sltUpdate()));

    mpKinectThread->start();
}

KinectItem::~KinectItem()
{
    this->closeKinect();
}

void KinectItem::sync()
{
    if (mpRender == nullptr)
    {
        qDebug() << "sync mpRender create";
        mpRender = new KinectRender();
        connect(window(), &QQuickWindow::beforeRendering, mpRender, &KinectRender::sltPaint, Qt::DirectConnection);
        mpRender->sltInit();
    }

    mpRender->setViewportSize(window()->size() * window()->devicePixelRatio());
    mpRender->setWindow(window());
}

void KinectItem::cleanup()
{
    if (mpRender != nullptr)
    {
        delete mpRender;
        mpRender = nullptr;
    }
}

void KinectItem::sltTexture(QOpenGLTexture *texture)
{
    qDebug() << "sltTexture QOpenGLTexture";

    if (mpRender != nullptr)
    {
        mpRender->setCurTexture(texture);
        if (window())
        {
            window()->update();
        }
    }
}

void KinectItem::sltQImage(std::shared_ptr<QImage> image)
{
    qDebug() << "sltTexture std::shared_ptr<QImage>";
//    mpCurImage = image;
//    QOpenGLTexture* texture = new QOpenGLTexture(*mpCurImage);
//    this->sltTexture(texture);
    if (mpRender != nullptr)
    {
//        qDebug() << "mpRender != nullptr";
//        mpRender->setCurTexture(texture);
        mpRender->setCurImage(image);
        if (window())
        {
            qDebug() << "window() update";
            window()->update();
        }
    }
}

void KinectItem::sltUpdate()
{
    auto curImage = mpKinectObj->getQImage();
    this->sltQImage(curImage);
}

void KinectItem::sltOperaKinect(KinectItem::OperaId operaid)
{
    switch (operaid)
    {
    case Open:
        mpKinectObj = KinectFactory::Instance()->getColorFrame();
        mpKinectThread->setKinectObj(mpKinectObj);
        break;

    case Color:
        mpKinectObj = KinectFactory::Instance()->getColorFrame();
        mpKinectThread->setKinectObj(mpKinectObj);
        break;

    case Depth:
        mpKinectObj = KinectFactory::Instance()->getDepthFrame();
        mpKinectThread->setKinectObj(mpKinectObj);
        break;

    case Frared:
        mpKinectObj = KinectFactory::Instance()->getFraredFrame();
        mpKinectThread->setKinectObj(mpKinectObj);
        break;

    case Bodyindex:
        mpKinectObj = KinectFactory::Instance()->getBodyIndexFrame();
        mpKinectThread->setKinectObj(mpKinectObj);
        break;

    case Greenscreen:
        mpKinectObj = KinectFactory::Instance()->getGreenScreenFrame();
        mpKinectThread->setKinectObj(mpKinectObj);
        break;

    case Bone:
        mpKinectObj = KinectFactory::Instance()->getBoneFrame();
        mpKinectThread->setKinectObj(mpKinectObj);
        break;

    case Funnyman:
        mpKinectObj = KinectFactory::Instance()->getFunnyManFrame();
        mpKinectThread->setKinectObj(mpKinectObj);
        break;

    default:
        this->closeKinect();
        break;
    }
}

void KinectItem::handleWindowChanged(QQuickWindow *win)
{
    if (win != nullptr)
    {
        qDebug() << "handleWindowChanged";
        connect(win, &QQuickWindow::beforeSynchronizing, this, &KinectItem::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &KinectItem::cleanup, Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

void KinectItem::closeKinect()
{
    if (mpKinectThread != nullptr)
    {
        while (mpKinectThread->isRunning())
        {
            mpKinectThread->stop();
        }

        delete mpKinectThread;
        mpKinectThread = nullptr;
    }

    mpKinectObj = nullptr;
}
