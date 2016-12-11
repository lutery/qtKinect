#ifndef KINECTFACTORY_H
#define KINECTFACTORY_H

#include <QObject>
#include <memory>
#include "kinectframeprotocol.h"

class KinectObj;
class KinectDepthObj;

/**
 * @brief The KinectFactory class KinectFactory工厂对象，用于维护Kinect资源
 */
class KinectFactory : public QObject
{
    Q_OBJECT
private:
    explicit KinectFactory(QObject *parent = 0);
    //内部初始化Kinect设备函数
    HRESULT initKinect();

public:
    //获取KinectFactory实例
    static KinectFactory* Instance();
    virtual ~KinectFactory();
    //获取颜色帧
    KinectFrameProtocol* getColorFrame();
    KinectFrameProtocol* getDepthFrame();
    //释放关闭颜色帧
    void closeColorFrame();
    void closeDepthFrame();
    //关闭Kinect对象
    void closeKinect();
    //关闭帧对象（通用）
    void closeFrameObj(KinectFrameProtocol** pFrameObj);

signals:

public slots:

private:
    //Kinect工厂
    static KinectFactory* pSelf;
    //判断Kinect设备是否初始化完成
    HRESULT mhrReady;
    //判断是否
    bool mOpened;
    //Kinect传感器对象
    IKinectSensor* mpKinect = nullptr;
    //颜色帧对象
    std::shared_ptr<KinectObj> mColorObj;
    std::shared_ptr<KinectDepthObj> mpDepthObj;
};

#endif // KINECTFACTORY_H
