#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <Common.h>
#include <set>
#include <optional>
#include <map>
#include <boost/multiprecision/cpp_int.hpp>

using ResolveMap = std::map<uint64_t, bool>;


using InputPair = BinaryTree::InputPair;
using SimMap = BinaryTree::BinaryTree;
using RetValMap = std::map<uint64_t, Common_Value_Output>;
using CombinationOutputVector = std::vector<RetValMap>;


class SimulatorFlag
{
    bool m_isSimulated{false};
private:
    SimulatorFlag(bool flag) : m_isSimulated(flag)
    {}
public:
    static SimulatorFlag& FlagInstance()
    {
        static SimulatorFlag flag(false);
        return flag;
    }
    bool operator()()
    {
        return m_isSimulated;
    }
    void operator()(bool flag)
    {
        m_isSimulated = flag;
    }
};

class Simulator
{
    using OutputMap = std::map<uint64_t, Common_Value_Output>;
    using UsedMap = std::set<uint64_t>;

    OutputMap m_retValues;
    UsedMap m_usedGates;
    using Integer = boost::multiprecision::cpp_int;

    void simulate(SimMap::iterator iterator, SimMap& map);
    void clear()
    {
        m_usedGates.clear();
        m_retValues.clear();
    }
    SimMap& getMap(SimMap& map, Integer combination)
    {
        std::vector<const BinaryTree::Common_Data*> inputVector;
        for(auto& it : map)
        {
            if(it.first.isInput())
                inputVector.push_back(&it.first);
        }
        for(size_t i = 0; i < inputVector.size(); i++)
        {
            bool isHigh = (combination & Integer(1) << i) != 0;
            inputVector[i]->setType(isHigh ? Common_Gate_Type::HIGH_STATE : Common_Gate_Type::LOW_STATE);
            inputVector[i]->defaultValue();
        }
        return map;
    }

    std::optional<Common_Value_Output> getValue(SimMap::iterator input1, SimMap::iterator endMap);
public:
    void Sim(SimMap& simMap)
    {
        for(SimMap::iterator it = simMap.begin(); it != simMap.end(); it++)
        {
            if(it->first.getType() == Common_Gate_Type::OUTPUT)
            {
                simulate(it, simMap);
            }
        }
    }
    ~Simulator()
    {
        clear();
    }
    Common_Value_Output getValue(uint64_t idGate)
    {
        auto it = m_retValues.find(idGate);
        return it != m_retValues.end() ? it->second :  Common_Value_Output::UNKNOWN;
    }
    std::optional<CombinationOutputVector> generateCombination(SimMap& simMap, const size_t inputCounter = 0)
    {
        if(!inputCounter)
            return std::nullopt;
        std::optional<CombinationOutputVector> ret = CombinationOutputVector();
        Integer combinationCounter = static_cast<Integer>(1) << inputCounter;
        for(Integer i = 0; i < combinationCounter; i++)
        {
            ret->emplace_back(RetValMap());
            clean(simMap);
            simMap = getMap(simMap, i);
            Sim(simMap);
            for(auto it = simMap.begin(); it != simMap.end(); it++)
            {
                if(it->first.isInput() || it->first.isOutput())
                    ret->back()[it->first.getId()] = getValue(it->first.getId());
            }
        }
        return ret;
    }
    void clean(SimMap& map);
    friend class TableOfTruth;
};

#endif // SIMULATOR_H
