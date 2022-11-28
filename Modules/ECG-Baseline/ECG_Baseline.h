#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>
#include <fstream>
#include <utility>

# define M_PIl          3.141592653589793238462643383279502884L

enum ConvolutionType
{
	Same = 0,
	Full = 1,
}; 


struct MovingAverageFilterParameters
{
	unsigned int windowLenght = 10;
	std::vector<float> convolutionWindow = {};
	std::vector<float> filteredSignal;
};

struct LMSFilterParameters
{
	float convergenceRate = 0.2f;
	std::vector<float> weights = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	std::vector<float> errors = {};
	std::vector<float> desireSignalCoefficients = { 1, 0.5, 0.25,0.125, 0.0625 };
	std::vector<float> desireSignal = {};
	std::vector<float> currentlyCalculatedSubvector = {};
	std::vector<float> filteredSignal;
};

struct ButterworthFilterParameters
{
	unsigned int fLowPass = 15;
	unsigned int fHighPass = 5;
	unsigned int fSampling = 360;
	unsigned int coefficientsNumber = 40;
	std::vector<float> lowPassFilter = {};
	std::vector<float> highPassFilter = {};
	std::vector<float> hammingWindow = {};
	std::vector<float> filteredSignal;
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

		void SetMAFWindowLenght(unsigned int size);
		unsigned int GetMAFWindowLenght();
		unsigned int GetBWFrequencyLowPassFilter();
		void SetBWFrequencyLowPassFilter(unsigned int freq);
		unsigned int GetBWFrequencyHighPassFilter();
		void SetBWFrequencyHighPassFilter(unsigned int freq);
		unsigned int GetBWFrequencySampling();
		void SetBWFrequencySampling(unsigned int freq);
		unsigned int GetBWCoefficientsNumber();
		void SetBWCoefficientsNumber(unsigned int number);
		
	private:

		std::vector<float> Convolution1D(std::unique_ptr<std::vector<float>>& originalSignal, std::vector<float> mask, ConvolutionType parameter);
		float CalculateConvolutionResultForChosenElementOfSignal(std::unique_ptr<std::vector<float>>& originalSignal, std::vector<float> mask, size_t currentIndex);
		
		void CreateMAFConvolutionWindow();
		void CreateBWBandPassFilter();
		void UpdateLMSWeights(float error);

		MovingAverageFilterParameters m_MovingAverageFilterParameters;
		LMSFilterParameters m_LMSFilterParameters;
		ButterworthFilterParameters m_ButterworthFilterParameters;

};

