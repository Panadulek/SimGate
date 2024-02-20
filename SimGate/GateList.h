#ifndef GATELIST_H
#define GATELIST_H
#include <QListWidget>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
#include <vector>
#include <QToolButton>
#include "GatePaths.h"
#include <memory>
#include <cstring>
#include "schemearea.h"
#include <QObject>
#include <QPushButton>

class ItemWidget final : public QWidget
{
    Q_OBJECT
    using GATETYPE = GatePaths::Gate_Path_Idx;
    std::unique_ptr<QPushButton> m_button;
    std::unique_ptr<char[]> m_describe;
    GATETYPE m_type;
    std::unique_ptr<QPushButton> createButton(const char* pathToImg)
    {
        if(!pathToImg || !*pathToImg)
        return nullptr;
        QPixmap pixmap(pathToImg);
        QIcon icon(pixmap);
        std::unique_ptr<QPushButton> button = std::make_unique<QPushButton>();
        button->setIcon(icon);
        button->setIconSize(QSize(50, 50));

        return button;
    }
    bool IsValid()
    {
        return m_button && m_describe;
    }
public:
        ItemWidget(QWidget* parent, const char* pathToImg, const char* describe, GATETYPE type)
            : QWidget(parent),  m_button(nullptr), m_describe(nullptr), m_type(type)
        {
            if(describe && *describe)
            {
                size_t size = strlen(describe) + 1;
                m_describe = std::make_unique<char[]>(size);
                memcpy(m_describe.get(), describe, size);
                m_button = createButton(pathToImg);
                if(m_button)
                {
                    QGridLayout* layout = new QGridLayout();
                    layout->addWidget(m_button.get());
                    layout->addWidget(new QLabel(m_describe.get()));
                    setLayout(layout);
                    m_button->setFlat(true);
                }
                else
                    m_describe.reset();
                QObject::connect(m_button.get(), &QPushButton::released, [this]()
                {
                    emit click(m_type, G_NOT_VALID_GATE_ID, 0.0, 0.0);
                });
            }
    }
    bool Pixmap(const QSize& size, QPixmap& ret)
    {
        bool isValid = IsValid();
        if(isValid)
            ret = m_button->icon().pixmap(size);
        return isValid;
    }
    const QPushButton* getButton()
    {
        return m_button.get();
    }
    GATETYPE getType(){
        return m_type;
    }
    ~ItemWidget(){}
signals:
    void click(Scheme::Gate::GateType type, uint64_t id, qreal x, qreal y);
};


class GateList
{
    std::vector<QWidget*> m_queueToDeallocate;
    using IDXS = GatePaths::Gate_Path_Idx;
    QWidget m_widget;
public:

    explicit GateList()
    {
        QGridLayout* layout = new QGridLayout(&m_widget);
        auto paths = GatePaths::instance();
        m_queueToDeallocate.reserve(static_cast<size_t>(IDXS::EnumSize));
        QWidget* ptr = new ItemWidget(nullptr, paths->AndPath(), "and_gate", IDXS::AND);
        layout->addWidget(ptr);
        m_queueToDeallocate[static_cast<size_t>(IDXS::AND)] = ptr;
        ptr = new ItemWidget(nullptr, paths->NandPath(), "nand_gate", IDXS::NAND);
        layout->addWidget(ptr, 0, 1);
        m_queueToDeallocate[static_cast<size_t>(IDXS::NAND)] = ptr;

        ptr = new ItemWidget(nullptr, paths->OrPath(), "or_gate", IDXS::OR);
        layout->addWidget(ptr, 1, 0);
        m_queueToDeallocate[static_cast<size_t>(IDXS::OR)] = ptr;

        ptr = new ItemWidget(nullptr, paths->NorPath(), "nor_gate", IDXS::NOR);
        layout->addWidget(ptr, 1, 1);
        m_queueToDeallocate[static_cast<size_t>(IDXS::NOR)] = ptr;

        ptr = new ItemWidget(nullptr, paths->XorPath(), "xor_gate", IDXS::XOR);
        layout->addWidget(ptr, 2, 0);
        m_queueToDeallocate[static_cast<size_t>(IDXS::XOR)] = ptr;

        ptr = new ItemWidget(nullptr, paths->NotPath(), "negate_gate", IDXS::NOT);
        layout->addWidget(ptr, 2, 1);
        m_queueToDeallocate[static_cast<size_t>(IDXS::NOT)] = ptr;

        ptr = new ItemWidget(nullptr, paths->LowState(), "low_state", IDXS::LOW_STATE);
        layout->addWidget(ptr, 3, 0);
        m_queueToDeallocate[static_cast<size_t>(IDXS::LOW_STATE)] = ptr;

        ptr = new ItemWidget(nullptr, paths->HighState(), "high_state", IDXS::HIGH_STATE);
        layout->addWidget(ptr, 3, 1);
        m_queueToDeallocate[static_cast<size_t>(IDXS::HIGH_STATE)] = ptr;

        ptr = new ItemWidget(nullptr, paths->OutputPath(), "output", IDXS::OUTPUT);
        layout->addWidget(ptr, 4, 0);
        m_queueToDeallocate[static_cast<size_t>(IDXS::OUTPUT)] = ptr;


    }
    ItemWidget* getItem(GatePaths::Gate_Path_Idx idx)
    {
        return reinterpret_cast<ItemWidget*> //in queueToDeallocate should be always ItemWidget*
                (
                    m_queueToDeallocate[static_cast<size_t>(idx)]
                );
    }
    ~GateList()
    {
        for(auto& toDelete : m_queueToDeallocate)
        {
            delete toDelete;
            toDelete = nullptr;
        }
    }
    operator QWidget*()
    {
        return &m_widget;
    }
};

#endif // GATELIST_H
