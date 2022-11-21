//
// Created by gosia on 19.11.2022.
//

#ifndef ECG_ANALYZER_WAVES_H
#define ECG_ANALYZER_WAVES_H
#include <iostream>
#include <vector>

using namespace std;

class Utils {
public:
    template <typename T, typename A> int arg_max(std::vector<T, A> const& vec);
    template <typename T, typename A> int arg_min(std::vector<T, A> const& vec);
    template <typename T> double getAverage(std::vector<T> const& v);
    template <typename T> vector<T> gradient(vector<T> input);
    template <typename T> vector<T> slicing(vector<T> const& v, int X, int Y);
};

class WavesDetector : public Utils {
private:
    vector<int>QRSonset;
    vector<int>QRSend;
    vector<int>Tend;
    vector<int>Ponset;
    vector<int>Pend;
    int findQPoint(int interval, int rLoc);
    int findSPoint(int interval, int rLoc);
    int findBoundaryPoint(vector<float> window);
    void findQRSonset();
    void findQRSend();
    void findTend();
    void findPwaveBoundaryPoints();

public:
    vector<float>filteredSignal;
    vector<int>rPeaks;

    WavesDetector(const vector<float>&filteredSignal, const vector<int>&rPeaks) :
    filteredSignal(filteredSignal), rPeaks(rPeaks){
        findQRSonset();
        findQRSend();
        findTend();
        findPwaveBoundaryPoints();
    };

    vector <int> getQRSonset() { return QRSonset; };
    vector <int> getQRSend() { return QRSend; };
    vector <int> getTend() { return Tend; };
    vector <int> getPonset() { return Ponset; };
    vector <int> getPend() { return Pend; };
};


// Temporary functions
vector <float> loadSignal();
vector <float> loadRpeaks();

#endif //ECG_ANALYZER_WAVES_H
