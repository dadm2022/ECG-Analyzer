//
// Created by Maria Nowak on 13.01.2023.
//

#ifndef ECG_ANALYZER_HRV1_H
#define ECG_ANALYZER_HRV1_H

#include <iostream>
#include <vector>

/*
    Example usage:
    std::shared_ptr<const std::vector<int>> RR_Peaks
    HRV1 hrv1(RR_Peaks); - creating class instance
    hrv1.calculateParams(); - calculating statistic and frequency parameters

    To get parameters use proper getter, for example to get meanRR use Get_meanRR();
 */
using namespace std;


struct HRV1_results {
    float meanRR;
    float valueSDNN;
    float valueRMSSD;
    unsigned int valueNN50;
    float valuePNN50;
    vector<float> outputPSD;
    vector<float> freqSpectrum;
    float HF;
    float LF;
    float VLF;
    float ULF;
    float TP;
    float LFHF;
};

class HRV1 {

public:
    HRV1(std::vector<int> RR_peaks) : m_RR_peaks(RR_peaks){};

    void calculateParams();
    float Get_meanRR();
    float Get_valueSDNN();
    float Get_valueRMSSD();
    unsigned int Get_valueNN50();
    float Get_valuePNN50();
    float Get_HF();
    float Get_LF();
    float Get_VLF();
    float Get_ULF();
    float Get_TP();
    float Get_LFHF();

private:

    vector<float> m_vectorRPeaks;
    std::shared_ptr<std::vector<int>> m_RR_peaks;
    HRV1_results m_Results;

    void calculateStatisticParams();
    void calculateFrequencyParams();

    vector<float> createTachogram();
    float calculateMeanRR();
    float calculateSDNN();
    float calculateRMSSD();
    unsigned int calculateNN50();
    float calculatePNN50();

    vector<float> prepareSigAbsolute();
    float getSigLen(vector<float> vectorRPeaks);
    vector<float> calculatePSD(const vector<float> sigSpline);
    vector<float> calculateFreqSpect(const vector<float> sigSpline);
    vector<float> cubicSpline();

};

#endif //ECG_ANALYZER_HRV1_H
