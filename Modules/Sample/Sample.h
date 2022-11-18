//
// Created by Bartosz Łukasik on 16/11/2022.
//
// Sample header file that should be used as reference
// for creating ECG algorithms.
//
// ImplNote: Module class MUST expose only one public method
// that handles all operations the module is created for
// e.g. R_Peaks MUST only have one public method execute()
// that will return indices of detected R peaks. All internal logic
// MUST NOT be exposed to external classes.
//

#ifndef ECG_ANALYZER_SAMPLE_H
#define ECG_ANALYZER_SAMPLE_H

#include <iostream>

class Sample {

    private:
        float a=0;
        float b=0;
        float c=0;

    public:
        Sample(float a, float b, float c);

    private:
        float calculateDelta() const;

    public:
        unsigned int getNumberOfRoots();
};

#endif //ECG_ANALYZER_SAMPLE_H
