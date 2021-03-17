#ifndef DUQFNODE_H
#define DUQFNODE_H

#include <QGraphicsObject>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QApplication>

#include "duqf-app/app-style.h"

#include "duqfslot.h"
#include "duqfgrid.h"

class DuQFNode : public QGraphicsObject
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    DuQFNode(QString title = "Node");
    //! Copy constructor.
    DuQFNode(const DuQFNode & other);
    ~DuQFNode() Q_DECL_OVERRIDE;

    enum { Type = UserType + 3 };
    int type() const Q_DECL_OVERRIDE { return Type; }

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr) Q_DECL_OVERRIDE;

    QString title() const;
    void setTitle(const QString &title);
    QString titleToolTip() const;
    void setTitleToolTip(const QString &toolTip);

    void setGrid(DuQFGrid *grid);
    DuQFGrid *grid() const;

    void addChildNode(DuQFNode *childNode);
    void addParentNode(DuQFNode *parentNode);
    void removeChildNode(DuQFNode *childNode);
    void removeParentNode(DuQFNode *parentNode);

    QList<DuQFNode *> childNodes() const;
    QList<DuQFNode *> parentNodes() const;
    bool isOrphan() const;
    bool hasChildren() const;
    bool hasParents() const;

public slots:
    void remove();

signals:
    void connectionInitiated(QPointF);
    void connectionMoved(QPointF);
    void connectionFinished(QPointF, QPointF);
    void removed();
    void moved(QPointF);

protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

private:
    DuQFGrid *m_grid;

    bool m_removing = false;
    // Appearance
    int m_cornerRadius = 5;
    int m_padding = 5;
    QRectF m_boundingRect;

    // Children
    QGraphicsTextItem *m_titleItem;
    DuQFSlot *m_defaultInputConnector;
    DuQFSlot *m_defaultOutputConnector;

    // Connected nodes
    QList<DuQFNode*> m_childrenNodes;
    QList<DuQFNode*> m_parentNodes;
};

#endif // DUQFNODE_H
