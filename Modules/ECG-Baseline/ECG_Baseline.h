// Created by Agnieszka Florkowska on 28 Nov 2022


#pragma once
#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>
#include <fstream>
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

// This module enables to remove the baseline wander from ECG signals.

class ECG_Baseline
{
	public:
		ECG_Baseline(std::shared_ptr<std::vector<float>> originalSignal);

		// In the case of Butterworth filtering's method, the user should enter the low pass frequency, high pass frequency, and number of coefficients.
		// Moreover, the sampling frequency should be set to loaded value from signal properties. 
		void SetSamplingFrequencyForButteworthFilter(unsigned int freq);
		void SetLowPassFrequencyForButteworthFilter(unsigned int freq);
		void SetHighPassFrequencyForButteworthFilter(unsigned int freq);
		void SetNumberOfCoefficientsForButteworthFilter(unsigned int number);

		// In the case of Moving Average filtering's method, the user should enter the window length.
		void SetWindowLengthForMovingAverageFilter(unsigned int length);

		// In the case of LMS filtering's method, the user should enter the convergance rate and the window length for Moving Average Filter.
		void SetConvergenceRateForLMSFilter(float rate);

		// Firstly, before calling the method, check if entered values are correct.
		// Then, based on user's choice, call the proper method, that returns the filtered signal. 

		// Input: 
		//		ButterworthFilterParameters 
		//			unsigned int m_fLowPass > 0 (prefered value =  15)
		//			unsigned int m_fHighPass > 0 (prefered value =  5)
		//			m_fLowPass > m_fHighPass
		//			unsigned int m_coefficientsNumber > 0 (prefered value =  40)
		// 
		// Output:
		//		std::vector<float> m_filteredSignalUsingButterworthFilter
		std::vector<float> GetFilteredSignalButterworthFilter();

		// Input: 
		//		MovingAverageFilterParameters 
		//			unsigned int m_windowLength > 0 (prefered value =  15)
		//
		// Output:
		//		std::vector<float> m_filteredSignalUsingMovingAverageFilter 
		std::vector<float> GetFilteredSignalMovingAverageFilter();

		// Input: 
		//		LMSFilterParameters 
		//			unsigned int m_convergenceRate > 0 (prefered value =  0.02)
		// 
		//		MovingAverageFilterParameters - needed for calculating the desired signal during LMS filtering
		//			unsigned int m_windowLength > 0 (prefered value =  15)
		//
		// Output:
		//		std::vector<float> m_filteredSignalUsingLMSFilter 
		std::vector<float> GetFilteredSignalLMSFilter();
		
		
	private:

		void MovingAverageFiltering();
		void LMSFiltering();
		void ButterworthFiltering();

		std::vector<float> Convolution1D(std::shared_ptr<std::vector<float>>& originalSignal, std::vector<float> mask, ConvolutionType parameter);
		float CalculateConvolutionResultForChosenElementOfSignal(std::shared_ptr<std::vector<float>> originalSignal, std::vector<float> mask, size_t currentIndex);
		
		MovingAverageFilterParameters m_MovingAverageFilterParameters;
		LMSFilterParameters m_LMSFilterParameters;
		ButterworthFilterParameters m_ButterworthFilterParameters;
		
		std::shared_ptr<std::vector<float>> m_originalSignal;

		std::vector<float> m_filteredSignalUsingMovingAverageFilter;
		std::vector<float> m_filteredSignalUsingLMSFilter;
		std::vector<float> m_filteredSignalUsingButterworthFilter;

};
