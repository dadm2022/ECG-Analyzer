#include"pch.h"
#include "ECG_Baseline.h"

using namespace std; 

ECG_Baseline::ECG_Baseline(std::shared_ptr<std::vector<float>> originalSignal) : m_originalSignal(originalSignal)
{
    m_filteredSignalUsingMovingAverageFilter = {};
    m_filteredSignalUsingLMSFilter = {};
    m_filteredSignalUsingButterworthFilter = {};
} 

void ECG_Baseline::ButterworthFiltering()
{
    m_filteredSignalUsingButterworthFilter.clear();
    m_ButterworthFilterParameters.ResetFilterParams();
    m_ButterworthFilterParameters.CalculateFilterCoeff();
    

    vector<float> filteredSignalLowPass = m_ButterworthFilterParameters.CalculateDifferenceEquation(m_originalSignal, m_ButterworthFilterParameters.GetLowPassFilterCoeff());
    std::shared_ptr<std::vector<float>> filteredSignalLowPassPointer;
    filteredSignalLowPassPointer = make_shared<std::vector<float>>(filteredSignalLowPass);
    m_filteredSignalUsingButterworthFilter = m_ButterworthFilterParameters.CalculateDifferenceEquation(filteredSignalLowPassPointer, m_ButterworthFilterParameters.GetHighPassFilterCoeff());
}

void ECG_Baseline::MovingAverageFiltering()
{
    
    m_filteredSignalUsingMovingAverageFilter.clear();
  
    std::vector<float> lowFiltered = {};
    std::vector<float> highFiltered = {};
    m_MovingAverageFilterParameters.ResetConvolutionWindow();

    m_MovingAverageFilterParameters.CreateMAFConvolutionWindow();

    
    if (m_MovingAverageFilterParameters.GetWindowLow().size() > 0)
    {
        lowFiltered = Convolution1D(m_originalSignal, m_MovingAverageFilterParameters.GetWindowLow(), ConvolutionType::Same);
        highFiltered = Convolution1D(m_originalSignal, m_MovingAverageFilterParameters.GetWindowHigh(), ConvolutionType::Same);
    }
    
 
    for (int i = 0; i < m_originalSignal->size(); i++)
    {
        m_filteredSignalUsingMovingAverageFilter.push_back(highFiltered.at(i) - lowFiltered.at(i));
    }
       

}

void ECG_Baseline::LMSFiltering()
{
    float sum = 0.0f;
    float error = 0.0f;
    m_filteredSignalUsingLMSFilter.clear();
    m_LMSFilterParameters.ResetWeights();
    m_filteredSignalUsingMovingAverageFilter.clear();

    MovingAverageFiltering(); //desired signal = filtered signal using another method 
    for (size_t currentIndex = 0; currentIndex < m_originalSignal->size(); currentIndex++)
    {
        sum = CalculateConvolutionResultForChosenElementOfSignal(m_originalSignal, m_LMSFilterParameters.GetWeights(), currentIndex);
        error = m_filteredSignalUsingMovingAverageFilter.at(currentIndex) - sum;
        m_LMSFilterParameters.UpdateLMSWeights(error);
        m_filteredSignalUsingLMSFilter.push_back(sum);
    }
}


float ECG_Baseline::CalculateConvolutionResultForChosenElementOfSignal(std::shared_ptr<std::vector<float>> originalSignal, std::vector<float> mask, size_t currentIndex)
{
    float sum = 0;
    size_t startingIndexForConv = (currentIndex < mask.size() - 1) ? 0 : currentIndex - (mask.size() - 1);
    size_t endingIndexForConv = (currentIndex < originalSignal->size() - 1) ? currentIndex : originalSignal->size() - 1;
    m_LMSFilterParameters.ResetCurrentlyCalculatedSubsignal();

    for (size_t j = startingIndexForConv; j <= endingIndexForConv; j++)
    {
        sum += (originalSignal->at(j) * mask.at(currentIndex - j));
        m_LMSFilterParameters.AddValueToCurrentlyCalculatedSubsignal(originalSignal->at(j));
    }

    return sum;
}

std::vector<float> ECG_Baseline::Convolution1D(std::shared_ptr<std::vector<float>> originalSignal, std::vector<float> mask, ConvolutionType parameter)
{
    size_t filteredSignalAfterConvolutionLength = originalSignal->size() + mask.size() - 1;

    std::vector<float> convolutionResult = {};
   
    for (size_t i = 0; i < filteredSignalAfterConvolutionLength; i++)
    {
        float sum = 0.0f;
        sum = CalculateConvolutionResultForChosenElementOfSignal(originalSignal, mask, i);
        convolutionResult.push_back(sum);
    }

    vector<float> results;
    if (parameter == ConvolutionType::Full)
    {
        results = convolutionResult;
    }
    else if (parameter == ConvolutionType::Same)
    {
        size_t difference = convolutionResult.size() - originalSignal->size();
        size_t startingIndex = (size_t)ceil(difference / 2);
        size_t endingIndex = difference - startingIndex;
        results = { convolutionResult.begin() + startingIndex, convolutionResult.end() - endingIndex };
    }

    return results;

}

std::vector<float> ECG_Baseline::GetFilteredSignalMovingAverageFilter()
{
    MovingAverageFiltering();
    return m_filteredSignalUsingMovingAverageFilter ;
}
std::vector<float> ECG_Baseline::GetFilteredSignalLMSFilter()
{
    LMSFiltering();
    return m_filteredSignalUsingLMSFilter;
}
std::vector<float> ECG_Baseline::GetFilteredSignalButterworthFilter()
{
    ButterworthFiltering();
    return m_filteredSignalUsingButterworthFilter;
}
void ECG_Baseline::SetWindowLengthForMovingAverageFilter(unsigned int lengthLow, unsigned int lengthHigh )
{
    m_MovingAverageFilterParameters.SetWindowLenghtLow(lengthLow);
    m_MovingAverageFilterParameters.SetWindowLenghtHigh(lengthHigh);
}

void ECG_Baseline::SetConvergenceRateForLMSFilter(float rate)
{
    m_LMSFilterParameters.SetConvergenceRate(rate);
}
void ECG_Baseline::SetSamplingFrequencyForButteworthFilter(unsigned int freq)
{
    m_ButterworthFilterParameters.SetBWFrequencySampling(freq);
}
void ECG_Baseline::SetLowPassFrequencyForButteworthFilter(unsigned int freq)
{
    m_ButterworthFilterParameters.SetBWFrequencyLowPassFilter(freq);
}
void ECG_Baseline::SetHighPassFrequencyForButteworthFilter(unsigned int freq)
{
    m_ButterworthFilterParameters.SetBWFrequencyHighPassFilter(freq);
}

std::vector<float> MovingAverageFilterParameters::GetWindowLow()
{
    return m_convolutionWindowLow;
}
std::vector<float> MovingAverageFilterParameters::GetWindowHigh()
{
    return m_convolutionWindowHigh;
}


MovingAverageFilterParameters::MovingAverageFilterParameters()
{
    m_windowLengthLow = 15;
    m_windowLengthHigh = 5;
    m_convolutionWindowLow = {};
}
void MovingAverageFilterParameters::SetWindowLenghtLow(unsigned int length)
{
    m_windowLengthLow = length;
}
void MovingAverageFilterParameters::SetWindowLenghtHigh(unsigned int length)
{
    m_windowLengthHigh = length;
}

void MovingAverageFilterParameters::CreateMAFConvolutionWindow()
{
    float maskValueLow = static_cast<float>(1.f / m_windowLengthLow);
    m_convolutionWindowLow.insert(m_convolutionWindowLow.begin(), m_windowLengthLow, maskValueLow);

    float maskValueHigh = static_cast<float>(1.f / m_windowLengthHigh);
    m_convolutionWindowHigh.insert(m_convolutionWindowHigh.begin(), m_windowLengthHigh, maskValueHigh);
}
void MovingAverageFilterParameters::ResetConvolutionWindow()
{
    m_convolutionWindowLow.clear();
    m_convolutionWindowHigh.clear();
}

LMSFilterParameters::LMSFilterParameters()
{
    m_convergenceRate = 0.2f;
    m_weights = { 0.0, 0.0, 0.0, 0.0, 0.0 };
    m_currentlyCalculatedSubsignal = {};
}
void LMSFilterParameters::SetConvergenceRate(float rate)
{
    m_convergenceRate = rate;
}
float LMSFilterParameters::GetConvergenceRate()
{
    return m_convergenceRate;
}
std::vector<float> LMSFilterParameters::GetWeights()
{
    return m_weights;
}
void LMSFilterParameters::UpdateLMSWeights(float error)
{
    for (size_t i = 0; i < m_currentlyCalculatedSubsignal.size(); i++)
    {
        m_weights.at(m_weights.size() - 1 - i) += (error * m_currentlyCalculatedSubsignal.at(i) * m_convergenceRate);
    }
}
void LMSFilterParameters::ResetWeights()
{
    m_weights = { 0.0, 0.0, 0.0, 0.0, 0.0 };
}
std::vector<float> LMSFilterParameters::GetCurrentlyCalculatedSubsignal()
{
    return m_currentlyCalculatedSubsignal;
}
void LMSFilterParameters::AddValueToCurrentlyCalculatedSubsignal(float value)
{
    m_currentlyCalculatedSubsignal.push_back(value);
}
void LMSFilterParameters::ResetCurrentlyCalculatedSubsignal()
{
    m_currentlyCalculatedSubsignal.clear();
}

ButterworthFilterParameters::ButterworthFilterParameters()
{
    m_fLowPass = 15;
    m_fHighPass = 1;
    m_fSampling = 360;
    m_lowPassFilterCoeff = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    m_lowPassFilterCoeff = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
 
}


unsigned int ButterworthFilterParameters::GetBWFrequencyLowPassFilter()
{
    return m_fLowPass;
}
void ButterworthFilterParameters::SetBWFrequencyLowPassFilter(unsigned int freq)
{
    m_fLowPass = freq;
}
unsigned int ButterworthFilterParameters::GetBWFrequencyHighPassFilter()
{
    return m_fHighPass;
}
void ButterworthFilterParameters::SetBWFrequencyHighPassFilter(unsigned int freq)
{
    m_fHighPass = freq;
}
unsigned int ButterworthFilterParameters::GetBWFrequencySampling()
{
    return m_fSampling;
}
void ButterworthFilterParameters::SetBWFrequencySampling(unsigned int freq)
{
    m_fSampling = freq;
}

void ButterworthFilterParameters::ResetFilterParams()
{
    m_lowPassFilterCoeff = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    m_lowPassFilterCoeff = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
  
}
std::vector<float> ButterworthFilterParameters::GetLowPassFilterCoeff()
{
    return m_lowPassFilterCoeff;
}
std::vector<float> ButterworthFilterParameters::GetHighPassFilterCoeff()
{
    return m_highPassFilterCoeff;
}

void ButterworthFilterParameters::CalculateFilterCoeff()
{
    CalculateLowPassFilterCoeff();
    CalculateHighPassFilterCoeff();
}

void ButterworthFilterParameters::CalculateLowPassFilterCoeff()
{
    /*float ff_lowPass = m_fLowPass / m_fSampling;
    const float ita = 1.0 / tan(M_PI * ff_lowPass);
    const float q = sqrt(2.0);
    float b0 = 1.0 / (1.0 + q * ita + ita * ita);
    float b1 = 2 * b0;
    float b2 = b0;
    float a1 = 2.0 * (ita * ita - 1.0) * b0;
    float a2 = -(1.0 - q * ita + ita * ita) * b0;*/
    float b0 = static_cast < float>(0.0144);
    float b1 = static_cast < float>(0.0288);
    float b2 = static_cast < float>(0.0144);
    float a1 = static_cast<float>(- 1.6330);
    float a2 = static_cast < float>(0.6906);


    m_lowPassFilterCoeff = { b0, b1, b2, a1, a2 };
}

void ButterworthFilterParameters::CalculateHighPassFilterCoeff()
{
   /* float ff_lowPass = m_fHighPass / m_fSampling;
    const float ita = 1.0 / tan(M_PI * ff_lowPass);
    const float q = sqrt(2.0);
    float b0 = (1.0 / (1.0 + q * ita + ita * ita)) * ita * ita;
    float b1 = - 2 * b0;
    float b2 = b0;
    float a1 = 2.0 * (ita * ita - 1.0) * b0;
    float a2 = -(1.0 - q * ita + ita * ita) * b0;*/

    float b0 = static_cast <float>(0.9877);
    float b1 = static_cast <float>(- 1.9755);
    float b2 = static_cast <float>(0.9877);
    float a1 = static_cast <float>(-1.9753);
    float a2 = static_cast <float>(0.9756);


    m_highPassFilterCoeff = { b0, b1, b2, a1, a2 };
}

std::vector<float> ButterworthFilterParameters::CalculateDifferenceEquation(std::shared_ptr<std::vector<float>> signal, std::vector<float> coeff)
{
    std::vector<float> filteredSignal = {};
    float currentValue = 0.0f;
    for (int i = 0; i < signal->size(); i++)
    {
        currentValue = 0.0f;
        if (i == 0) 
        {
            currentValue = m_lowPassFilterCoeff.at(0) * signal->at(i);
        }
        else if (i == 1)
        {
            currentValue = m_lowPassFilterCoeff.at(0) * signal->at(i) + m_lowPassFilterCoeff.at(1) * signal->at(i - 1) + m_lowPassFilterCoeff.at(3) * filteredSignal.at(i - 1);
        }
        else
        {
            currentValue = m_lowPassFilterCoeff.at(0) * signal->at(i) + m_lowPassFilterCoeff.at(1) * signal->at(i-1) + m_lowPassFilterCoeff.at(2) * signal->at(i - 2) - (m_lowPassFilterCoeff.at(3) * filteredSignal.at(i - 1)) - (m_lowPassFilterCoeff.at(4) * filteredSignal.at(i - 2));
        }

        filteredSignal.push_back(currentValue);
    }

    return filteredSignal;
}