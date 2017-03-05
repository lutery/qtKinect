#ifndef KinectGreenScreenInBl_H
#define KinectGreenScreenInBl_H

#include <kinectobj.h>

class KinectGreenScreenInBl : public KinectObj
{
    Q_OBJECT
public:
    explicit KinectGreenScreenInBl(IKinectSensor* pKinect = nullptr, QObject *parent = 0);
    virtual ~KinectGreenScreenInBl();

signals:

public slots:

private:
    const int IMAGE_D_WIDTH = 512;
    const int IMAGE_D_HEIGHT = 424;

    //坐标映射
    ICoordinateMapper* mpCoordinateMapper = nullptr;
    //多源读取类型
    IMultiSourceFrameReader* mpMultiSourceFrameReader = nullptr;
    //多源读取帧到来事件
    WAITABLE_HANDLE mhMultiSourceFrameArrived = NULL;
    //坐标映射帧到来事件
    WAITABLE_HANDLE mhCoordinateMapperChanged = NULL;
    //颜色坐标控件映射点
    ColorSpacePoint* mpColorCoordinates = nullptr;
    //颜色缓存
    RGBQUAD* mpColorRGBX;
    //缓存大小
    unsigned int mBufferSize = 0;

private:
    HRESULT initKinect() override;

public:
    HRESULT checkFrame() override;
    WAITABLE_HANDLE getWaitableHandle() override;
    bool update() override;
};

#endif // KinectGreenScreenInBl_H
