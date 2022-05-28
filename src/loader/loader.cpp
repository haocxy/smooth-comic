#include "loader.h"

#include <QApplication>
#include <QWidget>

namespace myapp {

int load(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget widget;
    widget.setWindowTitle("Hello, smooth-comic");
    widget.show();

    return QApplication::exec();
}

}
