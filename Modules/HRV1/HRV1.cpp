//
// Created by Maria Nowak on 13.01.2023.
//

#include "hrv1.h"
#include <cmath>
#include <stdlib.h>
#include "src/interpolation.h"
#include "src/fasttransforms.h"
#include "src/stdafx.h"

#define DIFF_50_MS 50
#define ONE_HUN_PERCENT 100
#define FREQUENCY_360 360

void HRV1::calculateStatisticParams() {

    m_Results.meanRR = calculateMeanRR();
    m_Results.valueSDNN = calculateSDNN();
    m_Results.valueRMSSD = calculateRMSSD();
    m_Results.valueNN50 = calculateNN50();
    m_Results.valuePNN50 = calculatePNN50();
}

void HRV1::calculateFrequencyParams() {

    std::vector<float> tachSpline = cubicSpline();
    m_Results.outputPSD = calculatePSD(tachSpline);
    m_Results.freqSpectrum = calculateFreqSpect(tachSpline);

    int n = m_Results.freqSpectrum.size();

    float HF = 0;
    float LF = 0;
    float VLF = 0;
    float ULF = 0;
    float LFHF, TP;

    for(int i = 1; i < n; i++) {
        if(0.40 >= m_Results.freqSpectrum[i] && m_Results.freqSpectrum[i]> 0.15) {
            HF += m_Results.outputPSD[i];
        }
        else if(0.15 >= m_Results.freqSpectrum[i] && m_Results.freqSpectrum[i] > 0.04) {
            LF += m_Results.outputPSD[i];
        }
        else if(0.04 >= m_Results.freqSpectrum[i] && m_Results.freqSpectrum[i] > 0.003) {
            VLF += m_Results.outputPSD[i];
        }
        else if(0.003 >= m_Results.freqSpectrum[i] && m_Results.freqSpectrum[i] > 0.0001){
            ULF += m_Results.outputPSD[i];
        }
    }

    LFHF = LF/HF;
    TP = HF + LF + VLF + ULF;

    m_Results.HF = HF;
    m_Results.LF = LF;
    m_Results.VLF = VLF;
    m_Results.ULF = ULF;
    m_Results.TP = TP;
    m_Results.LFHF = LFHF;

}

void HRV1::calculateParams() {

    if (m_RR_peaks->size() > 0) {
        m_vectorRPeaks = createTachogram();
        if (m_vectorRPeaks.size() > 0) {
            calculateStatisticParams();
            calculateFrequencyParams();
        }
    }
}

std::vector<float> HRV1::createTachogram() {

    /* Prepare RRpeaks values */
    std::vector<float> timeRPeaks;
    for (int i = 0; i < m_RR_peaks->size(); i++) {
        /* Scale samples to get time */
        timeRPeaks.push_back((float(m_RR_peaks->at(i) * float(1.0 / FREQUENCY_360)) * 1000);
    }
    /* Creating tachogram */
    std::vector<float> vectorRPeaks;
    for (int i = 1; i < timeRPeaks.size(); i++) {
        if (timeRPeaks[i] - timeRPeaks[i - 1] > 0) { // if 0 - no interval
            vectorRPeaks.push_back(timeRPeaks[i] - timeRPeaks[i - 1]);
        }
    }
    return vectorRPeaks;
}

/* RRmean - average value of all RR intervals */
/* Return: float meanRR */
float HRV1::calculateMeanRR() {

    float meanRR, sumRR = 0;
    for (int i = 0; i < m_vectorRPeaks.size(); i++) {
        sumRR += m_vectorRPeaks[i];
    }
    meanRR = sumRR / m_vectorRPeaks.size();
    return meanRR;
}

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

/* Return: float valueRMSSD - RMSSD value   */
float HRV1::calculateRMSSD() {

    float value = 0;
    float valueRMSSD;
    for (int i = 0; i < m_vectorRPeaks.size() - 1; i++) {
        value += pow(m_vectorRPeaks[i + 1] - m_vectorRPeaks[i], 2);
    }
    valueRMSSD =
            sqrt(value /
                 float(m_vectorRPeaks.size() -
                       1)); // -1, because it will be 1 less intervwals than sampels
    return valueRMSSD;
}

/* NN50 - number of RR intervals that differ by 50 or more milliseconds from the previous one */
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
/* Return: float valuePNN50 - pNN50 value [%] */
float HRV1::calculatePNN50() {

    float valuePNN50 = float(m_Results.valueNN50) / float(m_vectorRPeaks.size()) * ONE_HUN_PERCENT;
    return valuePNN50;
}

float HRV1::getSigLen(std::vector<float> vectorRPeaks) {

    float timeLen = 0;
    /* sum of intervals */
    for (int i = 0; i < vectorRPeaks.size(); i++) {
        timeLen += vectorRPeaks[i];
    }
    return timeLen;
}

std::vector<float> HRV1::prepareSigAbsolute() {

    /* Cumulative total (in seconds) */
    std::vector<float> sigAbsolute;
    sigAbsolute.reserve(m_vectorRPeaks.size());
    sigAbsolute.push_back(m_vectorRPeaks[0]/1000);
    for(int i = 1; i < m_vectorRPeaks.size(); i++) {
        sigAbsolute.push_back(sigAbsolute[i-1] + (m_vectorRPeaks[i]/float(1000)));
    }
    return std::move(sigAbsolute);
}

std::vector<float> HRV1::cubicSpline() {

    int n = m_vectorRPeaks.size();
    std::vector<float> sigAbsolute = prepareSigAbsolute();

    /* type conversion */
    double* newsigAbsolute = new double[n];
    double* newvectorRPeaks = new double[n];

    /* sigAbsolute vector to double array */
    for(int i = 0; i<sigAbsolute.size(); i++) {
        newsigAbsolute[i] = double(sigAbsolute[i]);
    }
    /* tachogram to double array */
    for(int i = 0; i<m_vectorRPeaks.size(); i++) {
        newvectorRPeaks[i] = double(m_vectorRPeaks[i]/(float(1000)));
    }

    alglib::real_1d_array *rx = new alglib::real_1d_array();
    rx->setcontent(n, newsigAbsolute);
    alglib::real_1d_array *ry = new alglib::real_1d_array();
    ry->setcontent(n, newvectorRPeaks);

    alglib::spline1dinterpolant s;
    /* building interpolate function */
    alglib::spline1dbuildcubic(*const_cast<const alglib::real_1d_array*>(rx), *const_cast<const alglib::real_1d_array*>(ry), s);

    /* interpolation */
    std::vector<float> sigSpline;
    sigSpline.reserve(n);
    float sigLen = (getSigLen(m_vectorRPeaks))/(float(1000));
    for(int i = 0; i < n; i++) {
        sigSpline.push_back(alglib::spline1dcalc(s, i*(sigLen/float(n))));
    }
    delete[] newsigAbsolute;
    delete[] newvectorRPeaks;
    return sigSpline;
}

/* PSD */
/* Parameter(s): vector<float> sigSpline tachogram after interpolation*/
/* Return: float<vector> outputPSD */
std::vector<float> HRV1::calculatePSD(const std::vector<float> sigSpline) {

    int n = sigSpline.size();

    /* type conversion */
    double * newInputSignal = new double[n];

    /* rewriting data from vector to array */
    for(int i = 0; i < n; i++) {
        newInputSignal[i] = (double)sigSpline[i];
    }
    alglib::real_1d_array s; // table needed for fft
    s.setcontent(n, newInputSignal); // Copying array to a s variable
    alglib::complex_1d_array newFftSignal; // Because the result is in complex numbers
    alglib::fftr1d(s, newFftSignal); //  fft

    std::vector<float> outputPSD;
    outputPSD.reserve(n);

    /* Calculation of one-sided spectrum */
    /* Spectral density determination*/
    for(int i = 0; i < n/2; i++) {
        if ( i == 0) {
            outputPSD.push_back(pow(sqrt(pow(newFftSignal[i].x, 2)+ pow(newFftSignal[i].y,2)),2)/(n));
        }
        else {
            outputPSD.push_back(2*pow(sqrt(pow(newFftSignal[i].x, 2) + pow(newFftSignal[i].y, 2)), 2)/(n));
        }
    }

    delete[] newInputSignal;
    return std::move(outputPSD);
}

/*Frequency vector 									                  */
/* Parameter(s): vector<float> sigSpline - tachogram after interpolation */
/* Return: float<vector> freqSpect - frequency spectrum */
std::vector<float> HRV1::calculateFreqSpect(const std::vector<float> sigSpline) {

    int n = sigSpline.size();
    std::vector<float> freqSpect;
    freqSpect.reserve(n/2);
    float step = 1.0f/(getSigLen(sigSpline)); // frequency step

    for(int i = 0; i < n/2; i++) {
        freqSpect.push_back(float(i)*step);
    }
    return std::move(freqSpect);
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

float HRV1::Get_HF(){
    return m_Results.HF;
}

float HRV1::Get_LF(){
    return m_Results.LF;
}

float HRV1::Get_VLF(){
    return m_Results.VLF;
}

float HRV1::Get_ULF(){
    return m_Results.ULF;
}

float HRV1::Get_TP(){
    return m_Results.TP;
}

float HRV1::Get_LFHF(){
    return m_Results.LFHF;
}