//
// Created by Piotr on 23.12.2022.
//

#include "hrvparameterstab.h"
#include <QtWidgets>
#include "pch.h"

//model dla MVC
HRVParametersTabModel::HRVParametersTabModel(std::shared_ptr<R_peaks::R_Peaks_Pakiet> detectedRPeaks) {
    HRV1 hrv1;
    auto res = hrv1.calculateParams(detectedRPeaks);
    meanRR = res->meanRR;
    valueSDNN = res->valueSDNN;
    valueRMSSD = res->valueRMSSD;
    valueSDSD = res->valueSDSD;
    valueSDANN = res->valueSDANN;
    valueSDNNIndex = res->valueSDNNindex;
    valueNN50 = res->valueNN50;
    valuePNN50 = res->valuePNN50;
    HF = res->HF;
    LF = res->LF;
    VLF = res->VLF;
    ULF = res->ULF;
    TP = res->TP;
    LFHF = res->LFHF;
}

float HRVParametersTabModel::getMeanRR() {
    return meanRR;
}

float HRVParametersTabModel::getValueSDNN() {
    return valueSDNN;
}

float HRVParametersTabModel::getValueRMSSD() {
    return valueRMSSD;
}

float HRVParametersTabModel::getValueSDSD() {
    return valueSDSD;
}

float HRVParametersTabModel::getValueSDANN() {
    return valueSDANN;
}

float HRVParametersTabModel::getValueSDNNIndex() {
    return valueSDNNIndex;
}

float HRVParametersTabModel::getValueNN50() {
    return valueNN50;
}

float HRVParametersTabModel::getValuePNN50() {
    return valuePNN50;
}

float HRVParametersTabModel::getHF() {
    return HF;
}

float HRVParametersTabModel::getLF() {
    return LF;
}

float HRVParametersTabModel::getVLF() {
    return VLF;
}

float HRVParametersTabModel::getULF() {
    return ULF;
}

float HRVParametersTabModel::getTP() {
    return TP;
}

float HRVParametersTabModel::getLFHF() {
    return LFHF;
}

HRVParametersTab::HRVParametersTab(std::shared_ptr<R_peaks::R_Peaks_Pakiet> detectedRPeaks,
                                   QWidget *parent)
        : QWidget(parent) {
    unique_ptr<HRVParametersTabModel> model(new HRVParametersTabModel(detectedRPeaks));
    unique_ptr<HRVParametersTabController> controller(new HRVParametersTabController);

    auto *layout = new QHBoxLayout;

    //    ustawienie własności tabeli
    hrvParametersTimeTable = new QTableWidget(this);
    hrvParametersTimeTable->setRowCount(8);
    hrvParametersTimeTable->setColumnCount(3);
    hrvParametersTimeTableHeader << "Parametr" << "Wartość" << "Jednostka";
    hrvParametersTimeTable->setHorizontalHeaderLabels(hrvParametersTimeTableHeader);
    hrvParametersTimeTable->verticalHeader()->setVisible(false);
    hrvParametersTimeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    hrvParametersTimeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    hrvParametersTimeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    hrvParametersTimeTable->setShowGrid(false);
    hrvParametersTimeTable->setStyleSheet("QTableView {selection-background-color: grey;}");
    hrvParametersTimeTable->setGeometry(QApplication::desktop()->screenGeometry());

//    wyświetlanie obliczonych parametrów analizy czasowej w tabeli; , 'f',2 - zmiana precyzji do 2 miejsc po przecinku
    hrvParametersTimeTable->setItem(0, 0, createCenterAllignedTableItem("RR"));
    hrvParametersTimeTable->setItem(0, 1, createCenterAllignedTableItem(QString::number(model->getMeanRR(), 'f',2)));
    hrvParametersTimeTable->setItem(0, 2, createCenterAllignedTableItem("ms"));

    hrvParametersTimeTable->setItem(1, 0, createCenterAllignedTableItem("SDNN"));
    hrvParametersTimeTable->setItem(1, 1, createCenterAllignedTableItem(QString::number(model->getValueSDNN(), 'f',2)));
    hrvParametersTimeTable->setItem(1, 2, createCenterAllignedTableItem("ms"));

    hrvParametersTimeTable->setItem(2, 0, createCenterAllignedTableItem("RMSSD"));
    hrvParametersTimeTable->setItem(2, 1, createCenterAllignedTableItem(QString::number(model->getValueRMSSD(), 'f',2)));
    hrvParametersTimeTable->setItem(2, 2, createCenterAllignedTableItem("ms"));

    hrvParametersTimeTable->setItem(3, 0, createCenterAllignedTableItem("SDSD"));
    hrvParametersTimeTable->setItem(3, 1, createCenterAllignedTableItem(QString::number(model->getValueSDSD(), 'f',2)));
    hrvParametersTimeTable->setItem(3, 2, createCenterAllignedTableItem("ms"));

    hrvParametersTimeTable->setItem(4, 0, createCenterAllignedTableItem("SDANN"));
    hrvParametersTimeTable->setItem(4, 1, createCenterAllignedTableItem(QString::number(model->getValueSDANN(), 'f',2)));
    hrvParametersTimeTable->setItem(4, 2, createCenterAllignedTableItem("ms"));

    hrvParametersTimeTable->setItem(5, 0, createCenterAllignedTableItem("SDNNindex"));
    hrvParametersTimeTable->setItem(5, 1, createCenterAllignedTableItem(QString::number(model->getValueSDNNIndex(), 'f',2)));
    hrvParametersTimeTable->setItem(5, 2, createCenterAllignedTableItem("ms"));

    hrvParametersTimeTable->setItem(6, 0, createCenterAllignedTableItem("NN50"));
    hrvParametersTimeTable->setItem(6, 1, createCenterAllignedTableItem(QString::number(model->getValueNN50(), 'f',2)));
    hrvParametersTimeTable->setItem(6, 2, createCenterAllignedTableItem("-"));

    hrvParametersTimeTable->setItem(7, 0, createCenterAllignedTableItem("PNN50"));
    hrvParametersTimeTable->setItem(7, 1, createCenterAllignedTableItem(QString::number(model->getValuePNN50(), 'f',2)));
    hrvParametersTimeTable->setItem(7, 2, createCenterAllignedTableItem("%"));

    hrvParametersFreqTable = new QTableWidget(this);
    hrvParametersFreqTable->setRowCount(6);
    hrvParametersFreqTable->setColumnCount(3);
    hrvParametersFreqTableHeader << "Parametr" << "Wartość" << "Jednostka";
    hrvParametersFreqTable->setHorizontalHeaderLabels(hrvParametersFreqTableHeader);
    hrvParametersFreqTable->verticalHeader()->setVisible(false);
    hrvParametersFreqTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    hrvParametersFreqTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    hrvParametersFreqTable->setSelectionMode(QAbstractItemView::SingleSelection);
    hrvParametersFreqTable->setShowGrid(false);
    hrvParametersFreqTable->setStyleSheet("QTableView {selection-background-color: grey;}");
    hrvParametersFreqTable->setGeometry(QApplication::desktop()->screenGeometry());

//    wyświetlanie obliczonych parametrów analizy częstotliwościowej w tabeli
    hrvParametersFreqTable->setItem(0, 1, createCenterAllignedTableItem(QString::number(model->getHF(), 'f',2)));
    hrvParametersFreqTable->setItem(0, 0, createCenterAllignedTableItem("HF"));
    hrvParametersFreqTable->setItem(0, 2, createCenterAllignedTableItem("s^2"));

    hrvParametersFreqTable->setItem(1, 0, createCenterAllignedTableItem("LF"));
    hrvParametersFreqTable->setItem(1, 1, createCenterAllignedTableItem(QString::number(model->getLF(), 'f',2)));
    hrvParametersFreqTable->setItem(1, 2, createCenterAllignedTableItem("s^2"));

    hrvParametersFreqTable->setItem(2, 0, createCenterAllignedTableItem("VLF"));
    hrvParametersFreqTable->setItem(2, 1, createCenterAllignedTableItem(QString::number(model->getVLF(), 'f',2)));
    hrvParametersFreqTable->setItem(2, 2, createCenterAllignedTableItem("s^2"));

    hrvParametersFreqTable->setItem(3, 0, createCenterAllignedTableItem("ULF"));
    hrvParametersFreqTable->setItem(3, 1, createCenterAllignedTableItem(QString::number(model->getULF(), 'f',2)));
    hrvParametersFreqTable->setItem(3, 2, createCenterAllignedTableItem("s^2"));

    hrvParametersFreqTable->setItem(4, 0, createCenterAllignedTableItem("TP"));
    hrvParametersFreqTable->setItem(4, 1, createCenterAllignedTableItem(QString::number(model->getTP(), 'f',2)));
    hrvParametersFreqTable->setItem(4, 2, createCenterAllignedTableItem("s^2"));

    hrvParametersFreqTable->setItem(5, 0, createCenterAllignedTableItem("LFHF"));
    hrvParametersFreqTable->setItem(5, 1, createCenterAllignedTableItem(QString::number(model->getLFHF(), 'f',2)));
    hrvParametersFreqTable->setItem(5, 2, createCenterAllignedTableItem("-"));

    hrvParametersTimeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    hrvParametersFreqTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    layout->addWidget(hrvParametersTimeTable);
    layout->addWidget(hrvParametersFreqTable);

    setLayout(layout);
}