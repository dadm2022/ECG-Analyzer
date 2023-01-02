//
// Created by gosia on 19.11.2022.
//
// Waves detection algorithm implementation according to:
// D. Sadhukhan and M. Mitra, "Detection of ECG characteristic features using slope thresholding and relative magnitude
// comparison," 2012 Third International Conference on Emerging Applications of Information Technology, 2012,
// pp. 122-126, doi: 10.1109/EAIT.2012.6407876.

#include "bits/stdc++.h"
#include "waves.h"
#include <iostream>

using namespace std;

template <typename T, typename A>
int Utils::arg_max(std::vector<T, A> const& vec) {
    return static_cast<int>(std::distance(vec.begin(), max_element(vec.begin(), vec.end())));
}

template <typename T, typename A>
int Utils::arg_min(std::vector<T, A> const& vec) {
    return static_cast<int>(std::distance(vec.begin(), min_element(vec.begin(), vec.end())));
}

template<typename T>
double Utils::getAverage(std::vector<T> const& v) {
    if (v.empty()) {return 0;}
    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

template <typename T>
vector<T> Utils::slicing(vector<T> const& v, int X, int Y){
    auto first = v.begin() + X;
    auto last = v.begin() + Y + 1;
    vector<T> vector(first, last);
    return vector;
}

template <typename T>
vector<T> Utils::gradient(vector<T> input){
    if (input.size() <= 1) return input;
    vector<T> res;
    for(int j=0; j<input.size(); j++) {
        int j_left = j - 1;
        int j_right = j + 1;
        if (j_left < 0) {
            j_left = 0;
            j_right = 1;
        }
        if (j_right >= input.size()){
            j_right = int(input.size()) - 1;
            j_left = j_right - 1;
        }
        double dist_grad = (input[j_right] - input[j_left]) / 2.0;
        res.push_back(T(dist_grad));
    }
    return res;
}

int WavesDetector::findQPoint(int interval, int rLoc){
    int start = rLoc - interval;
    if (rLoc < interval){start = 0;}

    vector<float> window;
    window = slicing(filteredSignal, start, rLoc);

    return arg_min(window) + start;
}

int WavesDetector::findSPoint(int interval, int rLoc){
    int start = rLoc;
    if (start > filteredSignal.size()) {start = (int) filteredSignal.size() - 20;}

    int end = rLoc + interval;
    if (end > filteredSignal.size()) {end = (int) filteredSignal.size();}

    vector<float> window;
    window = slicing(filteredSignal, start, end);

    return arg_min(window) + rLoc;
}

int WavesDetector::findPwaveBoundaryPoint(vector<float> window){
    vector<float> windowGradient = gradient(std::move(window));
    double threshold = getAverage(windowGradient) * 0.05;
    int i = 0;
    while ((i < windowGradient.size()) && (windowGradient[i] < threshold)) {i++;}
    return i;
}

void WavesDetector::findQRSonset() {
    int qLoc;

    for (int rLoc: rPeaks) {
        int q1Loc = findQPoint(20, rLoc), q2Loc = findQPoint(80, rLoc);

        if (q1Loc == q2Loc || filteredSignal[q1Loc] < filteredSignal[q2Loc]) {qLoc = q1Loc;} else {qLoc = q2Loc;};

        int start = qLoc - 40;
        if (qLoc < 40) {start = 0;}

        vector<float> window = slicing(filteredSignal, start, qLoc);
        reverse(window.begin(), window.end());
        vector<float> windowGradient = gradient(window);

        qrsOnsetPoints.push_back(start + 40 + 2 - arg_min(windowGradient));
    }
}


void WavesDetector::findQRSend() {
    int sLoc;

    for (int rLoc: rPeaks) {
        int s1Loc = findSPoint(20, rLoc), s2Loc = findSPoint(80, rLoc);

        if (s1Loc == s2Loc || filteredSignal[s1Loc] < filteredSignal[s2Loc]) {sLoc = s1Loc;} else {sLoc = s2Loc;}

        int end = sLoc + 40;
        if (sLoc > filteredSignal.size()) {end = int(filteredSignal.size());}

        vector<float> windowGradient = gradient(slicing(filteredSignal, sLoc, end));

        qrsEndPoints.push_back(sLoc + arg_min(windowGradient) - 2);
    }
}

void WavesDetector::findTend(){

    for(int idx = 0; idx < int(qrsEndPoints.size()) - 1; idx++ )
    {
        int windowSize = (qrsEndPoints[idx + 1] - qrsEndPoints[idx]) * 2 / 3;
        int start = qrsEndPoints[idx];

        vector<float> window = slicing(filteredSignal, start, start + windowSize);
        float offset = window[0];

        for(float & i : window){if(i < 0)i *= -1; i -= offset;}

        int tLoc = arg_max(window);

        vector<float> tWaveEndWindow = slicing(window, tLoc, int(window.size())-1);
        int tEnd = findPwaveBoundaryPoint(tWaveEndWindow) + tLoc + start;

        tEndPoints.push_back(tEnd);
    }
}

void WavesDetector::findPwaveBoundaryPoints(){

    for(int idx = 0; idx < int(min(qrsOnsetPoints.size(), tEndPoints.size())) - 1; idx++ )
    {
        int start = tEndPoints[idx], end = qrsOnsetPoints[idx + 1];
        if (end < start){start = end - 100;}

        vector<float> window = slicing(filteredSignal, start, end);
        int pLoc = arg_max(window);

        vector<float> pOnsetWaveWindow = slicing(window, 0, pLoc);
        reverse(pOnsetWaveWindow.begin(), pOnsetWaveWindow.end());
        int pOnset = int(pOnsetWaveWindow.size()) - findPwaveBoundaryPoint(pOnsetWaveWindow) - 1 + start;
        pOnsetPoints.push_back(pOnset);

        vector<float> pEndWaveWindow = slicing(window, pLoc, int(window.size())-1);
        int pEnd = findPwaveBoundaryPoint(pEndWaveWindow) + pLoc + start;
        pEndPoints.push_back(pEnd);
    }
}