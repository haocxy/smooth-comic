#include "register-qt-meta-types.h"

#include <QQmlEngine>

#include "gui-util/qtobj-strand-entry.h"
#include "gui/book/gui-book.h"
#include "gui/file-chooser/file-chooser.h"


namespace myapp {

void myapp::registerQtMetaTypes()
{
    qRegisterMetaType<MyApp_TaskWrapperForQtPlatform>();

    qmlRegisterType<GuiBook>("myapp", 1, 0, "Book");

    qmlRegisterType<FileTree>("myapp", 1, 0, "FileTree");
}

}
