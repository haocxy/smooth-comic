#include "controll-bar-icons.h"

#include "gui-util/font-icon-enum.win32.h"


namespace myapp::ControllBarIcons {

static const char *const fam = "Segoe Fluent Icons";

const FontIcon ToggleThumbArea{ fam, FontIconEnum::DockRightMirrored };

const FontIcon OpenComic{ fam, FontIconEnum::OpenFile };

const FontIcon Reload{ fam, FontIconEnum::Refresh };

const FontIcon SwitchLeft{ fam, FontIconEnum::SwitchLeft };

const FontIcon SwitchRight{ fam, FontIconEnum::SwitchRight };

const FontIcon GlobalMenu{ fam, FontIconEnum::More };

}
