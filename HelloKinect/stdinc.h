#ifndef STDINC_H
#define STDINC_H

#include <Kinect.h>

template<class Interface>
inline void SafeRelease(Interface* &pInterfaceToRelease)
{
    if (pInterfaceToRelease != nullptr)
    {
        pInterfaceToRelease->Release();
    }
}

#endif // STDINC_H
