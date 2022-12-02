#include "HilbertTransform.h"

#include <vector>
#include <algorithm>
#include <gsl/gsl_fft_complex.h>
#include <cmath>
#include <gsl/gsl_complex_math.h>
#include <numeric>


using namespace std;


void HilbertTransform::Normalize(vector<double> &v) const {

    double MAX = *std::max_element(begin(v), end(v));
    double MIN = *std::min_element(begin(v), end(v));
    double MAX_ABS = (abs(MAX) >= abs(MIN)) ? abs(MAX) : abs(MIN);

    for (auto i = 0; i < v.size(); i++) {
        v[i] = v[i] / MAX_ABS;
    }
}

template<typename T>
std::vector<T> HilbertTransform::Conv(std::vector<T> const &f, std::vector<T> const &g) const {
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


double HilbertTransform::Factorial(int n) const {
    if ((n == 0) || (n == 1))
        return 1;
    else
        return n * Factorial(n - 1);
}


vector<double> HilbertTransform::Filter(vector<double> signal, double fc1, double fc2) const {
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

    double alpha = 3; //parametr Kaisera, najczęsciej 3
    vector<double> window(N);
    for (auto i = 0; i < n.size(); i++) {
        double ans = pow(2 * n[i] / N - 1, 2);
        ans = M_PI * alpha * sqrt(1 - ans);

        ans = cyl_bessel_i(0, ans) / cyl_bessel_i(0, M_PI * alpha);
        window[i] = ans;
    }

    for (auto i = 0; i < y1.size(); i++) {
        y1[i] = window[i] * y1[i];
        y2[i] = window[i] * y2[i];
    }
    vector<double> c1 = Conv(y1, signal);
    Normalize(c1);
    vector<double> c2 = Conv(y2, c1);
    Normalize(c2);
    return c2;
}


double HilbertTransform::CalcRMSValue(vector<double> signal) const {
    double rms = 0;
    for (int i = 0; i < signal.size(); i++) {
        rms += signal[i] * signal[i];
    }
    return sqrt(rms / signal.size());
}

vector<double> HilbertTransform::Derivative(vector<double> signal) const {
    vector<double> deriv;
    for (int i = 1; i < signal.size() - 1; i++) { // skip boundaries
        double acc = 1. / (2 * fs) * (signal[i + 1] - signal[i]);
        deriv.push_back(acc);
    }
    return deriv;
}


vector<double> HilbertTransform::ComputeHilbertTransform(vector<double> signal, int first) const {
#define REAL(z, i) ((z)[2*(i)])
#define IMAG(z, i) ((z)[2*(i)+1])

    double data[2 * SIZE];

    for (int i = 0; i < SIZE; i++) {
        REAL(data, i) = signal[first + i];
        IMAG(data, i) = 0.0;
    }

    // fast fourier transform
    gsl_fft_complex_radix2_forward(data, 1, SIZE);

    // set DC component to 0
    REAL(data, 0) = 0.0;
    IMAG(data, 0) = 0.0;

    for (int i = 0; i < SIZE / 2; i++) {
        gsl_complex a, b;
        GSL_REAL(a) = REAL(data, i);
        GSL_IMAG(a) = IMAG(data, i);
        GSL_REAL(b) = 0.0;
        GSL_IMAG(b) = -1.0;

        gsl_complex c = gsl_complex_mul(a, b);
        REAL(data, i) = GSL_REAL(c);
        IMAG(data, i) = GSL_IMAG(c);
    }


    for (int i = SIZE / 2; i < SIZE; i++) {
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
    gsl_fft_complex_radix2_inverse(data, 1, SIZE);

    vector<double> h;
    for (int i = 0; i < SIZE; i++) {
        h.push_back(REAL(data, i));
    }

#undef REAL
#undef IMAG
    return h;
}


int HilbertTransform::CalcAverageDistance(vector<int> peaks) const {
    double distance = 0;
    for (int i = 1; i < peaks.size(); i++) {
        distance += (peaks[i] - peaks[i - 1]);
    }
    return distance / (peaks.size() - 1);
}


vector<int> HilbertTransform::GetPeaks(vector<double> electrocardiogram_signal) const {


    vector<double> signal = Filter(electrocardiogram_signal, 8, 20);
    signal.erase(signal.begin(), signal.begin() + 9); // offset caused by filters
    signal = this->Derivative(signal);

    int i = 0;
    double max_amplitude_old = 0;
    vector<int> peaks;
    vector<int> windows;
    while ((i + SIZE) < signal.size()) {

        windows.push_back(i);
        vector<double> signal_hilb = ComputeHilbertTransform(signal, i);
        double rms_value = CalcRMSValue(signal_hilb);
        auto max_amplitude = max_element(signal_hilb.begin(), signal_hilb.end());
        double threshold;
        if (rms_value >= 0.18 * *max_amplitude) {
            threshold = 0.39 * *max_amplitude;
            if (*max_amplitude > 2 * max_amplitude_old) {
                threshold = 0.0039 * max_amplitude_old;
            }
        } else { //rms_value < 0.18 * rms_value
            threshold = 1.6 * rms_value;
        }

        if (*max_amplitude < threshold) { //peak was not found
            throw ("Peak was not found");
        }

        double maximum = 0;
        double maximum_val = 0;
        for (int j = 0; j < signal_hilb.size(); j++) {
            if (signal_hilb[j] > threshold && signal_hilb[j] > maximum_val) {
                maximum = j;
                maximum_val = signal_hilb[j];
            }
            if (signal_hilb[j] < maximum_val && maximum_val > 0) {
                break;
            }
        }

        int peak_current = i + maximum;

        // if there are two R peaks closer than 200ms, calculate the average spacing between the R peaks and choose from the one that is closer to the average
        if (peaks.size() >= 3 && peak_current - peaks.back() < 0.2 * fs) { // 200ms * 360 1/s
            int peak_last = peaks.back();
            peaks.pop_back();
            int average_distance = CalcAverageDistance(peaks);
            int peak_second_to_last = peaks.back();

            int dist1 = abs(average_distance - (peak_second_to_last - peak_last));
            int dist2 = abs(average_distance - (peak_second_to_last - peak_current));
            if (dist1 <= dist2) { // peak_last is closer to the average distance between peaks
                peaks.push_back(peak_last);
            } else { // peak_current -||-
                peaks.push_back(peak_current);

            }
        } else {
            peaks.push_back(peak_current);
        }
        i = peak_current + 1; // the first element of the next window is the element next to the found peak
        max_amplitude_old = *max_amplitude;

    }


    return peaks;

}
