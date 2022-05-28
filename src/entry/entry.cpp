#include <iostream>

#include <QApplication>
#include <QWidget>


int entry(int argc, char *argv[])
{
    std::cout << "Hello, smooth-comic" << std::endl;

    QApplication app(argc, argv);

    QWidget widget;
    widget.setWindowTitle("Hello, smooth-comic");
    widget.show();

    return QApplication::exec();
}
