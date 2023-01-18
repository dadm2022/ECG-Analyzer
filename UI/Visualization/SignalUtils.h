//
// Created by Weronika on 16.01.2023.
//

#ifndef ECG_ANALYZER_SIGNALUTILS_H
#define ECG_ANALYZER_SIGNALUTILS_H
#include <tuple>
#include <vector>

namespace SignalUtils {
    std::tuple<double, double> getRangeFromTo(std::vector<float>&filteredSignal);
};
#endif //ECG_ANALYZER_SIGNALUTILS_H
