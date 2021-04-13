#ifndef OBJECTEDITWIDGET_H
#define OBJECTEDITWIDGET_H

#include <QScrollArea>

#include "ui_objecteditwidget.h"
#include "ramobject.h"

class ObjectEditWidget : public QScrollArea, protected Ui::ObjectEditWidget
{
    Q_OBJECT

public:
    explicit ObjectEditWidget(RamObject *o = nullptr, QWidget *parent = nullptr);

    RamObject *object() const;

public slots:
    void setObject(RamObject *object);

protected slots:
    virtual void update();
    virtual bool checkInput();

protected:
    QList<QMetaObject::Connection> _objectConnections;
    bool updating = false;

private slots:
    void objectRemoved(RamObject *o);
    void objectChanged(RamObject *o);

private:
    RamObject *_object;
    void connectEvents();
};

#endif // OBJECTEDITWIDGET_H
