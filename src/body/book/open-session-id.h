#pragma once

#include <chrono>

#include "core/basetype.h"


namespace myapp {

class OpenSessionId {
public:
    OpenSessionId() {}

    OpenSessionId(i64 id)
        : time_(std::chrono::steady_clock::now()), id_(id) {}

    bool operator==(const OpenSessionId &other) const {
        return time_ == other.time_ && id_ == other.id_;
    }

private:
    std::chrono::steady_clock::time_point time_;
    i64 id_{};
};

class OpenSessionIdGenerator {
public:
    OpenSessionIdGenerator(i64 initId = 1)
        : nextId_(initId) {}

    OpenSessionId next() {
        return OpenSessionId(nextId_++);
    }

private:
    i64 nextId_{ 1 };
};

}
