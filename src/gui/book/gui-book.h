#pragma once

#include <QObject>
#include <QPointer>

#include "core/declare_ptr.h"

#include "gui/gui-engine.h"


namespace myapp {

class Book;

class GuiEngine;


class GuiBook : public QObject {
    Q_OBJECT
    Q_PROPERTY(GuiEngine* guiEngine READ guiEngine WRITE setGuiEngine NOTIFY guiEngineChanged)
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
public:
    explicit GuiBook(QObject *parent = nullptr);

    virtual ~GuiBook();

    GuiEngine *guiEngine() const {
        return guiEngine_;
    }

    void setGuiEngine(GuiEngine *guiEngine);

    const QString &file() const;

    void setFile(const QString &file);

signals:
    void fileChanged();

    void guiEngineChanged();

private:
    QPointer<GuiEngine> guiEngine_;
    DeclarePtr<Book> book_;
    QString file_;
};

}
