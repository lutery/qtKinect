#ifndef KINECTITEM_H
#define KINECTITEM_H

#include <QQuickItem>
#include <memory>
#include <QImage>
#include "kinectframeprotocol.h"

class KinectRender;
class QQuickWindow;
class QOpenGLTexture;
class KinectThread;

class KinectItem : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(OperaId)

public:
    KinectItem();
    virtual ~KinectItem();

    enum OperaId
    {
        Open = 0,
        Color = 1,
        Depth = 2,
        Frared = 3,
        Bodyindex = 4,
        Greenscreen = 5,
        Bone = 6,
        Funnyman = 7,
        Close = 20
    };

signals:

public slots:
    void sync();
    void cleanup();
    void sltTexture(QOpenGLTexture* texture);
    void sltQImage(std::shared_ptr<QImage>);
    void sltUpdate();
    void sltOperaKinect(OperaId operaid);

private slots:
    void handleWindowChanged(QQuickWindow* win);
    void closeKinect();

private:
//    std::shared_ptr<QImage> mpCurImage = nullptr;
    KinectRender* mpRender = nullptr;
    KinectFrameProtocol* mpKinectObj = nullptr;
    KinectThread* mpKinectThread = nullptr;
};

#endif // KINECTITEM_H
