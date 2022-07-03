#pragma once

#include <string>


namespace myapp::gui {

class PropertyName {
public:
    PropertyName(const std::string &name)
        : name_(name) {}

    operator const char *() const {
        return name_.c_str();
    }

private:
    std::string name_;
};

}
