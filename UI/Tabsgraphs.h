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
#include "../DataController/DataController.h"

class Tabsgraphs : public QTabWidget
{
    Q_OBJECT;

public:
    Tabsgraphs(QWidget *parent = nullptr, std::shared_ptr<DataController> datacontroller=nullptr);


public slots:

    void RemoveTab();
    void addtab_QRSandPTDetection();
    void addtab_ECGBaselineMA();
    void addtab_ECGBaselineLMS();
    void addtab_ECGBaselineButterworth();

    void addtab_RPeaksDetectionPanTompkins();
    void addtab_RPeaksDetectionHilbert();
    void addtab_Histogram();
    void addtab_Poincare();

private:
    std::shared_ptr<DataController> dataController;
};


#endif //ECG_ANALYZER_TABSGRAPHS_H
