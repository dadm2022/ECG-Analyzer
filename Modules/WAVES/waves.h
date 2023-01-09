//
// Created by gosia on 19.11.2022.
//

#ifndef ECG_ANALYZER_WAVES_H
#define ECG_ANALYZER_WAVES_H
#include <vector>

/* Example usage:
    WavesDetector wavesDetector(filteredSignal, rPeaksLocs);
    std::vector<int> QRSonset = wavesDetector.getQRSonset();
    std::vector<int> Ponset = wavesDetector.getPonset();
*/

class Utils {
public:
    template <typename T, typename A> int arg_max(std::vector<T, A> const& vec);
    template <typename T, typename A> int arg_min(std::vector<T, A> const& vec);
    template <typename T> double getAverage(std::vector<T> const& v);
    template <typename T> std::vector<T> gradient(std::vector<T> input);
    template <typename T> std::vector<T> slicing(std::vector<T> const& v, int X, int Y);
};

class WavesDetector : public Utils {
private:
    std::vector<int>m_qrsOnsetPoints;
    std::vector<int>m_qrsEndPoints;
    std::vector<int>m_tEndPoints;
    std::vector<int>m_pOnsetPoints;
    std::vector<int>m_pEndPoints;
    int findQPoint(int interval, int rLoc);
    int findSPoint(int interval, int rLoc);
    int findPwaveBoundaryPoint(std::vector<float> window);
    void findQRSonset();
    void findQRSend();
    void findTend();
    void findPwaveBoundaryPoints();

public:
    std::vector<float>filteredSignal;
    std::vector<int>rPeaks;

    WavesDetector(std::vector<float> filteredSignal, std::vector<int> rPeaks) : filteredSignal(std::move(filteredSignal)), rPeaks(std::move(rPeaks)){
        findQRSonset();
        findQRSend();
        findTend();
        findPwaveBoundaryPoints();
    };

    std::vector <int> getQRSonset() { return m_qrsOnsetPoints; };
    std::vector <int> getQRSend() { return m_qrsEndPoints; };
    std::vector <int> getTend() { return m_tEndPoints; };
    std::vector <int> getPonset() { return m_pOnsetPoints; };
    std::vector <int> getPend() { return m_pEndPoints; };
};

#endif //ECG_ANALYZER_WAVES_H
