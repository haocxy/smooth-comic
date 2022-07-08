#include "register-qt-meta-types.h"

#include "gui-util/qtobj-strand-entry.h"


namespace myapp {

void myapp::registerQtMetaTypes()
{
    qRegisterMetaType<MyApp_TaskWrapperForQtPlatform>();
}

}
