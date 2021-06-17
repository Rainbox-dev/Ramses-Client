#ifndef RAMSTATUSWIDGET_H
#define RAMSTATUSWIDGET_H

#include <QPlainTextEdit>
#include <QSettings>

#include "duqf-widgets/duqfslider.h"

#include "ramobjectwidget.h"
#include "ramses.h"
#include "statuseditwidget.h"

/**
 * @brief The RamStatusWidget class is a small widget used to show a Status anywhere in the UI
 */
class RamStatusWidget : public RamObjectWidget
{
    Q_OBJECT
public:
    RamStatusWidget(RamStatus *status, QWidget *parent = nullptr);
    RamStatus *status() const;

private slots:
    void remove();
    void statusChanged(RamObject *o);
    void updateStatus(RamState*state, int completion, int version, QString comment);

private:
    RamStatus *_status;

    void completeUi();
    void connectEvents();
    DuQFSlider *completionBox;
    QLabel *userLabel;
    StatusEditWidget *statusEditWidget;

    bool _removing = false;
};

#endif // RAMSTATUSWIDGET_H
