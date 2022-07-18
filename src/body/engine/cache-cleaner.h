#pragma once

#include "core/thread.h"

#include "core/logger.h"


namespace myapp {

class Engine;


class CacheCleaner {
public:
    CacheCleaner(Engine &engine);

    ~CacheCleaner();

private:
    void loop();

    void cleanCache();

    class Logger : public logger::Logger {
    public:
        Logger() {
            setLevel(logger::Level::All);
        }

    protected:
        virtual void beforeWriteContent(logger::LogStream &stream) {
            stream << "CacheCleaner: ";
        }
    };

private:
    Engine &engine_;

    Logger logger_;

    std::atomic_bool stopped_{ false };

    Sleeper sleeper_;

    std::jthread thread_;
};

}
