#include "ramobject.h"


RamObject::RamObject(QString shortName, QString name, QString uuid, QString path, QObject *parent) : QObject(parent)
{
    _shortName = shortName;
    _name = name;
    if(_name == "" ) _name = shortName;
    _folderPath = path;
    if (uuid != "") _uuid = uuid;
    else _uuid = RamUuid::generateUuidString(_shortName);
    _dbi = DBInterface::instance();
}

QString RamObject::shortName() const
{
    return _shortName;
}

void RamObject::setShortName(const QString &shortName)
{
    _shortName = shortName;
    emit changed();
}

QString RamObject::name() const
{
    return _name;
}

void RamObject::setName(const QString &name)
{
    _name = name;
    emit changed();
}

QString RamObject::folderPath() const
{
    return _folderPath;
}

void RamObject::setFolderPath(const QString &folderPath)
{
    _folderPath = folderPath;
    emit changed();
}

QString RamObject::uuid() const
{
    return _uuid;
}

void RamObject::setUuid(const QString &uuid)
{
    _uuid = uuid;
    emit changed();
}