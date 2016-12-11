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

    //重置窗口的尺寸
    this->resize(1920, 1080);

    this->initView();
    this->initEvent();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::initView()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    QVBoxLayout* leftBarLayout = new QVBoxLayout(this);
    pOpenPB = new QPushButton(tr("打开Kinect"), this);
    pClosePB = new QPushButton(tr("关闭Kinect"), this);
    pShowTimePB = new QPushButton(tr("关闭时间"), this);
    pOpenDepthPB = new QPushButton(tr("DepthFrame"), this);

    leftBarLayout->addWidget(pOpenPB);
    leftBarLayout->addWidget(pOpenDepthPB);
    leftBarLayout->addWidget(pShowTimePB);
    leftBarLayout->addWidget(pClosePB);
    leftBarLayout->addStretch(1);

    pKinectWidget = new KinectWidget(this);
    mainLayout->addLayout(leftBarLayout);
    mainLayout->addWidget(pKinectWidget, 1);

    this->setLayout(mainLayout);
}

void Dialog::initEvent()
{
    connect(pOpenPB, SIGNAL(clicked(bool)), this, SLOT(openKinect()));
    connect(pOpenDepthPB, SIGNAL(clicked(bool)), this, SLOT(openDepthFrame()));
    connect(pClosePB, SIGNAL(clicked(bool)), this, SLOT(closeKinect()));
    connect(pShowTimePB, SIGNAL(clicked(bool)), this, SLOT(switchTime()));
}

void Dialog::openKinect()
{
    pKinectWidget->openKinect();
}

void Dialog::openDepthFrame()
{
    pKinectWidget->openDepthFrame();
}

void Dialog::closeKinect()
{
    pKinectWidget->closeKinect();
}

void Dialog::switchTime()
{
    pKinectWidget->mbShowTime = !(pKinectWidget->mbShowTime);

    if (pKinectWidget->mbShowTime)
    {
        pShowTimePB->setText(tr("关闭时间"));
    }
    else
    {
        pShowTimePB->setText(tr("打开时间"));
    }
}
