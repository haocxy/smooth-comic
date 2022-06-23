#pragma once


namespace myapp {

enum class BookOperationPriority {
    Init = 100,
    Gui = 99,
    Logic = 98,
    DeInit = 1,
};

}
