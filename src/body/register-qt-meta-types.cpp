#include "register-qt-meta-types.h"

#include "util/qobject-actor.h"
#include "util/qtobj-strand-entry.h"


namespace myapp {

void myapp::registerQtMetaTypes()
{
    qRegisterMetaType<MyApp_ActorEventWrapperForQtMetaObjSys>();
    qRegisterMetaType<MyApp_TaskWrapperForQtPlatform>();
}

}
