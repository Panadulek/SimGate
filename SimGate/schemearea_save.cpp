#include "schemearea.h"
#include <QFileDialog>
#include <QString>
#include <QDomDocument>
#include <QFile>
#include <QRegularExpression>
#include <QTextStream>
#include "HitBox.h"
#include <cstdint>
#include <QDebug>
#include <QXmlStreamReader>
#include <QTextStream>


#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <utility>

#define U_VALUE value.uValue
#define R_VALUE value.rValue

struct Element
{
    enum TYPE_INFO : uint8_t
    {
        X = 0,
        Y,
        GATE_TYPE,
        INPUT,
        GATE_ID,
        END_ENTITY,
        END_FILE,
    };

    TYPE_INFO _type;
    union
    {
        uint64_t uValue;
        qreal rValue;
    }value;

    Element(TYPE_INFO type, uint64_t value) : _type(type), value({.uValue = value})
    {}
    Element(TYPE_INFO type, qreal value) : _type(type), value({.rValue = value})
    {}
    Element(){}
};


void Scheme::Area::saveProject()
{
    QString filename = QFileDialog::getSaveFileName(nullptr, tr("SimGate"), QString(), tr("Gate Save (*.gateSave)"));
    if(filename.isEmpty())
        return;
    FILE* fd = fopen(filename.toUtf8().constData(), "wb+");
    if(!fd)
        return;
    constexpr size_t struct_size = sizeof(Element);
    for(auto& gate : m_gates)
    {
        const Element id(Element::TYPE_INFO::GATE_ID, gate->Id());
        const Element type(Element::TYPE_INFO::GATE_TYPE, static_cast<uint64_t>(gate->getType()));
        const Element x(Element::TYPE_INFO::X, (qreal)gate->x());
        const Element y(Element::TYPE_INFO::Y, (qreal)gate->y());
        auto gate1 = gate->getInput<HitBox::GATE_HIT_BOX_TYPE::INPUT_1>();
        const Element input1(Element::TYPE_INFO::INPUT, gate1 ? gate1->Id() : G_NOT_VALID_GATE_ID);
        auto gate2 = gate->getInput<HitBox::GATE_HIT_BOX_TYPE::INPUT_2>();
        const Element input2(Element::TYPE_INFO::INPUT, gate2 ? gate2->Id() : G_NOT_VALID_GATE_ID);
        const Element end(Element::TYPE_INFO::END_ENTITY, (uint64_t)1);
        fwrite(&id, struct_size, 1, fd);
        fwrite(&type, struct_size, 1, fd);
        fwrite(&x, struct_size, 1, fd);
        fwrite(&y, struct_size, 1, fd);
        fwrite(&input1, struct_size, 1, fd);
        fwrite(&input2, struct_size, 1, fd);
        fwrite(&end, struct_size, 1, fd);
    }
    const Element endFile(Element::TYPE_INFO::END_FILE, (uint64_t)1);
    fwrite(&endFile, struct_size, 1, fd);
    fclose(fd);
}

void Scheme::Area::loadProject()
{
    clearGates();
    clear();
    const QString filename = QFileDialog::getOpenFileName(nullptr, tr("SimGate"), QString(), tr("Gate Save (*.gateSave)"));
    FILE* fd = fopen(filename.toStdString().c_str(), "rb");
    constexpr size_t struct_size = sizeof(Element);
    if(fd)
    {
        Element reader; bool bDataExists = true; uint64_t id = G_NOT_VALID_GATE_ID; std::vector<std::pair<quint64, quint64>> ConnVec;
        qreal x = 0;
        qreal y = 0;
        Gate::GateType type = Gate::GateType::OUTPUT;
        while(bDataExists)
        {
            fread(&reader, struct_size, 1, fd);
            switch(reader._type)
            {
            case Element::TYPE_INFO::GATE_ID:
                id = reader.value.uValue;
                break;
            case Element::TYPE_INFO::GATE_TYPE:
            {
                type = static_cast<Gate::GateType>(reader.U_VALUE);
                break;
            }
            case Element::TYPE_INFO::INPUT:
            {
                if(reader.U_VALUE != G_NOT_VALID_GATE_ID)
                    ConnVec.push_back(std::make_pair(id, reader.U_VALUE));
                break;
            }
            case Element::TYPE_INFO::X:
                x = reader.R_VALUE;
                break;

            case Element::TYPE_INFO::Y:
                y = reader.R_VALUE;
                break;

            case Element::TYPE_INFO::END_ENTITY:
                draw(type, id, x, y );
                break;

            case Element::TYPE_INFO::END_FILE:
                bDataExists = false;
                break;

            }
        }

        if(ConnVec.size() > 0)
        {
            for(auto& pair : ConnVec)
            {
                Gate* output = getGate(pair.first);
                Gate* input = output->getInput<HitBox::GATE_HIT_BOX_TYPE::INPUT_1>();
                if(!input)
                {
                    Gate* _input = getGate(pair.second);
                    auto HitBoxInput = output->getHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_1>();
                    auto HitBoxoutput = _input->getHitBox<HitBox::GATE_HIT_BOX_TYPE::OUTPUT>();
                    emit HitBoxInput->ButtonHitBox()->released();
                    emit HitBoxoutput->ButtonHitBox()->released();

                }
                else if(output->getType() != Gate::GateType::OUTPUT && output->getType() != Gate::GateType::NOT)
                {
                     Gate* _input = getGate(pair.second);
                     auto HitBoxInput = output->getHitBox<HitBox::GATE_HIT_BOX_TYPE::INPUT_2>();
                     auto HitBoxoutput = _input->getHitBox<HitBox::GATE_HIT_BOX_TYPE::OUTPUT>();
                     emit HitBoxInput->ButtonHitBox()->released();
                     emit HitBoxoutput->ButtonHitBox()->released();
                }
            }
        }

    }
}
