#ifndef TABLEOFTRUTH_H
#define TABLEOFTRUTH_H
#include<QObject>
#include <Common.h>
#include <set>
#include <cstdint>
#include <stack>
#include <QDebug>
#include "simulator.h"
#include <QTableWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <boost/multiprecision/cpp_int.hpp>
#include <QVariant>
#include <QTableWidgetItem>
#include <QWidget>
#include <cmath>

class TableOfTruthEmitter : public QObject
{
    Q_OBJECT
    static bool m_valid;
    TableOfTruthEmitter(){}
public:
    static TableOfTruthEmitter*& Instance()
    {
        static TableOfTruthEmitter* instance = nullptr;
        if(!instance)
        {
            m_valid = true;
            instance = new TableOfTruthEmitter();
        }
        return instance;
    }
    static bool isValid()
    {
        return m_valid;
    }
public:
    void operator()()
    {
        m_valid = false;
        auto& i = Instance();
        emit i->RemoveDesc();
        delete i;
        i = nullptr;
    }
signals:
    void RemoveDesc();
    void UpdateDesc(const uint64_t idx, const QString desc);
};

class TableDialog : public QWidget
{
    Q_OBJECT
    TableDialog () : QWidget(nullptr, Qt::Widget), m_table(this)
    {
        QScreen* screen = QGuiApplication::primaryScreen();
        QRect availableGeometry = screen->availableGeometry();
        int width = availableGeometry.width() / 4;
        int height = availableGeometry.height() / 4;
        setGeometry(availableGeometry.x(), availableGeometry.y(), width, height);
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(&m_table);
        setLayout(layout);
        this->setWindowTitle("SimGate TableOfTruth");
    }
    QTableWidget m_table;
    friend class TableOfTruth;
    QTableWidget& refTable(){return m_table;}
    virtual void closeEvent(QCloseEvent *event) override
    {
        (*TableOfTruthEmitter::Instance())();
        QWidget::closeEvent(event);
    }
};


class TableOfTruth final : public QObject
{
    Q_OBJECT
public:

    TableDialog* init(const std::vector<uint64_t>& inputIds, const std::vector<uint64_t>& outputIds)
    {
        const size_t inputCounter = inputIds.size();
        const size_t outputCounter = outputIds.size();
        const size_t m_combinationCount = pow(2, inputCounter); // obliczanie ilości kombinacji dla projektu użytkownika
        TableDialog* dialog = new TableDialog();
        dialog->m_table.setColumnCount(inputCounter + outputCounter);
        const size_t columnCount = static_cast<uint64_t>(dialog->m_table.columnCount());
        for(size_t i = 0; i < columnCount; i++) // generowanie opisów w tabeli i na rysunku
        {
            if(i < inputCounter)
            {
                QString name = "Input";
                name += QString::number(i + 1);
                dialog->refTable().setHorizontalHeaderItem(i, new QTableWidgetItem(name));
                emit TableOfTruthEmitter::Instance()->UpdateDesc(inputIds[i], name);
            }
            else
            {
                QString name = "Output";
                const size_t outNumber = i - (inputCounter);
                name += QString::number(outNumber + 1);
                dialog->refTable().setHorizontalHeaderItem(i, new QTableWidgetItem(name));
                emit TableOfTruthEmitter::Instance()->UpdateDesc(outputIds[outNumber], name);
            }
        }
        dialog->m_table.setRowCount(m_combinationCount);
        dialog->m_table.setSortingEnabled(false);
        return dialog;;
    }
    void fill(uint64_t row, uint64_t col, const Common_Value_Output ret, TableDialog* td)
    {
        QTableWidgetItem* item = nullptr;
        switch(ret)
        {
            case Common_Value_Output::HIGH: item = new QTableWidgetItem("1"); break;
            case Common_Value_Output::LOW: item = new QTableWidgetItem("0"); break;
            default: item = new QTableWidgetItem("~"); break;
        }
        td->refTable().setItem(row, col, item);
    }
    void display(TableDialog* td)
    {
        td->update();
        td->show();
    }

};

#endif // TABLEOFTRUTH_H
