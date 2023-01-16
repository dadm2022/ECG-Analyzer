//
// Created by DiLLu on 05.01.2023.
//

#include "Tabsgraphs.h"
#include "chartCallout.h"

Tabsgraphs::Tabsgraphs(QWidget *parent) : QTabWidget(parent) {
    this->setStyleSheet("QWidget {background-color: #E7F6F2;}");
}

void Tabsgraphs::addtab_ECGBaselineMA() {

    std::vector <float> filteredSignal={1,2,3};
    std::vector <int> detectedRPeaks={2,4,6};
    auto *obj = new chartCallout(filteredSignal,detectedRPeaks);

    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "ECG Baseline - MA Filter");
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(obj);
    tab1->setLayout(vlayout);
}


void Tabsgraphs::addtab_ECGBaselineLMS() {

    std::vector <float> filteredSignal={1,2,3};
    std::vector <int> detectedRPeaks={2,4,6};
    auto *obj = new chartCallout(filteredSignal,detectedRPeaks);

    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "ECG Baseline - LMS Filter");
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(obj);
    tab1->setLayout(vlayout);


}
void Tabsgraphs::addtab_ECGBaselineButterworth() {

    std::vector <float> filteredSignal={1,2,3};
    std::vector <int> detectedRPeaks={2,4,6};
    auto *obj = new chartCallout(filteredSignal,detectedRPeaks);

    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "ECG Baseline - Butterworth Filter");
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(obj);
    tab1->setLayout(vlayout);

}

void Tabsgraphs::addtab_RPeaksDetectionPanTompkins(){
        QWidget *tab1 = new QWidget;
        this->addTab(tab1, "Pan Tompkins");
        QVBoxLayout *vlayout = new QVBoxLayout;
        tab1->setLayout(vlayout);
};

void Tabsgraphs::addtab_RPeaksDetectionHilbert(){
    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "Hilbert");
    QVBoxLayout *vlayout = new QVBoxLayout;
    tab1->setLayout(vlayout);
};

void Tabsgraphs::addtab_QRSandPTDetection(){
    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "QRS and PT Detection");
    QVBoxLayout *vlayout = new QVBoxLayout;
    tab1->setLayout(vlayout);
};



//
void Tabsgraphs::RemoveTab(){
    int currentTabIndex = this->currentIndex();
    this->removeTab(currentTabIndex);
}