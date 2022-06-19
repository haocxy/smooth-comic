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

class LogStream {
public:
    LogStream() {}

    template <typename T>
    LogStream &operator<<(const T &obj) {
        ss_ << obj;
        return *this;
    }

    LogStream &operator<<(const u8str &s) {
        ss_ << static_cast<std::string>(s);
        return *this;
    }

    LogStream &operator<<(const u32str &s) {
        ss_ << static_cast<std::string>(u8str(s));
        return *this;
    }

    std::string str() const {
        return ss_.str();
    }

private:
    std::ostringstream ss_;
};

class Logger {
private:

    class LogLine {
    public:
        LogLine(Logger &logger, Level level)
            : logger_(logger)
            , level_(level)
            , shouldLog_(logger_.shouldLog(level)) {

            if (shouldLog_) {
                logger_.beforeWriteContent(buffer_);
            }
        }

        template <typename T>
        LogLine(Logger &logger, Level level, const T &firstObj)
            : LogLine(logger, level) {

            *this << firstObj;
        }

        ~LogLine() {
            if (shouldLog_) {
                logger_.afterWriteContent(buffer_);
                writeLog(level_, buffer_.str());
            }
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
        void printPath(const fs::path &p) {
            buffer_ << p.generic_string();
        }

    private:
        Logger &logger_;
        const Level level_;
        const bool shouldLog_;
        LogStream buffer_;
    };

public:

    class LogLineMaker {
    public:
        LogLineMaker(Logger &logger, Level lineLevel)
            : logger_(logger), lineLevel_(lineLevel) {}

        template <typename T>
        LogLine operator<<(const T &firstObj) const {
            return LogLine(logger_, lineLevel_, firstObj);
        }

    private:
        Logger &logger_;
        const Level lineLevel_;
    };

public:
    explicit Logger(Level level = Level::Info)
        : level_(level)
        , d(*this, Level::Debug)
        , i(*this, Level::Info)
        , e(*this, Level::Error) {}

    virtual ~Logger() {}

    void setLevel(Level level) {
        level_ = level;
    }

    virtual void write(Level level, const std::string &content) {
        writeLog(level, content);
    }

protected:
    virtual void beforeWriteContent(LogStream &stream) {}

    virtual void afterWriteContent(LogStream &stream) {}

private:
    bool shouldLog(Level level) const {
        return level >= level_;
    }

private:
    Level level_{ Level::Info };

public:
    LogLineMaker d;
    LogLineMaker i;
    LogLineMaker e;
};

extern Logger gLogger;

} // namespace logger


namespace logger::control {

class Option {
public:
    Option() {}

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
    fs::path dir_;
    std::string basename_;
    bool alwaysFlush_ = false;
    bool writeToStdout_ = false;
};

void init(const Option &opt);

}
