#include "HilbertTransform.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <gsl/gsl_fft_complex.h>
#include <cmath>
#include <gsl/gsl_complex_math.h>
#include <numeric>
#include <boost/math/special_functions.hpp>

using namespace std;


void HilbertTransform::Normalize(vector<float> &v) const
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
std::vector<T> HilbertTransform::Conv(std::vector<T> const &f, std::vector<T> const &g) const
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
        {
            out[i] += (f[j] * g[i - j]);
        }
    }
    return move(out);
}


float HilbertTransform::Factorial(int n) const
{
    if ((n == 0) || (n == 1))
        return 1;
    else
        return n * Factorial(n - 1);
}


vector<float> HilbertTransform::Filter(const vector<float> &signal, float fc1, float fc2) const
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

    const float alpha = 3; //parametr Kaisera, najczęsciej 3
    vector<float> window;

    for_each(n.begin(), n.end(), [alpha, &window, N](float n)
    {
        float ans = pow(2 * n / N - 1, 2);
        ans = M_PI * alpha * sqrt(1 - ans);
        ans = boost::math::cyl_bessel_i(0, ans) / boost::math::cyl_bessel_i(0, M_PI * alpha);
        window.push_back(ans);
    });

    for (auto i = 0; i < y1.size(); i++)
    {
        y1[i] = window[i] * y1[i];
        y2[i] = window[i] * y2[i];
    }
    vector<float> c1 = Conv(y1, signal);
    Normalize(c1);
    vector<float> c2 = Conv(y2, c1);
    Normalize(c2);
    return c2;
}


float HilbertTransform::CalcRMSValue(vector<float> &signal) const
{
    float rms = 0.f;
    for_each(signal.begin(), signal.end(), [&rms](float x)
    {
        rms += x * x;
    });
    if (signal.size() == 0)
        return 0;
    return sqrt(rms / signal.size());
}


vector<float> HilbertTransform::Derivative(vector<float> &signal) const
{
    vector<float> deriv;
    for (int i = 1; i < signal.size() - 1; i++)
    { // skip boundaries
        float acc = 1. / (2 * m_fs) * (signal[i + 1] - signal[i]);
        deriv.push_back(acc);
    }
    return move(deriv);
}


vector<float> HilbertTransform::ComputeHilbertTransform(vector<float> signal, int first) const
{
#define REAL(z, i) ((z)[2*(i)])
#define IMAG(z, i) ((z)[2*(i)+1])

    double data[2 * m_kSize];

    for (int i = 0; i < m_kSize; i++)
    {
        REAL(data, i) = signal[first + i];
        IMAG(data, i) = 0.0;
    }

    // fast fourier transform
    gsl_fft_complex_radix2_forward(data, 1, m_kSize);

    // set DC component to 0
    REAL(data, 0) = 0.0;
    IMAG(data, 0) = 0.0;

    for (int i = 0; i < m_kSize / 2; i++)
    {
        gsl_complex a, b;
        GSL_REAL(a) = REAL(data, i);
        GSL_IMAG(a) = IMAG(data, i);
        GSL_REAL(b) = 0.0;
        GSL_IMAG(b) = -1.0;

        gsl_complex c = gsl_complex_mul(a, b);
        REAL(data, i) = GSL_REAL(c);
        IMAG(data, i) = GSL_IMAG(c);
    }


    for (int i = m_kSize / 2; i < m_kSize; i++)
    {
        gsl_complex a, b;
        GSL_REAL(a) = REAL(data, i);
        GSL_IMAG(a) = IMAG(data, i);
        GSL_REAL(b) = 0.0;
        GSL_IMAG(b) = 1.0;

        gsl_complex c = gsl_complex_mul(a, b);
        REAL(data, i) = GSL_REAL(c);
        IMAG(data, i) = GSL_IMAG(c);
    }

    // inverse fast fourier transform
    gsl_fft_complex_radix2_inverse(data, 1, m_kSize);

    vector<float> h;
    for (int i = 0; i < m_kSize; i++)
        h.push_back(REAL(data, i));

#undef REAL
#undef IMAG
    return h;
}


int HilbertTransform::CalcAverageDistance(vector<int> &peaks) const
{
    float distance = 0.0;
    for (int i = 1; i < peaks.size(); i++)
        distance += (peaks[i] - peaks[i - 1]);
    return distance / (peaks.size() - 1);
}


vector<int> HilbertTransform::GetPeaks(std::shared_ptr<const std::vector<float>> electrocardiogram_signal, int fs)
{
    m_fs = fs;

    vector<float> signal = Filter(*electrocardiogram_signal, 8, 20);
    signal.erase(signal.begin(), signal.begin() + 9); // offset caused by filters
    signal = this->Derivative(signal);

    int i = 0;
    float max_amplitude_old = 0.0;
    vector<int> peaks;
    vector<int> windows;
    while ((i + m_kSize) < signal.size())
    {

        windows.push_back(i);
        vector<float> signal_hilb = ComputeHilbertTransform(signal, i);
        float rms_value = CalcRMSValue(signal_hilb);
        auto max_amplitude = max_element(signal_hilb.begin(), signal_hilb.end());
        float threshold;
        if (rms_value >= 0.18 * *max_amplitude)
        {
            threshold = 0.39 * *max_amplitude;
            if (*max_amplitude > 2 * max_amplitude_old)
                threshold = 0.0039 * max_amplitude_old;
        } else
        { //rms_value < 0.18 * rms_value
            threshold = 1.6 * rms_value;
        }

        if (*max_amplitude < threshold)
        { //peak was not found
            cout << "Peak was not found" << endl;
            continue;
        }

        float maximum = 0.0;
        float maximum_val = 0.0;


        for (int j = 0; j < signal_hilb.size(); j++)
        {
            if (signal_hilb[j] > threshold && signal_hilb[j] > maximum_val)
            {
                maximum = j;
                maximum_val = signal_hilb[j];
            }
            if (signal_hilb[j] < maximum_val && maximum_val > 0)
                break;
        }

        int peak_current = i + maximum;

        // if there are two R peaks closer than 200ms, calculate the average spacing between the R peaks and choose from the one that is closer to the average
        if (peaks.size() >= 3 && peak_current - peaks.back() < 0.2 * m_fs)
        { // 200ms * 360 1/s
            int peak_last = peaks.back();
            peaks.pop_back();
            int average_distance = CalcAverageDistance(peaks);
            int peak_second_to_last = peaks.back();

            int dist1 = abs(average_distance - (peak_second_to_last - peak_last));
            int dist2 = abs(average_distance - (peak_second_to_last - peak_current));
            if (dist1 <= dist2) // peak_last is closer to the average distance between peaks
                peaks.push_back(peak_last);
            else // peak_current -||-
                peaks.push_back(peak_current);

        } else
        {
            peaks.push_back(peak_current);
        }
        i = peak_current + 1; // the first element of the next window is the element next to the found peak
        max_amplitude_old = *max_amplitude;

    }

    return move(peaks);

}
