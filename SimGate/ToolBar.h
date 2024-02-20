#ifndef TOOLBAR_H
#define TOOLBAR_H
#include <QToolBar>
#include <QPushButton>
class ToolBar
{
    QToolBar* m_toolBar;
    QPushButton* m_saveButton;
    QPushButton* m_loadButton;
    QPushButton* m_simButton;
    QPushButton* m_clearSim;
    QPushButton* m_tableButton;
public:
    enum class PosEnum : unsigned char
    {
        SAVE = 0,
        LOAD = 1,
        SIM = 2,
        CLEAR_SIM = 3,
        TABLE_OF_TRUTH = 4,
        LastButton,
    };
    explicit ToolBar() : m_toolBar(new QToolBar(nullptr))
    {
        m_saveButton = new QPushButton("Save project");
        m_toolBar->addWidget(m_saveButton);
        m_loadButton = new QPushButton("Load project");
        m_toolBar->addWidget(m_loadButton);
        m_simButton = new QPushButton("Simulate project");
        m_toolBar->addWidget(m_simButton);
        m_clearSim = new QPushButton("Clear Simulation");
        m_toolBar->addWidget(m_clearSim);
        m_tableButton = new QPushButton("Table of Truth");
        m_toolBar->addWidget(m_tableButton);
    }
    operator QToolBar* () { return m_toolBar; }
    template<PosEnum _btn>
    QPushButton* Button()
    {
        switch(_btn)
        {
        case PosEnum::SAVE:return m_saveButton;
        case PosEnum::LOAD: return m_loadButton;
        case PosEnum::SIM: return m_simButton;
        case PosEnum::CLEAR_SIM: return m_clearSim;
        case PosEnum::TABLE_OF_TRUTH: return m_tableButton;
        default: return nullptr;
        }
    }

};



#endif // TOOLBAR_H
