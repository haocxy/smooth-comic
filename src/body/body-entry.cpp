#include "body-entry.h"

#include <iostream>
#include <QApplication>

#include "core/logger.h"
#include "core/thread.h"
#include "engine/engine.h"
#include "gui/book-viewer.h"


using namespace logger::global_loggers;


namespace myapp {

int body_entry(int argc, char *argv[])
{
    ThreadUtil::setNameForCurrentThread("GUI");

    logger::control::Option opt;
    opt.setAlwaysFlush(true);
    opt.setLevel(logger::Level::All);
    opt.setWriteToStdout(true);

    logger::control::init(opt);

    logInfo << "body_entry()";

    QApplication app(argc, argv);

    Engine engine;

    BookViewer w(engine);
    w.show();

    return QApplication::exec();
}

}
