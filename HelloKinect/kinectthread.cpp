#include "kinectthread.h"
#include <QDebug>
#include <QTime>
//#include "kinectobj.h"

KinectThread::KinectThread() : QThread()
{
    mpKinectObj = nullptr;
    mbExit = false;
}

void KinectThread::stop()
{
    QMutexLocker locker(&mMutex);
    mbExit = true;
}

void KinectThread::setKinectObj(KinectFrameProtocol *pKinectObj)
{
    QMutexLocker locker(&mMutex);
    mpKinectObj = pKinectObj;
}

void KinectThread::run()
{
    while (!mbExit)
    {
        QMutexLocker locker(&mMutex);
        //判断KinectObj对象是否已经不为空
        if (mpKinectObj != nullptr)
        {
            break;
        }
    }

    //创建颜色帧到达信号
    HANDLE events[] = {reinterpret_cast<HANDLE>(mpKinectObj->getWaitableHandle())};
    QTime timer;
    timer.start();

    while (!mbExit)
    {
        QMutexLocker locker(&mMutex);
        auto waitableHandle = mpKinectObj->getWaitableHandle();

        if (events != NULL && waitableHandle != NULL)
        {
            events[0] = reinterpret_cast<HANDLE>(waitableHandle);
            //判断颜色帧是否已经到达
            switch (MsgWaitForMultipleObjects(1, events, FALSE, INFINITE, QS_ALLINPUT))
            {
            case WAIT_OBJECT_0 + 0:
                //颜色帧已经到达，那么就将到达的颜色帧
                HRESULT hr = mpKinectObj->checkFrame();
                if (SUCCEEDED(hr))
                {
                    //获取颜色帧成功，发出更新信号
                    qDebug() << "emit update";
                    emit update();
//                    std::shared_ptr<QImage> curImage = mpKinectObj->getQImage();
//                    emit update(curImage);
                }
                break;
            }
        }
        else
        {
            mpKinectObj->update();
                int curTime = timer.elapsed();
                if (curTime > 34)
                {
                    qDebug() << "emit update";
                    emit update();
//                    std::shared_ptr<QImage> curImage = mpKinectObj->getQImage();
//                    emit update(curImage);

                    timer.restart();
                }
        }
    }

    qDebug() << "exit thread";
}
