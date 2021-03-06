#ifndef LOCALSETTINGSWIDGET_H
#define LOCALSETTINGSWIDGET_H

#include "ui_localsettingswidget.h"

#include <QDateTime>

#include "duqf-widgets/duqffolderselectorwidget.h"
#include "ramses.h"

class LocalSettingsWidget : public QWidget, private Ui::LocalSettingsWidget
{
    Q_OBJECT

public:
    explicit LocalSettingsWidget(QWidget *parent = nullptr);
private slots:
    void setRamsesPath(QString p);
    void setDateFormat();
private:
    DuQFFolderSelectorWidget *folderSelector;
    QSettings m_settings;
};

#endif // LOCALSETTINGSWIDGET_H
