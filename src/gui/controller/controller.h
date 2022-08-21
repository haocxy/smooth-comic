#pragma once

#include <QObject>
#include <QPointer>

#include "core/declare_ptr.h"
#include "core/obj-handle.h"
#include "util/sigconns.h"
#include "gui-util/qtobj-strand-entry.h"

#include "gui/gui-engine.h"

#include "book/page-num.h"

#include "switch-direction.h"
#include "scale-mode.h"


namespace myapp {

class Engine;

class Book;

// 控制器
//
// 有很多功能都能够从多个地方触发，这样可以避免不同模块重复实现相同的功能，
// 也可以保证界面变化一致且易于维护
// 
// 所有 非展示性 的 GUI 操作（离如翻页、打开文件等）的控制中心
// 不要在 非展示性 的 GUI 模块间直接互相使用功能，而是通过控制器解除不同模块的耦合
class Controller : public QObject {
    Q_OBJECT

    Q_PROPERTY(GuiEngine *guiEngine READ guiEngine WRITE setGuiEngine NOTIFY guiEngineChanged)

    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
public:
    explicit Controller(QObject *parent = nullptr);

    explicit Controller(GuiEngine &engine, QObject *parent = nullptr);

    virtual ~Controller();

    Q_INVOKABLE void switchUpPage() {
        emit cmdSwitchPage(SwitchDirection::Up);
    }

    Q_INVOKABLE void switchDownPage() {
        emit cmdSwitchPage(SwitchDirection::Down);
    }

    Q_INVOKABLE void switchLeftPage() {
        emit cmdSwitchPage(SwitchDirection::Left);
    }

    Q_INVOKABLE void switchRightPage() {
        emit cmdSwitchPage(SwitchDirection::Right);
    }

    Q_INVOKABLE void relativelyScale(qreal relativeScale) {
        emit cmdRelativelyScale(relativeScale);
    }

    Q_INVOKABLE void translateBy(qreal dx, qreal dy) {
        emit cmdTranslateBy(dx, dy);
    }

    GuiEngine *guiEngine() {
        return guiEngine_;
    }

    void setGuiEngine(GuiEngine *guiEngine);

    Book &book() {
        return *book_;
    }

    void reloadComic();

    QString filePath() const {
        return filePath_;
    }

    void setFilePath(const QString &path);

signals:
    void cmdToggleThumbArea();

    void cmdOpenComic();

    void cmdRotatePageByOneStep();

    void cmdSwitchPage(SwitchDirection direction);

    void cmdJumpToPage(PageNum pageSeq);

    void cmdToggleScaleSettingPopup();

    void cmdSetScaleMode(ScaleMode mode);

    // 相对缩放：相对于当前缩放进行缩放
    void cmdRelativelyScale(qreal relativeScale);

    void cmdZoomIn();

    void cmdZoomOut();

    void cmdTranslateBy(qreal dx, qreal dy);

    void sigScaleRangeUpdated(float min, float max);

    void sigScaleUpdated(float scale);

    void guiEngineChanged();

    void filePathChanged();

private:
    QPointer<GuiEngine> guiEngine_;
    DeclarePtr<Book> book_;
    QString filePath_;
    SigConns sigConns_;
    QtObjStrandEntry strandEntry_;

    StrongHandle<Controller> handle_;
};

}
