#include "kinectthread.h"
#include "kinectobj.h"

KinectThread::KinectThread() : QThread()
{
    pKinectObj = nullptr;
}

void KinectThread::run()
{
    while (true)
    {
        if (pKinectObj != nullptr)
        {
            break;
        }
    }

    HANDLE events[] = {reinterpret_cast<HANDLE>(pKinectObj->mhColorFrameArrived)};

    while (true)
    {
        events[0] = reinterpret_cast<HANDLE>(pKinectObj->mhColorFrameArrived);

        switch (MsgWaitForMultipleObjects(1, events, FALSE, INFINITE, QS_ALLINPUT))
        {
        case WAIT_OBJECT_0 + 0:
            HRESULT hr = pKinectObj->checkColorFrame();
            if (SUCCEEDED(hr))
            {
                emit update();
            }
            break;
        }
    }
}
