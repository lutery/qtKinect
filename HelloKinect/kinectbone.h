#ifndef KINECTBONE_H
#define KINECTBONE_H

#include <kinectobj.h>
#include <QPointF>
#include <QColor>

/**
 * @brief The BodyInfo struct
 *
 * 身体节点信息结构体
 */
struct BodyInfo
{
    bool ok = false;
    Joint joints[JointType_Count];
    QPointF jointPoints[JointType_Count];
    HandState leftHandState = HandState_Unknown;
    HandState rightHandState = HandState_Unknown;
};

/**
 * @brief The KinectBone class
 *
 * 身体骨骼动画类
 */
class KinectBone : public KinectObj
{
    Q_OBJECT
public:
    explicit KinectBone(IKinectSensor* pKinect = nullptr, QObject *parent = 0);
    virtual ~KinectBone();

signals:

public slots:

private:
    //深度缓存宽度高度
    const int DEPTH_WIDTH = 512;
    const int DEPTH_HEIGHT = 424;
    //
    const int WNDWIDTH = 1024;
    const int WNDHEIGHT = 768;
    const float c_JointThickness = 3.0f;
    const float c_TrackedBoneThickness = 6.0f;
    const float c_InferredBoneThickness = 1.0f;
    const float c_HandSize = 30.0f;

    IBodyFrameReader* mpBodyFrameReader = nullptr;
    WAITABLE_HANDLE mhBodyFrameArrived = NULL;
    ICoordinateMapper* mpCoordinateMapper = nullptr;
    BodyInfo mBodyInfo[BODY_COUNT];

    QColor mWhiteColor;
    QColor mJointTrackedColor;
    QColor mJointInferredColor;
    QColor mBoneTrackedColor;
    QColor mBoneInferredColor;
    QColor mHandClosedColor;
    QColor mHandOpenColor;
    QColor mHandLassoColor;
    std::shared_ptr<QPainter> mpRenderTarget;

private:
    HRESULT initKinect();
    QPointF BodyToScreen(const CameraSpacePoint& bodyPoint, int width, int height);
    void SetBodyInfo(int i, BodyInfo* info);
    void InitDrawResource();
    void DrawBoneImage();
    void DrawBody(const Joint* pJoints, const QPointF* pJointPoints);
    void DrawBone(const Joint* pJoints, const QPointF* pJointPoints, JointType joint0, JointType joint1);
    void DrawHand(HandState handState, const QPointF& handPosition);

public:
    HRESULT checkFrame() override;
    WAITABLE_HANDLE getWaitableHandle() override;
    bool update() override;
};

#endif // KINECTBONE_H
