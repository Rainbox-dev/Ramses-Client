#ifndef PROGRESSPAGE_H
#define PROGRESSPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLabel>

#include "processmanager.h"

class ProgressPage : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressPage(QWidget *parent = nullptr);

public slots:
    void setText(const QString &text);
    void setTitle(const QString &title);
    void setMaximum(const int &max);
    void setValue(const int &value);

signals:

private slots:

private:
    void setupUi();

    QProgressBar *m_progressBar;
    QLabel *m_titleLabel;

};

#endif // PROGRESSPAGE_H
