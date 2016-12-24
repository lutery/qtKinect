#ifndef KINECTDEPTHOBJ_H
#define KINECTDEPTHOBJ_H

#include <kinectobj.h>

/**
 * @brief The KinectDepthObj class Kinect深度帧对象
 */
class KinectDepthObj : public KinectObj
{
    Q_OBJECT
public:
    explicit KinectDepthObj(IKinectSensor* pKinect = nullptr, QObject *parent = 0);
    virtual ~KinectDepthObj();

signals:

public slots:

private:
    //颜色帧阅读器
    IDepthFrameReader* mpDepthFrameReader = nullptr;
    //颜色帧RGB缓存
    RGBQUAD* mpColorRGBX;
    //颜色帧缓存的长度
    unsigned int mBufferSize = 0;
    //可等待的颜色帧到达事件句柄
    WAITABLE_HANDLE mhDepthFrameArrived;

private:
    HRESULT initKinect();

public:
    HRESULT checkFrame() override;
    WAITABLE_HANDLE getWaitableHandle() override;
};

#endif // KINECTDEPTHOBJ_H
