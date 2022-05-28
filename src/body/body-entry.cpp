#include "body-entry.h"

#include <QApplication>


#include "telescope-widget.h"



namespace myapp {

int body_entry(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TelescopeWidget w;
    w.show();

    return QApplication::exec();
}

}
