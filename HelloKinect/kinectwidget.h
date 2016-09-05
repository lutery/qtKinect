#ifndef KINECTWIDGET_H
#define KINECTWIDGET_H

#include <QWidget>
#include "stdinc.h"

class KinectObj;
class KinectThread;

class KinectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KinectWidget(QWidget *parent = 0);
    virtual ~KinectWidget();

signals:

public slots:
    void updateUI();

private:
    KinectObj* pKinectObj = nullptr;
    KinectThread* pKinectThread = nullptr;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // KINECTWIDGET_H
