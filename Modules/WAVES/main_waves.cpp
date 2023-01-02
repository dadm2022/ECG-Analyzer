//
// Created by gosia on 19.11.2022.
//
#include "waves.h"
#include <iostream>
#include <vector>
#include <fstream>

const string signal_path = "C:/Users/gosia/Desktop/DADM/projekt/100_MLII_filtered.dat";
const string r_peaks_path = "C:/Users/gosia/Desktop/DADM/projekt/r_peaks_100.dat";

using namespace std;


int main()
{
    vector <float> filteredSignal {loadSignal()};

    vector <float> rPeaks {loadRpeaks()};
    vector<int> rPeaksLocs(rPeaks.begin(), rPeaks.end());

    cout << "count of samples in signal = " << filteredSignal.size() << endl;
    cout << "count of R peaks = " << rPeaks.size() << endl;

    WavesDetector wavesDetector(filteredSignal, rPeaksLocs);

    vector<int> QRSonset = wavesDetector.getQRSonset();
    cout << "number of QRS-onset = " << QRSonset.size() << endl;
    std::ofstream out1File("C:/Users/gosia/Desktop/DADM/projekt/QRS-onset.txt");
    for (const auto &e : QRSonset) out1File << e << "\n";

    vector<int> QRSend = wavesDetector.getQRSend();
    std::ofstream out2File("C:/Users/gosia/Desktop/DADM/projekt/QRS-offset.txt");
    for (const auto &e : QRSend) out2File << e << "\n";

    cout << "number of QRS-end = " << QRSend.size() << endl;

    vector<int> Tend = wavesDetector.getTend();
    std::ofstream out3File("C:/Users/gosia/Desktop/DADM/projekt/T-end.txt");
    for (const auto &e : Tend) out3File << e << "\n";

    cout << "number of T-end = " << Tend.size() << endl;

    vector<int> Ponset = wavesDetector.getPonset();
    std::ofstream out4File("C:/Users/gosia/Desktop/DADM/projekt/pOnsetPoints.txt");
    for (const auto &e : Ponset) out4File << e << "\n";

    cout << "number of P-onset = " << Ponset.size() << endl;

    vector<int> Pend = wavesDetector.getPend();
    std::ofstream out5File("C:/Users/gosia/Desktop/DADM/projekt/pEndPoints.txt");
    for (const auto &e : Pend) out5File << e << "\n";

    cout << "number of P-end = " << Pend.size() << endl;

    return 0;
}

// Internal functions

vector<float> loadSignal() {
    cout << "[INFO] Loading signal..." << endl;

    ifstream fin(signal_path);
    float num;
    vector<float> signal;

    while (fin >> num)
        signal.push_back(num);

    return signal;
}

vector<float> loadRpeaks() {
    cout << "[INFO] Loading r_peaks..." << endl;

    ifstream fin(r_peaks_path);
    float num;
    vector<float> r_peaks;

    while (fin >> num)
        r_peaks.push_back(num);

    return r_peaks;
}
