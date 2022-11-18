//
// Created by Marcel Pikuła on 18/11/2022.
//

#ifndef ECG_ANALYZER_VIEWMODELUI_H
#define ECG_ANALYZER_VIEWMODELUI_H
#include "memory"

class Sample;

class ViewmodelUI
{
    unsigned int m_numberOfRoots = 0;
    std::shared_ptr<Sample> m_SampleModule = nullptr;

    void setNumberOfRoots();

    public:
        ViewmodelUI();
        unsigned int getNumberOfRoots();
};
#endif //ECG_ANALYZER_VIEWMODELUI_H
