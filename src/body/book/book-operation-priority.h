#pragma once


namespace myapp {

enum class BookOperationPriority {
    Init = 100,
    Err = 90,
    Gui = 50,
    Logic = 10,
    DeInit = 1,
};

}
