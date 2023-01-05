//
// Created by Piotr on 23.12.2022.
//

#ifndef ECG_ANALYZER_HRV1PARAMSTAB_H
#define ECG_ANALYZER_HRV1PARAMSTAB_H

#include <QWidget>
#include <QtCharts>
#include <QTableWidget>
#include "../Utils/TableUtils.h"
//#include    """R_PEAKS POTRZEBNY"""

using namespace QtCharts;

using TableUtils::createCenterAllignedTableItem;

class QTabWidget;

class QWidget;

class HRVParametersTabModel {
public:
    //    """R_PEAKS POTRZEBNY"""
    //explicit HRVParametersTabModel(std::shared_ptr<R_peaks::R_Peaks_Pakiet> detectedRPeaks);

    float getMeanRR();

    float getValueSDNN();

    float getValueRMSSD();

    float getValueSDSD();

    float getValueSDANN();

    float getValueSDNNIndex();

    float getValueNN50();

    float getValuePNN50();

    float getHF();

    float getLF();

    float getVLF();

    float getULF();

    float getTP();

    float getLFHF();

private:
    float meanRR;
    float valueSDNN;
    float valueRMSSD;
    float valueSDSD;
    float valueSDANN;
    float valueSDNNIndex;
    float valueNN50;
    float valuePNN50;
    float HF;
    float LF;
    float VLF;
    float ULF;
    float TP;
    float LFHF;
};

class HRVParametersTabController {

};

class HRVParametersTab : public QWidget {
public:

//    """R_PEAKS POTRZEBNY"""
//    explicit HRVParametersTab(std::shared_ptr<R_peaks::R_Peaks_Pakiet> detectedRPeaks,
//                              QWidget *parent = nullptr);

private:
    QTableWidget *hrvParametersTimeTable;
    QTableWidget *hrvParametersFreqTable;
    QStringList hrvParametersTimeTableHeader;
    QStringList hrvParametersFreqTableHeader;
};


#endif //ECG_ANALYZER_HRV1PARAMSTAB_H
