#include "pch.h"
#include "ECG_Baseline.h"

using namespace std; 

ECG_Baseline::ECG_Baseline()
{
} 

void ECG_Baseline::ButterworthFiltering(std::unique_ptr<std::vector<float>>& originalSignal)
{
    CreateBWBandPassFilter();
    vector<float> filteredSignalLowPass = Convolution1D(originalSignal, m_ButterworthFilterParameters.lowPassFilter, ConvolutionType::Same);
    std::unique_ptr<std::vector<float>> filteredSignalLowPassPointer;
    filteredSignalLowPassPointer = make_unique<std::vector<float>>(filteredSignalLowPass);
    m_ButterworthFilterParameters.filteredSignal = Convolution1D(filteredSignalLowPassPointer, m_ButterworthFilterParameters.highPassFilter, ConvolutionType::Same);
}

void ECG_Baseline::MovingAverageFiltering(std::unique_ptr<std::vector<float>>& originalSignal)
{
    CreateMAFConvolutionWindow();
    if (m_MovingAverageFilterParameters.convolutionWindow.size() > 0)
    {
        m_MovingAverageFilterParameters.filteredSignal = Convolution1D(originalSignal, m_MovingAverageFilterParameters.convolutionWindow, ConvolutionType::Same);
    }
}

void ECG_Baseline::LMSFiltering(std::unique_ptr<std::vector<float>>& originalSignal)
{
    float sum = 0.0;
    float error = 0.0;
    m_LMSFilterParameters.desireSignal = Convolution1D(originalSignal, m_LMSFilterParameters.desireSignalCoefficients, ConvolutionType::Same); // what is the desire signal for ECG?

    for (size_t currentIndex = 0; currentIndex < originalSignal->size(); currentIndex++)
    {
        sum = CalculateConvolutionResultForChosenElementOfSignal(originalSignal, m_LMSFilterParameters.weights, currentIndex);
        error = m_LMSFilterParameters.desireSignal.at(currentIndex) - sum;
        m_LMSFilterParameters.errors.push_back(error);
        UpdateLMSWeights(error);
        m_LMSFilterParameters.filteredSignal.push_back(sum);
    }
}

void ECG_Baseline::CreateBWBandPassFilter()
{
    // Vector of filter coefficients
    std::vector<float> vectorOfCoefficients;
    int firstValue = -m_ButterworthFilterParameters.coefficientsNumber;
    for (int i = 0; i < 2 * m_ButterworthFilterParameters.coefficientsNumber + 1; i++)
    {
        vectorOfCoefficients.push_back(firstValue);
        firstValue++;
    }

    // Frequency normalization
    unsigned int normfLowPass = m_ButterworthFilterParameters.fLowPass / (m_ButterworthFilterParameters.fSampling / 2);
    unsigned int normfHighPass = m_ButterworthFilterParameters.fHighPass / (m_ButterworthFilterParameters.fSampling / 2);

    // LowPass and HighPass Filter Coeff
    for (auto& coeff : vectorOfCoefficients)
    {
        if (coeff == 0)
        {
            m_ButterworthFilterParameters.lowPassFilter.push_back(2 * normfLowPass);
            m_ButterworthFilterParameters.highPassFilter.push_back(1 - 2 * normfHighPass);
        }
        else
        {
            m_ButterworthFilterParameters.lowPassFilter.push_back(sin((2 * M_PIl * normfLowPass * coeff) / (M_PIl * coeff)));
            m_ButterworthFilterParameters.highPassFilter.push_back(-sin((2 * M_PIl * normfHighPass * coeff) / (M_PIl * coeff)));
        }
    }

    // Create HammingWindow
    for (size_t i = 0; i < vectorOfCoefficients.size(); i++)
    {
        m_ButterworthFilterParameters.hammingWindow.push_back(0.54 - 0.46 * cos((2 * M_PIl * i) / (vectorOfCoefficients.size())));
    }

    // Windowing 
    for (size_t i = 0; i < vectorOfCoefficients.size(); i++)
    {
        m_ButterworthFilterParameters.lowPassFilter.at(i) = m_ButterworthFilterParameters.lowPassFilter.at(i) * m_ButterworthFilterParameters.hammingWindow.at(i);
        m_ButterworthFilterParameters.highPassFilter.at(i) = m_ButterworthFilterParameters.highPassFilter.at(i) * m_ButterworthFilterParameters.hammingWindow.at(i);
    }

}
void ECG_Baseline::UpdateLMSWeights(float error)
{
    for (size_t i = 0; i < m_LMSFilterParameters.currentlyCalculatedSubvector.size(); i++)
    {
        m_LMSFilterParameters.weights.at(m_LMSFilterParameters.weights.size() - 1 - i) = m_LMSFilterParameters.weights.at(m_LMSFilterParameters.weights.size() - 1 - i) + (error * m_LMSFilterParameters.currentlyCalculatedSubvector.at(i) * m_LMSFilterParameters.convergenceRate);
    }
}

void ECG_Baseline::CreateMAFConvolutionWindow()
{
    float maskValue = (float)1. / m_MovingAverageFilterParameters.windowLenght;
    for (size_t i = 0; i < m_MovingAverageFilterParameters.windowLenght; i++)
    {
        m_MovingAverageFilterParameters.convolutionWindow.push_back(maskValue);
    }
}

float ECG_Baseline::CalculateConvolutionResultForChosenElementOfSignal(std::unique_ptr<std::vector<float>>& originalSignal, std::vector<float> mask, size_t currentIndex)
{
    float sum = 0;
    size_t startingIndexForConv = (currentIndex < mask.size() - 1) ? 0 : currentIndex - (mask.size() - 1);
    size_t endingIndexForConv = (currentIndex < originalSignal->size() - 1) ? currentIndex : originalSignal->size() - 1;
    m_LMSFilterParameters.currentlyCalculatedSubvector.clear();

    for (size_t j = startingIndexForConv; j <= endingIndexForConv; j++)
    {
        sum += (originalSignal->at(j) * mask.at(currentIndex - j));
        m_LMSFilterParameters.currentlyCalculatedSubvector.push_back(originalSignal->at(j));
    }

    return sum;
}

std::vector<float> ECG_Baseline::Convolution1D(std::unique_ptr<std::vector<float>>& originalSignal, std::vector<float> mask, ConvolutionType parameter)
{
    size_t filteredSignalAfterConvolutionLength = originalSignal->size() + mask.size() - 1;

    std::vector<float> convolutionResult = {};
    float sum = 0.0;
    for (size_t i = 0; i < filteredSignalAfterConvolutionLength; i++)
    {
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

void ECG_Baseline::SetMAFWindowLenght(unsigned int size)
{
    m_MovingAverageFilterParameters.windowLenght = size;
}
unsigned int ECG_Baseline::GetMAFWindowLenght()
{
    return m_MovingAverageFilterParameters.windowLenght;
}

std::vector<float> ECG_Baseline::GetFilteredSignalMovingAverageFilter()
{
    return m_MovingAverageFilterParameters.filteredSignal;
}
std::vector<float> ECG_Baseline::GetFilteredSignalLMSFilter()
{
    return m_LMSFilterParameters.filteredSignal;
}
std::vector<float> ECG_Baseline::GetFilteredSignalButterworthFilter()
{
    return m_ButterworthFilterParameters.filteredSignal;
}

unsigned int ECG_Baseline::GetBWFrequencyLowPassFilter()
{
    return m_ButterworthFilterParameters.fLowPass;
}
void ECG_Baseline::SetBWFrequencyLowPassFilter(unsigned int freq)
{
    m_ButterworthFilterParameters.fLowPass = freq;
}
unsigned int ECG_Baseline::GetBWFrequencyHighPassFilter()
{
    return m_ButterworthFilterParameters.fHighPass;
}
void ECG_Baseline::SetBWFrequencyHighPassFilter(unsigned int freq)
{
    m_ButterworthFilterParameters.fHighPass = freq;
}
unsigned int ECG_Baseline::GetBWFrequencySampling()
{
    return m_ButterworthFilterParameters.fSampling;
}
void ECG_Baseline::SetBWFrequencySampling(unsigned int freq)
{
    m_ButterworthFilterParameters.fSampling = freq;
}
unsigned int ECG_Baseline::GetBWCoefficientsNumber()
{
    return m_ButterworthFilterParameters.coefficientsNumber;
}
void ECG_Baseline::SetBWCoefficientsNumber(unsigned int number)
{
    m_ButterworthFilterParameters.coefficientsNumber = number;
}
