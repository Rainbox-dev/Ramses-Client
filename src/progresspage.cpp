#include "progresspage.h"

ProgressPage::ProgressPage(QWidget *parent) : QWidget(parent)
{
    setupUi();

    ProcessManager *pm = ProcessManager::instance();
    connect(pm, &ProcessManager::text, this, &ProgressPage::setText);
    connect(pm, &ProcessManager::title, this, &ProgressPage::setTitle);
    connect(pm, &ProcessManager::progress, this, &ProgressPage::setValue);
    connect(pm, &ProcessManager::maximum, this, &ProgressPage::setMaximum);
}

void ProgressPage::setText(const QString &text)
{
    m_progressBar->setFormat(text);
}

void ProgressPage::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
}

void ProgressPage::setMaximum(const int &max)
{
    m_progressBar->setMaximum(max);
}

void ProgressPage::setValue(const int &value)
{
    m_progressBar->setValue(value);
}

void ProgressPage::setupUi()
{
    QVBoxLayout *l = new QVBoxLayout();
    l->setContentsMargins(100,0,100,0);

    l->addStretch();

    m_titleLabel = new QLabel("", this);
    l->addWidget(m_titleLabel);

    m_progressBar = new QProgressBar(this);
    l->addWidget(m_progressBar);

    l->addStretch();

    this->setLayout(l);
}
