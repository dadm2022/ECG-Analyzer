//
// Created by Piotr on 23.12.2022.
//

#include <algorithm>
#include "SignalUtils.h"

namespace SignalUtils {
    std::tuple<double, double> getRangeFromTo(std::vector<float> signal) {
        auto [minElement, maxElement] = std::minmax_element(signal.begin(), signal.end());
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
    }
}