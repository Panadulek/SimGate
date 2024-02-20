#ifndef COMMON_CLASSES_H
#define COMMON_CLASSES_H
#include <cstdint>
#include <vector>
#include <memory>
#include <exception>
#include <map>


#define G_RESULT_FILE_NAME  "RES_GATES.txt"

constexpr uint64_t G_NOT_VALID_GATE_ID = 0;

enum class Common_Gate_Type : uint8_t
{
    AND = 0,
    NAND = 1,
    OR = 2,
    NOR = 3,
    XOR = 4,
    NOT = 5,
    HIGH_STATE = 6,
    LOW_STATE = 7,
    OUTPUT = 8,
    EnumSize
};
enum class Common_Value_Output : uint8_t
{
    LOW = 0,
    HIGH = 0xFF,
    UNKNOWN = 2,
};

namespace BinaryTree
{

    struct Common_Data
    {
        const Common_Gate_Type m_type = Common_Gate_Type::EnumSize;
        const uint64_t m_id = 0;
        mutable Common_Value_Output m_out;
        Common_Data(Common_Gate_Type type, uint64_t id) : m_id(id)
        {
            setType(type);
        }
        Common_Data(uint64_t id) : m_id(id)
        {}
        void setType(const Common_Gate_Type type) const
        {
            const_cast<Common_Gate_Type&>(m_type) = type;
            defaultValue();
        }
        uint64_t getId() const {return m_id;}
        Common_Gate_Type getType() const {return m_type;}
        Common_Value_Output getOutput() const { return m_out; }
        const Common_Value_Output* getPtrOutput() const { return &m_out; }
        void setOutput(Common_Value_Output out) const { this->m_out = out; }
        bool operator < (const Common_Data& rcd) const { return m_id < rcd.getId(); }
        bool operator == (const uint64_t _id) { return m_id == _id; }
        bool operator == (const Common_Data& cd) { return m_id == cd.m_id; }
        bool isInput() const { return (m_type == Common_Gate_Type::HIGH_STATE || m_type == Common_Gate_Type::LOW_STATE); }
        bool isOutput() const { return m_type == Common_Gate_Type::OUTPUT; }
        ~Common_Data() {}
        void defaultValue() const
        {
            switch (m_type)
            {
            case Common_Gate_Type::HIGH_STATE:
                m_out = Common_Value_Output::HIGH;
                break;
            case Common_Gate_Type::LOW_STATE:
                m_out = Common_Value_Output::LOW;
                break;
            default:
                m_out = Common_Value_Output::UNKNOWN;
            }
        }
    };
    using InputPair = std::pair<const uint64_t, const uint64_t>;
    using BinaryTree = std::map<BinaryTree::Common_Data, InputPair>;
};
#endif
