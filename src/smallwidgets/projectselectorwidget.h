#ifndef PROJECTSELECTORWIDGET_H
#define PROJECTSELECTORWIDGET_H

#include <QComboBox>

#include "ramses.h"

class ProjectSelectorWidget : public QComboBox
{
    Q_OBJECT
public:
    ProjectSelectorWidget(QWidget *parent = nullptr);
private slots:
    void newProject(RamObject *obj);
    void projectRemoved(RamObject *o);
    void projectChanged(RamObject *o);
    void setCurrentProject(int index);
    void currentProjectChanged(RamProject *p);
};

#endif // PROJECTSELECTORWIDGET_H