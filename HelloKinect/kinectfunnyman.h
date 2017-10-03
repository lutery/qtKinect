#ifndef KINECTFUNNYMAN_H
#define KINECTFUNNYMAN_H

#include "kinectobj.h"

class QImage;

/**
 * @brief The KinectFunnyMan class 人脸跟随
 */
class KinectFunnyMan : public KinectObj
{
    Q_OBJECT
public:
    explicit KinectFunnyMan(IKinectSensor* pKinect, QObject *parent = 0);
    virtual ~KinectFunnyMan();

signals:

public slots:

private:
    bool mbExit = false;
    float mFunnyPosX = 0.f;
    float mFunnyPosY = 0.f;
    float mFunnyZoom = 0.f;
    ICoordinateMapper* mpCoordinateMapper = nullptr;
    IColorFrameReader* mpColorFrameReader = nullptr;
    IBodyFrameReader*  mpBodyFrameReader = nullptr;
    RGBQUAD* mpColorRGBX;
    unsigned int mBufferSize = 0;
    QImage* mpTempImage = nullptr;
    QImage* mpFunnyMan = nullptr;
    std::shared_ptr<QPainter> mpRenderTarget;
    bool mbReady = false;
    FunnyManState mMen[BODY_COUNT];

private:
    HRESULT initKinect();
    void checkColorFrame();
    void checkBodyFrame();
    void renderImage();

public:
    HRESULT checkFrame() override;
    WAITABLE_HANDLE getWaitableHandle() override;
    bool update() override;
};

#endif // KINECTFUNNYMAN_H
