#include "kinectthread.h"
#include <QDebug>
//#include "kinectobj.h"

KinectThread::KinectThread() : QThread()
{
    pKinectObj = nullptr;
    mbExit = false;
}

void KinectThread::stop()
{
    QMutexLocker locker(&mMutex);
    mbExit = true;
}

void KinectThread::run()
{
    while (!mbExit)
    {
        QMutexLocker locker(&mMutex);
        //判断KinectObj对象是否已经不为空
        if (pKinectObj != nullptr)
        {
            break;
        }
    }

    //创建颜色帧到达信号
    HANDLE events[] = {reinterpret_cast<HANDLE>(pKinectObj->getWaitableHandle())};

    while (!mbExit)
    {
        QMutexLocker locker(&mMutex);
        events[0] = reinterpret_cast<HANDLE>(pKinectObj->getWaitableHandle());

        //判断颜色帧是否已经到达
        switch (MsgWaitForMultipleObjects(1, events, FALSE, INFINITE, QS_ALLINPUT))
        {
        case WAIT_OBJECT_0 + 0:
            //颜色帧已经到达，那么就将到达的颜色帧
            HRESULT hr = pKinectObj->checkFrame();
            if (SUCCEEDED(hr))
            {
                //获取颜色帧成功，发出更新信号
                emit update();
            }
            break;
        }
    }

    qDebug() << "exit thread";
}
