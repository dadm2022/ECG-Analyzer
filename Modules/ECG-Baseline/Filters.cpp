#include "pch.h"
#include "Filters.h"

using namespace std; 

Filters::Filters()
{
    m_MAFWindowLenght = 81;
    m_MAFConvolutionWindow = {};
    m_LMSWeights.assign(5, 0.0);
    m_LMSDesireSignalCoefficients = { 1, 0.5, 0.25,0.125, 0.0625 };
    m_LMSDesireSignal = {};
    m_currentFilteringType = FilteringType::MovingAverage;
}

void Filters::SetMAFWindowLenght(unsigned int size)
{
    m_MAFWindowLenght = size;
}
unsigned int Filters::GetMAFWindowLenght()
{
    return m_MAFWindowLenght;
}

std::vector<float> Filters::GetFilteredSignal()
{
    return m_filteredSignal;
}


void Filters::Filtering(FilteringType type, std::unique_ptr<std::vector<float>>& originalSignal)
{
    m_currentFilteringType = type;
    switch (m_currentFilteringType)
    {
    case FilteringType::MovingAverage:
        MovingAverageFiltering(originalSignal);
        break;
   
    case FilteringType::LMS:
        LMSFiltering(originalSignal);
        break;
    
    default:
        break;
    }
    

}

void Filters::CreateMAFConvolutionWindow()
{
    float maskValue = (float) 1. / m_MAFWindowLenght;
    for (size_t i=0; i < m_MAFWindowLenght; i++)
    {
        m_MAFConvolutionWindow.push_back(maskValue);
    }
}
float Filters::CalculateConvolutionResultForChosenElementOfSignal(std::unique_ptr<std::vector<float>>& originalSignal, std::vector<float> mask, size_t currentIndex)
{
    float sum = 0;
    size_t startingIndexForConv = (currentIndex < mask.size() - 1) ? 0 : currentIndex - (mask.size() - 1);
    size_t endingIndexForConv = (currentIndex < originalSignal->size() - 1) ? currentIndex : originalSignal->size() - 1;
    m_LMSCurrentlyCalculatedSubvector.clear();

    for (size_t j = startingIndexForConv; j <= endingIndexForConv; j++)
    {
        sum += (originalSignal->at(j) * mask.at(currentIndex - j));
        if (m_currentFilteringType == FilteringType::LMS)
        {
            m_LMSCurrentlyCalculatedSubvector.push_back(originalSignal->at(j));
        }
    }

    return sum;
}
std::vector<float> Filters::Convolution1D(std::unique_ptr<std::vector<float>>& originalSignal, std::vector<float> mask, ConvolutionType parameter)
{
    size_t filteredSignalAfterConvolutionLength = originalSignal->size() + mask.size() - 1;

   // m_currentFilteringType == FilteringType::MovingAverage ? originalSignal->size() + mask.size() - 1 : originalSignal->size();

    std::vector<float> convolutionResult = {};
    float sum = 0.0;
    for (size_t i = 0; i < filteredSignalAfterConvolutionLength; i++) 
    {
        sum = CalculateConvolutionResultForChosenElementOfSignal(originalSignal, mask, i);
        convolutionResult.push_back(sum);
    }

    if (parameter == ConvolutionType::Full)
    {
        return convolutionResult;
    }
    else if (parameter == ConvolutionType::Same)
    {
        int difference = convolutionResult.size() - originalSignal->size();
        size_t startingIndex = ceil(difference/2);
        size_t endingIndex = difference - startingIndex;
        return { convolutionResult.begin() + startingIndex, convolutionResult.end() - endingIndex };
    }

   
}

void Filters::MovingAverageFiltering(std::unique_ptr<std::vector<float>> & originalSignal)
{
    CreateMAFConvolutionWindow();
    if (m_MAFConvolutionWindow.size() > 0)
    {
        m_filteredSignal = Convolution1D(originalSignal, m_MAFConvolutionWindow, ConvolutionType::Same);
    }
}

void Filters::LMSFiltering(std::unique_ptr<std::vector<float>>& originalSignal)
{
    float sum = 0.0;
    float error = 0.0;
    m_LMSDesireSignal = Convolution1D(originalSignal, m_LMSDesireSignalCoefficients, ConvolutionType::Same); // what is the desire signal for ECG?

    for (size_t currentIndex = 0; currentIndex < originalSignal->size(); currentIndex++)
    {
        sum = CalculateConvolutionResultForChosenElementOfSignal(originalSignal,  m_LMSWeights, currentIndex);
        error = m_LMSDesireSignal.at(currentIndex) - sum;
        m_LMSErrors.push_back(error);
        UpdateLMSWeights(error);
        m_filteredSignal.push_back(sum);
    } 
}

void Filters::UpdateLMSWeights(float error)
{
    for (size_t i = 0; i < m_LMSCurrentlyCalculatedSubvector.size(); i++)
    {
        m_LMSWeights.at(m_LMSWeights.size() - 1 - i) = m_LMSWeights.at(m_LMSWeights.size() - 1 - i) + (error * m_LMSCurrentlyCalculatedSubvector.at(i) * m_convergenceRate);
    }
}
