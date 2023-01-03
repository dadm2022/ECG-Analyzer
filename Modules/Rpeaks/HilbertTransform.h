/*
 * Sources:
 * A New QRS Detection Algorithm Based on the Hilbert Transform DS Benitez', PA Gaydecki, A Zaidi, AP Fitzpatric
 */

#ifndef ECG_ANALYZER_HILBERTTRANSFORM_H
#define ECG_ANALYZER_HILBERTTRANSFORM_H

#include <vector>
#include <memory>

/*
 * Example usage:
 * HilbertTransform hilbert = HilbertTransform();
 * std::shared_ptr<const std::vector<float>> electrocardiogram_signal = make_shared<std::vector<float>>(signal.data);
 * vector<int> peaks = hilbert.GetPeaks(electrocardiogram_signal);
 */
class HilbertTransform
{
public:
    HilbertTransform() = default;


    /*
     * @brief Function for finding R peaks using Hilbert Transform
     *@param[in] EKG signal
     *@return Indexes of R peaks for given signal
     */
    std::vector<int> GetPeaks(std::shared_ptr<const std::vector<float>> electrocardiogram_signal, int fs = 360);

private:
    static constexpr int m_kSize = 1024; // size of window in Hilbert Transform
    int m_fs; // electrocardiogram_signal sampling frequency
    /*
     * @brief compute Hilbert Transform on window with size HilbertTransform::m_kSize,
     * @param[in] first index to start
     */
    std::vector<float> ComputeHilbertTransform(std::vector<float> signal, int first) const;

    std::vector<float> Derivative(std::vector<float> &signal) const;

    std::vector<float> Filter(const std::vector<float> &signal, float fc1, float fc2) const;

    int CalcAverageDistance(std::vector<int> &peaks) const;

    float CalcRMSValue(std::vector<float> &signal) const;

    void Normalize(std::vector<float> &v) const;

    template<typename T>
    std::vector<T> Conv(std::vector<T> const &f, std::vector<T> const &g) const;

    float Factorial(int n) const;
};


#endif //ECG_ANALYZER_HILBERTTRANSFORM_H
