#ifndef KINECTFACTORY_H
#define KINECTFACTORY_H

#include <QObject>
#include <memory>
#include "kinectframeprotocol.h"

class KinectObj;

class KinectFactory : public QObject
{
    Q_OBJECT
private:
    explicit KinectFactory(QObject *parent = 0);
    HRESULT initKinect();

public:
    static KinectFactory* Instance();
    virtual ~KinectFactory();
    KinectFrameProtocol* getColorFrame();
    void closeColorFrame();
    void closeKinect();
    void closeFrameObj(KinectFrameProtocol** pFrameObj);

signals:

public slots:

private:
    static KinectFactory* pSelf;
    HRESULT mhrReady;
    bool mOpened;
    //Kinect传感器对象
    IKinectSensor* mpKinect = nullptr;
    std::shared_ptr<KinectObj> mColorObj;
};

#endif // KINECTFACTORY_H
