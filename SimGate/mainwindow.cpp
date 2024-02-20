#include "mainwindow.h"
#include <QGraphicsRectItem>
#include <future>
#include <QSharedMemory>
#include <Common.h>
#include "TableOfTruth.h"
#include <cassert>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(QSize(680, 480));
    m_mainWidget = new QWidget(this);
    m_boxLayout.push_back(new LAYOUT_TYPE(m_mainWidget));
    auto ptr = new QVBoxLayout;
    setCentralWidget(m_mainWidget);
    m_boxLayout.last()->addLayout(ptr);
    ptr->addWidget((QWidget*)m_gateList,3);
    m_gView = new QGraphicsView(m_mainWidget);
    m_boxLayout.last()->addWidget(m_gView, 3);
    m_schemeArea = new Scheme::Area(m_gView);
    m_gView->setScene(m_schemeArea);
    addToolBar((QToolBar*)m_toolBar);
    setupConnects();
}

MainWindow::~MainWindow()
{}


void MainWindow::setupConnects()
{

    //Drawing Gates
    QObject::connect(m_gateList.getItem(GatePaths::Gate_Path_Idx::AND), &ItemWidget::click, m_schemeArea, &Scheme::Area::draw);
    QObject::connect(m_gateList.getItem(GatePaths::Gate_Path_Idx::NAND), &ItemWidget::click, m_schemeArea, &Scheme::Area::draw);
    QObject::connect(m_gateList.getItem(GatePaths::Gate_Path_Idx::OR), &ItemWidget::click, m_schemeArea, &Scheme::Area::draw);
    QObject::connect(m_gateList.getItem(GatePaths::Gate_Path_Idx::NOR), &ItemWidget::click, m_schemeArea, &Scheme::Area::draw);
    QObject::connect(m_gateList.getItem(GatePaths::Gate_Path_Idx::XOR), &ItemWidget::click, m_schemeArea, &Scheme::Area::draw);
    QObject::connect(m_gateList.getItem(GatePaths::Gate_Path_Idx::NOT), &ItemWidget::click, m_schemeArea, &Scheme::Area::draw);
    QObject::connect(m_gateList.getItem(GatePaths::Gate_Path_Idx::HIGH_STATE), &ItemWidget::click, m_schemeArea, &Scheme::Area::draw);
    QObject::connect(m_gateList.getItem(GatePaths::Gate_Path_Idx::LOW_STATE), &ItemWidget::click, m_schemeArea, &Scheme::Area::draw);
    QObject::connect(m_gateList.getItem(GatePaths::Gate_Path_Idx::OUTPUT), &ItemWidget::click, m_schemeArea, &Scheme::Area::draw);

    //ToolBar
    QObject::connect(m_toolBar.Button<ToolBar::PosEnum::SAVE>(), &QPushButton::released, m_schemeArea, &Scheme::Area::saveProject);
    QObject::connect(m_toolBar.Button<ToolBar::PosEnum::LOAD>(), &QPushButton::released, m_schemeArea, &Scheme::Area::loadProject);
    QObject::connect(m_toolBar.Button<ToolBar::PosEnum::SIM>(), &QPushButton::released, m_schemeArea, &Scheme::Area::simulate);
    QObject::connect(m_toolBar.Button<ToolBar::PosEnum::CLEAR_SIM>(), &QPushButton::released, m_schemeArea, &Scheme::Area::clearSimulate);
    QObject::connect(m_toolBar.Button<ToolBar::PosEnum::TABLE_OF_TRUTH>(), &QPushButton::released, this, [&]()->void{
       if(TableOfTruthEmitter::isValid())
           return;
       TableOfTruth table;
       QObject::connect(TableOfTruthEmitter::Instance(), &TableOfTruthEmitter::UpdateDesc, m_schemeArea, &Scheme::Area::createTableOfTruthDesc);
       QObject::connect(TableOfTruthEmitter::Instance(), &TableOfTruthEmitter::RemoveDesc, m_schemeArea, &Scheme::Area::clearDesc);
       SimMap map = m_schemeArea->buildTree();
       std::vector<uint64_t> inputsVector(0);
       std::vector<uint64_t> outputVector(0);
       for(auto it = map.begin(); it != map.end(); it++)
       {
            if(it->first.isInput())
                inputsVector.push_back(it->first.getId());
            else if(it->first.isOutput())
                outputVector.push_back(it->first.getId());
       }
       TableDialog* dialog = table.init(inputsVector, outputVector);
       Simulator sim;
       std::optional<CombinationOutputVector> retVals = sim.generateCombination(map, inputsVector.size());
       size_t row = 0;
       assert(retVals.has_value());
       auto &retValsRef = retVals.value();
       for(size_t i = 0; i < retValsRef.size(); i++)
        {
           uint64_t col = 0;
           for(size_t j = 0; j < inputsVector.size(); j++)
           {
               auto val = retValsRef[i][inputsVector[j]];
               table.fill(row, col++, val, dialog);
           }
           for(size_t j = 0; j < outputVector.size(); j++)
           {
               auto val = retValsRef[i][outputVector[j]];
               table.fill(row, col++, val, dialog);
           }
           row++;
        }

       table.display(dialog);
    });

}


