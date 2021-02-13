#include "helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    writeDebugToFile = false;

    toolBar = new QToolBar(this);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addAction(actionHelp);
    toolBar->addAction(actionDebug);
    toolBar->addAction(actionAbout);

    //Add window buttons
    SeparatorWidget *sW = new SeparatorWidget();
    QPushButton *quitButton = new QPushButton(QIcon(":/icons/close"),"");
    connect(quitButton,SIGNAL(clicked()),this,SLOT(reject()));
    toolBar->addWidget(sW);
    dockButton = new QPushButton();
    dockButton->setIcon(QIcon(":/icons/link"));
    dockButton->setCheckable(true);
    dockButton->setChecked(true);
    connect(dockButton,SIGNAL(clicked(bool)),this,SLOT(dockButton_clicked(bool)));
    toolBar->addWidget(dockButton);
    toolBar->addWidget(quitButton);

    this->layout()->setMenuBar(toolBar);

    this->setWindowFlags(Qt::FramelessWindowHint);

    toolBarClicked = false;
    toolBar->installEventFilter(this);

    showHelp(0);
    showPage(0);
}

// ======== GENERAL

void HelpDialog::showPage(int i)
{
    mainWidget->setCurrentIndex(i);

    actionAbout->setChecked(false);
    actionDebug->setChecked(false);
    actionHelp->setChecked(false);

    switch (i)
    {
    case 0:
        actionHelp->setChecked(true);
        break;
    case 1:
        actionDebug->setChecked(true);
        break;
    case 2:
        actionAbout->setChecked(true);
        break;
    default:
        break;
    }
}

void HelpDialog::showDebug(QString m,QString type)
{
    if (type =="remote" && !remoteDebugButton->isChecked()) return;
    if (type =="local" && !localDebugButton->isChecked()) return;
    if (type =="connexion" && !networkDebugButton->isChecked()) return;
    if (type =="debug") return;
    QString message = "\n\n";
    message += type.toUpper();
    message += "\n";
    QDateTime currentTime = QDateTime::currentDateTime();
    message += currentTime.toString("[yyyy-MM-dd HH:mm:ss] ");
    message += m;
    debugText->setPlainText(debugText->toPlainText() + message);
    debugText->moveCursor(QTextCursor::End);

    if (writeDebugToFile)
    {
        if (debugFile.open(QIODevice::Append | QIODevice::Text))
        {
            QTextStream out(&debugFile);
            out.setCodec("UTF-8");
            out << message;
            debugFile.close();
        }

    }
}

void HelpDialog::showHelp(int i)
{
    helpTabs->setCurrentIndex(i);
}

void HelpDialog::setToolButtonStyle(int s)
{
    if (s == 0)
    {
        toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    }
    else if (s == 1)
    {
        toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    }
    else if (s == 2)
    {
        toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    else if (s == 3)
    {
        toolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    }
}

// ======== ACTIONS

void HelpDialog::on_actionAbout_triggered()
{
    showPage(2);
}

void HelpDialog::on_actionDebug_triggered()
{
    showPage(1);
}

void HelpDialog::on_actionHelp_triggered()
{
    showPage(0);
}

// ========= BUTTONS

void HelpDialog::dockButton_clicked(bool checked)
{
    emit dock(checked);
}

// ======== EVENTS

void HelpDialog::hideEvent(QHideEvent *e)
{
    // 'false' means hidden..
    emit visibilityChanged(false);
    QDialog::hideEvent(e);
}

void HelpDialog::showEvent(QShowEvent *e)
{
    // 'true' means visible..
    emit visibilityChanged(true);
    QDialog::showEvent(e);
}

bool HelpDialog::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseButtonPress)
  {
      QMouseEvent *mouseEvent = (QMouseEvent*)event;
      if (mouseEvent->button() == Qt::LeftButton)
      {
        toolBarClicked = true;
        dragPosition = mouseEvent->globalPos() - this->frameGeometry().topLeft();
        event->accept();
      }
      return true;
  }
  else if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *mouseEvent = (QMouseEvent*)event;
    if (mouseEvent->buttons() & Qt::LeftButton && toolBarClicked)
    {
        if (this->isMaximized()) this->showNormal();
      this->move(mouseEvent->globalPos() - dragPosition);
        emit dock(false);
        dockButton->setChecked(false);
      event->accept();
    }
    return true;
  }
  else if (event->type() == QEvent::MouseButtonRelease)
  {
      toolBarClicked = false;
      return true;
  }
  else
  {
      // standard event processing
      return QObject::eventFilter(obj, event);
  }
}

void HelpDialog::on_fileDebugButton_clicked(bool checked)
{
    if (checked)
    {
        QString filePath = QFileDialog::getSaveFileName(this,"Select a log file","","Log Files (*.log)");
        if (filePath != "")
        {
            debugFile.setFileName(filePath);
            writeDebugToFile = true;
        }
        else
        {
            writeDebugToFile = false;
            fileDebugButton->setChecked(false);
        }
    }
    else
    {
        writeDebugToFile = false;
    }
}
