#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
//#include "Kinect/inc/Kinect.h"
//#include "Kinect.h"
//#include <Kinect.h>
#include "stdinc.h"

//class KinectObj;
//class KinectThread;
class KinectWidget;
class QPushButton;

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
//    void updateUI();
    void openKinect();
    void closeKinect();
    void switchTime();

private:
    Ui::Dialog *ui;
//    KinectObj* pKinectObj = nullptr;
//    KinectThread* pKinectThread = nullptr;
    KinectWidget* pKinectWidget = nullptr;
    QPushButton* pOpenPB = nullptr;
    QPushButton* pClosePB = nullptr;
    QPushButton* pShowTimePB = nullptr;

private:
    void initView();
    void initEvent();

protected:
//    void paintEvent(QPaintEvent* event) override;
};

#endif // DIALOG_H
