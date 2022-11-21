//
// Created by gosia on 19.11.2022.
//

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
double Utils :: getAverage(std::vector<T> const& v) {
    if (v.empty()) {return 0;}
    return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

template <typename T>
vector<T> Utils :: slicing(vector<T> const& v, int X, int Y){
    auto first = v.begin() + X;
    auto last = v.begin() + Y + 1;
    vector<T> vector(first, last);
    return vector;
}

template <typename T>
vector<T> Utils :: gradient(vector<T> input){
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
    int end = rLoc;

    if (rLoc < interval){start = 0;}

    vector<float> window;
    window = slicing(filteredSignal, start, end);

    return arg_min(window) + start;
}

int WavesDetector::findSPoint(int interval, int rLoc){
    int start = rLoc;
    int end = rLoc + interval;

    if (end > filteredSignal.size()) {end = (int) filteredSignal.size();}
    if (start > filteredSignal.size()) {start = (int) filteredSignal.size() - 20;}

    vector<float> window;
    window = slicing(filteredSignal, start, end);

    return arg_min(window) + rLoc;
}

int WavesDetector :: findBoundaryPoint(vector<float> window ){
    vector<float> windowGradient = gradient(std::move(window));
    double threshold = getAverage(windowGradient) * 0.05;
    int i = 0;
    while ((i < windowGradient.size()) && (windowGradient[i] < threshold)) {i++;}
    return i;
}

void WavesDetector::findQRSonset() {
    cout << "[INFO] Get QRS-onset..." << endl;
    int qLoc;
    vector<int> qrsOnsetPoints;

    for (int rLoc: rPeaks) {
        int q1Loc = findQPoint(20, rLoc);
        int q2Loc = findQPoint(80, rLoc);

        if (q1Loc == q2Loc || filteredSignal[q1Loc] < filteredSignal[q2Loc]) {qLoc = q1Loc;} else {qLoc = q2Loc;};

        int start = qLoc - 40;
        int end = qLoc;
        if (qLoc < 40) {start = 0;}

        vector<float> window;
        window = slicing(filteredSignal, start, end);
        reverse(window.begin(), window.end());
        vector<float> windowGradient = gradient(window);

        qrsOnsetPoints.push_back(start + 40 + 2 - arg_min(windowGradient));
    }
    QRSonset = qrsOnsetPoints;
}


void WavesDetector::findQRSend() {
    cout << "[INFO] Get QRS-end..." << endl;
    int sLoc;
    vector<int> qrsEndPoints;

    for (int rLoc: rPeaks) {
        int s1Loc = findSPoint(20, rLoc);
        int s2Loc = findSPoint(80, rLoc);

        if (s1Loc == s2Loc || filteredSignal[s1Loc] < filteredSignal[s2Loc]) {sLoc = s1Loc;} else {sLoc = s2Loc;}

        int start = sLoc;
        int end = sLoc + 40;

        if (sLoc > filteredSignal.size()) {end = int(filteredSignal.size());}

        vector<float> window;
        window = slicing(filteredSignal, start, end);
        vector<float> windowGradient = gradient(window);

        qrsEndPoints.push_back(sLoc + arg_min(windowGradient) - 2);
    }
    QRSend = qrsEndPoints;
}

void WavesDetector::findTend(){
    cout << "[INFO] Get T-end..." << endl;
    vector <int> tEndPoints;

    for(int idx = 0; idx < int(QRSend.size()) - 1; idx++ )
    {
        int windowSize = (QRSend[idx + 1] - QRSend[idx]) * 2 / 3;
        int start = QRSend[idx];
        int end = start + windowSize;

        vector<float> window = slicing(filteredSignal, start, end);
        float offset = window[0];

        for(float & i : window){
            if(i < 0)i *= -1;
            i -= offset;}

        int tLoc = arg_max(window);

        vector<float> tWaveEndWindow = slicing(window, tLoc, int(window.size())-1);
        int tEnd = findBoundaryPoint(tWaveEndWindow) + tLoc + start;

        tEndPoints.push_back(tEnd);
    }
    Tend = tEndPoints;
}

void WavesDetector::findPwaveBoundaryPoints(){
    cout << "[INFO] Get P-onset and P-end..." << endl;
    vector <int> pOnsetPoints;
    vector <int> pEndPoints;

    for(int idx = 0; idx < int(min(QRSonset.size(), Tend.size())) - 1; idx++ )
    {
        int start = Tend[idx];
        int end = QRSonset[idx+1];
        if (end < start){start = end - 100;}

        vector<float> window = slicing(filteredSignal, start, end);
        int pLoc = arg_max(window);

        vector<float> pOnsetWaveWindow = slicing(window, 0, pLoc);
        reverse(pOnsetWaveWindow.begin(), pOnsetWaveWindow.end());
        int pOnset = int(pOnsetWaveWindow.size()) - findBoundaryPoint(pOnsetWaveWindow) - 1 + start;
        pOnsetPoints.push_back(pOnset);

        vector<float> pEndWaveWindow = slicing(window, pLoc, int(window.size())-1);
        int pEnd = findBoundaryPoint(pEndWaveWindow) + pLoc + start;
        pEndPoints.push_back(pEnd);
    }
    Ponset = pOnsetPoints;
    Pend = pEndPoints;
}