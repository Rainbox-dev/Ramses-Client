#include "userlistmanagerwidget.h"

UserListManagerWidget::UserListManagerWidget(QWidget *parent) :
    ObjectListManagerWidget(
        Ramses::instance()->users(),
        new UserEditWidget(),
        "Users",
        parent )
{
    m_listEditWidget->setEditMode(ObjectListEditWidget::RemoveObjects);
    QStringList dontRemove;
    dontRemove << "Ramses" << "Removed";
    m_listEditWidget->setDontRemoveShortNameList(dontRemove);
}

void UserListManagerWidget::createObject()
{
    RamUser *user = new RamUser("NEW","J. Doe");

    Ramses::instance()->users()->append(user);

    editObject(user);
}

