#ifndef KINECTBODYINDEX_H
#define KINECTBODYINDEX_H

#include <kinectobj.h>

class KinectBodyIndex : public KinectObj
{
    Q_OBJECT
public:
    explicit KinectBodyIndex(IKinectSensor* pKinect = nullptr, QObject *parent = 0);
    virtual ~KinectBodyIndex();

signals:

public slots:

private:
    // 玩家索引帧读取器
    IBodyIndexFrameReader*  mpBodyIndexFrameReader = nullptr;
    //颜色帧RGB缓存
    RGBQUAD* mpColorRGBX;
    //颜色帧缓存的长度
    unsigned int mBufferSize = 0;
    //可等待的颜色帧到达事件句柄
    WAITABLE_HANDLE mhBodyIndexFrameArrived;

private:
    HRESULT initKinect();

public:
    HRESULT checkFrame() override;
    WAITABLE_HANDLE getWaitableHandle() override;
};

#endif // KINECTBODYINDEX_H
