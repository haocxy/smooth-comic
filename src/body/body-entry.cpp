#include "body-entry.h"

#include <QApplication>


#include "artboard-widget.h"



namespace myapp {

int body_entry(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ArtboardWidget w;
    w.show();

    return QApplication::exec();
}

}
