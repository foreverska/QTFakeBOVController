#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "blowoffcontroller.h"
#include "twitchcontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    BlowoffController bovController(&engine);
    bovController.enumeratePorts();
    TwitchController twController(&engine);
    twController.setBovController(&bovController);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.rootContext()->setContextProperty("bovController", &bovController);
    engine.rootContext()->setContextProperty("twController", &twController);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
