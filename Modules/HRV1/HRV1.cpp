//
// Created by Maria Nowak on 13.01.2023.
//
#define DIFF_50_MS 50
#define ONE_HUN_PERCENT 100
#define FREQUENCY_360 360

#include "HRV1.h"
using namespace std;

void HRV1::calculateStatisticParams() {

    m_Results.meanRR = calculateMeanRR();
    m_Results.valueSDNN = calculateSDNN();
    m_Results.valueRMSSD = calculateRMSSD();
    m_Results.valueNN50 = calculateNN50();
    m_Results.valuePNN50 = calculatePNN50();
}

void HRV1::calculateFrequencyParams() {
    // to_do
}

void HRV1::calculateParams(){

    if (m_RR_peaks.size() > 0){
        m_vectorRPeaks = createTachogram(m_RR_peaks);
        if (m_vectorRPeaks.size() > 0){
            calculateStatisticParams();
            //calculateFrequencyParams();
        }
    }

}

vector<float> HRV1::createTachogram() {

    /* Prepare RRpeaks values */
    vector<float> timeRPeaks;
    for (int i = 0; i <  m_RR_peaks.size(); i++) {
        /* Skale samples to get time */
        timeRPeaks.push_back((float( m_RR_peaks[i]) * float(1.0 / FREQUENCY_360)) *
                             1000);
    }
    /* Creating tachogram */
    vector<float> vectorRPeaks;
    for (int i = 1; i < timeRPeaks.size(); i++) {
        if (timeRPeaks[i] - timeRPeaks[i - 1] >
            0) { // if 0 - no interval
            vectorRPeaks.push_back(timeRPeaks[i] - timeRPeaks[i - 1]);
        }
    }
    return vectorRPeaks;
}

/* RRmean - average value of all RR intervals
/* Parameter(s): vector<float> vectorRPeaks - tachogram */
/* Return: float meanRR */
float HRV1::calculateMeanRR() {

    float meanRR, sumRR = 0;
    for (int i = 0; i < m_vectorRPeaks.size(); i++) {
        sumRR += m_vectorRPeaks[i];
    }
    meanRR = sumRR / m_vectorRPeaks.size();
    return meanRR;
}

/* SDNN
/* Parameter(s): vector<float> vectorRPeaks - tachogram, float meanRR 	*/
/* Return: float valueSDNN - SDNN value */
float HRV1::calculateSDNN() {

    float value = 0;
    float valueSDNN;
    for (int i = 0; i < m_vectorRPeaks.size(); i++) {
        value += pow(m_vectorRPeaks[i] - m_Results.meanRR, 2);
    }
    valueSDNN = sqrt(value / (m_vectorRPeaks.size() - 1));
    return valueSDNN;
}

/*RMSSD
/* Parameter(s): vector<float> vectorRPeaks - tachogram 	*/
/* Return: float valueRMSSD - RMSSD value   */
float HRV1::calculateRMSSD() {

    float value = 0;
    float valueRMSSD;
    for (int i = 0; i < m_vectorRPeaks.size() - 1; i++) {
        value += pow(m_vectorRPeaks[i + 1] - m_vectorRPeaks[i], 2);
    }
    valueRMSSD = sqrt(
            value / float(m_vectorRPeaks.size() - 1)); // -1, because it will be 1 less intervwals than sampels
    return valueRMSSD;
}

/* NN50 - number of RR intervals that differ by 50 or more milliseconds from the previous one */
/* Parameter(s): vector<float> vectorRPeaks - tachogram  */
/* Return: unsigned int valueNN50 - NN50 value	*/
unsigned int HRV1::calculateNN50() {

    unsigned int valueNN50 = 0;
    for (int i = 0; i < m_vectorRPeaks.size() - 1; i++) {
        if (abs(m_vectorRPeaks[i + 1] - m_vectorRPeaks[i]) > DIFF_50_MS) {
            valueNN50++;
        }
    }
    return valueNN50;
}

/* pNN50 - the ratio of NN50 occurring in whole record to all RR interwals */
/* Parameter(s): int n - rozmiar, unsigned int calc_valueNN50 */
/* Return: float valuePNN50 - pNN50 value [%] */
float HRV1::calculatePNN50(const int n, const unsigned int calc_valueNN50) {

    float valuePNN50 = float(m_Results.valueNN50) / float(m_vectorRPeaks.size()) * ONE_HUN_PERCENT;
    return valuePNN50;
}

float HRV1::Get_meanRR(){
    return m_Results.meanRR;
}

float HRV1::Get_valueSDNN(){
    return m_Results.valueSDNN;
}

float HRV1::Get_valueRMSSD(){
    return m_Results.valueRMSSD;
}

unsigned int HRV1::Get_valueNN50(){
    return m_Results.valueNN50;
}

float HRV1::Get_valuePNN50(){
    return m_Results.valuePNN50;
}