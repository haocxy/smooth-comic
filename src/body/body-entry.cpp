#include "body-entry.h"

#include <QApplication>

#include "engine/engine.h"
#include "gui/book-viewer-widget.h"



namespace myapp {

int body_entry(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Engine engine;

    BookViewerWidget w(engine);
    w.show();

    return QApplication::exec();
}

}
