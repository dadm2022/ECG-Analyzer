#include "PanTompkins.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#include <math.h>

using namespace std;


void PanTompkins::Normalize(vector<double> &v) const {

    double MAX = *std::max_element(begin(v), end(v));
    double MIN = *std::min_element(begin(v), end(v));
    double MAX_ABS = (abs(MAX) >= abs(MIN)) ? abs(MAX) : abs(MIN);

    for (auto i = 0; i < v.size(); i++) {
        v[i] = v[i] / MAX_ABS;
    }
}

template<typename T>
std::vector<T> PanTompkins::Conv(std::vector<T> const &f, std::vector<T> const &g) const {
    int const nf = f.size();
    int const ng = g.size();
    int const n = nf + ng - 1;
    std::vector<T> out(n, T());
    for (auto i(0); i < n; ++i) {
        int const jmn = (i >= ng - 1) ? i - (ng - 1) : 0;
        int const jmx = (i < nf - 1) ? i : nf - 1;
        for (auto j(jmn); j <= jmx; ++j) {
            out[i] += (f[j] * g[i - j]);
        }
    }
    return out;
}


vector<double> PanTompkins::Filter(vector<double> signal, double fc1, double fc2) const {

    double M = 5;
    double N = 2 * M + 1;

    vector<double> n1;
    for (int i = -M; i <= M; i++) {
        n1.push_back(i);
    }
    double fc = fc1 / (fs / 2);

    //  Impulse response of a discrete low-pass filter
    vector<double> y1;
    for (double value: n1) {
        double result;
        if ((M_PI * value) != 0) {
            result = sin(2 * M_PI * fc * value) / (M_PI * value);
        } else {
            result = 2 * fc;
        }
        y1.push_back(result);
    }

    // Impulse response of a discrete high-pass filter
    vector<double> y2;
    fc = fc2 / (fs / 2);
    for (double value: n1) {
        double result;
        if ((M_PI * value) != 0) {
            result = -sin(2 * M_PI * fc * value) / (M_PI * value);
        } else {
            result = 1 - (2 * fc);
        }
        y2.push_back(result);
    }

    vector<double> n(N); //n = 0:N-1
    iota(n.begin(), n.end(), 0);

    vector<double> window(N);
    for (auto i = 0; i < n.size(); i++) {
        window[i] = 0.54 - 0.46 * cos(2 * M_PI * n[i] / (N - 1));
    }

    for (auto i = 0; i < y1.size(); i++) {
        y1[i] = window[i] * y1[i];
    }
    for (auto i = 0; i < y2.size(); i++) {
        y2[i] = window[i] * y2[i];
    }
    vector<double> c1 = Conv(y1, signal);
    Normalize(c1);
    c1.erase(c1.begin(), c1.begin() + 6);

    vector<double> c2 = Conv(y2, c1);
    Normalize(c2);
    c2.erase(c2.begin(), c2.begin() + 16);

    return c2;
}


std::vector<int> PanTompkins::GetPeaks(std::vector<double> electrocardiogram_signal) const {

    // I. Pre-processing
    vector<double> signal1 = Filter(electrocardiogram_signal, 15, 5);

    // differentiation y[n] =1/8(−x[n−2]−2x[n−1] + 2x[n+ 1] +x[n+ 2])
    vector<double> signal2(signal1.size());
    for (auto i = 2; i < signal1.size() - 2; i++) {
        double val = 1. / 8 * (-signal1[i - 2] - 2 * signal1[i - 1] + 2 * signal1[i + 1] + signal1[i + 2]);
        signal2[i - 2] = val;
    }
    Normalize(signal2);

    // exponentiation
    for (auto i = 0; i < signal2.size(); i++) {
        signal2[i] = pow(signal2[i], 2);
    }
    Normalize(signal2);

    // integration
    int C = 0.15 * fs; //150ms
    vector<double> window(C, 1. / C);
    vector<double> signal3 = Conv(window, signal2);
    Normalize(signal3);


    // II. Thresholding
    const int halfWindow = 360;
    const int fullWindow = 2 * halfWindow + 1;
    vector<int> peaks;
    vector<int> false_peaks;
    double signal_level = *max_element(signal3.begin(), signal3.begin() + fullWindow) * 1. / 3;
    double noise_level = 0.5 * signal_level;
    int i = 0;
    int step = 0.2 * fs;
    double local_max = 0;
    double local_max_val = 0;
    while (i + step < signal3.size()) {
        double threshold = noise_level + 0.25 * (signal_level - noise_level);

        local_max_val = 0;
        while (signal3[i + 1] > signal3[i]) { // find next peak
            local_max = i;
            local_max_val = signal3[i];

            i += 1;
        }

        if (local_max_val >= threshold) { // peak is the signal peak
            peaks.push_back(local_max);
            signal_level = 0.125 * local_max_val + 0.875 * signal_level;
            i = local_max + step;

        } else if (local_max_val < threshold) { // peak is the noise peak
            false_peaks.push_back(local_max);
            noise_level = 0.125 * local_max_val + 0.875 * noise_level;
        }

        i += 1;

    }

    vector<int> peaks_ekg; // in electrocardiogram_signal find maximums which are closer than 150ms to integrated signal peak
    for (int i = 0; i < peaks.size(); i++) {
        auto local = max_element(electrocardiogram_signal.begin() + peaks[i] - int(0.150 * fs), electrocardiogram_signal.begin() + peaks[i] + int(0.150 * fs));
        int local_idx = distance(electrocardiogram_signal.begin(), local);
        peaks_ekg.push_back(local_idx);
    }

    return peaks_ekg;

}
