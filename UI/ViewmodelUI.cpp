//
// Created by Marcel Pikuła on 18/11/2022.
//

#include "ViewmodelUI.h"
#include "../Modules/Sample/Sample.h"

ViewmodelUI::ViewmodelUI()
{
    m_SampleModule = std::make_shared<Sample>(1,4,1);
    setNumberOfRoots();
}

void ViewmodelUI::setNumberOfRoots()
{
    m_numberOfRoots = m_SampleModule->getNumberOfRoots();
}

unsigned int ViewmodelUI::getNumberOfRoots()
{
    return m_SampleModule->getNumberOfRoots();
}
