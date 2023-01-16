//
// Created by Weronika on 16.01.2023.
//

#include "SignalUtils.h"
#include <algorithm>
namespace SignalUtils {
    std::tuple<double, double> getRangeFromTo(std::vector<float> &filteredSignal) {
        auto [minElement, maxElement] = std::minmax_element(filteredSignal.begin(), filteredSignal.end());
        double from, to;
        if (*minElement < 0) {
            from = *minElement * 1.1;
        } else {
            from = *minElement * 0.9;
        }
        if (*maxElement < 0) {
            to = *maxElement * 0.9;
        } else {
            to = *maxElement * 1.1;
        }
        return {from, to};
    }}