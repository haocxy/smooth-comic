#include "book.h"


namespace myapp {

Book::Book(Engine &engine, QObject *parent)
    : QObject(parent)
    , mEngine(engine)
    , mLoader(engine) {

}

Book::~Book()
{
}

}
