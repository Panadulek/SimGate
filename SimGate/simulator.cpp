#include "simulator.h"
#include <stdexcept>
#include <assert.h>
template<Common_Gate_Type GATE_TYPE>
uint8_t gate_callback(Common_Value_Output, Common_Value_Output);
using gate_callback_type = uint8_t(*)(Common_Value_Output, Common_Value_Output);
using T = Common_Gate_Type;
template<>
uint8_t gate_callback<Common_Gate_Type::AND>(Common_Value_Output a, Common_Value_Output b)
{
    return  ( (uint8_t) a &  (uint8_t) b ) ;
}
template<>
uint8_t gate_callback<Common_Gate_Type::NAND>(Common_Value_Output a, Common_Value_Output b)
{
    return ~gate_callback<Common_Gate_Type::AND>(a, b);
}
template<>
uint8_t gate_callback<Common_Gate_Type::OR>(Common_Value_Output a, Common_Value_Output b)
{
    return (uint8_t)a | (uint8_t)b;
}
template<>
uint8_t gate_callback<Common_Gate_Type::NOR>(Common_Value_Output a, Common_Value_Output b)
{
    return ~gate_callback<Common_Gate_Type::OR>(a, b);
}
template<>
uint8_t gate_callback<Common_Gate_Type::XOR>(Common_Value_Output a , Common_Value_Output b)
{
    return (uint8_t)a ^ (uint8_t)b;
}
template<>
uint8_t gate_callback<Common_Gate_Type::NOT>(Common_Value_Output a, Common_Value_Output)
{
    return ~(uint8_t)a;
}
template<>
uint8_t gate_callback<Common_Gate_Type::HIGH_STATE>(Common_Value_Output, Common_Value_Output)
{
    constexpr uint8_t high_value = 0xff;
    return high_value;
}
template<>
uint8_t gate_callback<Common_Gate_Type::LOW_STATE>(Common_Value_Output, Common_Value_Output)
{
    constexpr uint8_t low_value = 0;
    return low_value;
}
template<>
uint8_t gate_callback<Common_Gate_Type::OUTPUT>(Common_Value_Output ret, Common_Value_Output)
{
    return (uint8_t)ret;
}


static bool isNegated(Common_Gate_Type type)
{
    return ((type == Common_Gate_Type::NOT) || (type == Common_Gate_Type::NAND) || (type == Common_Gate_Type::NOT));
}


static gate_callback_type getCallback(Common_Gate_Type type)
{
    switch(type)
    {
    case T::NOT: return gate_callback<T::NOT>;
    case T::AND: return gate_callback<T::AND>;
    case T::NAND: return gate_callback<T::NAND>;
    case T::OR: return gate_callback<T::OR>;
    case T::NOR: return gate_callback<T::NOR>;
    case T::XOR: return gate_callback<T::XOR>;
    case T::HIGH_STATE: return gate_callback<T::HIGH_STATE>;
    case T::LOW_STATE: return gate_callback<T::LOW_STATE>;
    case T::OUTPUT:
    default:
        return gate_callback<T::OUTPUT>;
    }
}

static Common_Value_Output getDefaultValue(Common_Gate_Type type)
{
    return isNegated(type) ? Common_Value_Output::HIGH : Common_Value_Output::LOW;
}
std::optional<Common_Value_Output> Simulator::getValue(SimMap::iterator input1, SimMap::iterator endMap)
{
    std::optional<Common_Value_Output> val1 { std::nullopt };
    if(input1 == endMap)
        val1 = Common_Value_Output::LOW;
    else if(input1->first.getOutput() != Common_Value_Output::UNKNOWN)
    {
        val1 = input1->first.getOutput();
        if(input1->first.isInput())
            m_retValues[input1->first.getId()] = val1.value();
    }
    else if(m_usedGates.contains(input1->first.getId()))
    {
        val1 = getDefaultValue(input1->first.getType());
    }
    return val1;
}
 void Simulator::simulate(SimMap::iterator iterator, SimMap& map)
{
    if(iterator == map.end())
        throw std::logic_error("Iterator == Map::End");
    m_usedGates.insert(iterator->first.getId());
    gate_callback_type callback = getCallback(iterator->first.getType());
    SimMap::iterator input1 = map.find(BinaryTree::Common_Data(iterator->second.first));
    SimMap::iterator input2 = map.find(BinaryTree::Common_Data(iterator->second.second));
    std::optional<Common_Value_Output> val1 = getValue(input1, map.end());
    std::optional<Common_Value_Output> val2 = getValue(input2, map.end());
    if(!val1.has_value())
    {
        assert(input1 != map.end());
        simulate(input1, map);
        val1 = input1->first.getOutput();
    }
    if(!val2.has_value())
    {
        assert(input2 != map.end());
        simulate(input2, map);
        val2 = input2->first.getOutput();
    }
    assert(val1.has_value() && val2.has_value());
    uint8_t retVal = callback(val1.value(), val2.value());
    iterator->first.setOutput(retVal ? Common_Value_Output::HIGH : Common_Value_Output::LOW);
    m_retValues.insert({iterator->first.getId(), iterator->first.getOutput()});
}

 void Simulator::clean(SimMap& map)
 {
    clear();
    for(auto& it : map)
    {
        it.first.defaultValue();
    }
 }
