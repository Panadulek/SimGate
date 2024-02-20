#ifndef SCHEMEAREA_H
#define SCHEMEAREA_H

#include <QGraphicsScene>
#include <QObject>
#include <QWidget>
#include <QGraphicsPixmapItem>
#include "GatePaths.h"
#include <string>
#include <QKeyEvent>
#include <set>
#include <tuple>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include "HitBox.h"
#include <QVariant>
#include "Wire.h"
#include "simulator.h"
#include <QRandomGenerator64>
#include <Common.h>
#include <cstdlib>
#include <QGraphicsView>
#include <QGraphicsSimpleTextItem>
#include <map>


namespace Scheme
{
    class GateEmitter : public QObject
    {
         Q_OBJECT
        GateEmitter(){}
        GateEmitter(const GateEmitter& t) = delete;
        GateEmitter(const GateEmitter&& t) = delete;
        GateEmitter& operator = (const GateEmitter&) = delete;
        GateEmitter& operator=(GateEmitter&&) = delete;
        signals:
            void changeInput();
        friend class Gate;
    };
    class Gate final : public QGraphicsPixmapItem
    {
    public:

        using GateType = GatePaths::Gate_Path_Idx;

     private:
        GateType m_type;
        bool m_valid = false;
        std::vector<HitBox::GateHitBox*> m_hitboxes;
        bool m_isOutput;
        quint64 m_id;
        GateEmitter m_emitter;
        QGraphicsSimpleTextItem* m_label{nullptr};
        void changeTypeInput()
        {
            const auto paths = GatePaths::instance();
            if(paths)
            {
                QPixmap _pixmap = pixmap();
                if(m_type == GateType::LOW_STATE )
                {
                    _pixmap.load(paths->HighState());
                    m_type = GateType::HIGH_STATE;
                }
                else if(m_type == GateType::HIGH_STATE)
                {
                    _pixmap.load(paths->LowState());
                    m_type = GateType::LOW_STATE;
                }
                setPixmap(_pixmap);
                emit m_emitter.changeInput();
            }
        }
    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override
        {
            if(event->button() == Qt::RightButton)
                changeTypeInput();
            QGraphicsPixmapItem::mousePressEvent(event);
        }
    public:
        Gate(const GateType& type, QGraphicsItem* parent = nullptr, bool isOutput = false):
            QGraphicsPixmapItem(parent), m_type(type), m_isOutput(isOutput), m_id(G_NOT_VALID_GATE_ID)
        {
            auto createPixmap = [&](const char*path) -> bool
            {
                if(!path || !*path)
                    return false;
                QPixmap pixmap(path);
                setPixmap(pixmap);
                return true;
            };
            const GatePaths* path = GatePaths::instance();
            switch(type)
            {
            case GateType::AND:
                m_valid = createPixmap(path->instance()->AndPath());
                m_hitboxes.push_back(new HitBox::AndHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_1>(this));
                m_hitboxes.push_back(new HitBox::AndHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_2>(this));
                m_hitboxes.push_back(new HitBox::AndHitBox<HitBox::GATE_HIT_BOX_TYPE::OUTPUT>(this));
                break;
            case GateType::NAND:
                m_valid = createPixmap(path->instance()->NandPath());
                m_hitboxes.push_back(new HitBox::NandHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_1>(this));
                m_hitboxes.push_back(new HitBox::NandHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_2>(this));
                m_hitboxes.push_back(new HitBox::NandHitBox<HitBox::GATE_HIT_BOX_TYPE::OUTPUT>(this));
                break;
            case GateType::OR:
                m_valid = createPixmap(path->instance()->OrPath());
                m_hitboxes.push_back(new HitBox::OrHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_1>(this));
                m_hitboxes.push_back(new HitBox::OrHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_2>(this));
                m_hitboxes.push_back(new HitBox::OrHitBox<HitBox::GATE_HIT_BOX_TYPE::OUTPUT>(this));
                break;
            case GateType::NOR:
                m_valid = createPixmap(path->instance()->NorPath());
                m_hitboxes.push_back(new HitBox::NorHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_1>(this));
                m_hitboxes.push_back(new HitBox::NorHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_2>(this));
                m_hitboxes.push_back(new HitBox::NorHitBox<HitBox::GATE_HIT_BOX_TYPE::OUTPUT>(this));
                break;
            case GateType::XOR:
                m_valid = createPixmap(path->instance()->XorPath());
                m_hitboxes.push_back(new HitBox::XorHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_1>(this));
                m_hitboxes.push_back(new HitBox::XorHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_2>(this));
                m_hitboxes.push_back(new HitBox::XorHitBox<HitBox::GATE_HIT_BOX_TYPE::OUTPUT>(this));
                break;
            case GateType::NOT:
                m_valid = createPixmap(path->instance()->NotPath());
                m_hitboxes.push_back(new HitBox::NotHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_1>(this));
                m_hitboxes.push_back(new HitBox::NotHitBox<HitBox::GATE_HIT_BOX_TYPE::OUTPUT>(this));
                break;
            case GateType::HIGH_STATE:
                 m_valid = createPixmap(path->instance()->HighState());
                 m_hitboxes.push_back(new HitBox::SignalHitBox<HitBox::GATE_HIT_BOX_TYPE::OUTPUT>(this));
                break;
            case GateType::LOW_STATE:
                m_valid = createPixmap(path->instance()->LowState());
                m_hitboxes.push_back(new HitBox::SignalHitBox<HitBox::GATE_HIT_BOX_TYPE::OUTPUT>(this));
                break;
            case GateType::OUTPUT:
                m_valid = createPixmap(path->instance()->OutputPath());
                m_hitboxes.push_back(new HitBox::OutputHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_1>(this));
                break;
            default:
                m_valid = createPixmap(nullptr);
            };
            if(m_valid)
            {
                setFlag(QGraphicsItem::ItemIsMovable, true);
                setFlag(QGraphicsItem::ItemIsSelectable, true);
                setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
            }
        }

        bool isValid()
        {
            if(m_id == G_NOT_VALID_GATE_ID)
                return false;
            return m_valid;
        }
        bool isOutput()
        {
            return m_isOutput;
        }
        GateType getType(){return m_type;}
        virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override
        {
            for(auto& hitbox : m_hitboxes)
            {
                hitbox->fixLine();
            }
            return QGraphicsItem::itemChange(change, value);
        }
        template<HitBox::GATE_HIT_BOX_TYPE type>
        Gate* getInput()
        {
            if(m_type == GateType::HIGH_STATE || m_type == GateType::LOW_STATE)
                return nullptr;
            switch(type)
            {
                case HitBox::GATE_HIT_BOX_TYPE::INPUT_1:
            {
                    auto wire = m_hitboxes[0]->getWire();
                    return wire ? reinterpret_cast<Gate*>(wire->Input()->getOwner()) : nullptr; // tutaj zaimplementowac algorytm do wyciagniecia gate'a z wire'a

            }
                case HitBox::GATE_HIT_BOX_TYPE::INPUT_2:
                    if(m_type == GateType::NOT || m_type == GateType::OUTPUT)
                        return nullptr;
                    return reinterpret_cast<Gate*>(m_hitboxes[1]->getWire()->Input()->getOwner()); // tutaj zaimplementowac algorytm do wyciagniecia gate'a z wire'a
                default:
                    return nullptr;
            }
        }
        template<HitBox::GATE_HIT_BOX_TYPE type>
        HitBox::GateHitBox* getHitBox()
        {
            if(m_type == Scheme::Gate::GateType::HIGH_STATE || m_type == Scheme::Gate::GateType::LOW_STATE)
            {
                if(type == HitBox::GATE_HIT_BOX_TYPE::OUTPUT)
                {
                    return m_hitboxes[0];
                }
                 return nullptr;
            }
            if(m_type == Scheme::Gate::GateType::OUTPUT)
            {
                if(type == HitBox::GATE_HIT_BOX_TYPE::INPUT_1)
                    return m_hitboxes[0];
                else
                    return nullptr;
            }
            if(m_type == Scheme::Gate::GateType::NOT)
            {
                if(type == HitBox::GATE_HIT_BOX_TYPE::INPUT_1)
                    return m_hitboxes[0];
                else if(type == HitBox::GATE_HIT_BOX_TYPE::OUTPUT)
                    return m_hitboxes[1];
                else
                    return nullptr;
            }
           return m_hitboxes[static_cast<uint8_t>(type)];
        }
        void setId(const uint64_t& id)
        {
            if(isValid())
                return;
            m_id = id;
        }
       const quint64& Id() const
       {
           return m_id;
       }
       GateEmitter* emitter()
       {
            return  &m_emitter;
       }
        void changeOutput(const Common_Value_Output value)
        {
            QString path;
            switch(value)
            {
                case Common_Value_Output::HIGH:
                    path = GatePaths::instance()->HighState();
                break;
                case Common_Value_Output::LOW:
                    path = GatePaths::instance()->LowState();
                break;
                case Common_Value_Output::UNKNOWN:
                    path = GatePaths::instance()->OutputPath();
                break;
            }
            auto pixmap = this->pixmap();
            pixmap.load(path);
            this->setPixmap(pixmap);

        }

        void createDesc(const QString& desc)
        {
            if(desc.isEmpty())
            {
                clearDesc();
                return;
            }
            if(!m_label)
                m_label = new QGraphicsSimpleTextItem(desc, this);
            const QRectF rect = m_label->sceneBoundingRect();
            qreal x, y, w, h;
            rect.getRect(&x, &y, &w, &h);
            m_label->setPos(w/4, -h);
        }
        void clearDesc()
        {
            if(!m_label)
                return;
            m_label->setText("");
            m_label->setParentItem(nullptr);
            delete m_label;
            m_label = nullptr;
        }

    };



    class Area : public QGraphicsScene
    {
        Q_OBJECT
        std::set<Scheme::Gate*> m_gates;
        std::set<uint64_t> m_usedGates;
        std::set<Scheme::Gate*> m_outputs;
        void KeyDeleteReaction()
        {
            auto list = this->selectedItems();
            if(list.size() == 1)
            {
                QGraphicsItem* item = dynamic_cast<Scheme::Gate*>(list[0]);
                if(item)
                {
                    Scheme::Gate* gate = reinterpret_cast<Scheme::Gate*>(item);
                    if(gate)
                    {
                        auto toRemove = m_outputs.find(gate);
                        if(toRemove != m_outputs.end())
                            m_outputs.erase(toRemove);
                    }
                    auto iterator = m_gates.find(gate);
                    if(iterator != m_gates.end())
                    {
                        m_gates.erase(iterator);
                        removeItem(item);
                        delete item;
                    }
                    if(SimulatorFlag::FlagInstance()())
                    {
                        clearSimulate();
                    }
                }
                else
                {
                    Wire::GraphicsWire* wire = reinterpret_cast<Wire::GraphicsWire*>(list[0]);
                    delete wire->getParent();
                }
            }
        }

        SimMap createSimMap()
        {
            SimMap map;
            for(auto it : m_gates)
            {
                auto input1 = it->getInput<HitBox::GATE_HIT_BOX_TYPE::INPUT_1>();
                auto input2 = it->getInput<HitBox::GATE_HIT_BOX_TYPE::INPUT_2>();
                map.insert(decltype(map)::value_type(BinaryTree::Common_Data(it->getType(), it->Id()), std::make_pair(input1 ? input1->Id() : G_NOT_VALID_GATE_ID, input2 ? input2->Id() : G_NOT_VALID_GATE_ID)));
            }
            return map;
        }
    public:
        explicit Area(QObject *parent = nullptr);
        void saveProject();
        void loadProject();
        SimMap buildTree()
        {
            return createSimMap();
        }
        bool addGate(Scheme::Gate* gate)
        {
            bool inserted = false;
            std::set<Scheme::Gate*>::iterator iterator;
            std::tie(iterator, inserted) = m_gates.insert(gate);
            if(!inserted || iterator == m_gates.end())
            {
                delete gate;
                return false;
            }
            else
                this->addItem(gate);
            return true;
        }
        void clearGates()
        {
            m_gates.clear();
            m_usedGates.clear();
            m_outputs.clear();
        }
        void simulate()
        {
            SimMap map = buildTree();
            Simulator sim;
            sim.Sim(map);
            SimulatorFlag::FlagInstance()(true);
            for(auto& it : m_gates)
            {
                if(it->isOutput())
                {
                    it->changeOutput(sim.getValue(it->Id()));
                    continue;
                }
                Common_Value_Output retVal = sim.getValue(it->Id());
                auto wires = it->getHitBox<HitBox::GATE_HIT_BOX_TYPE::OUTPUT>()->getOutputs();
                QColor pen;
                switch(retVal)
                {
                    case Common_Value_Output::HIGH:
                        pen = Qt::green;
                    break;
                    case Common_Value_Output::LOW:
                        pen = Qt::red;
                    break;
                    case Common_Value_Output::UNKNOWN:
                        pen = Qt::black;
                }
                for(auto& wireIt : wires)
                {
                    wireIt->setColor(pen);
                    wireIt->drawWire();
                }
            }
        }

        Gate* getGate(const uint64_t id)
        {
            for(auto& it : m_gates)
            {
                if(it->Id() == id)
                    return it;
            }
            return nullptr;
        }
    public slots:
        void clearSimulate()
        {
            if(!SimulatorFlag::FlagInstance()())
                return;
            SimulatorFlag::FlagInstance()(false);
            for(auto& it : m_gates)
            {
                if(it->isOutput())
                {
                    it->changeOutput( Common_Value_Output::UNKNOWN );
                }
                auto hitbox = it->getHitBox<HitBox::GATE_HIT_BOX_TYPE::OUTPUT>();
                if(hitbox)
                {
                    auto wires = hitbox->getOutputs();
                    for(auto& wireIt : wires)
                    {
                        wireIt->setColor(QColor(Qt::black));
                        wireIt->drawWire();
                    }
                }
            }
        }
        void draw(
                Scheme::Gate::GateType type,
                uint64_t _id = G_NOT_VALID_GATE_ID,
                qreal x = 0.0,
                qreal y = 0.0
                )
        {
            Gate* gate = new Gate(type, nullptr, type == Scheme::Gate::GateType::OUTPUT);
            static QRandomGenerator64 gen;
            quint64 tmpId = _id;
            while(tmpId == G_NOT_VALID_GATE_ID)
            {
                tmpId = gen.generate();
                for(auto& gate : m_gates)
                {
                    if(gate->Id() == tmpId)
                    {
                        tmpId = G_NOT_VALID_GATE_ID;
                        break;
                    }
                }
            }
            gate->setId(tmpId);
            if(!gate->isValid())
            {
                delete gate;
                return;
            }
            if(x != 0.0 && y != 0.0)
            {
                gate->setX(x);
                gate->setY(y);
            }
            addGate(gate);
            if(gate->isOutput())
                m_outputs.insert(gate);
            QObject::connect(gate->emitter(), &GateEmitter::changeInput, this, [=, this](){
                if(SimulatorFlag::FlagInstance()())
                {
                    simulate();
                }
            });
        }

       void keyReleaseEvent(QKeyEvent* event) override
       {
           int key = event->key();
           switch(key)
           {
           case Qt::Key_Delete:
           {
                KeyDeleteReaction();
                break;
           }
           default:
               break;
           };
       }
       bool IsIdxDuplicated(quint64 id)
       {
            for(auto& gate : m_gates)
            {
                if(gate->Id() == id)
                    return true;
            }
            return false;
       }

       void createTableOfTruthDesc(const uint64_t idx, const QString desc)
       {
            for(Scheme::Gate* it : m_gates)
            {
                if(it->Id() == idx)
                {
                    it->createDesc(desc);
                }
            }
       }

       void clearDesc()
       {
           for(auto& it : m_gates)
           {
               it->clearDesc();
           }
       }

    };


}
#endif // SCHEMEAREA_H
