#ifndef KINECTRENDER_H
#define KINECTRENDER_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QSize>
#include <QOpenGLShaderProgram>
#include <QMutex>
#include <memory>

class QQuickWindow;

class KinectRender : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit KinectRender(QObject *parent = nullptr);
    ~KinectRender();

    void setViewportSize(const QSize& size);
    void setWindow(QQuickWindow* pWin);
    void setCurImage(std::shared_ptr<QImage> pImage);
    void setCurTexture(QOpenGLTexture* curTexture);

signals:

public slots:
    void sltInit();
    void sltPaint();

protected:
    std::shared_ptr<QImage> mpCurImage;
    QOpenGLTexture* getCurTexture();
    QMutex mMutex;
    QOpenGLTexture* mCurTexture;
    QSize mViewportSize;
    QQuickWindow* mpWindow;
    QOpenGLShaderProgram* mProgram;
};

#endif // KINECTRENDER_H
