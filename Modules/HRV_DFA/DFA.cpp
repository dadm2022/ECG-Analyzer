

#include "DFA.h"

using namespace std;

void polyfit::calculateCoefficient(vector<int>& x)
    {
        float N = x.size();
        float numerator
            = (N * sum_xy - sum_x * sum_y);
        float denominator
            = (N * sum_x_square - sum_x * sum_x);
        coeff = numerator / denominator;
    }

void calculateConstantTerm(vector<int>& x)
    {
        float N = x.size();
        float numerator
            = (sum_y * sum_x_square - sum_x * sum_xy);
        float denominator
            = (N * sum_x_square - sum_x * sum_x);
        constTerm = numerator / denominator;
    }

int sizeOfData(vector<int>& x)
    {
        return x.size();
    }

float coefficient()
    {
        if (coeff == 0)
            calculateCoefficient();
        return coeff;
    }

float constant()
    {
        if (constTerm == 0)
            calculateConstantTerm();
        return constTerm;
    }

void takeInput(vector<int>& x, vector<double>& yy)
    {
        for (size_t i = 0; i < x.size(); i++) 
        {
            float xi;
            float yi;
            xi = x.at(i);
            yi = y.at(i);
            sum_xy += xi * yi;
            sum_x += xi;
            sum_y += yi;
            sum_x_square += xi * xi;
            sum_y_square += yi * yi;
        }
    }
    
void returnP (vector<double>& p)
    {
		p.push_back(coeff);
		p.push_back(constTerm);
	}





void polyval(vector<float>& p, vector<int>& x, vector<float>& y_temp)
{ 
	float value;
	for( size_t i = 0; i < x.size(); i++ )
    {
		value = p.at(0)*x.at(i) + p.at(1);
		y_temp.push_back(value);
	}
}

