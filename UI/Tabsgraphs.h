//
// Created by DiLLu on 05.01.2023.
//

#ifndef ECG_ANALYZER_TABSGRAPHS_H
#define ECG_ANALYZER_TABSGRAPHS_H
#include <QWidget>
#include <QTabWidget>
#include <iostream>
#include <QtCharts>
#include <QVBoxLayout>
//#include "chartCallout.h"
class Tabsgraphs : public QTabWidget
{
    Q_OBJECT;

public:
    explicit Tabsgraphs(QWidget *parent = nullptr);


public slots:
    void addtab_ECGBaseline();
    void RemoveTab();
    void addtab_RPeaksDetection();
    void addtab_QRSandPTDetection();
private:

};


#endif //ECG_ANALYZER_TABSGRAPHS_H
