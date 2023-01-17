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

struct Waves
{
    std::vector<int> QRSonset;
    std::vector<int> QRSend;
    std::vector<int> Tend;
    std::vector<int> Ponset;
    std::vector<int> Pend;
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
    std::vector<int> hilbertTransformRPeaks;
    std::vector<int> panTompkinsRPeaks;

    Waves waves = {};



public:
    DataController(std::string pathToFile);

    const std::vector<float> getLMSFilteredSignal();
    const std::vector<float> getButterworthFilteredSignal();
    const std::vector<float> getMovingAverageFilteredSignal();
    const std::vector<int> getHilbertRPeaks(std::shared_ptr<std::vector<float>> filteredSignal);
    const std::vector<int> getPanTompkinsRPeaks(std::shared_ptr<std::vector<float>> filteredSignal);
    const Waves getWaves(std::vector<float> filteredSignal, std::vector<int> rPeaks);

};

#endif //ECG_ANALYZER_DATACONTROLLER_H
