#ifndef KINECTOBJ_H
#define KINECTOBJ_H

#include <QObject>
#include <QLinkedList>
#include <QImage>
#include <QMutex>
#include <memory>
#include "stdinc.h"

class KinectObj : public QObject
{
    Q_OBJECT
public:
    explicit KinectObj(QObject *parent = 0);
    virtual ~KinectObj();

signals:

public slots:

private:
    const int IMAGE_WIDTH = 1920;
    const int IMAGE_HEIGHT = 1080;
    IKinectSensor* mpKinect;
    IColorFrameReader* mpColorFrameReader;
    RGBQUAD* mpColorRGBX;
    unsigned int mBufferSize = 0;
    QLinkedList<std::shared_ptr<QImage>> mBufferImage;
    QMutex* pMutex = nullptr;

    static void myImageCleanupHandler(void *info);

private:
    HRESULT initKinect();

public:
    WAITABLE_HANDLE mhColorFrameArrived;

    HRESULT checkColorFrame();
    void addQImage(QImage* image);
    std::shared_ptr<QImage> getQImage();
};

#endif // KINECTOBJ_H
