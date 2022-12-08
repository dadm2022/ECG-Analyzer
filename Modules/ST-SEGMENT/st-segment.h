

#ifndef ECG_ANALYZER_SAMPLE_H
#define ECG_ANALYZER_SAMPLE_H

#include "iostream"
#include <vector>



class stsegment {

    public:
    stsegment();
 

    private:

    void findJpoints(std::vector<int> R_peaks);

    public:

};

#endif //ECG_ANALYZER_SAMPLE_H
