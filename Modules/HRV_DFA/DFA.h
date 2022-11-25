#include <cstdio>
#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>
#include <numeric>

using namespace std;

class polyfit {
private: 
    // Store the coefficient/slope in the best fitting line
    float coeff;
 
    // Store the constant term in the best fitting line
    float constTerm;
 
    // Contains sum of product of all (i-th x) and (i-th y)
    float sum_xy;
 
    // Contains sum of all (i-th x)
    float sum_x;
 
    // Contains sum of all (i-th y)
    float sum_y;
 
    // Contains sum of square of all (i-th x)
    float sum_x_square;
 
    // Contains sum of square of all (i-th y)
    float sum_y_square;
 
public:
    // Constructor
    polyfit()
    {
        coeff = 0;
        constTerm = 0;
        sum_y = 0;
        sum_y_square = 0;
        sum_x_square = 0;
        sum_x = 0;
        sum_xy = 0;
    }
 
    // Function that calculate the coefficient/slope
    void calculateCoefficient(vector<int>& x);
 
    // Member function that will calculate the constant term 
    void calculateConstantTerm(vector<int>& x);
 
    // Function that return the size of data set
    int sizeOfData(vector<int>& x);
 
    // Function that return the coefficient/
    // slope of the best fitting line
    float coefficient();

 
    // Function that return the constant
    // term of the best fitting line
    float constant();
 
 
    // Function to take input from the dataset
    void takeInput(vector<int>& x, vector<double>& yy);

    // Function to return vector of 2 values - a and b
    void returnP(vector<double>& p);
};





void polyval(vector<double>& p, vector<int>& x, vector<double>& y_temp);
