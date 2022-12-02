/*
 * Sources:
 * A New QRS Detection Algorithm Based on the Hilbert Transform DS Benitez', PA Gaydecki, A Zaidi, AP Fitzpatric
 */

#ifndef ECG_ANALYZER_HILBERTTRANSFORM_H
#define ECG_ANALYZER_HILBERTTRANSFORM_H

#include <vector>

/*
 * Example usage:
 * HilbertTransform hilbert = HilbertTransform();
 * vector<int> peaks = hilbert.GetPeaks(signal);
 */
class HilbertTransform {
public:
    HilbertTransform() = default;

    /*
     * @brief Function for finding R peaks using Hilbert Transform
     *@param[in] EKG signal
     *@return Indexes of R peaks for given signal
     */
    std::vector<int> GetPeaks(std::vector<double> electrocardiogram_signal) const;

private:
    const int SIZE = 1024; // size of window in Hilbert Transform
    const int fs = 360; // electrocardiogram_signal sampling frequency
    /*
     * @brief compute Hilbert Transform on window with size HilbertTransform::SIZE,
     * @param[in] first index to start
     */
    std::vector<double> ComputeHilbertTransform(std::vector<double> signal, int first) const;

    std::vector<double> Derivative(std::vector<double> signal) const;

    std::vector<double> Filter(std::vector<double> signal, double fc1, double fc2) const;

    int CalcAverageDistance(std::vector<int> peaks) const;

    double CalcRMSValue(std::vector<double> signal) const;

    void Normalize(std::vector<double> &v) const;

    template<typename T>
    std::vector<T> Conv(std::vector<T> const &f, std::vector<T> const &g) const;

    double Factorial(int n) const;
};


#endif //ECG_ANALYZER_HILBERTTRANSFORM_H
