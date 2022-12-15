#pragma once

#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>
#include <utility>
#include <numeric>
#define M_PIl 3.141592653589793238462643383279502884L

enum ConvolutionType
{
	Same = 0,
	Full = 1,
}; 


class MovingAverageFilterParameters
{
public:
	MovingAverageFilterParameters();

	void SetWindowLenght(unsigned int length);
	unsigned int GetWindowLenght();
	std::vector<float> GetConvolutionWindow();
	void ResetConvolutionWindow();
	void CreateMAFConvolutionWindow();

private:
	unsigned int m_windowLength;
	std::vector<float> m_convolutionWindow;
};

class LMSFilterParameters
{
public:
	LMSFilterParameters();
	void SetConvergenceRate(float rate);
	float GetConvergenceRate();
	std::vector<float> GetWeights();
	void ResetWeights();
	void UpdateLMSWeights(float error);
	std::vector<float> GetCurrentlyCalculatedSubsignal();
	void AddValueToCurrentlyCalculatedSubsignal(float value);
	void ResetCurrentlyCalculatedSubsignal();

private:
	float m_convergenceRate;
	std::vector<float> m_weights;
	std::vector<float> m_currentlyCalculatedSubsignal;

};

class ButterworthFilterParameters
{
public:
	ButterworthFilterParameters();

	void CreateBWBandPassFilter();

	unsigned int GetBWFrequencyLowPassFilter();
	void SetBWFrequencyLowPassFilter(unsigned int freq);
	unsigned int GetBWFrequencyHighPassFilter();
	void SetBWFrequencyHighPassFilter(unsigned int freq);
	unsigned int GetBWFrequencySampling();
	void SetBWFrequencySampling(unsigned int freq);
	unsigned int GetBWCoefficientsNumber();
	void SetBWCoefficientsNumber(unsigned int number);

	std::vector<float> GetLowPassFilter();
	std::vector<float> GetHighPassFilter();

	void ResetFilterParams();

private:
	unsigned int m_fLowPass;
	unsigned int m_fHighPass;
	unsigned int m_fSampling;
	unsigned int m_coefficientsNumber;
	std::vector<float> m_lowPassFilter;
	std::vector<float> m_highPassFilter;
	std::vector<float> m_hammingWindow;
};


class ECG_Baseline
{
	public:
		ECG_Baseline();

		void MovingAverageFiltering(std::unique_ptr<std::vector<float>>& originalSignal);
		void LMSFiltering(std::unique_ptr<std::vector<float>>& originalSignal);
		void ButterworthFiltering(std::unique_ptr<std::vector<float>>& originalSignal);

		std::vector<float> GetFilteredSignalMovingAverageFilter();
		std::vector<float> GetFilteredSignalLMSFilter();
		std::vector<float> GetFilteredSignalButterworthFilter();
		
		void SetSamplingFrequencyForButteworthFilter(unsigned int freq);
		void SetLowPassFrequencyForButteworthFilter(unsigned int freq);
		void SetHighPassFrequencyForButteworthFilter(unsigned int freq);
		void SetWindowLengthForMovingAverageFilter(unsigned int length);
		void SetNumberOfCoefficientsForButteworthFilter(unsigned int number);
		void SetConvergenceRateForLMSFilter(float rate);

	private:

		std::vector<float> Convolution1D(std::unique_ptr<std::vector<float>>& originalSignal, std::vector<float> mask, ConvolutionType parameter);
		float CalculateConvolutionResultForChosenElementOfSignal(std::unique_ptr<std::vector<float>>& originalSignal, std::vector<float> mask, size_t currentIndex);
		
		MovingAverageFilterParameters m_MovingAverageFilterParameters;
		LMSFilterParameters m_LMSFilterParameters;
		ButterworthFilterParameters m_ButterworthFilterParameters;
		
		std::vector<float> m_filteredSignalUsingMovingAverageFilter;
		std::vector<float> m_filteredSignalUsingLMSFilter;
		std::vector<float> m_filteredSignalUsingButterworthFilter;

};
