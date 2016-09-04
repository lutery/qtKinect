#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
//#include "Kinect/inc/Kinect.h"
//#include "Kinect.h"
//#include <Kinect.h>
#include "stdinc.h"

class KinectObj;
class KinectThread;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void updateUI();

private:
    Ui::Dialog *ui;
    KinectObj* pKinectObj = nullptr;
    KinectThread* pKinectThread = nullptr;
//    const int IMAGE_WIDTH = 1920;
//    const int IMAGE_HEIGHT = 1080;
//    IKinectSensor* mpKinect;
//    IColorFrameReader* mpColorFrameReader;
//    WAITABLE_HANDLE mhColorFrameArrived;
//    RGBQUAD* mpColorRGBX;
//    unsigned int nBufferSize;

private:
//    HRESULT initKinect();
//    void checkColorFrame();

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // DIALOG_H
