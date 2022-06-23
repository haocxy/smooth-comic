#include "body-entry.h"

#include <iostream>
#include <QApplication>

#include <boost/program_options.hpp>

#include "core/logger.h"
#include "core/thread.h"
#include "core/debug-option/option-center.h"

#include "engine/engine.h"
#include "gui/book-viewer.h"

#include "register-qt-meta-types.h"

#include "cmd-option.h"


namespace myapp {

using logger::gLogger;

static void initDebugOptions(const std::map<std::string, std::string> &rawDebugOptions)
{
    debug_option::OptionCenter &center = debug_option::OptionCenter::instance();

    for (const auto &pair : rawDebugOptions) {
        center.set(pair.first, pair.second, debug_option::OptionSource::CmdLine);
    }

    center.publishAllOptions();
}

static void listDebugOptions(std::ostream &out)
{
    debug_option::OptionCenter &center = debug_option::OptionCenter::instance();

    const std::string deco = "    ";

    out << "Debug options:" << std::endl;

    center.eachOption([&deco, &out](const std::string &name, const debug_option::OptionInfo &optionInfo) {
        out << deco << name << " [" << optionInfo.type << "]";
        if (!optionInfo.desc.empty()) {
            out << " " << optionInfo.desc;
        }
        out << std::endl;
    });
}

int body_entry(int argc, char *argv[])
{
    ThreadUtil::setNameForCurrentThread("GUI");


    // init default global logger
    logger::control::Option opt;
    opt.setAlwaysFlush(true);
    opt.setWriteToStdout(true);

    logger::control::init(opt);

    gLogger.setLevel(logger::Level::All);


    // parse command line
    CmdOption cmdOption(argc, argv);
    if (cmdOption.hasError()) {
        std::cerr << "Cannot parse command line because:" << cmdOption.errMsg() << std::endl;
        cmdOption.showHelp(std::cerr);
        return 1;
    }

    if (cmdOption.needHelp()) {
        cmdOption.showHelp(std::cout);
        return 0;
    }

    if (cmdOption.needListDebugOptions()) {
        listDebugOptions(std::cout);
        return 0;
    }

    // init debug options
    initDebugOptions(cmdOption.debugOptions());


    gLogger.i << "body_entry()";

    QApplication app(argc, argv);

    registerQtMetaTypes();

    Engine engine;

    BookViewer w(engine);
    w.show();

    return QApplication::exec();
}

}
