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

enum FilteringType
{
	MovingAverage = 0,
	LMS = 1,
	Butterworth = 2
};

enum ConvolutionType
{
	Same = 0,
	Full = 1,
}; 

class Filters
{
	public:
		Filters();
		void Filtering(FilteringType type, std::unique_ptr<std::vector<float>>& originalSignal);
		void SetMAFWindowLenght(unsigned int size);
		unsigned int GetMAFWindowLenght();
		std::vector<float> GetFilteredSignal();

	private:

		void MovingAverageFiltering(std::unique_ptr<std::vector<float>>& originalSignal);
		void LMSFiltering( std::unique_ptr<std::vector<float>>& originalSignal);

		void CreateMAFConvolutionWindow();

		std::vector<float> Convolution1D(std::unique_ptr<std::vector<float>>& originalSignal, std::vector<float> mask, ConvolutionType parameter);
		float CalculateConvolutionResultForChosenElementOfSignal(std::unique_ptr<std::vector<float>>& originalSignal, std::vector<float> mask, size_t currentIndex);
		void UpdateLMSWeights(float error);

		std::vector<float> m_filteredSignal;

		std::vector<float> m_LMSWeights;
		std::vector<float> m_LMSErrors;
		std::vector<float> m_LMSDesireSignalCoefficients;
		std::vector<float> m_LMSDesireSignal;
		std::vector<float> m_LMSCurrentlyCalculatedSubvector;
		std::vector<float> m_MAFConvolutionWindow;

		FilteringType m_currentFilteringType;
		unsigned int m_MAFWindowLenght;
		float m_convergenceRate = 0.2;

	


};

