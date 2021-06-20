#include "shotlistmanagerwidget.h"

ShotListManagerWidget::ShotListManagerWidget(QWidget *parent):
    ObjectListManagerWidget(
        new ShotEditWidget(),
        "Shots",
        parent)
{
    this->setSortable(true);
    this->setContainingType(RamObject::Shot);
    changeProject(Ramses::instance()->currentProject());
    connect(Ramses::instance(), &Ramses::currentProjectChanged, this, &ShotListManagerWidget::changeProject);
}

void ShotListManagerWidget::createObject()
{
    RamProject *project = Ramses::instance()->currentProject();
    if (!project) return;
    if (project->sequences()->count() == 0 ) return;
    RamSequence *seq = RamSequence::sequence( currentFilter() );
    if (!seq) seq = qobject_cast<RamSequence*>( project->sequences()->at(0) );
    if(!seq) return;

    RamShot *shot = new RamShot(
                "NEW",
                seq,
                "New Shot"
                );

    project->shots()->append(shot);
}

void ShotListManagerWidget::changeProject(RamProject *project)
{
    // empty list
    this->clear();
    if (!project) return;
    this->setList( project->sequences() );
}
