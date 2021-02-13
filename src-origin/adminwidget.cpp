#include "adminwidget.h"

AdminWidget::AdminWidget(DBInterface *db, Updater *up, QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    dbi = db;
    updater = up;

    error = new QErrorMessage();
}

void AdminWidget::init()
{
    statusAdminList->clear();
    statusesAdminReset();

    stagesAdminList->clear();
    stagesAdminReset();

    projectAdminList->clear();
    projectsAdminReset();

    shotsAdminList->clear();
    shotsAdminReset();

    //statuses
    foreach(RAMStatus *status,updater->getStatuses())
    {
        newStatus(status);
    }

    //stages
    foreach(RAMStage *stage,updater->getStages())
    {
        newStage(stage);
    }

    //projects
    foreach(RAMProject *project,updater->getProjects())
    {
        newProject(project);
    }

    //shots
    if (updater->getCurrentProject() != 0)
    {
        for(int i = 0 ; i < updater->getCurrentProject()->getShots().count() ; i++)
        {
            RAMShot *shot = updater->getCurrentProject()->getShots()[i];
            newShot(shot,i);
        }

    }
}

// GENERAL

void AdminWidget::on_adminTab_currentChanged(int index)
{
    statusesAdminReset();
    stagesAdminReset();
    projectsAdminReset();
    shotsAdminReset();
}

// STATUS

void AdminWidget::newStatus(RAMStatus *rs)
{
    // Create UI item
    QListWidgetItem *item = new QListWidgetItem(rs->getShortName() + " | " + rs->getName());
    item->setBackgroundColor(rs->getColor());
    item->setToolTip(rs->getDescription());
    item->setData(Qt::UserRole,rs->getId());
    statusAdminList->addItem(item);
}

void AdminWidget::on_addStatusButton_clicked()
{
    // Create a new Default Status
    QString name = "New Status";
    QString shortName = "New";
    QColor color = QColor("#6d6d6d");
    QString description = "";

    // create status
    RAMStatus *rs = new RAMStatus(dbi,name,shortName,color,description,"",true);

    // Add the status to the list
    updater->addStatus(rs);

    // Create UI item
    newStatus(rs);

    // Select item
    statusAdminList->setCurrentRow(statusAdminList->count()-1);
    on_statusAdminList_itemClicked(statusAdminList->item(statusAdminList->count()-1));
}

void AdminWidget::on_statusAdminList_itemClicked(QListWidgetItem *i)
{
    RAMStatus *s = updater->getStatus(i->data(Qt::UserRole).toString());

    statusNameEdit->setText(s->getName());
    statusShortNameEdit->setText(s->getShortName());
    if (s->getShortName() == "STB" || s->getShortName() == "OK" || s->getShortName() == "TODO")
    {
        statusShortNameEdit->setEnabled(false);
    }
    else
    {
        statusShortNameEdit->setEnabled(true);
    }
    statusColorEdit->setText(s->getColor().name().right(6));
    statusDescriptionEdit->setPlainText(s->getDescription());

    statusConfigWidget->setEnabled(true);
}

void AdminWidget::on_statusColorButton_clicked()
{
    this->setEnabled(false);
    QColorDialog cd(QColor("#" + statusColorEdit->text()));
    cd.setOptions(QColorDialog::DontUseNativeDialog);
    cd.setWindowFlags(Qt::FramelessWindowHint);
    //cd.move(this->parentWidget()->parentWidget()->geometry().center().x()-cd.geometry().width()/2,this->parentWidget()->parentWidget()->geometry().center().y()-cd.geometry().height()/2);
    if (cd.exec())
    {
        QColor color = cd.selectedColor();
        QString colorHex = color.name();
        statusColorEdit->setText(colorHex.right(6));
    }
    this->setEnabled(true);
}

void AdminWidget::on_statusApplyButton_clicked()
{
    int currentRow = statusAdminList->currentRow();
    if (currentRow < 0) return;

    QString name = statusNameEdit->text();
    QString shortName = statusShortNameEdit->text();
    QString description = statusDescriptionEdit->toPlainText();
    QColor color("#" + statusColorEdit->text());

    RAMStatus *rs = updater->getStatus(statusAdminList->currentItem()->data(Qt::UserRole).toString());
    rs->setColor(color);
    rs->setName(name);
    rs->setShortName(shortName);
    rs->setDescription(description);
    rs->update();

    //update UI
    QListWidgetItem *item = statusAdminList->currentItem();
    item->setText(shortName + " | " + name);
    item->setBackgroundColor(color);
    item->setToolTip(description);
}

void AdminWidget::on_removeStatusButton_clicked()
{
    int currentRow = statusAdminList->currentRow();
    if (currentRow < 0) return;

    RAMStatus *rs = updater->getStatus(statusAdminList->currentItem()->data(Qt::UserRole).toString());

    QString sN = rs->getShortName();
    if (sN == "OK" || sN == "STB" || sN == "TODO")
    {
        error->showMessage("Cannot remove OK, STB and TODO statuses.");
        return;
    }

    updater->removeStatus(rs);
    rs->remove();
    QListWidgetItem *item = statusAdminList->takeItem(currentRow);
    delete item;

    statusesAdminReset();
}

void AdminWidget::statusesAdminReset()
{
    statusAdminList->setCurrentRow(-1);
    statusNameEdit->setText("");
    statusShortNameEdit->setText("");
    statusColorEdit->setText("");
    statusDescriptionEdit->setPlainText("");
    statusConfigWidget->setEnabled(false);
}

// STAGE

void AdminWidget::newStage(RAMStage *rs)
{
    // Create UI item
    QListWidgetItem *item = new QListWidgetItem(rs->getShortName() + " | " + rs->getName());
    item->setData(Qt::UserRole,rs->getId());
    stagesAdminList->addItem(item);
}

void AdminWidget::on_addStageButton_clicked()
{
    // Create a new default stage
    QString name = "New Stage";
    QString shortName = "New";

    // Check if already exists
    for(int i = 0 ; i < updater->getStages().count() ; i++)
    {
        RAMStage *stage = updater->getStages()[i];
        if (stage->getName() == name && stage->getShortName() == shortName)
        {
            //select item
            stagesAdminList->setCurrentRow(i);
            on_stagesAdminList_itemClicked(stagesAdminList->item(i));
            return;
        }
    }


    RAMStage *rs = new RAMStage(dbi,name,shortName,true);
    updater->addStage(rs);
    newStage(rs);

    //select item
    stagesAdminList->setCurrentRow(stagesAdminList->count()-1);
    on_stagesAdminList_itemClicked(stagesAdminList->item(stagesAdminList->count()-1));
}

void AdminWidget::on_stagesAdminList_itemClicked(QListWidgetItem *item)
{
     RAMStage *s = updater->getStage(item->data(Qt::UserRole).toString());

     stageNameEdit->setText(s->getName());
     stageShortNameEdit->setText(s->getShortName());
     stageConfigWidget->setEnabled(true);
}

void AdminWidget::on_stageApplyButton_clicked()
{
    int currentRow = stagesAdminList->currentRow();
    if (currentRow < 0) return;

    RAMStage *s = updater->getStage(stagesAdminList->currentItem()->data(Qt::UserRole).toString());

    QString name = stageNameEdit->text();
    QString shortName = stageShortNameEdit->text();

    s->setName(name);
    s->setShortName(shortName);
    s->update();

    //update UI
    QListWidgetItem *item = stagesAdminList->item(currentRow);
    item->setText(shortName + " | " + name);
}

void AdminWidget::on_removeStageButton_clicked()
{
    int currentRow = stagesAdminList->currentRow();
    if (currentRow < 0) return;

    RAMStage *rs = updater->getStage(stagesAdminList->currentItem()->data(Qt::UserRole).toString());

    updater->removeStage(rs);
    rs->remove();
    QListWidgetItem *item = stagesAdminList->takeItem(currentRow);
    delete item;

    stagesAdminReset();
}

void AdminWidget::stagesAdminReset()
{
    stagesAdminList->setCurrentRow(-1);
    stageNameEdit->setText("");
    stageShortNameEdit->setText("");
    stageConfigWidget->setEnabled(false);
}

// PROJECT

void AdminWidget::newProject(RAMProject *rp)
{
    // Create UI item
    QListWidgetItem *item = new QListWidgetItem(rp->getShortName() + " | " + rp->getName());
    item->setData(Qt::UserRole,rp->getId());
    projectAdminList->addItem(item);
}

void AdminWidget::on_addProjectButton_clicked()
{
    // Create a new default project
    QString name = "New Project";
    QString shortName = "New";

    RAMProject *rp = new RAMProject(dbi,name,shortName,updater->getDefaultStatus(),true);
    updater->addProject(rp);
    newProject(rp);

    //select item
    projectAdminList->setCurrentRow(projectAdminList->count()-1);
    on_projectAdminList_itemClicked(projectAdminList->item(projectAdminList->count()-1));
}

void AdminWidget::on_projectAdminList_itemClicked(QListWidgetItem *item)
{
     int currentRow = projectAdminList->currentRow();
     if (currentRow < 0) return;

     RAMProject *p = updater->getProject(item->data(Qt::UserRole).toString());

     projectNameEdit->setText(p->getName());
     projectShortNameEdit->setText(p->getShortName());

     //populate stages combo box and list
     projectStagesList->clear();
     projectStagesComboBox->clear();
     QList<RAMStage*> pStages = p->getStages();

     foreach(RAMStage*s,updater->getStages())
     {
         //check if it is used
         bool usedByProject = false;
         foreach(RAMStage *ps,pStages)
         {
             if (ps == s)
             {
                 //add to list
                 QListWidgetItem *i = new QListWidgetItem(s->getShortName());
                 i->setToolTip(s->getName());
                 i->setData(Qt::UserRole,s->getId());
                 projectStagesList->addItem(i);
                 usedByProject = true;
                 break;
             }
         }
         //add to combo box
         if (!usedByProject)
         {
            projectStagesComboBox->addItem(s->getShortName(),s->getId());
         }
     }

     projectConfigWidget->setEnabled(true);
}

void AdminWidget::on_projectApplyButton_clicked()
{
    int currentRow = projectAdminList->currentRow();
    if (currentRow < 0) return;

    RAMProject *rp = updater->getProject(projectAdminList->currentItem()->data(Qt::UserRole).toString());

    QString name = projectNameEdit->text();
    QString shortName = projectShortNameEdit->text();

    rp->setName(name);
    rp->setShortName(shortName);
    rp->update();

    //update UI
    QListWidgetItem *item = projectAdminList->item(currentRow);
    item->setText(shortName + " | " + name);
}

void AdminWidget::on_removeProjectButton_clicked()
{
    int currentRow = projectAdminList->currentRow();
    if (currentRow < 0) return;

    RAMProject *rp = updater->getProject(projectAdminList->currentItem()->data(Qt::UserRole).toString());
    updater->removeProject(rp);
    rp->remove();
    QListWidgetItem *item = projectAdminList->takeItem(currentRow);
    delete item;

    projectsAdminReset();
}

void AdminWidget::on_projectAddStageButton_clicked()
{
    if(projectStagesComboBox->currentIndex() < 0) return;

    QString sId = projectStagesComboBox->currentData().toString();
    QString pId = projectAdminList->currentItem()->data(Qt::UserRole).toString();
    RAMProject *rp = updater->getProject(pId);
    RAMStage *rs = updater->getStage(sId);
    rp->addStage(rs,true);

    //update UI
    //remove from combobox
    projectStagesComboBox->removeItem(projectStagesComboBox->currentIndex());
    //add to list
    QListWidgetItem *i = new QListWidgetItem(rs->getShortName());
    i->setToolTip(rs->getName());
    i->setData(Qt::UserRole,rs->getId());
    projectStagesList->addItem(i);
}

void AdminWidget::on_createAssetsButton_clicked()
{
    emit message("Creating assets","general");
    emit working(true);
    if (projectStagesList->currentRow() < 0) return;

    RAMStage *rs = updater->getStage(projectStagesList->currentIndex().data(Qt::UserRole).toString());

    updater->getProject(projectAdminList->currentItem()->data(Qt::UserRole).toString())->createStageAssets(rs);

    emit message("Assets created for stage " + rs->getShortName(),"general");
    emit working(false);
}

void AdminWidget::on_removeStageProjectButton_clicked()
{
    if (projectStagesList->currentRow() < 0) return;

    RAMStage *rs = updater->getStage(projectStagesList->currentIndex().data(Qt::UserRole).toString());
    RAMProject *rp = updater->getProject(projectAdminList->currentItem()->data(Qt::UserRole).toString());

    rp->removeStage(rs,true);

    //update UI
    //remove from list
    QListWidgetItem *item = projectStagesList->takeItem(projectStagesList->currentRow());
    delete item;
    //add to combobox
    projectStagesComboBox->addItem(rs->getShortName(),rs->getId());
}

void AdminWidget::projectsAdminReset()
{
    projectAdminList->setCurrentRow(-1);
    projectNameEdit->setText("");
    projectShortNameEdit->setText("");
    projectConfigWidget->setEnabled(false);
    projectStagesList->clear();
    //reload stages into combo box
    projectStagesComboBox->clear();
    projectStagesList->clear();
}

// SHOT

void AdminWidget::newShot(RAMShot *rs, int row)
{
    //update list
    QListWidgetItem *item = new QListWidgetItem(rs->getName());
    item->setData(Qt::UserRole,rs->getId());
    shotsAdminList->insertItem(row,item);
}

int AdminWidget::getNewShotRow()
{
    int row = 0;
    if (shotsAdminList->currentRow() >= 0)
    {
        row = shotsAdminList->currentRow() + 1;
    }
    else if (shotsAdminList->count() > 0)
    {
        row = shotsAdminList->count();
    }

    return row;
}

void AdminWidget::resetShotsOrder()
{
    QStringList ids;
    foreach(RAMShot *rs,updater->getCurrentProject()->getShots())
    {
        ids << rs->getId();
    }
    dbi->resetShotsOrder(ids);
}

void AdminWidget::on_addShotButton_clicked()
{
    if (updater->getCurrentProject() == 0) return;
    //only if new
    bool ok = true;
    int row = 0;
    foreach(RAMShot *rs,updater->getCurrentProject()->getShots())
    {
        if (rs->getName() == "000")
        {
            ok = false;
            row = updater->getCurrentProject()->getShots().indexOf(rs);
        }
    }

    if (ok)
    {
        row = getNewShotRow();

        RAMShot *rs = new RAMShot(dbi,"000",0.0,true);
        //update UI
        newShot(rs,row);
        updater->getCurrentProject()->addShot(rs,row,true);
    }
    //select shot
    shotsAdminList->clearSelection();
    shotsAdminList->setCurrentRow(row);
    on_shotsAdminList_itemClicked(shotsAdminList->item(row));

}

void AdminWidget::on_batchAddShotButton_clicked()
{
    if (updater->getCurrentProject() == 0) return;
    this->setEnabled(false);
    AddShotsDialog as;
    //as.move(this->parentWidget()->geometry().center().x()-as.geometry().width()/2,this->parentWidget()->geometry().center().y()-as.geometry().height()/2);
    if (as.exec())
    {
        QStringList shotNames = as.getShots();
        QList<QStringList> newShots;
        int row = getNewShotRow();
        int newRow = row;
        QString projectId = updater->getCurrentProject()->getId();
        foreach(QString name,shotNames)
        {
            RAMShot *rs = new RAMShot(dbi,name,0.0,false);

            QStringList shot;
            shot << name << "0" << rs->getId();
            newShots << shot;

            //update UI
            newShot(rs,newRow);
            updater->getCurrentProject()->addShot(rs,newRow);
            newRow++;
        }

        dbi->addInsertShots(newShots,projectId,row);
    }

    this->setEnabled(true);
}

void AdminWidget::on_importShotsButton_clicked()
{
    if (updater->getCurrentProject() == 0) return;
    this->setEnabled(false);

    ImporterDialog imp;
    if (imp.exec())
    {
        QString file = imp.getPath();
        QString prefix = imp.getPrefix();
        QString suffix = imp.getSuffix();
        bool audio = imp.getAudio();
        bool video = imp.getVideo();
        if (file == "" || !QFile(file).exists())
        {
            emit message("File not found.","general");
            this->setEnabled(true);
            return;
        }

        if (file.toLower().endsWith(".edl")) importEDL(file);
        else if (file.toLower().endsWith(".xml")) importXML(file,prefix,suffix,video,audio);
        else
        {
            //TODO open file to try to find what it is
        }
    }
    this->setEnabled(true);
}

void AdminWidget::on_shotsAdminList_itemClicked(QListWidgetItem *item)
{
    RAMShot *rs = updater->getCurrentProject()->getShot(item->data(Qt::UserRole).toString());

    shotNameEdit->setText(rs->getName());
    shotDurationSpinBox->setValue(rs->getDuration());
    shotConfigWidget->setEnabled(true);
}

void AdminWidget::on_shotApplyButton_clicked()
{
    int currentRow = shotsAdminList->currentRow();
    if (currentRow < 0) return;

    RAMShot *rs = updater->getCurrentProject()->getShot(shotsAdminList->currentItem()->data(Qt::UserRole).toString());

    QString name = shotNameEdit->text();
    double duration = shotDurationSpinBox->value();

    rs->setName(name);
    rs->setDuration(duration);
    rs->update();

    //update UI
    QListWidgetItem *item = shotsAdminList->item(currentRow);
    item->setText(name);
}

void AdminWidget::on_removeShotButton_clicked()
{
    if (shotsAdminList->currentRow() < 0) return;
    QStringList ids;
    foreach(QListWidgetItem *item,shotsAdminList->selectedItems())
    {
        QString id = item->data(Qt::UserRole).toString();
        ids << id;
        RAMShot *rs = updater->getCurrentProject()->getShot(id);
        if (rs == 0) continue;
        updater->getCurrentProject()->removeShot(rs);
        rs->remove(false);
        delete item;
    }

    dbi->removeShots(ids,updater->getCurrentProject()->getId());

    shotsAdminReset();
}

void AdminWidget::on_renameShotsButton_clicked()
{
    if (shotsAdminList->currentRow() < 0) return;

    this->setEnabled(false);

    QList<RAMShot*> shots;
    foreach(QListWidgetItem *item,shotsAdminList->selectedItems())
    {
        QString id = item->data(Qt::UserRole).toString();
        RAMShot *rs = updater->getCurrentProject()->getShot(id);
        if (rs == 0) continue;
        shots << rs;
    }
    RenameDialog rd(dbi,shots);
    rd.exec();

    this->setEnabled(true);
}

void AdminWidget::on_moveShotUpButton_clicked()
{
    if (shotsAdminList->currentRow() < 0) return;

    QList<QListWidgetItem *> items;

    //sort selection
    QList<int> rows;
    foreach(QListWidgetItem *item,shotsAdminList->selectedItems())
    {
        rows << shotsAdminList->row(item);
    }
    std::sort(rows.begin(),rows.end());

    //if first row is already on top
    if (rows[0] == 0) return;

    foreach(int row,rows)
    {
        items << shotsAdminList->item(row);
    }

    foreach(QListWidgetItem *item,items)
    {
        QString id = item->data(Qt::UserRole).toString();

        RAMShot *rs = updater->getCurrentProject()->getShot(id);
        int index = updater->getCurrentProject()->getShots().indexOf(rs)-1;

        updater->getCurrentProject()->removeShot(rs);
        updater->getCurrentProject()->addShot(rs,index);

        shotsAdminList->takeItem(index+1);
        shotsAdminList->insertItem(index,item);
    }

    //reselect items
    foreach(QListWidgetItem *item,items)
    {
        item->setSelected(true);
    }

    //update db
    resetShotsOrder();
}

void AdminWidget::on_moveShotDownButton_clicked()
{
    if (shotsAdminList->currentRow() < 0) return;

    QList<QListWidgetItem *> items;

    //sort selection
    QList<int> rows;
    foreach(QListWidgetItem *item,shotsAdminList->selectedItems())
    {
        rows << shotsAdminList->row(item);
    }
    std::sort(rows.begin(),rows.end());

    //if last row is already on top
    if (rows[rows.count()-1] == shotsAdminList->count()-1) return;

    for(int i = rows.count() -1 ; i >= 0 ; i--)
    {
        items << shotsAdminList->item(rows[i]);
    }

    foreach(QListWidgetItem *item,items)
    {
        QString id = item->data(Qt::UserRole).toString();

        RAMShot *rs = updater->getCurrentProject()->getShot(id);
        int index = updater->getCurrentProject()->getShots().indexOf(rs)+1;

        updater->getCurrentProject()->removeShot(rs);
        updater->getCurrentProject()->addShot(rs,index);

        shotsAdminList->takeItem(index-1);
        shotsAdminList->insertItem(index,item);
    }

    //reselect items
    foreach(QListWidgetItem *item,items)
    {
        item->setSelected(true);
    }

    //update db
    resetShotsOrder();
}

void AdminWidget::shotsAdminReset()
{
    shotsAdminList->setCurrentRow(-1);
    shotNameEdit->setText("");
    shotDurationSpinBox->setValue(0.0);
    shotConfigWidget->setEnabled(false);
}

void AdminWidget::importEDL(QString f)
{
    emit message("Importing EDL " + f + " (not yet implemented)","warning");
}

void AdminWidget::importXML(QString f,QString prefix,QString suffix,bool video,bool audio)
{
    emit message("Importing XML " + f,"general");

    //open file and load data
    QFile *xmlFile = new QFile(f);
    if (!xmlFile->open(QFile::ReadOnly | QFile::Text))
    {
        emit message("XML File could not be opened.","warning");
        return;
    }
    QByteArray xmlData = xmlFile->readAll();
    xmlFile->close();
    delete xmlFile;

    //create reader
    XMLReader xml(xmlData);

    //parse file to get needed informations
    QList<QStringList> shotsFound;
    QString timebase = "24";
    while(!xml.atEnd())
    {
        if (xml.readNext() == QXmlStreamReader::StartElement)
        {
            //find sequence
            if (xml.name().toString() == "sequence")
            {
                //find rate and media
                while(xml.readNextStartElement())
                {
                    if (xml.name().toString() == "rate")
                    {
                        //find timebase
                        while (xml.readNextStartElement())
                        {
                            if (xml.name() == "timebase") timebase = xml.readElementText();
                            else xml.skipCurrentElement();
                        }
                    }
                    else if (xml.name().toString() == "media")
                    {
                        emit message("Media found","debug");
                        //find video and audio
                        while (xml.readNextStartElement())
                        {
                            if (xml.name().toString() == "video" && video)
                            {
                                if (xml.name().toString() == "video") emit message("Video found","debug");
                                //find track
                                while (xml.readNextStartElement())
                                {
                                    if (xml.name() == "track")
                                    {
                                        emit message("Track found","debug");
                                        //find clipitem
                                        while (xml.readNextStartElement())
                                        {
                                            if (xml.name() == "clipitem")
                                            {
                                                //find name, start, end
                                                QString name = "";
                                                QString start = "0";
                                                QString end = "0";
                                                while (xml.readNextStartElement())
                                                {
                                                    if (xml.name().toString() == "name")
                                                        name = xml.readElementText();
                                                    else if (xml.name().toString() == "start")
                                                        start = xml.readElementText();
                                                    else if (xml.name().toString() == "end")
                                                        end = xml.readElementText();
                                                    else xml.skipCurrentElement();
                                                }
                                                QStringList details;
                                                details << name << start << end;
                                                shotsFound << details;
                                            }
                                            else xml.skipCurrentElement();//<track>
                                        }
                                    }
                                    else xml.skipCurrentElement();//<video>
                                }
                            }
                            else if (xml.name().toString() == "audio" && audio)
                            {
                                if (xml.name().toString() == "video") emit message("Audio found","debug");
                                //find track
                                while (xml.readNextStartElement())
                                {
                                    if (xml.name() == "track")
                                    {
                                        emit message("Track found","debug");
                                        //find clipitem
                                        while (xml.readNextStartElement())
                                        {
                                            if (xml.name() == "clipitem")
                                            {
                                                //find name, start, end
                                                QString name = "";
                                                QString start = "0";
                                                QString end = "0";
                                                while (xml.readNextStartElement())
                                                {
                                                    if (xml.name().toString() == "name")
                                                        name = xml.readElementText();
                                                    else if (xml.name().toString() == "start")
                                                        start = xml.readElementText();
                                                    else if (xml.name().toString() == "end")
                                                        end = xml.readElementText();
                                                    else xml.skipCurrentElement();
                                                }
                                                QStringList details;
                                                details << name << start << end;
                                                shotsFound << details;
                                            }
                                            else xml.skipCurrentElement();//<track>
                                        }
                                    }
                                    else xml.skipCurrentElement();//<video> or <audio>
                                }
                            }
                            else xml.skipCurrentElement();//<media>
                        }
                    }
                    else xml.skipCurrentElement(); //<sequence>
                }
            }
        }
    }

    if (xml.hasError())
    {
          emit message("XML error","warning");
          emit message(xml.errorString(),"warning");
    }

    //Add shots

    //get order (if a row is selected, or else insert after the last row)
    int order = getNewShotRow();
    int newRow = order;

    shotsAdminReset();

    //getProject
    QString projectId = updater->getCurrentProject()->getId();

    QList<QStringList> shotsReady;

    foreach(QStringList s,shotsFound)
    {
        float duration = (s[2].toFloat() - s[1].toFloat()) / timebase.toFloat();
        QString name = prefix + s[0] + suffix;

        //create shot
        RAMShot *rs = new RAMShot(dbi,name,duration,false);

        //build lists for dbi
        QStringList currentShot;
        currentShot << name << QString::number(duration) << rs->getId();
        shotsReady << currentShot;


        //update UI
        newShot(rs,newRow);
        updater->getCurrentProject()->addShot(rs,newRow);
        newRow++;
    }

    dbi->addInsertShots(shotsReady,projectId,order);
}

