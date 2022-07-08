#include "controller.h"

#include "book/book.h"


namespace myapp {

Controller::Controller(Engine &engine, QObject *parent)
    : QObject(parent)
{
    book_ = new Book(engine);
}

Controller::~Controller()
{
}

}
