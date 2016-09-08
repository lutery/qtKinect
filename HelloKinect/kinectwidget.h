#ifndef KINECTWIDGET_H
#define KINECTWIDGET_H

#include <QWidget>
#include "stdinc.h"

class KinectObj;
class KinectThread;
class QTime;
class QPainter;
class QPoint;
class QRect;

class KinectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KinectWidget(QWidget *parent = 0);
    virtual ~KinectWidget();

signals:

public slots:
    void updateUI();

public:
    bool mbShowTime = true;

private:
    KinectObj* pKinectObj = nullptr;
    KinectThread* pKinectThread = nullptr;
    QTime* mpTime = nullptr;
    QRect* mpDrawTimeRC = nullptr;

protected:
    void paintEvent(QPaintEvent *event) override;
    void paintTime(QPainter& painter);

public:
    void openKinect();
    void closeKinect();
};

#endif // KINECTWIDGET_H
