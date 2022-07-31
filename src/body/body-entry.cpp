#include "body-entry.h"

#include <iostream>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QFontDatabase>
#include <QTranslator>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include "core/logger.h"
#include "core/thread.h"
#include "core/system.h"
#include "core/debug-option/option-center.h"

#include "engine/engine.h"
#include "engine/path-manager.h"

#include "gui/register-qt-meta-types.h"
#include "gui/app-window.h"


class QtResourceSystemIniter {
public:
    QtResourceSystemIniter() {
        Q_INIT_RESOURCE(resources);
        Q_INIT_RESOURCE(qml);
    }

    ~QtResourceSystemIniter() {
        Q_CLEANUP_RESOURCE(resources);
        Q_CLEANUP_RESOURCE(qml);
    }
};

static const QtResourceSystemIniter gQtResourceSystemIniter;

namespace myapp {

using logger::gLogger;

static void initDebugOptions(const std::map<std::string, std::string> &rawDebugOptions)
{
    debug_option::OptionCenter &center = debug_option::OptionCenter::instance();

    for (const auto &pair : rawDebugOptions) {
        center.set(pair.first, pair.second, debug_option::OptionSource::CmdLine);
    }

    center.publishAllOptions();
}

static void listDebugOptions(std::ostream &out)
{
    debug_option::OptionCenter &center = debug_option::OptionCenter::instance();

    const std::string deco = "    ";

    out << "Debug options:" << std::endl;

    center.eachOption([&deco, &out](const std::string &name, const debug_option::OptionInfo &optionInfo) {
        out << deco << name << " [" << optionInfo.type << "]";
        if (!optionInfo.desc.empty()) {
            out << " " << optionInfo.desc;
        }
        out << std::endl;
    });
}

static QString contentOf(QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    return file.readAll();
}

int body_entry(int argc, char *argv[])
{
    ThreadUtil::setNameForCurrentThread("GUI");

    // init default global logger
    logger::control::Option opt;
    opt.setAlwaysFlush(true);
    opt.setWriteToStdout(true);

    logger::control::init(opt);
    gLogger.setLevel(logger::Level::All);
    gLogger.d << "body_entry() after init logger";


    //// parse command line
    //CmdOption cmdOption(argc, argv);
    //if (cmdOption.hasError()) {
    //    std::cerr << "Cannot parse command line because:" << cmdOption.errMsg() << std::endl;
    //    cmdOption.showHelp(std::cerr);
    //    return 1;
    //}

    //if (cmdOption.needHelp()) {
    //    cmdOption.showHelp(std::cout);
    //    return 0;
    //}

    //if (cmdOption.needListDebugOptions()) {
    //    listDebugOptions(std::cout);
    //    return 0;
    //}

    gLogger.d << "body_entry() after parse command line";


    // init debug options
    //initDebugOptions(cmdOption.debugOptions());


    gLogger.d << "body_entry() after init debug options";

    logger::gLogger.d << "userHome ===> : " << SystemUtil::userHome();

    QApplication app(argc, argv);

    if (SystemType::IsWindows) {
        // 解决在Windows平台调整窗口大小时窗口内容出现黑色底色的问题
        QQuickWindow::setGraphicsApi(QSGRendererInterface::GraphicsApi::OpenGL);
    }

    registerQtMetaTypes();

    QTranslator translator;
    if (translator.load(QLocale(), "trans", ".", ":/trans")) {
        QCoreApplication::installTranslator(&translator);
    }

    QQmlApplicationEngine qmlAppEngine;
    qmlAppEngine.load(QUrl("qrc:/MainWindow.qml"));

    Engine engine;

    const QString cssPath = ":/styles/default.css";

    QFile fontFile(":/styles/MaterialIcons-Regular.ttf");
    fontFile.open(QIODevice::ReadOnly);
    QByteArray fontData = fontFile.readAll();

    qDebug() << "fontData size: " << fontData.size();

    QFontDatabase::addApplicationFontFromData(fontData);

    qDebug() << "after addApplicationFontFromData";

    qApp->setStyleSheet(contentOf(cssPath));

    qDebug() << "after setStyleSheet";

    //AppWindow w(engine);

    //w.show();

    return QApplication::exec();
}

}
