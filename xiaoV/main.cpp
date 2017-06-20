#include <QApplication>
//#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QScreen>
#include <QDebug>
#ifdef Q_OS_ANDROIO
#include <QtAndroidExtras>
#endif

//plugins
#include "webserver.h"

//robot
#include "robot.h"
#include "robot/camera/qmlcamera.h"
#include "robot/head/qmlrobothead.h"
#include "robot/feet/tmrobotwheel.h"
#include "qmlrobot.h"
#include "tmglobaltool.h"

static QString APP_LANG = QStringLiteral("ch");
static QString APP_NAME = QStringLiteral("XiaoV");
static QString APP_VERSION = QStringLiteral("5.0");

void initPlugins();
void initQmlStyle();
void initQmlEngine(QQmlEngine *engine);
void initRobotEngine(QQmlEngine *engine);

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qApp->setApplicationName(APP_NAME);
    qApp->setApplicationName("Tami Tech Cor.");
    qApp->setOrganizationName("Tami");
    qApp->setOrganizationDomain("Tamigroup.org");
    qApp->setApplicationVersion(APP_VERSION);

    initPlugins();

    QQuickView view;

    initQmlEngine(view.engine());

    initQmlStyle();

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/qml/mainV5.qml")));
    view.setFlags(Qt::FramelessWindowHint);
    view.setModality(Qt::WindowModal);
    view.show();

    return app.exec();
}

void initPlugins()
{
    qmlRegisterType<QmlCamera>("TmRobot",1,0,"RobotEye");
    qmlRegisterType<QmlRobot>("TmRobot",1,0,"Robot");
    qmlRegisterType<TmRobotWheel>("TmRobot",1,0,"RobotWheel");
}
void initQmlStyle()
{
    if (qgetenv("QT_QUICK_CONTROLS_STYLE").isEmpty()) {
        qputenv("QT_QUICK_CONTROLS_STYLE", "Flat");
    }
}

void initQmlEngine(QQmlEngine *engine)
{
    engine->setOfflineStoragePath(QString("./"));
    QObject::connect(engine, SIGNAL(quit()), qApp, SLOT(quit()));


    QString BSF_TEST_FLAG = "false" ;//"false";
    engine->rootContext()->setContextProperty("bsf_test_flag", BSF_TEST_FLAG);
    engine->rootContext()->setContextProperty("app_root_dir",qApp->applicationDirPath());
    engine->rootContext()->setContextProperty("g_rootDir",qApp->applicationDirPath());
    engine->rootContext()->setContextProperty("g_testFlag",qApp->applicationDirPath());



    //and golbal property dp
    QScreen *m_screen = qApp->primaryScreen();
    int m_dpi = m_screen->physicalDotsPerInch() * m_screen->devicePixelRatio();
    bool isMoble = false;

#if defined(Q_OS_IOS)
    //IOS integration of scaling (retina, non-retina, 4k) does itself.
    m_dpi = m_screen->physicalDotsPerInch();
    isMoble = true;
#endif

#if defined(Q_OS_ANDROID)
    // https://bugreports.qt-project.org/browse/QTBUG-35701
    // recalculate dpi for Android

#endif

    // now calculate the dp ratio
    qreal dp = m_dpi / 160.f;

    engine->rootContext()->setContextProperty("g_dp",dp);
    engine->rootContext()->setContextProperty("g_isMobile",isMoble);
}

void initRobotEngine(QQmlEngine *engine)
{
    Robot robot;
    engine->rootContext()->setContextObject(&robot);

}
