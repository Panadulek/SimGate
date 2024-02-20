#ifndef HITBOX_H
#define HITBOX_H
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QPalette>
#include <set>
#include <QGraphicsScene>
#include <utility>

class Wire;
namespace HitBox
{

class HitBox : public QGraphicsProxyWidget
{
    Q_OBJECT
protected:
    QPushButton* m_button;
    void* m_parentOwner = nullptr;
    HitBox(QGraphicsItem* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags()) : QGraphicsProxyWidget(parent, flags)
    {
        m_parentOwner = (void*)parent ;
        m_button = new QPushButton(nullptr);
        setWidget(m_button);
        constexpr QSize maxSize(20, 10);
        m_button->setMaximumSize(maxSize);
        QPalette pal = m_button->palette();
        pal.setColor(QPalette::Button, QColor(Qt::blue));
        m_button->setAutoFillBackground(true);
        m_button->setPalette(pal);
        m_button->update();
    }
public:
    QPushButton* ButtonHitBox(){return m_button;}
    void* getOwner(){return m_parentOwner;}
    virtual ~HitBox()
    {}
};

enum class GATE_HIT_BOX_TYPE : uint8_t
{
    INPUT_1 = 0,
    INPUT_2,
    OUTPUT
};
enum class ColorOutput : uint8_t
{
    Red = 0,
    Green = 1,
    Black = 2
};

class GateHitBox : public HitBox
{
Q_OBJECT
protected:
    GATE_HIT_BOX_TYPE m_typeHitBox;
    Wire* m_wire = nullptr;
    std::vector<Wire*> m_outputWires;
    GateHitBox(  GATE_HIT_BOX_TYPE type, QGraphicsItem* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    bool m_input;
public:
    void setupWire(Wire* wire);
    void fixLine();
    bool isInput()
    {
        return m_input;
    }
    Wire* getWire(){return m_wire;}
    void removeWire(Wire*);
    const std::vector<Wire*>& getOutputs()
    {
        return m_outputWires;
    }
    virtual ~GateHitBox();

};



template<GATE_HIT_BOX_TYPE HITBOX_TYPE>
class AndHitBox : public GateHitBox
{

public:
    AndHitBox(QGraphicsItem* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags()) : GateHitBox(HITBOX_TYPE, parent, flags)
    {
        auto gate = this->parentItem();
        auto rect = gate->sceneBoundingRect();
        qreal x1, y1, w, h;
        rect.getRect(&x1, &y1, &w, &h);

        if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_1)
        {
            y1 = (0.27 * h);
            m_input = true;
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_2)
        {
            y1 = (0.61 * h);
            m_input = true;
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::OUTPUT)
        {
            y1 = (0.44 * h);
            x1 = (0.90 * w);
            m_input = false;
        }
        setPos(QPointF(x1, y1));
    }
};


template<GATE_HIT_BOX_TYPE HITBOX_TYPE>
class NandHitBox : public GateHitBox
{
public:
    NandHitBox(QGraphicsItem* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags()) : GateHitBox(HITBOX_TYPE, parent, flags)
    {
        auto gate = this->parentItem();
        auto rect = gate->sceneBoundingRect();
        qreal x1, y1, w, h;
        rect.getRect(&x1, &y1, &w, &h);

        if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_1)
        {
            y1 = (0.27 * h);
            m_input = true;
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_2)
        {
            y1 = (0.61 * h);
            m_input = true;
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::OUTPUT)
        {
            y1 = (0.44 * h);
            x1 = (0.90 * w);
            m_input = false;
        }
        setPos(QPointF(x1, y1));
    }
};

template<GATE_HIT_BOX_TYPE HITBOX_TYPE>
class OrHitBox : public GateHitBox
{
public:
    OrHitBox(QGraphicsItem* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags()) : GateHitBox(HITBOX_TYPE, parent, flags)
    {
        auto gate = this->parentItem();
        auto rect = gate->sceneBoundingRect();
        qreal x1, y1, w, h;
        rect.getRect(&x1, &y1, &w, &h);

        if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_1)
        {
            y1 = (0.31 * h);
            m_input = true;
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_2)
        {
            y1 = (0.60 * h);
            m_input = true;
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::OUTPUT)
        {
            y1 = (0.46 * h);
            x1 = (0.90 * w);
            m_input = false;
        }
        setPos(QPointF(x1, y1));
    }
};

template<GATE_HIT_BOX_TYPE HITBOX_TYPE>
class NorHitBox : public GateHitBox
{
public:
    NorHitBox(QGraphicsItem* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags()) : GateHitBox(HITBOX_TYPE, parent, flags)
    {
        auto gate = this->parentItem();
        auto rect = gate->sceneBoundingRect();
        qreal x1, y1, w, h;
        rect.getRect(&x1, &y1, &w, &h);

        if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_1)
        {
            y1 = (0.31 * h);
            m_input = true;
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_2)
        {
            y1 = (0.60 * h);
            m_input = true;
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::OUTPUT)
        {
            y1 = (0.46 * h);
            x1 = (0.90 * w);
            m_input = false;
        }
        setPos(QPointF(x1, y1));
    }
};



template<GATE_HIT_BOX_TYPE HITBOX_TYPE>
class XorHitBox : public GateHitBox
{
public:
    XorHitBox(QGraphicsItem* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags()) : GateHitBox(HITBOX_TYPE, parent, flags)
    {
        auto gate = this->parentItem();
        auto rect = gate->sceneBoundingRect();
        qreal x1, y1, w, h;
        rect.getRect(&x1, &y1, &w, &h);

        if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_1)
        {
            y1 = (0.31 * h);
            m_input = true;
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_2)
        {
            y1 = (0.60 * h);
            m_input = true;
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::OUTPUT)
        {
            y1 = (0.46 * h);
            x1 = (0.90 * w);
            m_input = false;
        }
        setPos(QPointF(x1, y1));
    }
};

template<GATE_HIT_BOX_TYPE HITBOX_TYPE>
class NotHitBox : public GateHitBox
{
public:
    NotHitBox(QGraphicsItem* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags()) : GateHitBox(HITBOX_TYPE, parent, flags)
    {
        auto gate = this->parentItem();
        auto rect = gate->sceneBoundingRect();
        qreal x1, y1, w, h;
        rect.getRect(&x1, &y1, &w, &h);

        if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_1)
        {
            y1 = (0.46 * h);
            m_input = true;
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_2)
        {
            throw std::exception();
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::OUTPUT)
        {
            y1 = (0.46 * h);
            x1 = (0.92 * w);
            m_input = false;
        }
        setPos(QPointF(x1, y1));
    }
};



template<GATE_HIT_BOX_TYPE HITBOX_TYPE>
class SignalHitBox : public GateHitBox
{
public:
    SignalHitBox(QGraphicsItem* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags()) : GateHitBox(HITBOX_TYPE, parent, flags)
    {
        auto gate = this->parentItem();
        auto rect = gate->sceneBoundingRect();
        qreal x1, y1, w, h;
        rect.getRect(&x1, &y1, &w, &h);

        if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_1)
        {
            throw std::exception();
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_2)
        {
            throw std::exception();
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::OUTPUT)
        {
            constexpr QSize maxSize(10, 20);
            m_button->setMaximumSize(maxSize);
            y1 = (0.80 * h);
            x1 = (0.40 * w);
            m_input = false;
        }
        setPos(QPointF(x1, y1));
    }
};

template<GATE_HIT_BOX_TYPE HITBOX_TYPE>
class OutputHitBox : public GateHitBox
{
public:
    OutputHitBox(QGraphicsItem* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags()) : GateHitBox(HITBOX_TYPE, parent, flags)
    {
        auto gate = this->parentItem();
        auto rect = gate->sceneBoundingRect();
        qreal x1, y1, w, h;
        rect.getRect(&x1, &y1, &w, &h);

        if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_1)
        {
            constexpr QSize maxSize(10, 20);
            m_button->setMaximumSize(maxSize);
            y1 = (0.80 * h);
            x1 = (0.40 * w);
            m_input = true;
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::INPUT_2)
        {
            throw std::exception();
        }
        else if(HITBOX_TYPE == GATE_HIT_BOX_TYPE::OUTPUT)
        {
            throw std::exception();
        }
        setPos(QPointF(x1, y1));
    }
};


void HitBoxConnect(GateHitBox* input, GateHitBox* output);
void HitBoxConnectRef(GateHitBox*& input, GateHitBox*& output);

};
#endif // HITBOX_H
