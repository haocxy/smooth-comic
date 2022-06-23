#include "body-entry.h"

#include <iostream>
#include <QApplication>

#include "core/logger.h"
#include "core/thread.h"
#include "core/debug-option/option-center.h"

#include "engine/engine.h"
#include "gui/book-viewer.h"

#include "register-qt-meta-types.h"


namespace myapp {

using logger::gLogger;

int body_entry(int argc, char *argv[])
{
    ThreadUtil::setNameForCurrentThread("GUI");

    logger::control::Option opt;
    opt.setAlwaysFlush(true);
    opt.setWriteToStdout(true);

    logger::control::init(opt);

    gLogger.setLevel(logger::Level::All);

    gLogger.i << "body_entry()";

    debug_option::OptionCenter &debugOptionCenter = debug_option::OptionCenter::instance();

    //debugOptionCenter.set("need-page-loaded-log", "0", debug_option::OptionSource::CmdLine);

    debugOptionCenter.publishAllOptions();

    QApplication app(argc, argv);

    registerQtMetaTypes();

    Engine engine;

    BookViewer w(engine);
    w.show();

    return QApplication::exec();
}

}
