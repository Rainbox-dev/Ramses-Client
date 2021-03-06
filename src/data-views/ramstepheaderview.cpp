#include "ramstepheaderview.h"

RamStepHeaderView::RamStepHeaderView(QWidget *parent):
    QHeaderView(Qt::Horizontal, parent)
{
    m_abyss = DuUI::getColor("abyss-grey");
    m_dark = DuUI::getColor("dark-grey");
    m_medium = DuUI::getColor("medium-grey");
    m_lessLight = DuUI::getColor("less-light-grey");
    m_light = DuUI::getColor("light-grey");
    m_textFont = qApp->font();
    m_textFont.setPixelSize( DuUI::getSize("font", "size-medium"));
    m_detailsFont = m_textFont;
    m_detailsFont.setItalic(true);
    m_padding = 10;

    this->setMinimumHeight( 42 );
}

void RamStepHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    if (!painter) return;

    if (logicalIndex == 0) return QHeaderView::paintSection(painter,rect,logicalIndex);

    painter->setRenderHint(QPainter::Antialiasing);

    // Get the step
    quintptr iptr = this->model()->headerData( logicalIndex, Qt::Horizontal, Qt::UserRole).toULongLong();
    if (iptr == 0) return QHeaderView::paintSection(painter,rect,logicalIndex);
    RamStep *step = reinterpret_cast<RamStep*>( iptr );

    // Draw title
    QRect titleRect( rect.left(), rect.top(), rect.width(), rect.height() - 16 );
    QPen textPen( m_lessLight );
    painter->setPen( textPen );
    painter->setFont( m_textFont );
    painter->drawText( titleRect, Qt::AlignCenter | Qt::AlignVCenter | Qt::TextWordWrap, step->name(), &titleRect);

    // Draw status

    // BG Bar
    QBrush statusBrush( m_abyss );
    int statusWidth = rect.width() - 20 ;
    QRect statusRect( rect.left() + 10, titleRect.bottom() + 5, statusWidth, 6 );
    QPainterPath path;
    path.addRoundedRect(statusRect, 5, 5);
    painter->fillPath(path, statusBrush);

    if (m_timeTracking || m_completionRatio)
    {
        // Draw a timebar first
        float latenessRatio = step->latenessRatio();
        if (m_timeTracking && latenessRatio > 0 && step->estimation() > 0)
        {
            QColor timeColor;
            if ( latenessRatio < 1.1 ) timeColor = QColor(32,62,32);
            else if ( latenessRatio < 1.2 ) timeColor = QColor(76,30,0);
            else if ( latenessRatio < 1.3 ) timeColor = QColor(93,31,0);
            else if ( latenessRatio < 1.4 ) timeColor = QColor(118,27,0);
            else if ( latenessRatio < 1.5 ) timeColor = QColor(140,23,0);
            else if ( latenessRatio < 1.6 ) timeColor = QColor(168,14,0);
            else if ( latenessRatio < 1.7 ) timeColor = QColor(188,0,0);
            else if ( latenessRatio < 1.8 ) timeColor = QColor(214,0,0);
            else if ( latenessRatio < 1.9 ) timeColor = QColor(236,0,0);
            else timeColor = QColor(255,0,0);
            statusBrush.setColor( timeColor );

            float timeSpentDays = RamStatus::hoursToDays( step->timeSpent()/3600 );
            float ratio = timeSpentDays / step->estimation();

            statusRect.setWidth( statusWidth * ratio );
            if (statusRect.right() > rect.right() - 10) statusRect.setRight( rect.right() - 10);
            QPainterPath timePath;
            timePath.addRoundedRect(statusRect, 3, 3);
            painter->fillPath(timePath, statusBrush);
        }

        if (m_completionRatio)
        {
            // Set a color according to the completion
            float completionRatio = step->completionRatio() / 100.0;
            QColor completionColor;
            if (completionRatio < 0.12) completionColor = QColor( 197, 0, 0);
            else if (completionRatio < 0.25) completionColor = QColor( 197, 98, 17);
            else if (completionRatio < 0.5) completionColor = QColor( 197, 179, 40);
            else if (completionRatio < 0.75) completionColor = QColor( 128, 197, 37);
            else if (completionRatio < 0.88) completionColor = QColor( 100, 172, 69);
            else if (completionRatio < 0.98) completionColor = QColor( 55, 172, 23);
            else completionColor = QColor( 6, 116, 24);

            // Draw completion ratio
            statusBrush.setColor( completionColor );
            statusRect.setWidth(statusWidth * completionRatio);
            QPainterPath completionPath;
            completionPath.addRoundedRect(statusRect, 5, 5);
            painter->fillPath(completionPath, statusBrush);
        }

    }



}

void RamStepHeaderView::setCompletionRatio(bool newCompletionRatio)
{
    m_completionRatio = newCompletionRatio;
}

void RamStepHeaderView::setTimeTracking(bool newTimeTracking)
{
    m_timeTracking = newTimeTracking;
}
