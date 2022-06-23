#include "debug-option.h"

#include "option-center.h"


namespace debug_option {

namespace DebugOptionHelper {

void addOption(
    std::string &&name,
    std::string &&type,
    std::function<void(const OptionRawData &data)> &&callback,
    std::string &&desc) {

    OptionCenter::instance().addOption(name, std::move(type), std::move(callback), std::move(desc));
}

} // namespace PreferenceEntryHelper

}
