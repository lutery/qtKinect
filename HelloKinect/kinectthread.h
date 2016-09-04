#ifndef KINECTTHREAD_H
#define KINECTTHREAD_H

#include <QObject>
#include <QThread>

class KinectObj;

class KinectThread : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE;

public:
    explicit KinectThread();

signals:
    void update();

public slots:

public:
    KinectObj* pKinectObj;
};

#endif // KINECTTHREAD_H
