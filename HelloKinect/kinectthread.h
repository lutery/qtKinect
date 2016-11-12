#ifndef KINECTTHREAD_H
#define KINECTTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "kinectframeprotocol.h"

//class KinectObj;

/**
 * @brief The KinectThread class Kinect线程对象
 */
class KinectThread : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE;

public:
    explicit KinectThread();
    void stop();

signals:
    //更新信号
    void update();

public slots:

public:
    //Kinect对象
//    KinectObj* pKinectObj;
    KinectFrameProtocol* pKinectObj;
    QMutex mMutex;
    bool mbExit;
};

#endif // KINECTTHREAD_H
