#pragma once

#include <string_view>
#include <functional>
#include <sstream>

#include "core/logger.h"
#include "option-convert.h"


namespace debug_option {

namespace DebugOptionHelper {

void addOptionCenterAfterLoadCallback(
    const std::string &name,
    std::function<void(const OptionRawData &data)> &&callback);

} // namespace ProgramOptionHelper

// 调试选项
// 
// 提供一个可以在启动程序时调整程序中某些参数的机制。
// 
// TODO 未来可以搞成可以仅通过修改一个编译期宏就可以完全禁用这一机制且不产生任何运行时开销的方式，
//     这样就可以在正式发布程序时不需要修改代码就去掉这一机制的开销，仅相当于定义了一个常量。
// 
// 注意！
// 虽然这套工具完全可以用来支持程序的配置，但不要这样做，正式的配置应该是明确的、结构化的，而非散落在代码各处。
// 正如类名中的 Debug 所表示的，这套机制仅用来做调试用，例如可以在程序中指定一个时间值以使某些逻辑变慢，以观察对其他模块的影响。
// 在正式发布的程序中应该完全禁用这一机制。
template <typename T>
class DebugOption
{
public:
    DebugOption(const std::string_view &name, const T &defaultValue = T())
        : name_(name)
        , value_(defaultValue) {

        DebugOptionHelper::addOptionCenterAfterLoadCallback(name_, [this](const OptionRawData &data) {
            if (initialized_) {
                return;
            }
            data >> value_;
            initialized_ = true;
            logger::gLogger.d << "DebugOption initialized, name: [" << name_ << "], value: [" << value_ << "]";
        });

        logger::gLogger.d << "DebugOption constructed, name: [" << name_ << "], default: [" << value_ << "]";
    }

    const T &operator*() const {
        return value_;
    }

    const T *operator->() const {
        return &value_;
    }

private:
    const std::string name_;
    bool initialized_ = false;
    T value_;
};

}
