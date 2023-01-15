//
// Created by Klaudiusz Przetacznik on 15/01/2023.
//

#ifndef ECG_ANALYZER_TWAVEALT_H
#define ECG_ANALYZER_TWAVEALT_H

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
/**
 * Example of usage:
 *
 * std::shared_ptr<const std::vector<int>>& m_tEndPoints
 * std::shared_ptr<const std::vector<float>>& FilteredSignal
 * int m_fs
 *
 * TWaveAlt alt(tEndPoints, FilteredSignal, fs);
 * alt.DetectAlt();
 * float alt_percentage = alt.GetAlt(); - percentage of detected alternans compared to the whole signal
 * bool alt_present = alt.GetIfAlt(); - true if alternans crosses the threshold of 10%
 *
 */
class TWaveAlt{
private:

    std::shared_ptr<const std::vector<int>> tInd;
    std::shared_ptr<const std::vector<float>> signal;
    int fs;

    float Alt;
    bool IfAlt;
    int ind_len;
    int tpoint_size;

    /**
     * TWaveAlt::argMax()
     * calculates highest value in the T-wave using a window of 0.15*fs
     * returns a vector of highest T-wave values for each wave
     */
    std::vector<float> argMax();
public:
    //constructor
    TWaveAlt(std::shared_ptr<const std::vector<int>>& m_tEndPoints, std::shared_ptr<const std::vector<float>>& FilteredSignal, int m_fs);

    /**
     * TWaveAlt::DetectAlt();
     * detects T-wave alternans by comparing T-wave peaks in 4 values window with a stride of 1
     * sets values of TWaveAlt::Alt and TWaveAlt::IfAlt accordingly
     */
    void DetectAlt();

    //TWave::IfAlt getter
    bool GetIfAlt() {return IfAlt;};

    //TWave::Alt getter
    float GetAlt() {return Alt;};


};

#endif //ECG_ANALYZER_TWAVEALT_H
