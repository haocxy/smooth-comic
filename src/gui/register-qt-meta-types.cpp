#include "register-qt-meta-types.h"

#include "util/qtobj-strand-entry.h"


namespace myapp {

void myapp::registerQtMetaTypes()
{
    qRegisterMetaType<MyApp_TaskWrapperForQtPlatform>();
}

}
