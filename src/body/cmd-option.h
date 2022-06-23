#pragma once

#include <map>
#include <vector>
#include <string>

#include <boost/program_options.hpp>

#include "core/fs.h"
#include "core/strutil.h"


namespace myapp {

class CmdOption {
public:
    CmdOption(int argc, char *argv[]) {
        if (argc <= 0) {
            throw std::logic_error("CmdOption::CmdOption(...) bad argc");
        }

        namespace po = boost::program_options;

        exe_ = fs::absolute(argv[0]);

        std::vector<std::string> debugOptions;

        po::options_description_easy_init add = desc_.add_options();
        add("list-debug-options", "Show all usable debug options");
        add("debug-option,d", po::value<std::vector<std::string>>(&debugOptions), "Set debug option");
        add("help,h", "Show this message");

        posdesc_.add("file", -1);

        try {
            po::store(po::command_line_parser(argc, argv).options(desc_).positional(posdesc_).run(), varmap_);
            po::notify(varmap_);
        } catch (const std::exception &e) {
            hasError_ = true;
            errMsg_ = e.what();
            return;
        }

        debugOptions_ = parseDebugOptions(debugOptions);
    }

    bool hasError() const {
        return hasError_;
    }

    const std::string &errMsg() const {
        return errMsg_;
    }

    bool needHelp() const {
        return hasError_ || varmap_.count("help") != 0;
    }

    void showHelp(std::ostream &out) {
        const std::string tab = "    ";
        const std::string app = exe_.filename().generic_string();
        out << "Usage: " << std::endl;
        out << tab << app << "[Oprions] file(s)" << std::endl;
        out << desc_ << std::endl;
    }

    const std::map<std::string, std::string> &debugOptions() const {
        return debugOptions_;
    }

private:
    static std::map<std::string, std::string> parseDebugOptions(const std::vector<std::string> &vec) {
        std::map<std::string, std::string> map;
        for (const std::string &s : vec) {
            strutil::strviews parts = strutil::split(s, '=');
            if (parts.size() == 2) {
                map[std::string(parts[0])] = std::string(parts[1]);
            }
        }
        return map;
    }

private:
    boost::program_options::options_description desc_{ "Options" };
    boost::program_options::positional_options_description posdesc_;
    boost::program_options::variables_map varmap_;
    bool hasError_{ false };
    std::string errMsg_;
    fs::path exe_;
    std::map<std::string, std::string> debugOptions_;
};

}
