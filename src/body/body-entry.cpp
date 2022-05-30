#include "body-entry.h"

#include <iostream>
#include <QApplication>

#include "engine/engine.h"
#include "gui/book-viewer-widget.h"

#include "util/test-actor.h"


namespace myapp {

int body_entry(int argc, char *argv[])
{
    CalcActor calcActor;
    AskActor askActor(calcActor);

    while (true) {
        std::string line;
        std::getline(std::cin, line);
    }


    return 0;
    QApplication app(argc, argv);

    Engine engine;

    BookViewerWidget w(engine);
    w.show();

    return QApplication::exec();
}

}
