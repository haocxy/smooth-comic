#pragma once

#include <string>
#include <sstream>
#include <fstream>

#include "fs.h"
#include "basetype.h"



namespace logger
{

enum class Level {
    Unkown,
    All,
    Debug,
    Info,
    Warn,
    Error,
    Off,
};

inline const char *tostr(Level level) {
    switch (level) {
    case Level::Debug:
        return "d";
    case Level::Info:
        return "i";
    case Level::Warn:
        return "w";
    case Level::Error:
        return "e";
    default:
        return "?";
    }
}

bool shouldLog(logger::Level level);

class Writer {
public:

    Writer(const fs::path &dir, const std::string &basename);

    void write(const void *data, size_t len);

    void flush() {
        out_.flush();
    }

private:
    const fs::path base_;
    std::ofstream out_;
    std::tm lastTime_;
};

void writeLog(logger::Level level, const std::string &content);

class LogLine
{
public:
    LogLine(logger::Level level)
        : shouldLog_(logger::shouldLog(level))
        , level_(level) { }

    ~LogLine() {
        if (shouldLog_) {
            writeLog(level_, buffer_.str());
        }
    }

    template <typename T>
    LogLine(Level level, const T &obj)
        : LogLine(level) {
        *this << obj;
    }

    template <typename T>
    LogLine &operator<<(const T &obj) {
        if (shouldLog_) {
            buffer_ << obj;
        }
        return *this;
    }

    LogLine &operator<<(const std::u32string &s) {
        if (shouldLog_) {
            buffer_ << static_cast<std::string>(u8str(s));
        }
        return *this;
    }

    LogLine &operator<<(const char *str) {
        if (shouldLog_) {
            if (str) {
                buffer_ << str;
            } else {
                buffer_ << "(nullptr)";
            }
        }
        return *this;
    }

    LogLine &operator<<(const fs::path &p) {
        if (shouldLog_) {
            printPath(p);
        }
        return *this;
    }

private:
    void printPath(const fs::path &p);

private:
    const bool shouldLog_;
    const logger::Level level_;
    std::ostringstream buffer_;
};

template <Level level>
class Logger {
public:
    Logger() {}

    template <typename T>
    LogLine operator<<(const T &obj) {
        return LogLine(level, obj);
    }
};

namespace global_loggers {

extern Logger<Level::Debug> logDebug;

extern Logger<Level::Error> logError;

extern Logger<Level::Info> logInfo;

} // namespace global_loggers

} // namespace logger


namespace logger::control {

class Option {
public:
    Option() {}

    Level level() const {
        return level_;
    }

    void setLevel(Level level) {
        level_ = level;
    }

    void setLevel(const std::string &str);

    const fs::path &dir() const {
        return dir_;
    }

    void setDir(const fs::path &dir) {
        dir_ = dir;
    }

    const std::string &basename() const {
        return basename_;
    }

    void setBasename(const std::string &basename) {
        basename_ = basename;
    }

    bool isAlwaysFlush() const {
        return alwaysFlush_;
    }

    void setAlwaysFlush(bool alwaysFlush) {
        alwaysFlush_ = alwaysFlush;
    }

    bool shouldWriteToStdout() const {
        return writeToStdout_;
    }

    void setWriteToStdout(bool writeToStdout) {
        writeToStdout_ = writeToStdout;
    }

private:
    Level level_ = Level::Info;
    fs::path dir_;
    std::string basename_;
    bool alwaysFlush_ = false;
    bool writeToStdout_ = false;
};

void init(const Option &opt);

}
