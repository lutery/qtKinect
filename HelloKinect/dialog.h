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

/**
 * @brief The Dialog class 主对话框类
 */
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
//    void updateUI();
    //打开Kinect对象
    void openKinect();
    //关闭Kinect对象
    void closeKinect();
    //显示事件
    void switchTime();

private:
    Ui::Dialog *ui;
//    KinectObj* pKinectObj = nullptr;
//    KinectThread* pKinectThread = nullptr;
    //Kinect显示部件
    KinectWidget* pKinectWidget = nullptr;
    //打开按钮
    QPushButton* pOpenPB = nullptr;
    //关闭按钮
    QPushButton* pClosePB = nullptr;
    //显示事件按钮
    QPushButton* pShowTimePB = nullptr;

private:
    //初始化视图
    void initView();
    //初始化事件
    void initEvent();

protected:
//    void paintEvent(QPaintEvent* event) override;
};

#endif // DIALOG_H
