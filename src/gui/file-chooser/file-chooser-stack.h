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

    void removeTooOldHistories() {
        bool changed = false;
        while (frames_.size() > limit_) {
            frames_.pop_front();
            changed = true;
        }
        emit framesChanged();
    }

private:
    int limit_{ 20 };
    std::deque<uptr<FileChooserStackFrame>> frames_;
};

}
