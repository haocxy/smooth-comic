#include "async-deleter.h"


namespace myapp {

AsyncDeleter::AsyncDeleter()
    : thread_([this] { loop(); }) {
}

AsyncDeleter::~AsyncDeleter()
{
    taskQueue_.stop();
}

void AsyncDeleter::loop()
{
    ThreadUtil::setNameForCurrentThread("myapp.async-deleter");

    while (true) {
        std::optional<std::function<void()>> taskOpt = taskQueue_.pop();
        if (!taskOpt) {
            break;
        }
        std::function<void()> &task = *taskOpt;
        if (task) {
            task();
        }
    }
}

}
