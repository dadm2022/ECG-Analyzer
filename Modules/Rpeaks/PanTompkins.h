/*
 * Sources:
 * J. Pan and W. J. Tompkins, "A Real-Time QRS Detection Algorithm," in IEEE Transactions on Biomedical Engineering, vol. BME-32, no. 3, pp. 230-236, March 1985, doi: 10.1109/TBME.1985.325532.
 * Lab1 qrs
 * https://en.wikipedia.org/wiki/Pan%E2%80%93Tompkins_algorithm
 */

#ifndef ECG_ANALYZER_PANTOMPKINS_H
#define ECG_ANALYZER_PANTOMPKINS_H


#include <vector>

/*
 * Example usage:
 * PanTompkins pan_tompkins = PanTompkins();
 * vector<int> peaks = pan_tompkins.GetPeaks(signal);
 */
class PanTompkins {
public:
    PanTompkins() = default;

    /*@brief Function for finding R peaks using Pan Tompkins method
     *@param[in] EKG signal
     *@return indexes of R peaks for given signal
     */
    std::vector<int> GetPeaks(std::vector<double> electrocardiogram_signal) const;

private:
    const int fs = 360;

    std::vector<double> Filter(std::vector<double> signal, double fc1, double fc2) const;

    template<typename T>
    std::vector<T> Conv(std::vector<T> const &f, std::vector<T> const &g) const;

    void Normalize(std::vector<double> &v) const;
};


#endif //ECG_ANALYZER_PANTOMPKINS_H
