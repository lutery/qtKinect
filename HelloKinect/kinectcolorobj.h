#ifndef KINECTCOLOROBJ_H
#define KINECTCOLOROBJ_H

#include "kinectobj.h"

class KinectColorObj : public KinectObj
{
    Q_OBJECT
public:
    explicit KinectColorObj(IKinectSensor* pKinect = nullptr, QObject *parent = 0);
    virtual ~KinectColorObj();

signals:

public slots:

private:
    //颜色帧读取对象
    IColorFrameReader* mpColorFrameReader;
    //颜色帧RGB缓存
    RGBQUAD* mpColorRGBX;
    //颜色帧缓存的长度
    unsigned int mBufferSize = 0;
    //可等待的颜色帧到达事件句柄
    WAITABLE_HANDLE mhColorFrameArrived;

private:
    //初始化Kinect对象
    HRESULT initKinect();

public:
    //获取当前帧缓存的RGB缓存
    HRESULT checkFrame() override;
    WAITABLE_HANDLE getWaitableHandle() override;
    bool update() override;
};

#endif // KINECTCOLOROBJ_H
