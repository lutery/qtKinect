/*********************************************************
* Copyright: chenghui
* Author: chenghui
* Date: 20160925
* Description:KinectObj实现从Kinect获取彩色帧的功能
*
**********************************************************/
#include <QDebug>
#include "kinectobj.h"

/**
 * @brief KinectObj::KinectObj 构造函数
 * @param parent 传入父类，当父类释放的时候，该类也会被释放
 */
KinectObj::KinectObj(IKinectSensor* pKinect, QObject *parent) : QObject(parent)
{
    mpKinect = pKinect;
    pMutex = new QMutex();
}

/**
 * @brief KinectObj::~KinectObj 析构函数，释放资源
 */
KinectObj::~KinectObj()
{
    if (mBufferImage.count() != 0)
    {
        mBufferImage.clear();
    }

    if (pMutex != nullptr)
    {
        pMutex->lock();
        pMutex->unlock();

        delete pMutex;
        pMutex = nullptr;
    }
}

/**
 * @brief KinectObj::myImageCleanupHandler QImage内存释放函数
 * @param info 传入相关的信息，这里传入的就是内存地址，只要把这个释放了就可以释放内存
 */
void KinectObj::myImageCleanupHandler(void *info)
{
    delete[] info;
}

void KinectObj::addQImage(QImage *image)
{
    pMutex->lock();
    mBufferImage.append(std::shared_ptr<QImage>(image));
    //这里有一个未解之谜，如果仅仅只是绘制在主DIALOG主对话框中得化
    //那么即使这里填写60，也不会有延时
    if (mBufferImage.count() > MAX_FRAME_COUNT)
    {
        mBufferImage.pop_front();
    }
    qDebug() << "BufferImage Size is " << mBufferImage.size();
    pMutex->unlock();
}

std::shared_ptr<QImage> KinectObj::getQImage()
{
    if (mBufferImage.count() <= 0)
    {
        return nullptr;
    }

    pMutex->lock();
    auto pImage = mBufferImage.first();
    mBufferImage.pop_front();
    pMutex->unlock();

    return pImage;
}
