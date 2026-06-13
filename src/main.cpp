#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "gamecontroller.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Material: Function
    GameController gameController;

    // Expose GameController to QML
    engine.rootContext()->setContextProperty("gameController", &gameController);

    const QUrl url(u"qrc:/AstroStrike/qml/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    
    engine.load(url);

    return app.exec();
}
