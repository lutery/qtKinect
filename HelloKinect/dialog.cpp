#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
//#include "kinectobj.h"
//#include "kinectthread.h"
#include "kinectwidget.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

//    this->setFixedWidth(1920);
//    this->setFixedHeight(1080);
    this->resize(1920, 1080);

//    pKinectObj = new KinectObj();

//    pKinectThread = new KinectThread();
//    pKinectThread->pKinectObj = pKinectObj;

//    connect(pKinectThread, SIGNAL(update()), this, SLOT(updateUI()));

//    pKinectThread->start();
    this->initView();
}

Dialog::~Dialog()
{
    delete ui;

//    if (pKinectThread != nullptr)
//    {
//        if (pKinectThread->isRunning())
//        {
//            pKinectThread->exit();
//        }

//        delete pKinectThread;
//    }

//    if (pKinectObj != nullptr)
//    {
//        delete pKinectObj;
//    }
}

void Dialog::initView()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    QVBoxLayout* leftBarLayout = new QVBoxLayout(this);
    pOpenPB = new QPushButton(tr("打开Kinect"), this);
    pClosePB = new QPushButton(tr("关闭Kinect"), this);

    leftBarLayout->addWidget(pOpenPB);
    leftBarLayout->addWidget(pClosePB);
    leftBarLayout->addStretch(1);

    pKinectWidget = new KinectWidget(this);
    mainLayout->addLayout(leftBarLayout);
    mainLayout->addWidget(pKinectWidget, 1);

    this->setLayout(mainLayout);
}

//void Dialog::updateUI()
//{
//    this->update();
//}

//void Dialog::paintEvent(QPaintEvent *event)
//{
//    QDialog::paintEvent(event);

//    QPainter painter(this);
//    auto pImage = pKinectObj->getQImage();
//    if (pImage != nullptr)
//    {
//        painter.drawImage(0, 0, *pImage);
//    }
//}
