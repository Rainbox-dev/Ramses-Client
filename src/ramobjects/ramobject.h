#ifndef RAMOBJECT_H
#define RAMOBJECT_H

#include <QObject>
#include <QMap>

#include "dbinterface.h"
#include "ramuuid.h"
#include "config.h"
#include "duqf-utils/utils.h"

class ObjectDockWidget;
class ObjectEditWidget;

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
                    StepStatusHistory,
                    ScheduleEntry};
    Q_ENUM( ObjectType )

    enum SubFolder { NoFolder,
                   ConfigFolder,
                   AdminFolder,
                   PreProdFolder,
                   ProdFolder,
                   PostProdFolder,
                   AssetsFolder,
                   ShotsFolder,
                   ExportFolder,
                   TemplatesFolder,
                   PublishFolder,
                   VersionsFolder,
                   PreviewFolder,
                   UsersFolder,
                   ProjectsFolder,
                   TrashFolder};
    Q_ENUM( SubFolder )

    explicit RamObject(QObject *parent = nullptr);
    explicit RamObject(QString uuid, QObject *parent = nullptr);
    explicit RamObject(QString shortName, QString name, QString uuid = "", QObject *parent = nullptr);

    virtual QString shortName() const;
    void setShortName(const QString &shortName);

    virtual QString name() const;
    void setName(const QString &name);

    QString comment() const;
    void setComment(const QString comment);

    QString uuid() const;

    RamObject::ObjectType objectType() const;
    void setObjectType(ObjectType type);

    int order() const;
    void setOrder(int order);

    QString path(SubFolder subFolder = NoFolder, bool create = false) const;
    QStringList listFiles(SubFolder subFolder = NoFolder) const;
    QStringList listFiles(SubFolder subFolder, QString subPath) const;
    void deleteFile(QString fileName, SubFolder folder=NoFolder) const;
    void revealFolder(SubFolder subFolder = NoFolder);

    QString subFolderName(SubFolder folder) const;

    QString filterUuid() const;

    bool is(const RamObject *other) const;

    static RamObject *obj(QString uuid);


public slots:
    virtual void update();
    virtual void removeFromDB() {};
    virtual void remove(bool updateDB = true);
    virtual void edit(bool s = true) { Q_UNUSED(s) };

signals:
    void changed(RamObject *);
    void orderChanged(RamObject *, int p, int n);
    void orderChanged();
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
    bool m_removing = false;
    bool m_editReady = false;
    QString m_filterUuid;

    static QMap<QString, RamObject*> m_existingObjects;

    void setEditWidget(ObjectEditWidget *w );
    void showEdit(bool show = true);
    virtual QString folderPath() const { return QString(); };

    ObjectEditWidget *ui_editWidget = nullptr;

private:
    RamObject::ObjectType _objectType = Generic;



    ObjectDockWidget *m_dockWidget = nullptr;

};

#endif // RAMOBJECT_H
