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

// todo - check if all libraries needed

using namespace std;



class inputData
{
    private:
    vector <double> data;
    vector <long double> y;
    vector <long double> yk;
    double xmean;
    int length;
    double temp;

    public:
    inputData()
    {
        xmean=0;
        length = 0;
        temp = 0;
    }

    // change code for data in proper application!!!!!!!

    // pass the data from module R-Peaks
    // fs needs checking from io module
    vector<long double> readInput(vector<int>& input, int fs);

    vector<int> createNarray(int start, int end);

};



class polyfit {
private: 

    // coeffs y=ax+b
    long double a;
    long double b;
 
    // for sum algorithm
    double sumXY;
    double sumX;
    double sumY;
    double sumXsquare;
    double sumYsquare;

    double value;

    // iterators
    int j, k, m;

    // for modulo
    int mod;

    //data in polyfit algorithm
    vector <int> x;
    vector <long double> y_cut;
    vector <long double> yy;
    vector <double> p;
    vector <long double> y_est;
    vector <long double> yf;

    // for root mean square error
    vector <double> diff;
    vector <double> diff2;
    double sum;
    double mean_sqr;
    double sq;
    vector <double> f;

    // DFA
    vector <long double> fa;
    vector <int> na;

    // calculate coeff
    void calculateCoeff(vector<int>& x, vector<double>& p, vector<long double>& y_est);

    // Function to take input from the dataset
    void takeInput(vector<int>& x, vector<long double>& y);

    void diffVec(vector<long double>& y_cut, vector<long double>& yf, vector<double>& diff);

    void pow2Vec(vector<double>& diff, vector<double>& diff2);

    double sumDouble(vector<double>& diff2, double &sum);


    template <typename T> T* log10Vec (vector<T> &f, vector<T> &flog);

 
public:
    // Constructor
    polyfit()
    {
        a = 0.0;
        b = 0.0;
        sumY = 0;
        sumYsquare = 0;
        sumXsquare = 0;
        sumX = 0;
        sumXY = 0;
        value = 0;

        sum = 0;
        mean_sqr = 0;
        sq = 0;
    }

    void loopPoly(vector<long double>& yk, vector<int>& n);
    vector<double> returnF();

    // part for DFA
    double calcAlfa(int nDiv, bool choose);


};


    // funkcja do liczenia wyznacznika macierzy, do algorytmu macierzowego
    // not used
    static double CalcDeterminant(vector<vector<double>> Matrix);
