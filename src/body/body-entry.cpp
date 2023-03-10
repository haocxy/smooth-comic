#include "body-entry.h"

#include <iostream>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFontDatabase>
#include <QTranslator>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQuickWindow>
#include <QQmlContext>

#include "core/logger.h"
#include "core/thread.h"
#include "core/system.h"
#include "core/debug-option/option-center.h"

#include "engine/path-manager.h"

#include "gui/register-qt-meta-types.h"
#include "gui/app-window.h"

#include "gui/gui-engine.h"
#include "gui/app-qml-engine.h"
#include "gui/app-quick-view.h"
#include "gui/app-quick-window.h"


class QtResourceSystemIniter {
public:
    QtResourceSystemIniter() {
        Q_INIT_RESOURCE(resources);
        Q_INIT_RESOURCE(qml);
        Q_INIT_RESOURCE(icon);
    }

    ~QtResourceSystemIniter() {
        Q_CLEANUP_RESOURCE(resources);
        Q_CLEANUP_RESOURCE(qml);
        Q_CLEANUP_RESOURCE(icon);
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

    qDebug() << "default open file dir: " << QString::fromStdU32String(SystemUtil::defaultOpenFileDir().generic_u32string());

    QFile fontFile(":/styles/MaterialIcons-Regular.ttf");
    fontFile.open(QIODevice::ReadOnly);
    QByteArray fontData = fontFile.readAll();
    qDebug() << "fontData size: " << fontData.size();
    QFontDatabase::addApplicationFontFromData(fontData);

    if (SystemUtil::platformType == PlatformType::Windows) {
        // ?????????Windows??????????????????????????????????????????????????????????????????
        QQuickWindow::setGraphicsApi(QSGRendererInterface::GraphicsApi::Software);
    }

    registerQtMetaTypes();

    QTranslator translator;
    if (translator.load(QLocale(), "trans", ".", ":/trans")) {
        QCoreApplication::installTranslator(&translator);
    }

    GuiEngine guiEngine;
    AppQmlEngine appQmlEngine(guiEngine);

    uptr<AppQuickWindow> appQuickWindow;
    uptr<AppQuickView> appQuickView;
    if (SystemUtil::isWindowedPlatform()) {
        appQuickWindow = new AppQuickWindow(appQmlEngine);
        appQuickWindow->show();
    } else {
        appQuickView = new AppQuickView(appQmlEngine);
        appQuickView->show();
    }

    const QString cssPath = ":/styles/default.css";



    qDebug() << "after addApplicationFontFromData";

    qApp->setStyleSheet(contentOf(cssPath));

    qDebug() << "after setStyleSheet";

    uptr<AppWindow> w;
    if (false) {
        w = new AppWindow(guiEngine);
        w->resize(800, 600);
        w->show();
    }

    return QApplication::exec();
}

}
