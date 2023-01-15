//
// Created by Maria Nowak on 13.01.2023.
//

#ifndef ECG_ANALYZER_HRV1_H
#define ECG_ANALYZER_HRV1_H

#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

struct HRV1_results {
    float meanRR;
    float valueSDNN;
    float valueRMSSD;
    float valueSDNNindex;
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
    HRV1(std::vector<int> RR_peaks) : m_RR_peaks(RR_peaks){}; // later add as std::shared_ptr

    void calculateParams();
    float Get_meanRR();
    float Get_valueSDNN();
    float Get_valueRMSSD();
    unsigned int Get_valueNN50();
    float Get_valuePNN50();


private:
    void calculateStatisticParams();
    void calculateFrequencyParams();
    vector<float> createTachogram();
    float calculateMeanRR();
    float calculateSDNN();
    float calculateRMSSD();
    unsigned int calculateNN50();
    float calculatePNN50();
    vector<float> m_vectorRPeaks;
    HRV1_results m_Results;
    vector<int> m_RR_peaks // add leater as shared ptr
};

#endif //ECG_ANALYZER_HRV1_H
