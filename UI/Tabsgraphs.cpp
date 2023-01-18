//
// Created by DiLLu on 05.01.2023.
//

#include "Tabsgraphs.h"
#include "chartCallout.h"
#include "Visualization/ECG_chartCallout.h"
#include "Visualization/Peaks_chartCallout.h"



Tabsgraphs::Tabsgraphs(QWidget *parent) : QTabWidget(parent) {
    this->setStyleSheet("QWidget {background-color: #E7F6F2;}");
}

void Tabsgraphs::addtab_ECGBaselineMA() {


    std::vector <float> filteredSignal={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
//    std::vector <int> detectedRPeaks={2,4,6};
    auto *obj = new MovingAverageFilteredSignal(filteredSignal);

    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "ECG Baseline - MA Filter");
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(obj);
    tab1->setLayout(vlayout);
}


void Tabsgraphs::addtab_ECGBaselineLMS() {

    std::vector <float> filteredSignal={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    std::vector <int> detectedRPeaks={2,4,6};
    auto *obj = new LMSFilteredSignal(filteredSignal);

    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "ECG Baseline - LMS Filter");
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(obj);
    tab1->setLayout(vlayout);


}
void Tabsgraphs::addtab_ECGBaselineButterworth() {

    std::vector <float> filteredSignal={1,2,3};
    std::vector <int> detectedRPeaks={2,4,6};
    auto *obj = new ButterworthFilteredSignal(filteredSignal);

    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "ECG Baseline - Butterworth Filter");
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(obj);
    tab1->setLayout(vlayout);

}

void Tabsgraphs::addtab_RPeaksDetectionPanTompkins(){

        std::vector <float> filteredSignal={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
        std::vector <int> detectedRPeaks={2,4,6,7,8,10,14,16,20};
        std::vector <int> waves1 = {1, 2, 3, 4, 5, 7, 15, 17};
        std::vector <int> waves2 = {1, 2, 4, 4, 5, 8, 15, 18};
        std::vector <int> waves3 = {1, 2, 3, 4, 5, 9, 15, 19};
        std::vector <int> waves4 = {1, 2, 3, 4, 5, 10, 14, 15};
        std::vector <int> waves5 = {1, 2, 3, 4, 5, 11, 14, 16};
        std::vector <int> waves6 = {1, 2, 3, 4, 5, 10, 14, 15};
        std::vector <int> waves7 = {3, 5, 7, 9, 10, 11, 14, 16};
        auto *obj = new PeaksChartCallout(filteredSignal, detectedRPeaks, waves1, waves2, waves3, waves4, waves5, waves6, waves7);
        QWidget *tab1 = new QWidget;
        this->addTab(tab1, "QRSPT - Pan Tompkins");

        QVBoxLayout *vlayout = new QVBoxLayout;
        vlayout->addWidget(obj);
        tab1->setLayout(vlayout);
};

void Tabsgraphs::addtab_RPeaksDetectionHilbert(){
    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "QRSPT - Hilbert");
    QVBoxLayout *vlayout = new QVBoxLayout;
    tab1->setLayout(vlayout);
};

void Tabsgraphs::addtab_QRSandPTDetection(){
    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "QRS and PT Detection");
    QVBoxLayout *vlayout = new QVBoxLayout;
    tab1->setLayout(vlayout);
};

void Tabsgraphs::addtab_HRV2(){
    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "HRV2");
    QVBoxLayout *vlayout = new QVBoxLayout;
    tab1->setLayout(vlayout);
};

//
void Tabsgraphs::RemoveTab(){
    int currentTabIndex = this->currentIndex();
    this->removeTab(currentTabIndex);
}