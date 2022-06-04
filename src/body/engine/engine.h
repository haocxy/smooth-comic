#pragma once


namespace myapp {

class AsyncDeleter;

class ThumbCache;


class Engine {
public:
    Engine();

    ~Engine();

    AsyncDeleter &asyncDeleter() {
        return *asyncDeleter_;
    }

    ThumbCache &thumbCache() {
        return *thumbCache_;
    }

private:
    AsyncDeleter *asyncDeleter_{};
    ThumbCache *thumbCache_{};
};

}

