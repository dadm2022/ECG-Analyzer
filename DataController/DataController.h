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
#include "./../Modules/TWaveAlt/TWaveAlt.h"
#include "./../Modules/HRV_DFA/DFA.h"
#include "./../Modules/HRV2/HRV2.h"
#include "./../Modules/HEART_CLASS/heart_class.h"
#include "./../Modules/ST-SEGMENT/st-segment.h"


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

struct TinnTriangleCorners
{
    double Nx;
    double Ny;
    double Mx;
    double My;
    double Tx;
    double Ty;
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

    long double alpha1;
    long double alpha2;


public:
    DataController();

    void initialize(std::string pathToFile);

    const char getSex();
    const unsigned int getAge();

    const std::vector<float> getLMSFilteredSignal();
    const std::vector<float> getButterworthFilteredSignal();
    const std::vector<float> getMovingAverageFilteredSignal();

    const std::vector<int> getHilbertRPeaks(std::shared_ptr<std::vector<float>> filteredSignal);
    const std::vector<int> getPanTompkinsRPeaks(std::shared_ptr<std::vector<float>> filteredSignal);

    const Waves getWaves(std::vector<float> filteredSignal, std::vector<int> rPeaks);

    const bool getAlternans(std::shared_ptr<const std::vector<int>>& m_tEndPoints, std::shared_ptr<const std::vector<float>>& FilteredSignal);
    const float getAlternansValue(std::shared_ptr<const std::vector<int>> &m_tEndPoints,
                                                  std::shared_ptr<const std::vector<float>> &filteredSignal);

    const long double getAlpha1(std::shared_ptr<const std::vector<int>> rPeaks);
    const long double getAlpha2(std::shared_ptr<const std::vector<int>> rPeaks);

    const TinnTriangleCorners getTinnTriangleCorners(std::shared_ptr<std::vector<int>> rPeaks);
    const std::vector<int> getHRV2Histogram(std::shared_ptr<std::vector<int>> rPeaks);

    std::shared_ptr<std::vector<activations_t>> getHeartActivationClassification(std::vector<int> &rPeaks, std::vector<int> &P, std::vector<int> &QRSend, std::vector<int> &QRSonset);

    std::shared_ptr<stseg> getSTSegment(std::vector<int> &QRSend, std::vector<int> &RPeaks, std::vector<float> &FilteredSignal, std::vector<int> &QRS_onset, std::vector<int> &TPeak );

};

#endif //ECG_ANALYZER_DATACONTROLLER_H
