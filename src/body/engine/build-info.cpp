#include "build-info.h"


namespace BuildInfo {

#if defined(MYAPP_SOLUTION_ROOT_DIR)
const std::optional<fs::path> solutionDir{ MYAPP_SOLUTION_ROOT_DIR };
#else
const std::optional<fs::path> solutionDir{ std::nullopt };
#endif

}
