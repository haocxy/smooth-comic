#include "register-qt-meta-types.h"

#include "util/qobject-actor.h"


namespace myapp {

void myapp::registerQtMetaTypes()
{
    qRegisterMetaType<MyApp_ActorEventWrapperForQtMetaObjSys>();
}

}
