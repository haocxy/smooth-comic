#include "debug-option.h"

#include "option-center.h"


namespace debug_option {

namespace DebugOptionHelper {

void addOptionCenterAfterLoadCallback(
    const std::string &name, std::function<void(const OptionRawData &data)> &&callback) {

    OptionCenter::instance().addAfterLoadCallback(name, std::move(callback));
}

} // namespace PreferenceEntryHelper

}
