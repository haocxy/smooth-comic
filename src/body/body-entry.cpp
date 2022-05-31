#include "body-entry.h"

#include <iostream>
#include <QApplication>

#include "core/logger.h"
#include "engine/engine.h"
#include "gui/book-viewer-widget.h"

#include "util/actor-demo.h"


using namespace logger::global_loggers;


namespace myapp {

int body_entry(int argc, char *argv[])
{
    logger::control::Option opt;
    opt.setAlwaysFlush(true);
    opt.setLevel(logger::Level::All);
    opt.setWriteToStdout(true);

    logger::control::init(opt);

    logInfo << "body_entry()";

    demo::actor::actor_demo_01();


    return 0;
    QApplication app(argc, argv);

    Engine engine;

    BookViewerWidget w(engine);
    w.show();

    return QApplication::exec();
}

}
