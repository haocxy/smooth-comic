#include "controller.h"

namespace myapp {

Controller::Controller(Book &book, QObject *parent)
    : QObject(parent)
    , book_(book)
{
}

Controller::~Controller()
{
}

}
