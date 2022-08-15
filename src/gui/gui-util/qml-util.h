#pragma once

#include <QQmlListProperty>

namespace myapp::qmlutil {

template <typename ContainerType>
class QmlListAdapter : public ContainerType {
public:
    using Elem = typename ContainerType::value_type;

    QmlListAdapter() {}

    QQmlListProperty<Elem> readonlyQmlList() const {
        return QQmlListProperty<Elem>(
            this, nullptr, &QmlListAdapter::qmlElemCount, &QmlListAdapter::qmlElemAt);
    }

private:
    static qsizetype qmlElemCount(QQmlListProperty<Elem> *list) {
        return reinterpret_cast<QmlListAdapter *>(list->object)->size();
    }

    static Elem *qmlElemAt(QQmlListProperty<Elem> *list, qsizetype i) {
        return (*reinterpret_cast<QmlListAdapter *>(list->object))[i];
    }
};

}
