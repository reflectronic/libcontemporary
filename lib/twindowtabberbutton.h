//
// Created by Victor Tran on 13/4/2022.
//

#ifndef LIBCONTEMPORARY_TWINDOWTABBERBUTTON_H
#define LIBCONTEMPORARY_TWINDOWTABBERBUTTON_H

#include <QWidget>
#include "libcontemporary_global.h"

struct tWindowTabberButtonPrivate;
class tStackedWidget;
class tWindowTabber;
class LIBCONTEMPORARY_EXPORT tWindowTabberButton : public QWidget {
    Q_OBJECT
public:
    tWindowTabberButton(tWindowTabber *parent = nullptr);
    tWindowTabberButton(const QIcon& icon, const QString& text);
    tWindowTabberButton(const QIcon& icon, const QString& text, tStackedWidget *toTrack, QWidget* widgetToTrack);
    virtual ~tWindowTabberButton();

    void setText(const QString& text);
    void setIcon(const QIcon& icon);
    void setSelected(bool selected);

    void setParent(tWindowTabber* tabber);
    void syncWithStackedWidget(tStackedWidget *stackedWidget, QWidget* widget);

    signals:
        void activated();

protected:
    friend tWindowTabber;
    void actionEvent(QActionEvent *event) override;

private:
    tWindowTabberButtonPrivate* d;

    void init();
    void configureAction(QAction* action);
    void configureActionStrip();
};


#endif//LIBCONTEMPORARY_TWINDOWTABBERBUTTON_H
