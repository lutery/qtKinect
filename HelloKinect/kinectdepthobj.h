#ifndef KINECTDEPTHOBJ_H
#define KINECTDEPTHOBJ_H

#include <QObject>
#include <QLinkedList>
#include <QImage>
#include <QMutex>
#include <memory>
#include <stdinc.h>

class KinectDepthObj : public QObject
{
    Q_OBJECT
public:
    explicit KinectDepthObj(IKinectSensor* pKinect = nullptr, QObject *parent = 0);
    virtual ~KinectDepthObj();

signals:

public slots:


};

#endif // KINECTDEPTHOBJ_H
