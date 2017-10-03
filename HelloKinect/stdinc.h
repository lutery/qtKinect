#ifndef STDINC_H
#define STDINC_H

#include <Kinect.h>
#include <QPointF>

template<class Interface>
inline void SafeRelease(Interface* &pInterfaceToRelease)
{
    if (pInterfaceToRelease != nullptr)
    {
        pInterfaceToRelease->Release();
    }
}

enum class FunnyMan
{
    Offline = 0,
    Online
};

struct FunnyManState
{
    FunnyMan man = FunnyMan::Offline;
    float angle = 0.f;
    float zoom = 1.f;
    QPointF pos = QPointF(0, 0);
    __forceinline void update()
    {
        angle -= 2.f;
        if (angle < 0.f)
        {
            angle += 360.f;
        }
    }
};

#endif // STDINC_H
