#ifndef KINECTTHREAD_H
#define KINECTTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QOpenGLTexture>
#include <memory>
#include <QImage>
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
    void setKinectObj(KinectFrameProtocol* pKinectObj);

signals:
    //更新信号
    void update();
    void update(std::shared_ptr<QImage>);

public slots:

public:
    //Kinect对象
//    KinectObj* pKinectObj;
    KinectFrameProtocol* mpKinectObj;
    QMutex mMutex;
    bool mbExit;
};

#endif // KINECTTHREAD_H
