#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QList>
#include "GateList.h"
#include "schemearea.h"
#include <QGraphicsView>
#include "ToolBar.h"
#define LAYOUT_TYPE QHBoxLayout

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QWidget* m_mainWidget;
    QList<LAYOUT_TYPE*> m_boxLayout;
    GateList m_gateList;
    QGraphicsView* m_gView;
    Scheme::Area* m_schemeArea;
    ToolBar m_toolBar;
public:
    MainWindow(QWidget *parent = nullptr);
    void setupConnects();
    ~MainWindow();
};
#endif // MAINWINDOW_H
