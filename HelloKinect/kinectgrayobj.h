#ifndef KINECTGRAYOBJ_H
#define KINECTGRAYOBJ_H

#include <kinectobj.h>

class KinectGrayObj : public KinectObj
{
    Q_OBJECT
public:
    explicit KinectGrayObj(IKinectSensor* pKinect = nullptr, QObject *parent = 0);
    virtual ~KinectGrayObj();

signals:

private:
    IInfraredFrameReader* mpFraredFrameReader;
    //颜色帧RGB缓存
    RGBQUAD* mpColorRGBX;
    unsigned int mBufferSize = 0;
    WAITABLE_HANDLE mhFraredFrameArrived;

private:
    HRESULT initKinect() override;

public slots:
    HRESULT checkFrame() override;
    WAITABLE_HANDLE getWaitableHandle() override;
    bool update() override;
};

#endif // KINECTGRAYOBJ_H
