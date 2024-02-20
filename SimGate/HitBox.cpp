#include "HitBox.h"
#include "Wire.h"
#include <algorithm>
HitBox::GateHitBox::GateHitBox(GATE_HIT_BOX_TYPE type, QGraphicsItem* parent, Qt::WindowFlags flags): HitBox(parent, flags), m_typeHitBox(type)
{
    QObject::connect(m_button, &QPushButton::released, [this](){
        static GateHitBox* output = nullptr;
        static GateHitBox* input = nullptr;

        if(m_wire && isInput()) // jako input nie mozemy miec 2 polaczen
        {
            output = nullptr;
            input = nullptr;
            return;
        }

        if((isInput() && input) || (!isInput() && output)) // pewnosc ze inputa nie polaczymy z inputem i outputa z outputem.
        {
            output = nullptr;
            input = nullptr;
            return;
        }

        if(!input && isInput())
        {
            input = this;
        }

        if(!output && !isInput())
        {
            output = this;
        }
        HitBoxConnectRef(input, output);

    });
}

void HitBox::GateHitBox::setupWire(Wire* wire)
{
    if(isInput())
        m_wire = wire;
    else
        m_outputWires.push_back(wire);
}
void HitBox::GateHitBox::fixLine()
{
    if(isInput() && m_wire)
    {
        m_wire->drawWire();
    }
    else
    {
        for(auto& wire : m_outputWires)
            wire->drawWire();
    }
}
void HitBox::GateHitBox::removeWire(Wire* wire)
{
    if(wire == m_wire)
        m_wire = nullptr;
    else
    {
        auto iterator = std::find(m_outputWires.begin(), m_outputWires.end(), wire);
        if(iterator == m_outputWires.end())
            return;
        m_outputWires.erase(iterator);
    }
}

HitBox::GateHitBox::~GateHitBox()
{
    bool _isInput = isInput();
    if(_isInput && m_wire)
    {
        delete m_wire;
    }
   else if(!_isInput)
    {
        for(size_t i = 0; i < m_outputWires.size(); i++)
        {
            auto it = m_outputWires[i];
            if(it)
            {
                delete it;
                --i;
            }
        }
    }
}

void HitBox::HitBoxConnect(GateHitBox* input, GateHitBox* output)
{
    if(input && output)
    {
        auto _input = input->getOwner();
        auto _output = output->getOwner();
        if(_input == _output)
        {
            input = nullptr;
            output = nullptr;
            return;
        }
        auto wire = new Wire(input, output);
        input->setupWire(wire);
        output->setupWire(wire);
        input = nullptr;
        output = nullptr;
    }
}
void HitBox::HitBoxConnectRef(GateHitBox*& input, GateHitBox*& output)
{
    if(input && output)
    {
        if(input->getOwner() == output->getOwner())
        {
            input = nullptr;
            output = nullptr;
            return;
        }
        auto wire = new Wire(input, output);
        input->setupWire(wire);
        output->setupWire(wire);
        input = nullptr;
        output = nullptr;
    }
}
