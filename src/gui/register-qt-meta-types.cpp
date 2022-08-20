#include "register-qt-meta-types.h"

#include <QQmlEngine>

#include "gui-util/qtobj-strand-entry.h"
#include "gui/book/gui-book.h"
#include "gui/file-chooser/file-chooser.h"
#include "gui/controller/controller.h"
#include "gui/book-area/page-area/page-reader-quick-item.h"


namespace myapp {

void myapp::registerQtMetaTypes()
{
    qRegisterMetaType<MyApp_TaskWrapperForQtPlatform>();

    qmlRegisterType<GuiBook>("myapp", 1, 0, "Book");

    qmlRegisterType<FileChooser>("myapp", 1, 0, "FileChooser");

    qmlRegisterType<PageReaderQuickItem>("myapp", 1, 0, "PageReader");

    qmlRegisterType<Controller>("myapp", 1, 0, "Controller");
}

}
