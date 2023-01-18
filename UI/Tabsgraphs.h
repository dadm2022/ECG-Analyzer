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

class Tabsgraphs : public QTabWidget
{
    Q_OBJECT;

public:
    explicit Tabsgraphs(QWidget *parent = nullptr);


public slots:

    void RemoveTab();
    void addtab_QRSandPTDetection();
    void addtab_ECGBaselineMA();
    void addtab_ECGBaselineLMS();
    void addtab_ECGBaselineButterworth();

    void addtab_RPeaksDetectionPanTompkins();
    void addtab_RPeaksDetectionHilbert();
    void addtab_HRV2();

private:

};


#endif //ECG_ANALYZER_TABSGRAPHS_H
