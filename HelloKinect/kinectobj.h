#ifndef KINECTOBJ_H
#define KINECTOBJ_H

#include <QObject>
#include <QLinkedList>
#include <QImage>
#include <QMutex>
#include <memory>
#include "stdinc.h"
#include "kinectframeprotocol.h"

/**
 * @brief The KinectObj class Kinect颜色帧对象
 */
class KinectObj : public QObject, public KinectFrameProtocol
{
    Q_OBJECT
public:
    explicit KinectObj(IKinectSensor* pKinect = nullptr, QObject *parent = 0);
    virtual ~KinectObj();

signals:

public slots:

private:
    const int IMAGE_WIDTH = 1920;
    const int IMAGE_HEIGHT = 1080;
    const int MAX_FRAME_COUNT = 60;
    //Kinect传感器
    IKinectSensor* mpKinect;
    //颜色帧读取对象
    IColorFrameReader* mpColorFrameReader;
    //颜色帧RGB缓存
    RGBQUAD* mpColorRGBX;
    //颜色帧缓存的长度
    unsigned int mBufferSize = 0;
    //颜色帧每帧缓存
    QLinkedList<std::shared_ptr<QImage>> mBufferImage;
    //获取已存储的帧缓存锁
    QMutex* pMutex = nullptr;
    //可等待的颜色帧到达事件句柄
    WAITABLE_HANDLE mhColorFrameArrived;

    //清除帧缓存内存，用于QImage释放的时候释放相应的内存
    static void myImageCleanupHandler(void *info);

private:
    //初始化Kinect对象
    HRESULT initKinect();

public:
    //获取当前帧缓存的RGB缓存
    HRESULT checkFrame() override;
    //添加当前帧缓存到缓存队列
    void addQImage(QImage* image);
    //从缓存队列头获取帧缓存的一个缓存帧
    std::shared_ptr<QImage> getQImage() override;
    WAITABLE_HANDLE getWaitableHandle() override;
};

#endif // KINECTOBJ_H
