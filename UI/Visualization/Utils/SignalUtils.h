//
// Created by Piotr on 23.12.2022.
//

#ifndef ECG_ANALYZER_SIGNALUTILS_H
#define ECG_ANALYZER_SIGNALUTILS_H

#include <tuple>
#include <vector>

namespace SignalUtils {
    std::tuple<double, double> getRangeFromTo(const std::vector<float>& signal);
};

#endif //ECG_ANALYZER_SIGNALUTILS_H
