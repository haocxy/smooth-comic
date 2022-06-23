#pragma once

#include <QScrollArea>
#include <QDockWidget>


namespace myapp {

class Book;

class ScrollAreaDockWidget : public QDockWidget {
    Q_OBJECT
public:
    explicit ScrollAreaDockWidget(QScrollArea *scrollArea, const QString &title, QWidget *parent);

    virtual ~ScrollAreaDockWidget();
};

}

