//
// Created by DiLLu on 05.01.2023.
//

#include "Tabsgraphs.h"
#include "chartCallout.h"
#include "Visualization/ECG_chartCallout.h"
#include "Visualization/Peaks_chartCallout.h"
#include "Visualization/HRV2_chartCallout.h"


Tabsgraphs::Tabsgraphs(QWidget *parent, std::shared_ptr<DataController> datacontroller) : QTabWidget(parent) {
    this->setStyleSheet("QWidget {background-color: #E7F6F2;}");
    dataController = datacontroller;
}

void Tabsgraphs::addtab_ECGBaselineMA() {
    auto filteredSignal = dataController->getMovingAverageFilteredSignal();
    auto *obj = new MovingAverageFilteredSignal(filteredSignal);

    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "ECG Baseline - MA Filter");
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(obj);
    tab1->setLayout(vlayout);
}


void Tabsgraphs::addtab_ECGBaselineLMS() {
    auto filteredSignal = dataController->getLMSFilteredSignal();
    auto *obj = new LMSFilteredSignal(filteredSignal);

    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "ECG Baseline - LMS Filter");
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(obj);
    tab1->setLayout(vlayout);


}
void Tabsgraphs::addtab_ECGBaselineButterworth() {
    auto filteredSignal = dataController->getButterworthFilteredSignal();
    auto *obj = new ButterworthFilteredSignal(filteredSignal);

    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "ECG Baseline - Butterworth Filter");
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(obj);
    tab1->setLayout(vlayout);

}

void Tabsgraphs::addtab_RPeaksDetectionPanTompkins(){
        auto filteredSignal = dataController->getButterworthFilteredSignal(); // zmiana algorytmu filtracji
        auto detectedPeaks = dataController->getPanTompkinsRPeaks(std::make_shared<std::vector <float>>(filteredSignal));
        auto Waves = dataController->getWaves(dataController->getButterworthFilteredSignal(), detectedPeaks);

        auto *obj = new PeaksChartCallout(filteredSignal, detectedPeaks, Waves.QRSonset, Waves.QRSend, Waves.Tend, Waves.Tend, Waves.Ponset, Waves.Pend, Waves.Pend);
        QWidget *tab1 = new QWidget;
        this->addTab(tab1, "QRSPT - Pan Tompkins");

        QVBoxLayout *vlayout = new QVBoxLayout;
        vlayout->addWidget(obj);
        tab1->setLayout(vlayout);
};

void Tabsgraphs::addtab_RPeaksDetectionHilbert(){
    auto filteredSignal = dataController->getButterworthFilteredSignal(); // zmiana algorytmu filtracji
    auto detectedPeaks = dataController->getHilbertRPeaks(std::make_shared<std::vector <float>>(filteredSignal));
    auto Waves = dataController->getWaves(dataController->getButterworthFilteredSignal(), detectedPeaks);

    auto *obj = new PeaksChartCallout(filteredSignal, detectedPeaks, Waves.QRSonset, Waves.QRSend, Waves.Tend, Waves.Tend, Waves.Ponset, Waves.Pend, Waves.Pend);

    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "QRSPT - Hilbert");
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(obj);
    tab1->setLayout(vlayout);
};

void Tabsgraphs::addtab_QRSandPTDetection(){

    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "QRS and PT Detection");
    QVBoxLayout *vlayout = new QVBoxLayout;
    tab1->setLayout(vlayout);
};

void Tabsgraphs::addtab_HRV2(){

    //histogram mock
    std::vector<int> bins = {6,7,8};
    std::vector<int> dupa = {1,2,3};
    int xM =5;
    int yM =5;
    int xT =7;
    int yT =10;
    int xN =7;
    int yN =5;

    auto *obj = new HRV2HistogramCallout(bins, dupa, xM, yM, xT, yT, xN, yN);

    QWidget *tab1 = new QWidget;
    this->addTab(tab1, "HRV2");
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(obj);
    tab1->setLayout(vlayout);
};

//
void Tabsgraphs::RemoveTab(){
    int currentTabIndex = this->currentIndex();
    this->removeTab(currentTabIndex);
}