import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import KinectItemQML 1.0
import OpenGLUnderQML 1.0

ApplicationWindow {
    id: idMainWindow
    visible: true
    width: 1280
    height: 720

    color: Qt.rgba(0, 0, 1, 1)

//    KinectItem{
//    Squircle {
//            SequentialAnimation on t {
//                NumberAnimation { to: 4; duration: 250; easing.type: Easing.InQuad }
//                NumberAnimation { to: 0; duration: 250; easing.type: Easing.OutQuad }
//                loops: Animation.Infinite
//                running: true
//            }
//        }

    RowLayout {
        anchors.fill: parent

        Rectangle{
            Layout.fillHeight: true
//            Layout.fillWidth: true
            width: 120
            color: Qt.rgba(1, 0, 1, 1)

            ColumnLayout {
                anchors.fill: parent

                Button{
                    text: "打开Kinect"
                    Layout.alignment: Qt.AlignCenter
                    onClicked: {
                        kinect.sltOperaKinect(KinectItem.Open);
                    }
                }

                Button{
                    text: "彩色帧"
                    Layout.alignment: Qt.AlignCenter
                    onClicked: {
                        kinect.sltOperaKinect(KinectItem.Color);
                    }
                }

                Button{
                    text: "深度帧"
                    Layout.alignment: Qt.AlignCenter
                    onClicked: {
                        kinect.sltOperaKinect(KinectItem.Depth);
                    }
                }

                Button{
                    text:"Frared帧"
                    Layout.alignment: Qt.AlignCenter
                    onClicked: {
                        kinect.sltOperaKinect(KinectItem.Frared);
                    }
                }

                Button{
                    text:"身体索引帧"
                    Layout.alignment: Qt.AlignCenter
                    onClicked: {
                        kinect.sltOperaKinect(KinectItem.Bodyindex);
                    }
                }

                Button{
                    text:"人脸跟随"
                    Layout.alignment: Qt.AlignCenter
                    onClicked: {
                        kinect.sltOperaKinect(KinectItem.Funnyman);
                    }
                }

                Button{
                    text:"绿屏裁剪"
                    Layout.alignment: Qt.AlignCenter
                    onClicked: {
                        kinect.sltOperaKinect(KinectItem.Greenscreen);
                    }
                }

                Button{
                    text:"骨骼索引"
                    Layout.alignment: Qt.AlignCenter
                    onClicked: {
                        kinect.sltOperaKinect(KinectItem.Bone);
                    }
                }

                Button{
                    text:"关闭Kinect"
                    Layout.alignment: Qt.AlignCenter
                    onClicked: {
                        kinect.sltOperaKinect(KinectItem.Close);
                    }
                }
            }
        }

        Rectangle{
            Layout.fillHeight: true

            color:Qt.rgba(1, 0, 0, 1)

            KinectItem{
                id:kinect

                anchors.fill: parent
            }
        }
    }
}
