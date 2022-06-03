#pragma once

#include "util/qobject-actor.h"


namespace myapp {

class ImgCache : public QObjectActor {
    Q_OBJECT
public:
    explicit ImgCache(QObject *parent = 0);

    virtual ~ImgCache() {}
};

}
