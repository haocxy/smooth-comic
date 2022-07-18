#pragma once

#include <stdexcept>


namespace myapp {

enum class PageDirection {
    Up, Down, Left, Right
};

inline PageDirection nextOf(PageDirection dir)
{
    switch (dir) {
    case PageDirection::Up:
        return PageDirection::Right;
    case PageDirection::Right:
        return PageDirection::Down;
    case PageDirection::Down:
        return PageDirection::Left;
    case PageDirection::Left:
        return PageDirection::Up;
    default:
        throw std::logic_error("nextOf(PageDirection): bad dir enum");
    }
}

}
