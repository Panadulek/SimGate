#ifndef WIRE_H
#define WIRE_H
#include "HitBox.h"
#include <QGraphicsLineItem>
#include <QGraphicsScene>
class Wire final : public QObject
{
    QGraphicsLineItem* m_line = nullptr;
    HitBox::GateHitBox* m_input = nullptr;
    HitBox::GateHitBox* m_output = nullptr;
    QGraphicsScene* m_scene = nullptr;
    QColor m_color = Qt::black;
    bool m_valid = true;
    bool m_defaultDelete = false;



public:
    class GraphicsWire final : public QGraphicsLineItem
    {
        Wire* m_info;
    public:
        GraphicsWire(Wire* info, qreal x1, qreal y1, qreal x2, qreal y2) : QGraphicsLineItem(x1, y1, x2, y2), m_info(info)
        {}
        Wire* getParent()
        {
            return m_info;
        }
    };


    QGraphicsLineItem *getLine() const {return m_line;}
    Wire(HitBox::GateHitBox* input, HitBox::GateHitBox* output) : QObject(nullptr)
    {
        if(!input || (input == output) || !output )
            m_valid = false;
        if(m_valid)
        {
            m_input = input;
            m_output = output;
            m_scene = m_input->scene();
            if(m_scene != m_output->scene())
                m_valid = false;
            if(m_valid)
                drawWire();
        }
    }

    void drawWire()
    {
        if(m_line)
        {
            m_scene->removeItem(m_line);
            auto pos = m_output->scenePos();
            auto pos2 = m_input->scenePos();
            auto pen = m_line->pen();
            pen.setColor(m_color);
            m_line->setPen(pen);
            m_line->setLine(pos.rx(), pos.ry(), pos2.rx(), pos2.ry());
            m_scene->addItem(m_line);
        }
        else
        {
            auto pos = m_output->scenePos();
            auto pos2 = m_input->scenePos();
            if(pos == pos2)
            {
                m_line = nullptr;
            }
            m_line = new GraphicsWire(this, pos.rx(), pos.ry(), pos2.rx(), pos2.ry());
            auto pen = m_line->pen();
            pen.setColor(m_color);
            m_line->setPen(pen);
            m_scene->addItem(m_line);
            m_line->setFlag(QGraphicsItem::ItemIsSelectable, true);
        }
    }
    void setColor(const QColor& color)
    {
        m_color = color;
    }
    HitBox::GateHitBox* Input() // <> Output --- > Input <>
    {
        return m_output;
    }
    ~Wire()
    {
        delete m_line;
        m_input->removeWire(this);
        m_output->removeWire(this);

     }

};


#endif // WIRE_H
