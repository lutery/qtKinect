#ifndef KINECTFRAMEPROTOCOL_H
#define KINECTFRAMEPROTOCOL_H

#include <QImage>
#include <memory>
#include "stdinc.h"

class KinectFrameProtocol
{
public:
    virtual WAITABLE_HANDLE getWaitableHandle() = 0;
    virtual HRESULT checkFrame() = 0;
    virtual std::shared_ptr<QImage> getQImage() = 0;
    virtual bool update() = 0;
};

#endif // KINECTFRAMEPROTOCOL_H
