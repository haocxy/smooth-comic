#include "body-entry.h"

#include <QApplication>


#include "gui/book-viewer-widget.h"



namespace myapp {

int body_entry(int argc, char *argv[])
{
    QApplication app(argc, argv);

    BookViewerWidget w;
    w.show();

    return QApplication::exec();
}

}
