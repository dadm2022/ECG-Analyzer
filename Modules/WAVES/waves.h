//
// Created by gosia on 19.11.2022.
//

#ifndef ECG_ANALYZER_WAVES_H
#define ECG_ANALYZER_WAVES_H
#include <complex>
#include <iostream>
#include <utility>
#include <valarray>
#include <vector>
#include <memory>

using namespace std;

class Utils {
public:
    template <typename T, typename A> int arg_min(std::vector<T, A> const& vec);
    template <typename T> vector<T> gradient(vector<T> input);
    template <typename T> vector<T> slicing(vector<T> const& v, int X, int Y);
};


class QRS  : public Utils{
public:
    int findQPoint(int interval, int rLoc, const vector<float>& filteredSignal, const vector<int>& rPeaks);
    int findSPoint(int interval, int rLoc, const vector<float>& filteredSignal, const vector<int>& rPeaks);
};


class WavesDetector : public QRS {
private:
    vector<int>QRSonset;
    vector<int>QRSend;
    vector<int>findQRSonset();
    vector<int>findQRSend();
public:
    vector<float>filteredSignal;
    vector<int>rPeaks;

    WavesDetector(const vector<float>&filteredSignal, const vector<int>&rPeaks) :
    filteredSignal(filteredSignal), rPeaks(rPeaks){
        QRSonset = findQRSonset();
        QRSend = findQRSend();
    };

    vector <int> getQRSonset() { return QRSonset; };
    vector <int> getQRSend() { return QRSend; };
};


// Temporary functions
vector <float> loadSignal();
vector <float> loadRpeaks();

#endif //ECG_ANALYZER_WAVES_H
