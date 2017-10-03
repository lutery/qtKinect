//#include "dialog.h"
//#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "kinectitem.h"
#include "squircle.h"

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    Dialog w;
//    w.show();

//    return a.exec();
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<KinectItem>("KinectItemQML", 1, 0, "KinectItem");
    qmlRegisterType<Squircle>("OpenGLUnderQML", 1, 0, "Squircle");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
