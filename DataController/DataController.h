//
// Created by Bartosz Łukasik on 17/01/2023.
//

#ifndef ECG_ANALYZER_DATACONTROLLER_H
#define ECG_ANALYZER_DATACONTROLLER_H

#include "./../DataAccessor/DataAccessor.h"
#include "./../Modules/ECG-Baseline/ECG_Baseline.h"
#include "./../Modules/Rpeaks/HilbertTransform.h"
#include "./../Modules/Rpeaks/PanTompkins.h"
#include "./../Modules/WAVES/waves.h"

struct SignalDetails
{
    unsigned int numberOfSignals = 0;
    char sex;
    unsigned int age = 0;
};


class DataController
{
    DataAccessor dataAccessor = {};
    SignalDetails m_signalDetails;

    std::vector<float> m_rawSignal;

    std::vector<float> m_LMSFilteredSignal;
    std::vector<float> m_ButterworthFilteredSignal;
    std::vector<float> m_MovingAverageFilteredSignal;

    ECG_Baseline ecgBaseline = {NULL};
    HilbertTransform hilbertTransform;
    PanTompkins panTompkins;

public:
    DataController(std::string pathToFile);

    const std::vector<float> getLMSFilteredSignal();
    const std::vector<float> getButterworthFilteredSignal();
    const std::vector<float> getMovingAverageFilteredSignal();

};

#endif //ECG_ANALYZER_DATACONTROLLER_H
