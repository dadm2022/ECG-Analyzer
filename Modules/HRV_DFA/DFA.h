#include <cstdio>
#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <functional>
#include <bits/stdc++.h>
#include <cmath>
#include <fstream>

using namespace std;

class polyfit {
private: 
    // Store the coefficient/slope in the best fitting line
    double coeff;
 
    // Store the constant term in the best fitting line
    double constTerm;
 
    // Contains sum of product of all (i-th x) and (i-th y)
    double sum_xy;
 
    // Contains sum of all (i-th x)
    double sum_x;
 
    // Contains sum of all (i-th y)
    double sum_y;
 
    // Contains sum of square of all (i-th x)
    double sum_x_square;
 
    // Contains sum of square of all (i-th y)
    double sum_y_square;

    double value;
 
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
        value = 0.0;
    }
 
    // Function that calculate the coefficient/slope
    void calculateCoefficient(vector<int>& x);
 
    // Member function that will calculate the constant term 
    void calculateConstantTerm(vector<int>& x);
 
    // Function that return the size of data set
    int sizeOfData(vector<int>& x);
 
    // Function that return the coefficient/
    // slope of the best fitting line
    double coefficient(vector<int>& x);

 
    // Function that return the constant
    // term of the best fitting line
    double constant(vector<int>& x);
 
 
    // Function to take input from the dataset
    void takeInput(vector<int>& x, vector<double>& yy);

    // Function to return vector of 2 values - a and b
    void returnP(vector<double>& p);

    void polyval(vector<double>& p, vector<int>& x, vector<double>& y_temp);

};

    void diffVec(vector<double>& y_cut, vector<double>& yf, vector<double>& diff);

    void pow2Vec(vector<double>& diff, vector<double>& diff2);

    void sumVec(vector<double>& y, vector<double>& yk);

    void sumDouble(vector<double>& diff2, double &sum);

    void log10Vec(vector<double>& f, vector<double>& flog);
    void log10Vec(vector<int>& n, vector<int>& nlog);
