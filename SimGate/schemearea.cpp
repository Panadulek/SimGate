#include "schemearea.h"

Scheme::Area::Area(QObject *parent)
    : QGraphicsScene{parent}
{
    draw(Scheme::Gate::GateType::OUTPUT);
}

