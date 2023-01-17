//
// Created by Klaudiusz Przetacznik on 15/01/2023.
//


#include "TWaveAlt.h"

TWaveAlt::TWaveAlt(std::shared_ptr<const std::vector<int>>& m_tEndPoints, std::shared_ptr<const std::vector<float>>& FilteredSignal) {
    tInd = m_tEndPoints;
    signal = FilteredSignal;
    ind_len = (int)(fs*0.15);
    tpoint_size = (m_tEndPoints -> size());
    Alt = 0;
    IfAlt = false;
}

std::vector<float> TWaveAlt::argMax(){
    std::vector<float> altMax;
    auto it = 0;
    float temp;
    while(it < tpoint_size){
        if(tInd -> at(it) - ind_len >= 0){
            temp = *std::max_element(&signal ->at(tInd->at(it) - ind_len),&signal ->at(tInd->at(it)));
            altMax.push_back(temp);
        }
        it++;
    }
    return std::move(altMax);
}

 void TWaveAlt::DetectAlt() {
    auto it = 0;
    int altCount = 0;
    std::vector<float> altMax = TWaveAlt::argMax();
    while (it < altMax.size() - 3) {

        float diffA1A3 = fabs(altMax[it] - altMax[it + 2]);
        float diffA2A4 = fabs(altMax[it + 1] - altMax[it + 3]);
        float diffA1A2 = fabs(altMax[it] - altMax[it + 1]);
        float diffA3A4 = fabs(altMax[it + 2] - altMax[it + 3]);
        if ( diffA1A3 < diffA1A2 && diffA2A4 < diffA3A4 && diffA2A4 < diffA1A2 && diffA1A3 < diffA3A4 ) {
            altCount++;
        }
        it++;
    }
    float percentage = (float) altCount / (float) it;

    TWaveAlt::Alt = percentage * 100;

    if (percentage < 0.1) {
        TWaveAlt::IfAlt = false;
    }
    else{
        TWaveAlt::IfAlt = true;
    }

}



