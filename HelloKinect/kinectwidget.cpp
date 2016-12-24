#include "kinectwidget.h"
#include "kinectobj.h"
#include "kinectthread.h"
#include <QPainter>
#include <QDebug>
#include <QRect>
#include <QTime>
#include <QDateTime>
#include <QPoint>
#include "kinectfactory.h"

KinectWidget::KinectWidget(QWidget *parent) : QWidget(parent)
{
    mpTime = new QTime();
    mpTime->start();

    pKinectThread = new KinectThread();

    connect(pKinectThread, SIGNAL(update()), this, SLOT(updateUI()));

    pKinectThread->start();
}

/**
 * @brief KinectWidget::~KinectWidget 析构函数
 */
KinectWidget::~KinectWidget()
{
    //定时器对象，用来统计时间
    if (mpTime != nullptr)
    {
        delete mpTime;
    }

    //绘制时间区域对对象，对来表示时间的绘制区域
    if (mpDrawTimeRC != nullptr)
    {
        delete mpDrawTimeRC;
    }

    //释放Kinect资源
    this->closeKinect();
}

/**
 * @brief KinectWidget::paintEvent 绘制录制图像
 * @param event
 */
void KinectWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (pKinectObj != nullptr)
    {
        //创建绘制对象
        QPainter painter(this);
        //获取当前录制的图像
        auto pImage = pKinectObj->getQImage();
        if (pImage != nullptr)
        {
            //获取控件的尺寸
            int width = this->width();
            int height = this->height();

            QRect target(0, 0, width, height);
            QRect source(0, 0, pImage->width(), pImage->height());

            //将图像绘制到控件上面
            painter.drawImage(target, *pImage, source);

            //计算这次绘制与上次绘制的时间差，用来计算帧数
            static int preTime = 0;
            int curTime = mpTime->elapsed();
//            qDebug() << "paintEvent curTime - preTime = " << (curTime - preTime);
            preTime = curTime;
        }

        //绘制时间
        paintTime(painter);
    }
}

/**
 * @brief KinectWidget::paintTime 绘制时间
 * @param painter
 */
void KinectWidget::paintTime(QPainter& painter)
{
    //判断是否需要显示时间
    if (mbShowTime)
    {
        if (mpDrawTimeRC == nullptr)
        {
            mpDrawTimeRC = new QRect(0, 0, this->width(), this->height());
        }

        static int preTime = 0;
        int curTime = mpTime->elapsed();
        int fps = 1000 / ((curTime - preTime) == 0 ? 1 : (curTime - preTime));
        QString strFps = QString("FPS:%1").arg(fps);

        painter.save();
        painter.setPen(Qt::red);
        painter.setFont(QFont("Arial", 30));
//        QTime currentTime = QTime::currentTime();
//        QString strTime = QString("%1:%2:%3.%4").arg(currentTime.hour()).arg(currentTime.minute())
//                            .arg(currentTime.second()).arg(currentTime.msec());
//        painter.drawText(*mpDrawTimeRC, strTime);
        painter.drawText(*mpDrawTimeRC, strFps);
        preTime = curTime;
        painter.restore();
    }
}

/**
 * @brief KinectWidget::openKinect 打开Kinect对象
 */
void KinectWidget::openKinect()
{
    //获取颜色帧对象，并赋值到子线程终
    pKinectObj = KinectFactory::Instance()->getColorFrame();
    pKinectThread->pKinectObj = pKinectObj;
}

void KinectWidget::openDepthFrame()
{
    pKinectObj = KinectFactory::Instance()->getDepthFrame();
    pKinectThread->pKinectObj = pKinectObj;
}

void KinectWidget::openFraredFrame()
{
    pKinectObj = KinectFactory::Instance()->getFraredFrame();
    pKinectThread->pKinectObj = pKinectObj;
}

void KinectWidget::openBodyIndexFrame()
{
    pKinectObj = KinectFactory::Instance()->getBodyIndexFrame();
    pKinectThread->pKinectObj = pKinectObj;
}

/**
 * @brief KinectWidget::closeKinect 关闭Kinect对象
 */
void KinectWidget::closeKinect()
{
    //判断线程是否存在
    if (pKinectThread != nullptr)
    {
        //如果线程还在运行则开始停止线程
        while (pKinectThread->isRunning())
        {
            pKinectThread->stop();
        }

        //释放线程对象
        delete pKinectThread;
        pKinectThread = nullptr;
    }

    //设置Kinect帧对象为空
    pKinectObj = nullptr;
//    KinectFactory::Instance()->closeColorFrame();

//    KinectFactory::Instance()->closeKinect();
}

/**
 * @brief KinectWidget::updateUI 接收子线程更新ui槽函数
 */
void KinectWidget::updateUI()
{
    this->update();
}


