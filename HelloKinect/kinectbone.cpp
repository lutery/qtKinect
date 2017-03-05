#include "kinectbone.h"
#include <QSize>
#include <QPainter>
#include <QDebug>

KinectBone::KinectBone(IKinectSensor* pKinect, QObject *parent) : KinectObj(pKinect, parent)
{
    this->mWhiteColor = QColor::fromRgb(255, 255, 255);
    this->mJointTrackedColor = QColor::fromRgb(69, 191, 69);
    this->mJointInferredColor = QColor::fromRgb(255, 255, 0);
    this->mBoneTrackedColor = QColor::fromRgb(0, 255, 0);
    this->mBoneInferredColor = QColor::fromRgb(189, 189, 189);
    this->mHandClosedColor = QColor::fromRgb(255, 0, 0, 128);
    this->mHandLassoColor = QColor::fromRgb(0, 0, 255, 128);
    this->mHandOpenColor = QColor::fromRgb(0, 255, 0, 128);

    this->initKinect();
}

KinectBone::~KinectBone()
{
    SafeRelease(this->mpCoordinateMapper);

    if (this->mhBodyFrameArrived && this->mpBodyFrameReader)
    {
        this->mpBodyFrameReader->UnsubscribeFrameArrived(this->mhBodyFrameArrived);
        this->mhBodyFrameArrived = NULL;
    }

    SafeRelease(this->mpBodyFrameReader);
}

HRESULT KinectBone::initKinect()
{
    IBodyFrameSource* pBodyFrameSource = nullptr;

    HRESULT hr = S_FALSE;

    if (this->mpKinect != nullptr)
    {
        hr = this->mpKinect->get_BodyFrameSource(&pBodyFrameSource);

        if (SUCCEEDED(hr))
        {
            hr = pBodyFrameSource->OpenReader(&this->mpBodyFrameReader);
        }

        if (SUCCEEDED(hr))
        {
            hr = this->mpBodyFrameReader->SubscribeFrameArrived(&this->mhBodyFrameArrived);
        }

        if (SUCCEEDED(hr))
        {
            hr = this->mpKinect->get_CoordinateMapper(&this->mpCoordinateMapper);
        }
    }

    SafeRelease(pBodyFrameSource);

    return hr;
}

QPointF KinectBone::BodyToScreen(const CameraSpacePoint &bodyPoint, int width, int height)
{
    DepthSpacePoint depthPoint = {0};
    this->mpCoordinateMapper->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);

    float screenPointX = static_cast<float>(depthPoint.X * width) / DEPTH_WIDTH;
    float screenPointY = static_cast<float>(depthPoint.Y * height) / DEPTH_HEIGHT;

    qDebug() << "screenPointX: " << screenPointX << ", screenPointY: " << screenPointY;

    return QPointF(screenPointX, screenPointY);
}

void KinectBone::SetBodyInfo(int i, BodyInfo *info)
{
    memcpy(this->mBodyInfo + i, info, sizeof(BodyInfo));
    this->mBodyInfo[i].ok = true;
}

void KinectBone::InitDrawResource()
{

}

void KinectBone::DrawBoneImage()
{
    this->InitDrawResource();

    QImage* image = new QImage(QSize(IMAGE_WIDTH, IMAGE_HEIGHT), QImage::Format_RGBX8888);
    this->mpRenderTarget = std::shared_ptr<QPainter>(new QPainter(image));

    for (int i = 0; i < BODY_COUNT; ++i)
    {
        if (!this->mBodyInfo[i].ok)
        {
            continue;
        }

        this->mBodyInfo[i].ok = false;
        this->DrawBody(this->mBodyInfo[i].joints, this->mBodyInfo[i].jointPoints);
        this->DrawHand(this->mBodyInfo[i].leftHandState, this->mBodyInfo[i].jointPoints[JointType_HandLeft]);
        this->DrawHand(this->mBodyInfo[i].rightHandState, this->mBodyInfo[i].jointPoints[JointType_HandRight]);
    }

    this->addQImage(image);
}

void KinectBone::DrawBody(const Joint *pJoints, const QPointF *pJointPoints)
{
    DrawBone(pJoints, pJointPoints, JointType_Head, JointType_Neck);
    DrawBone(pJoints, pJointPoints, JointType_Neck, JointType_SpineShoulder);
    DrawBone(pJoints, pJointPoints, JointType_SpineShoulder, JointType_SpineMid);
    DrawBone(pJoints, pJointPoints, JointType_SpineMid, JointType_SpineBase);
    DrawBone(pJoints, pJointPoints, JointType_SpineShoulder, JointType_ShoulderRight);
    DrawBone(pJoints, pJointPoints, JointType_SpineShoulder, JointType_ShoulderLeft);
    DrawBone(pJoints, pJointPoints, JointType_SpineBase, JointType_HipRight);
    DrawBone(pJoints, pJointPoints, JointType_SpineBase, JointType_HipLeft);

    DrawBone(pJoints, pJointPoints, JointType_ShoulderRight, JointType_ElbowRight);
    DrawBone(pJoints, pJointPoints, JointType_ElbowRight, JointType_WristRight);
    DrawBone(pJoints, pJointPoints, JointType_WristRight, JointType_HandRight);
    DrawBone(pJoints, pJointPoints, JointType_HandRight, JointType_HandTipRight);
    DrawBone(pJoints, pJointPoints, JointType_WristRight, JointType_ThumbRight);

    DrawBone(pJoints, pJointPoints, JointType_ShoulderLeft, JointType_ElbowLeft);
    DrawBone(pJoints, pJointPoints, JointType_ElbowLeft, JointType_WristLeft);
    DrawBone(pJoints, pJointPoints, JointType_WristLeft, JointType_HandLeft);
    DrawBone(pJoints, pJointPoints, JointType_HandLeft, JointType_HandTipLeft);
    DrawBone(pJoints, pJointPoints, JointType_WristLeft, JointType_ThumbLeft);

    DrawBone(pJoints, pJointPoints, JointType_HipRight, JointType_KneeRight);
    DrawBone(pJoints, pJointPoints, JointType_KneeRight, JointType_AnkleRight);
    DrawBone(pJoints, pJointPoints, JointType_AnkleRight, JointType_FootRight);

    DrawBone(pJoints, pJointPoints, JointType_HipLeft, JointType_KneeLeft);
    DrawBone(pJoints, pJointPoints, JointType_KneeLeft, JointType_AnkleLeft);
    DrawBone(pJoints, pJointPoints, JointType_AnkleLeft, JointType_FootLeft);

    for (int i = 0;  i < JointType_Count; ++i)
    {
        if (pJoints[i].TrackingState == TrackingState_Inferred)
        {
            this->mpRenderTarget->setBrush(this->mJointInferredColor);
            this->mpRenderTarget->drawEllipse(pJointPoints[i], c_JointThickness, c_JointThickness);
        }
        else if (pJoints[i].TrackingState == TrackingState_Tracked)
        {
            this->mpRenderTarget->setBrush(this->mJointTrackedColor);
            this->mpRenderTarget->drawEllipse(pJointPoints[i], c_JointThickness, c_JointThickness);
        }
    }
}

void KinectBone::DrawBone(const Joint *pJoints, const QPointF *pJointPoints, JointType joint0, JointType joint1)
{
    TrackingState joint0State = pJoints[joint0].TrackingState;
    TrackingState joint1State = pJoints[joint1].TrackingState;

    QPen tempPen;
    tempPen.setWidthF(this->c_TrackedBoneThickness);

    if ((joint0State == TrackingState_NotTracked) || (joint1State == TrackingState_NotTracked))
    {
        return;
    }

    if ((joint0State == TrackingState_Inferred) && (joint1State == TrackingState_Inferred))
    {
        return;
    }

    if ((joint0State == TrackingState_Tracked) && (joint1State == TrackingState_Tracked))
    {
        tempPen.setColor(this->mBoneTrackedColor);
        this->mpRenderTarget->setPen(tempPen);
        this->mpRenderTarget->drawLine(QLineF(pJointPoints[joint0], pJointPoints[joint1]));
    }
    else
    {
        tempPen.setColor(this->mBoneInferredColor);
        this->mpRenderTarget->setPen(tempPen);
        this->mpRenderTarget->drawLine(QLineF(pJointPoints[joint0], pJointPoints[joint1]));
    }
}

void KinectBone::DrawHand(HandState handState, const QPointF &handPosition)
{
    switch (handState)
    {
    case HandState_Closed:
        this->mpRenderTarget->setBrush(this->mHandClosedColor);
        this->mpRenderTarget->drawEllipse(handPosition, c_HandSize, c_HandSize);
        break;

    case HandState_Open:
        this->mpRenderTarget->setBrush(this->mHandOpenColor);
        this->mpRenderTarget->drawEllipse(handPosition, c_HandSize, c_HandSize);
        break;

    case HandState_Lasso:
        this->mpRenderTarget->setBrush(this->mHandLassoColor);
        this->mpRenderTarget->drawEllipse(handPosition, c_HandSize, c_HandSize);
        break;
    }
}

HRESULT KinectBone::checkFrame()
{
    if (this->mpBodyFrameReader == nullptr)
    {
        return S_FALSE;
    }

    IBodyFrameArrivedEventArgs* pArgs = nullptr;
    IBodyFrameReference* pBFrameRef = nullptr;
    IBodyFrame* pBodyFrame = nullptr;
    IBody* ppBody[BODY_COUNT] = {0};

    HRESULT hr = this->mpBodyFrameReader->GetFrameArrivedEventData(this->mhBodyFrameArrived, &pArgs);

    if (SUCCEEDED(hr))
    {
        hr = pArgs->get_FrameReference(&pBFrameRef);
    }

    if (SUCCEEDED(hr))
    {
        hr = pBFrameRef->AcquireFrame(&pBodyFrame);
    }

    if (SUCCEEDED(hr))
    {
        hr = pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, ppBody);
    }

    if (SUCCEEDED(hr))
    {
        for (int i = 0; i < BODY_COUNT; ++i)
        {
            BOOLEAN bTracked = false;
            hr = ppBody[i]->get_IsTracked(&bTracked);
            if (!ppBody[i] || FAILED(hr) || !bTracked)
            {
                continue;
            }

            IBody* pNowBody = ppBody[i];
            BodyInfo info;
            
            pNowBody->get_HandLeftState(&info.leftHandState);
            pNowBody->get_HandRightState(&info.rightHandState);

            hr = pNowBody->GetJoints(JointType_Count, info.joints);

            if (SUCCEEDED(hr))
            {
                for (int j = 0; j < JointType_Count; ++j)
                {
                    info.jointPoints[i] = BodyToScreen(info.joints[j].Position, WNDWIDTH, WNDHEIGHT);
                    this->SetBodyInfo(i, &info);
                }
            }
        }
    }

    //开始绘制图像并保存到队列当中
    if (SUCCEEDED(hr))
    {
        this->DrawBoneImage();
    }

    for (int i = 0; i < BODY_COUNT; ++i)
    {
        SafeRelease(ppBody[i]);
    }

    SafeRelease(pBodyFrame);
    SafeRelease(pBFrameRef);
    SafeRelease(pArgs);
}

WAITABLE_HANDLE KinectBone::getWaitableHandle()
{
    return this->mhBodyFrameArrived;
}

bool KinectBone::update()
{
    return false;
}
