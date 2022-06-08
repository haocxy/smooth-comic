#pragma once

#include "core/logger.h"

namespace myapp {

class CacheActorLogger : public logger::Logger {
public:
    CacheActorLogger(const std::string &actorName, const fs::path &archiveFile)
        : actorName_(actorName), archiveFile_(archiveFile) {
        setLevel(logger::Level::All);
    }

    void beforeWriteContent(logger::LogStream &stream) override {
        stream << actorName_ << "(" << archiveFile_ << ") ";
    }

private:
    const std::string actorName_;
    const fs::path archiveFile_;
};

}
