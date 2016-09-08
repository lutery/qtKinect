#include "kinectwidget.h"
#include "kinectobj.h"
#include "kinectthread.h"
#include <QPainter>
#include <QDebug>
#include <QRect>
#include <QTime>
#include <QDateTime>
#include <QPoint>

KinectWidget::KinectWidget(QWidget *parent) : QWidget(parent)
{
//    pKinectObj = new KinectObj();
//    pKinectThread = new KinectThread();
//    pKinectThread->pKinectObj = pKinectObj;

//    connect(pKinectThread, SIGNAL(update()), this, SLOT(updateUI()));

//    pKinectThread->start();

    mpTime = new QTime();
    mpTime->start();
}

KinectWidget::~KinectWidget()
{
//    if (pKinectThread != nullptr)
//    {
//        if (pKinectThread->isRunning())
//        {
//            pKinectThread->exit();
//        }

//        delete pKinectThread;
//    }

//    if (pKinectObj != nullptr)
//    {
//        delete pKinectObj;
//    }

    if (mpTime != nullptr)
    {
        delete mpTime;
    }

    if (mpDrawTimeRC != nullptr)
    {
        delete mpDrawTimeRC;
    }

    this->closeKinect();
}

void KinectWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (pKinectObj != nullptr)
    {
        QPainter painter(this);
        auto pImage = pKinectObj->getQImage();
        if (pImage != nullptr)
        {
            int width = this->width();
            int height = this->height();

            QRect target(0, 0, width, height);
            QRect source(0, 0, pImage->width(), pImage->height());

            painter.drawImage(target, *pImage, source);

            static int preTime = 0;
            int curTime = mpTime->elapsed();
//            qDebug() << "paintEvent curTime - preTime = " << (curTime - preTime);
            preTime = curTime;
        }

        paintTime(painter);
    }
}

void KinectWidget::paintTime(QPainter& painter)
{
    if (mbShowTime)
    {
        if (mpDrawTimeRC == nullptr)
        {
            mpDrawTimeRC = new QRect(0, 0, this->width(), this->height());
        }

        painter.save();
        painter.setPen(Qt::red);
        painter.setFont(QFont("Arial", 30));
        QTime currentTime = QTime::currentTime();
        QString strTime = QString("%1:%2:%3.%4").arg(currentTime.hour()).arg(currentTime.minute())
                            .arg(currentTime.second()).arg(currentTime.msec());
        painter.drawText(*mpDrawTimeRC, strTime);
        painter.restore();
    }
}

void KinectWidget::openKinect()
{
    pKinectObj = new KinectObj();
    pKinectThread = new KinectThread();
    pKinectThread->pKinectObj = pKinectObj;

    connect(pKinectThread, SIGNAL(update()), this, SLOT(updateUI()));

    pKinectThread->start();
}

void KinectWidget::closeKinect()
{
    if (pKinectThread != nullptr)
    {
        if (pKinectThread->isRunning())
        {
            pKinectThread->exit();
        }

        delete pKinectThread;
        pKinectThread = nullptr;
    }

    if (pKinectObj != nullptr)
    {
        delete pKinectObj;
        pKinectObj = nullptr;
    }
}

void KinectWidget::updateUI()
{
//    static int preTime = 0;
//    int curTime = mpTime->elapsed();
//    qDebug() << "curTime - preTime = " << (curTime - preTime);
//    preTime = curTime;
    this->update();
}


