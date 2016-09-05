#include "kinectwidget.h"
#include "kinectobj.h"
#include "kinectthread.h"
#include <QPainter>
#include <QDebug>
#include <QRect>

KinectWidget::KinectWidget(QWidget *parent) : QWidget(parent)
{
    pKinectObj = new KinectObj();
    pKinectThread = new KinectThread();
    pKinectThread->pKinectObj = pKinectObj;

    connect(pKinectThread, SIGNAL(update()), this, SLOT(updateUI()));

    pKinectThread->start();
}

KinectWidget::~KinectWidget()
{
    if (pKinectThread != nullptr)
    {
        if (pKinectThread->isRunning())
        {
            pKinectThread->exit();
        }

        delete pKinectThread;
    }

    if (pKinectObj != nullptr)
    {
        delete pKinectObj;
    }
}

void KinectWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    auto pImage = pKinectObj->getQImage();
    if (pImage != nullptr)
    {
        int width = this->width();
        int height = this->height();

        QRect target(0, 0, width, height);
        QRect source(0, 0, pImage->width(), pImage->height());

        painter.drawImage(target, *pImage, source);
    }
}

void KinectWidget::updateUI()
{
    this->update();
}


