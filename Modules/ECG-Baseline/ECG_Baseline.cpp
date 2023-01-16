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
    m_ButterworthFilterParameters.CreateBWBandPassFilter();
    vector<float> filteredSignalLowPass = Convolution1D(m_originalSignal, m_ButterworthFilterParameters.GetLowPassFilter(), ConvolutionType::Same);
    std::shared_ptr<std::vector<float>> filteredSignalLowPassPointer;
    filteredSignalLowPassPointer = make_shared<std::vector<float>>(filteredSignalLowPass);
    m_filteredSignalUsingButterworthFilter = Convolution1D(filteredSignalLowPassPointer, m_ButterworthFilterParameters.GetHighPassFilter(), ConvolutionType::Same);
}

void ECG_Baseline::MovingAverageFiltering()
{
    
    m_filteredSignalUsingMovingAverageFilter.clear();
    m_MovingAverageFilterParameters.ResetConvolutionWindow();

    m_MovingAverageFilterParameters.CreateMAFConvolutionWindow();
    if (m_MovingAverageFilterParameters.GetConvolutionWindow().size() > 0)
    {
        m_filteredSignalUsingMovingAverageFilter = Convolution1D(m_originalSignal, m_MovingAverageFilterParameters.GetConvolutionWindow(), ConvolutionType::Same);
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
    return m_filteredSignalUsingMovingAverageFilter;
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
void ECG_Baseline::SetWindowLengthForMovingAverageFilter(unsigned int length)
{
    m_MovingAverageFilterParameters.SetWindowLenght(length);
}
void ECG_Baseline::SetNumberOfCoefficientsForButteworthFilter(unsigned int number)
{
    m_ButterworthFilterParameters.SetBWCoefficientsNumber(number);
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


MovingAverageFilterParameters::MovingAverageFilterParameters()
{
    m_windowLength = 1;
    m_convolutionWindow = {};
}
void MovingAverageFilterParameters::SetWindowLenght(unsigned int length)
{
    m_windowLength = length;
}
unsigned int MovingAverageFilterParameters::GetWindowLenght()
{
    return m_windowLength;
}
std::vector<float> MovingAverageFilterParameters::GetConvolutionWindow()
{
    return m_convolutionWindow;
}
void MovingAverageFilterParameters::CreateMAFConvolutionWindow()
{
    float maskValue = static_cast<float>(1.f / m_windowLength);
    m_convolutionWindow.insert(m_convolutionWindow.begin(), m_windowLength, maskValue);
}
void MovingAverageFilterParameters::ResetConvolutionWindow()
{
    m_convolutionWindow.clear();
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
    m_fLowPass = 0;
    m_fHighPass = 0;
    m_fSampling = 0;
    m_coefficientsNumber = 0;
    m_lowPassFilter = {};
    m_highPassFilter = {};
    m_hammingWindow = {};
}
void ButterworthFilterParameters::CreateBWBandPassFilter()
{
    // Vector of filter coefficients
    std::vector<float> vectorOfCoefficients;
    unsigned int numberOfCoefficients = 2 * m_coefficientsNumber + 1;
    vectorOfCoefficients.resize(numberOfCoefficients);
    std::iota(vectorOfCoefficients.begin(), vectorOfCoefficients.end(), - m_coefficientsNumber);
    
    // Frequency normalization
    unsigned int normfLowPass = m_fLowPass / (m_fSampling / 2);
    unsigned int normfHighPass = m_fHighPass / (m_fSampling / 2);

    // LowPass and HighPass Filter Coeff
    for (auto& coeff : vectorOfCoefficients)
    {
        if (coeff == 0)
        {
            m_lowPassFilter.push_back(2.0f * normfLowPass);
            m_highPassFilter.push_back(1.0f - 2.0f * normfHighPass);
        }
        else
        {
            m_lowPassFilter.push_back(static_cast<float>(sin((2.0f * M_PIl * normfLowPass * coeff) / (M_PIl * coeff))));
            m_highPassFilter.push_back(static_cast<float>(-sin((2.0f * M_PIl * normfHighPass * coeff) / (M_PIl * coeff))));
        }
    }

    // Create HammingWindow
    for (size_t i = 0; i < vectorOfCoefficients.size(); i++)
    {
        m_hammingWindow.push_back(static_cast<float>(0.54 - 0.46 * cos((2 * M_PIl * i) / (vectorOfCoefficients.size()))));
    }

    // Windowing 
    for (size_t i = 0; i < vectorOfCoefficients.size(); i++)
    {
        m_lowPassFilter.at(i) *= m_hammingWindow.at(i);
        m_highPassFilter.at(i) *= m_hammingWindow.at(i);
    }
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
unsigned int ButterworthFilterParameters::GetBWCoefficientsNumber()
{
    return m_coefficientsNumber;
}
void ButterworthFilterParameters::SetBWCoefficientsNumber(unsigned int number)
{
    m_coefficientsNumber = number;
}
void ButterworthFilterParameters::ResetFilterParams()
{
    m_lowPassFilter.clear();
    m_highPassFilter.clear();
    m_hammingWindow.clear();
}
std::vector<float> ButterworthFilterParameters::GetLowPassFilter()
{
    return m_lowPassFilter;
}
std::vector<float> ButterworthFilterParameters::GetHighPassFilter()
{
    return m_highPassFilter;
}