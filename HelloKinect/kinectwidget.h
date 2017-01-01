#ifndef KINECTWIDGET_H
#define KINECTWIDGET_H

#include <QWidget>
#include "stdinc.h"
#include "kinectframeprotocol.h"

class KinectObj;
class KinectThread;
class QTime;
class QPainter;
class QPoint;
class QRect;

/**
 * @brief The KinectWidget class 显示Kinect帧部件
 */
class KinectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KinectWidget(QWidget *parent = 0);
    virtual ~KinectWidget();

signals:

public slots:
    void updateUI();

public:
    //是否显示时间
    bool mbShowTime = true;

private:
    KinectFrameProtocol* pKinectObj = nullptr;
    //Kinect获取帧线程
    KinectThread* pKinectThread = nullptr;
    //时间对象
    QTime* mpTime = nullptr;
    //绘制时间区域
    QRect* mpDrawTimeRC = nullptr;

protected:
    //重载绘制事件
    void paintEvent(QPaintEvent *event) override;
    //绘制事件
    void paintTime(QPainter& painter);

public:
    //打开Kinect
    void openKinect();
    void openDepthFrame();
    void openFraredFrame();
    void openBodyIndexFrame();
    void openGreenScreenFrame();
    //关闭Kinect
    void closeKinect();
};

#endif // KINECTWIDGET_H
