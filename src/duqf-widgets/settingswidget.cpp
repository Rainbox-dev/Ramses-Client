#include "settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi();

    connect(mainList, SIGNAL(currentRowChanged(int)), this, SLOT(mainList_currentRowChanged(int)));
}

void SettingsWidget::setupUi()
{
    this->setObjectName(QStringLiteral("SettingsWidget"));
    this->resize(878, 479);
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    mainList = new QListWidget(this);
    mainList->setObjectName(QStringLiteral("mainList"));
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(mainList->sizePolicy().hasHeightForWidth());
    mainList->setSizePolicy(sizePolicy);
    mainList->setFrameShape(QFrame::NoFrame);
    mainList->setLineWidth(0);
    mainList->setResizeMode(QListView::Fixed);
    mainList->setLayoutMode(QListView::SinglePass);

    horizontalLayout->addWidget(mainList);

    mainStackWidget = new QStackedWidget(this);
    mainStackWidget->setObjectName(QStringLiteral("mainStackWidget"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(mainStackWidget->sizePolicy().hasHeightForWidth());
    mainStackWidget->setSizePolicy(sizePolicy1);

    horizontalLayout->addWidget(mainStackWidget);

    horizontalLayout->setStretch(0, 1);
    horizontalLayout->setStretch(1, 10);

    mainStackWidget->setCurrentIndex(-1);
}

void SettingsWidget::addPage(QWidget *ui, QString title, QIcon icon)
{
    mainStackWidget->addWidget(ui);
    QListWidgetItem *tab = new QListWidgetItem(icon,title);
    mainList->addItem(tab);
}

void SettingsWidget::setCurrentIndex(int index)
{
    mainStackWidget->setCurrentIndex(index);
}

void SettingsWidget::mainList_currentRowChanged(int currentRow)
{
    mainStackWidget->setCurrentIndex(currentRow);
}
