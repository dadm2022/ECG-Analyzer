#include "PanTompkins.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

#include <math.h>

using namespace std;


void PanTompkins::Normalize(vector<float> &v) const
{

    float MAX = *std::max_element(begin(v), end(v));
    float MIN = *std::min_element(begin(v), end(v));
    float MAX_ABS = (abs(MAX) >= abs(MIN)) ? abs(MAX) : abs(MIN);

    for_each(v.begin(), v.end(), [MAX_ABS](float &element)
    {
        element = element / MAX_ABS;
    });
}

// https://stackoverflow.com/questions/24518989/how-to-perform-1-dimensional-valid-convolution
template<typename T>
std::vector<T> PanTompkins::Conv(std::vector<T> const &f, std::vector<T> const &g) const
{
    int const nf = f.size();
    int const ng = g.size();
    int const n = nf + ng - 1;
    std::vector<T> out(n, T());
    for (auto i(0); i < n; ++i)
    {
        int const jmn = (i >= ng - 1) ? i - (ng - 1) : 0;
        int const jmx = (i < nf - 1) ? i : nf - 1;
        for (auto j(jmn); j <= jmx; ++j)
            out[i] += (f[j] * g[i - j]);
    }
    return move(out);
}


vector<float> PanTompkins::Filter(vector<float> signal, float fc1, float fc2) const
{

    const float M = 5.0;
    const float N = 2 * M + 1;

    vector<float> n1;
    for (int i = -M; i <= M; i++)
        n1.push_back(i);
    float fc = fc1 / (m_fs / 2);

    //  Impulse response of a discrete low-pass filter
    vector<float> y1;
    for (float value: n1)
    {
        float result;
        if ((M_PI * value) != 0)
            result = sin(2 * M_PI * fc * value) / (M_PI * value);
        else
            result = 2 * fc;
        y1.push_back(result);
    }

    // Impulse response of a discrete high-pass filter
    vector<float> y2;
    fc = fc2 / (m_fs / 2);
    for (float value: n1)
    {
        float result;
        if ((M_PI * value) != 0)
            result = -sin(2 * M_PI * fc * value) / (M_PI * value);
        else
            result = 1 - (2 * fc);
        y2.push_back(result);
    }

    vector<float> n(N); //n = 0:N-1
    iota(n.begin(), n.end(), 0);

    vector<float> window(N);
    for (auto i = 0; i < n.size(); i++)
        window[i] = 0.54 - 0.46 * cos(2 * M_PI * n[i] / (N - 1));

    for (auto i = 0; i < y1.size(); i++)
        y1[i] = window[i] * y1[i];

    for (auto i = 0; i < y2.size(); i++)
        y2[i] = window[i] * y2[i];

    vector<float> c1 = Conv(y1, signal);
    Normalize(c1);
    c1.erase(c1.begin(), c1.begin() + 6);

    vector<float> c2 = Conv(y2, c1);
    Normalize(c2);
    c2.erase(c2.begin(), c2.begin() + 16);

    return c2;
}


std::vector<int> PanTompkins::GetPeaks(std::shared_ptr<const std::vector<float>> electrocardiogram_signal, int fs)
{
    m_fs = fs;

    // I. Pre-processing
    vector<float> signal1 = Filter(*electrocardiogram_signal, 5, 15);

    // differentiation y[n] =1/8(−x[n−2]−2x[n−1] + 2x[n+ 1] +x[n+ 2])
    vector<float> signal2(signal1.size());
    for (auto i = 2; i < signal1.size() - 2; i++)
    {
        float val = 1. / 8 * (-signal1[i - 2] - 2 * signal1[i - 1] + 2 * signal1[i + 1] + signal1[i + 2]);
        signal2[i - 2] = val;
    }
    Normalize(signal2);

    // exponentiation
    for_each(signal2.begin(), signal2.end(), [](float &i)
    {
        i = pow(i, 2);
    });
    Normalize(signal2);

    // integration
    int C = 0.15 * m_fs; //150ms
    vector<float> window(C, 1. / C);
    vector<float> signal3 = Conv(window, signal2);
    Normalize(signal3);


    // II. Thresholding
    const int halfWindow = 360;
    const int fullWindow = 2 * halfWindow + 1;
    vector<int> peaks;
    vector<int> false_peaks;
    float signal_level = *max_element(signal3.begin(), signal3.begin() + fullWindow) * 1. / 3;
    float noise_level = 0.5 * signal_level;
    int i = 0;
    int step = 0.2 * m_fs;
    float local_max = 0.0;
    float local_max_val = 0.0;
    while (i + step < signal3.size())
    {
        float threshold = noise_level + 0.25 * (signal_level - noise_level);

        local_max_val = 0;
        while (signal3[i + 1] > signal3[i])
        { // find next peak
            local_max = i;
            local_max_val = signal3[i];

            i += 1;
        }

        if (local_max_val >= threshold)
        { // peak is the signal peak
            peaks.push_back(local_max);
            signal_level = 0.125 * local_max_val + 0.875 * signal_level;
            i = local_max + step;

        } else if (local_max_val < threshold)
        { // peak is the noise peak
            false_peaks.push_back(local_max);
            noise_level = 0.125 * local_max_val + 0.875 * noise_level;
        }

        i += 1;

    }

    vector<int> peaks_ekg; // in electrocardiogram_signal find maximums which are closer than 150ms to integrated signal peak
    for_each(peaks.begin(), peaks.end(), [electrocardiogram_signal, &peaks_ekg, this](float peak)
    {
        auto local = max_element(electrocardiogram_signal->begin() + peak - int(0.150 * m_fs), electrocardiogram_signal->begin() + peak + int(0.150 * m_fs));
        int local_idx = distance(electrocardiogram_signal->begin(), local);
        peaks_ekg.push_back(local_idx);
    });

    return move(peaks_ekg);

}
