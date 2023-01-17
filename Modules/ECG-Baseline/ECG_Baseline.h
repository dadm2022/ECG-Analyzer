// Created by Agnieszka Florkowska on 28 Nov 2022


#pragma once
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>
#include <utility>
#include <numeric>
#define M_PIl 3.141592653589793238462643383279502884L
#define M_PI 3.14159265358979323846

enum ConvolutionType
{
	Same = 0,
	Full = 1,
}; 


class MovingAverageFilterParameters
{
public:
	MovingAverageFilterParameters();

	void SetWindowLenghtLow(unsigned int length);
	void SetWindowLenghtHigh(unsigned int length);
	std::vector<float> GetWindowLow();
	std::vector<float> GetWindowHigh();
	void ResetConvolutionWindow();
	void CreateMAFConvolutionWindow();

private:
	unsigned int m_windowLengthLow;
	unsigned int m_windowLengthHigh;
	std::vector<float> m_convolutionWindowLow;
	std::vector<float> m_convolutionWindowHigh;
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

	std::vector<float> CalculateDifferenceEquation(std::shared_ptr<std::vector<float>> signal, std::vector<float> coeff);

	unsigned int GetBWFrequencyLowPassFilter();
	void SetBWFrequencyLowPassFilter(unsigned int freq);
	unsigned int GetBWFrequencyHighPassFilter();
	void SetBWFrequencyHighPassFilter(unsigned int freq);
	unsigned int GetBWFrequencySampling();
	void SetBWFrequencySampling(unsigned int freq);

	void CalculateFilterCoeff();
	std::vector<float> GetLowPassFilterCoeff();
	std::vector<float> GetHighPassFilterCoeff();

	void ResetFilterParams();

private:

	void CalculateLowPassFilterCoeff();
	void CalculateHighPassFilterCoeff();

	unsigned int m_fLowPass;
	unsigned int m_fHighPass;
	unsigned int m_fSampling;
	std::vector<float> m_lowPassFilterCoeff;
	std::vector<float> m_highPassFilterCoeff;
	
};

// This module enables to remove the baseline wander from ECG signals.

class ECG_Baseline
{
	public:
		ECG_Baseline(std::shared_ptr<std::vector<float>> originalSignal);

		// In the case of Butterworth filtering's method, the user should enter the low pass frequency, high pass frequency.
		// Moreover, the sampling frequency should be set to loaded value from signal properties. 
		void SetSamplingFrequencyForButteworthFilter(unsigned int freq);
		void SetLowPassFrequencyForButteworthFilter(unsigned int freq);
		void SetHighPassFrequencyForButteworthFilter(unsigned int freq);

		// In the case of Moving Average filtering's method, the user should enter the window length.
		void SetWindowLengthForMovingAverageFilter(unsigned int lengthLow, unsigned int lengthHigh);
		// Input:
		//			lengthLow > lengthHigh

		// In the case of LMS filtering's method, the user should enter the convergance rate and the window length for Moving Average Filter.
		void SetConvergenceRateForLMSFilter(float rate);

		// Firstly, before calling the method, check if entered values are correct and
		// if pointer to raw signal is not null.
		// Then, based on user's choice, call the proper method, that returns the filtered signal. 

		// Input: 
		//		std::shared_ptr<std::vector<float>> m_originalSignal != nullptr
		//		ButterworthFilterParameters 
		//			unsigned int m_fLowPass > 0 (default value =  15)
		//			unsigned int m_fHighPass > 0 (default value =  1)
		//			m_fLowPass > m_fHighPass
		//			unsigned int m_fSampling > 0

		// 
		// Output:
		//		std::vector<float> m_filteredSignalUsingButterworthFilter
		std::vector<float> GetFilteredSignalButterworthFilter();

		// Input: 
		//		std::shared_ptr<std::vector<float>> m_originalSignal != nullptr
		//		MovingAverageFilterParameters 
		//			unsigned int m_windowLengthLow > 0 (default value =  15)
		//			unsigned int m_windowLengthHigh > 0 (default value =  5)
		//
		// Output:
		//		std::vector<float> m_filteredSignalUsingMovingAverageFilter 
		std::vector<float> GetFilteredSignalMovingAverageFilter();

		// Input: 
		//		std::shared_ptr<std::vector<float>> m_originalSignal != nullptr
		//		LMSFilterParameters 
		//			float m_convergenceRate > 0 (default value =  0.02)
		// 
		//		MovingAverageFilterParameters - needed for calculating the desired signal during LMS filtering
		//			unsigned int m_windowLengthLow > 0 (default value =  15)
		//			unsigned int m_windowLengthHigh > 0 (default value =  5)
		//
		// Output:
		//		std::vector<float> m_filteredSignalUsingLMSFilter 
		std::vector<float> GetFilteredSignalLMSFilter();
		
		
	private:

		void MovingAverageFiltering();
		void LMSFiltering();
		void ButterworthFiltering();

		std::vector<float> Convolution1D(std::shared_ptr<std::vector<float>> originalSignal, std::vector<float> mask, ConvolutionType parameter);
		float CalculateConvolutionResultForChosenElementOfSignal(std::shared_ptr<std::vector<float>> originalSignal, std::vector<float> mask, size_t currentIndex);
		
		MovingAverageFilterParameters m_MovingAverageFilterParameters;
		LMSFilterParameters m_LMSFilterParameters;
		ButterworthFilterParameters m_ButterworthFilterParameters;
		
		std::shared_ptr<std::vector<float>> m_originalSignal;

		std::vector<float> m_filteredSignalUsingMovingAverageFilter;
		std::vector<float> m_filteredSignalUsingLMSFilter;
		std::vector<float> m_filteredSignalUsingButterworthFilter;

};
