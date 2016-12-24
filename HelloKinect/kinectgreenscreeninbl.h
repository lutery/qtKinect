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
    ICoordinateMapper* mpCoordinateMapper = nullptr;
    IMultiSourceFrameReader* mpMultiSourceFrameReader = nullptr;
    WAITABLE_HANDLE mhMultiSourceFrameArrived = NULL;
    ColorSpacePoint* mpColorCoordinates = nullptr;
    RGBQUAD* mpColorRGBX;
    unsigned int mBufferSize = 0;

private:
    HRESULT initKinect() override;

public:
    HRESULT checkFrame() override;
    WAITABLE_HANDLE getWaitableHandle() override;
};

#endif // KinectGreenScreenInBl_H
