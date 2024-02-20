#ifndef GATEPATHS_H
#define GATEPATHS_H
#include <cstdint>
#include <memory>
#include <vector>
#include <Common.h>
class GatePaths final
{
public:
    using Gate_Path_Idx = Common_Gate_Type;
    static GatePaths* instance()
    {
        static GatePaths s;
        return &s;
    }
    const char* AndPath() { return m_gatePaths[Idx(Gate_Path_Idx::AND)].get(); }
    const char* NandPath() { return m_gatePaths[Idx(Gate_Path_Idx::NAND)].get(); }
    const char* OrPath() { return m_gatePaths[Idx(Gate_Path_Idx::OR)].get(); }
    const char* NorPath() { return m_gatePaths[Idx(Gate_Path_Idx::NOR)].get(); }
    const char* XorPath() { return m_gatePaths[Idx(Gate_Path_Idx::XOR)].get(); }

    const char* NotPath() { return m_gatePaths[Idx(Gate_Path_Idx::NOT)].get(); }
    const char* HighState() { return m_gatePaths[Idx(Gate_Path_Idx::HIGH_STATE)].get(); }
    const char* LowState() { return m_gatePaths[Idx(Gate_Path_Idx::LOW_STATE)].get(); }
    const char* OutputPath() { return m_gatePaths[Idx(Gate_Path_Idx::OUTPUT)].get(); }
private:
    std::vector<std::unique_ptr<char[]>> m_gatePaths;
    uint8_t Idx(Gate_Path_Idx e) { return static_cast<uint8_t>(e); }
    GatePaths()
    {
        m_gatePaths.resize(static_cast<size_t>(Gate_Path_Idx::EnumSize));
        m_gatePaths[Idx(Gate_Path_Idx::AND)] = std::make_unique<char[]>(sizeof("://Img/and.svg"));
        memcpy(m_gatePaths[Idx(Gate_Path_Idx::AND)].get(), "://Img/and.svg", sizeof("://Img/and.svg"));

        m_gatePaths[Idx(Gate_Path_Idx::NAND)] = std::make_unique<char[]>(sizeof("://Img/nand.svg"));
        memcpy(m_gatePaths[Idx(Gate_Path_Idx::NAND)].get(), "://Img/nand.svg", sizeof("://Img/nand.svg"));

       m_gatePaths[Idx(Gate_Path_Idx::OR)] = std::make_unique<char[]>(sizeof("://Img/or.svg"));
        memcpy(m_gatePaths[Idx(Gate_Path_Idx::OR)].get(), "://Img/or.svg", sizeof("://Img/or.svg"));

       m_gatePaths[Idx(Gate_Path_Idx::NOR)] = (std::make_unique<char[]>(sizeof("://Img/nor.svg")));
        memcpy(m_gatePaths[Idx(Gate_Path_Idx::NOR)].get(), "://Img/nor.svg", sizeof("://Img/nor.svg"));

        m_gatePaths[Idx(Gate_Path_Idx::XOR)] = (std::make_unique<char[]>(sizeof("://Img/xor.svg")));
        memcpy(m_gatePaths[Idx(Gate_Path_Idx::XOR)].get(), "://Img/xor.svg", sizeof("://Img/xor.svg"));

        m_gatePaths[Idx(Gate_Path_Idx::NOT)] = (std::make_unique<char[]>(sizeof("://Img/negate.svg")));
        memcpy(m_gatePaths[Idx(Gate_Path_Idx::NOT)].get(), "://Img/negate.svg", sizeof("://Img/negate.svg"));

        m_gatePaths[Idx(Gate_Path_Idx::HIGH_STATE)] = (std::make_unique<char[]>(sizeof("://Img/High_State.svg")));
        memcpy(m_gatePaths[Idx(Gate_Path_Idx::HIGH_STATE)].get(), "://Img/High_State.svg", sizeof("://Img/High_State.svg"));

        m_gatePaths[Idx(Gate_Path_Idx::LOW_STATE)] = (std::make_unique<char[]>(sizeof("://Img/Low_State.svg")));
        memcpy(m_gatePaths[Idx(Gate_Path_Idx::LOW_STATE)].get(), "://Img/Low_State.svg", sizeof("://Img/Low_State.svg"));

        m_gatePaths[Idx(Gate_Path_Idx::OUTPUT)] = (std::make_unique<char[]>(sizeof("://Img/Neutral_State.svg")));
        memcpy(m_gatePaths[Idx(Gate_Path_Idx::OUTPUT)].get(), "://Img/Neutral_State.svg", sizeof("://Img/Neutral_State.svg"));
    }

};

#endif // GATEPATHS_H
