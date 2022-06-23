#include "proc-startup-info.h"

#include "fs.h"


namespace
{

static fs::path g_dir;

class Init
{
public:
    Init() {
        g_dir = fs::current_path();
    }
};

static const Init g_init;

} // namespace anonymous



namespace proc_startup_info
{

const fs::path &dir() {
    return g_dir;
}

} // namespace proc_startup_info
