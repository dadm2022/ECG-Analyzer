//
// Created by gosia on 19.11.2022.
//

#include "bits/stdc++.h"
#include <iostream>
#include "waves.h"
#include <cmath>

using namespace std;

template <typename T, typename A>
int Utils::arg_min(std::vector<T, A> const& vec) {
    return static_cast<int>(std::distance(vec.begin(), min_element(vec.begin(), vec.end())));
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

int QRS::findQPoint(int interval, int rLoc, const vector<float>& filteredSignal, const vector<int>& rPeaks){
    int start = rLoc - interval;
    int end = rLoc;

    if (rLoc < interval)
        start = 0;

    vector<float> window;
    window = slicing(filteredSignal, start, end);

    int qLoc = arg_min(window) + start;

    return qLoc;
}

int QRS::findSPoint(int interval, int rLoc, const vector<float>& filteredSignal, const vector<int>& rPeaks){
    int start = rLoc;
    int end = rLoc + interval;

    if (end > filteredSignal.size())
        end = (int) filteredSignal.size();
    if (start > filteredSignal.size())
        start = (int) filteredSignal.size() - 20;

    vector<float> window;
    window = slicing(filteredSignal, start, end);

    int sLoc = arg_min(window) + rLoc;

    return sLoc;
}

vector <int> WavesDetector::findQRSonset() {
    int qLoc;
    vector<int> qrsOnsetPoints;

    for (int rLoc: rPeaks) {
        int q1Loc = findQPoint(20, rLoc, filteredSignal, rPeaks);
        int q2Loc = findQPoint(80, rLoc, filteredSignal, rPeaks);

        if (q1Loc == q2Loc || filteredSignal[q1Loc] < filteredSignal[q2Loc])
            qLoc = q1Loc;
        else
            qLoc = q2Loc;

        int start = qLoc - 40;
        int end = qLoc;

        if (qLoc < 40)
            start = 0;

        vector<float> window;
        window = slicing(filteredSignal, start, end);
        reverse(window.begin(), window.end());
        vector<float> windowGradient = gradient(window);

        int qrsOnset = start + 40 + 2 - arg_min(windowGradient);

        qrsOnsetPoints.push_back(qrsOnset);
    }
    return qrsOnsetPoints;
}


vector <int> WavesDetector::findQRSend() {
    cout << "[INFO] Get QRS-end..." << endl;
    int sLoc;
    vector<int> qrsEndPoints;

    for (int rLoc: rPeaks) {
        int s1Loc = findSPoint(20, rLoc, filteredSignal, rPeaks);
        int s2Loc = findSPoint(80, rLoc, filteredSignal, rPeaks);

        if (s1Loc == s2Loc || filteredSignal[s1Loc] < filteredSignal[s2Loc])
            sLoc = s1Loc;
        else
            sLoc = s2Loc;

        int start = sLoc;
        int end = sLoc + 40;

        if (sLoc > filteredSignal.size())
            end = int(filteredSignal.size());

        vector<float> window;
        window = slicing(filteredSignal, start, end);
        vector<float> windowGradient = gradient(window);

        int qrsEnd = sLoc + arg_min(windowGradient) - 2;

        qrsEndPoints.push_back(qrsEnd);
    }
    return qrsEndPoints;
}

