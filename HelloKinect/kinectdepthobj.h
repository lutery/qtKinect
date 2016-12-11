#ifndef KINECTDEPTHOBJ_H
#define KINECTDEPTHOBJ_H

#include <QObject>
#include <QLinkedList>
#include <QImage>
#include <QMutex>
#include <memory>
#include <stdinc.h>
#include "kinectframeprotocol.h"

/**
 * @brief The KinectDepthObj class Kinect深度帧对象
 */
class KinectDepthObj : public QObject, public KinectFrameProtocol
{
    Q_OBJECT
public:
    explicit KinectDepthObj(IKinectSensor* pKinect = nullptr, QObject *parent = 0);
    virtual ~KinectDepthObj();

signals:

public slots:

private:
    const int IMAGE_WIDTH = 1920;
    const int IMAGE_HEIGHT = 1080;
    const int MAX_FRAME_COUNT = 60;
    //Kinect传感器
    IKinectSensor* mpKinect;
    //颜色帧阅读器
    IDepthFrameReader* mpDepthFrameReader = nullptr;
    //颜色帧RGB缓存
    RGBQUAD* mpColorRGBX;
    //颜色帧缓存的长度
    unsigned int mBufferSize = 0;
    //颜色帧每帧缓存
    QLinkedList<std::shared_ptr<QImage>> mBufferImage;
    //获取已存储的帧缓存锁
    QMutex* pMutex = nullptr;
    //可等待的颜色帧到达事件句柄
    WAITABLE_HANDLE mhDepthFrameArrived;
    //清除帧缓存内存，用于QImage释放的时候释放相应的内存
    static void myImageCleanupHandler(void *info);

private:
    HRESULT initKinect();

public:
    HRESULT checkFrame() override;
    void addQImage(QImage* image);
    std::shared_ptr<QImage> getQImage() override;
    WAITABLE_HANDLE getWaitableHandle() override;
};

#endif // KINECTDEPTHOBJ_H
