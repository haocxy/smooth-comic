#pragma once

#include <deque>

#include <QObject>
#include <QQmlListProperty>

#include "file-chooser-stack-frame.h"


namespace myapp {

class FileChooserStack : public QObject {
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<FileChooserStackFrame> frames READ frames NOTIFY framesChanged)


public:
    explicit FileChooserStack(QObject *parent = nullptr);

    virtual ~FileChooserStack();

    int limit() const {
        return limit_;
    }

    void setLimit(int limit) {
        limit_ = limit;
        removeTooOldHistories();
    }

    QQmlListProperty<FileChooserStackFrame> frames() {
        return QQmlListProperty<FileChooserStackFrame>(
            this, nullptr, &FileChooserStack::frameCount, &FileChooserStack::frameAt);
    }

    void push(const fs::path &dir);

    void pop();

    size_t frameCount() const {
        return frames_.size();
    }

    FileChooserStackFrame *topFrame() {
        if (!frames_.empty()) {
            return frames_.back().get();
        } else {
            return nullptr;
        }
    }

signals:
    void dirChanged();

    void framesChanged();

private:
    static qsizetype frameCount(QQmlListProperty<FileChooserStackFrame> *list) {
        return reinterpret_cast<FileChooserStack *>(list->object)->frames_.size();
    }

    static FileChooserStackFrame *frameAt(QQmlListProperty<FileChooserStackFrame> *list, qsizetype i) {
        return reinterpret_cast<FileChooserStack *>(list->object)->frames_[i].get();
    }

    enum class ShouldEmit {
        Yes, No
    };

    void removeTooOldHistories(ShouldEmit shouldEmit = ShouldEmit::Yes) {
        bool changed = false;
        while (frames_.size() > limit_) {
            frames_.pop_front();
            changed = true;
        }
        if (shouldEmit == ShouldEmit::Yes) {
            emit framesChanged();
        }
    }

private:
    int limit_{ 3 };
    std::deque<uptr<FileChooserStackFrame>> frames_;
};

}
