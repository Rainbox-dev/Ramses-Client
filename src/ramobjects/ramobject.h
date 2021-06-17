#ifndef RAMOBJECT_H
#define RAMOBJECT_H

#include <QObject>
#include <QMap>

#include "dbinterface.h"
#include "ramuuid.h"

class RamObject : public QObject
{
    Q_OBJECT
public:
    enum ObjectType { Application,
                    Asset,
                    AssetGroup,
                    FileType,
                    Generic,
                    Item,
                    Pipe,
                    PipeFile,
                    Project,
                    Sequence,
                    Shot,
                    State,
                    Status,
                    Step,
                    User,
                    ObjectList,
                    ObjectUberList,
                    StepStatusHistory};
    Q_ENUM( ObjectType )

    explicit RamObject(QObject *parent = nullptr);
    explicit RamObject(QString uuid, QObject *parent = nullptr);
    explicit RamObject(QString shortName, QString name, QString uuid = "", QObject *parent = nullptr);

    QString shortName() const;
    void setShortName(const QString &shortName);

    QString name() const;
    void setName(const QString &name);

    QString comment() const;
    void setComment(const QString comment);

    QString uuid() const;

    RamObject::ObjectType objectType() const;
    void setObjectType(ObjectType type);

    int order() const;
    void setOrder(int order);

    bool is(const RamObject *other);

    static RamObject *obj(QString uuid);

public slots:
    virtual void update();
    virtual void remove();

signals:
    void changed(RamObject *);
    void orderChanged(RamObject *, int p, int n);
    void removed(RamObject *);

protected:
    DBInterface *m_dbi;
    QString m_shortName;
    QString m_name;
    // Check if changed to limit number of signals
    bool m_dirty;

    QString m_uuid;
    QString m_comment;
    int m_order = -1;
    bool m_orderChanged = false;
    bool m_removing;

    static QMap<QString, RamObject*> m_existingObjects;

private:
    RamObject::ObjectType _objectType = Generic;

};

#endif // RAMOBJECT_H
