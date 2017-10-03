#ifndef SQUIRCLERENDERER_H
#define SQUIRCLERENDERER_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QQuickWindow>
#include <QOpenGLTexture>
#include <QImage>

class SquircleRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit SquircleRenderer(): m_t(0), m_program(0) { }
    ~SquircleRenderer();

    void setT(qreal t) { m_t = t; }
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }

signals:

public slots:
    void init();
    void paint();

protected:
    void initTexture();

private:
//    QOpenGLTexture* mTexture;
    QOpenGLTexture* mTextures[5];
    QSize m_viewportSize;
    qreal m_t;
    QOpenGLShaderProgram *m_program;
    QQuickWindow *m_window;
    QImage* mpImage;
};

#endif // SQUIRCLERENDERER_H
