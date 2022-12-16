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

#include <gsl/gsl_fit.h>


// todo - check if all libraries needed

using namespace std;

#ifndef ECG_ANALYZER_SAMPLE_H
#define ECG_ANALYZER_SAMPLE_H



class Polyfit {
private:

    std::shared_ptr<std::vector<int>> m_Input;
    int m_Fs;

    vector <long double> m_Yk;
    int m_Start, m_End;
    vector <int> m_N;

    vector <long double> m_YCut;

    vector <long double> m_Yf;
    vector <long double> m_Yest;
    vector <double> m_P;

    vector <long double> m_Diff;
    vector <long double> m_Diff2;

    vector <double> m_F;
    vector <double> m_FLog;
    vector <double> m_NLog;

    // DFA
    vector <long double> m_Fa;
    vector <long double> m_Na;
    int m_NDiv;

    // FUNCTIONS
    void createNarray();
    void readInput();

    void loopPoly();

    // calculate coeff
    template <typename T> void calculateCoeff(vector<T>& x, vector <long double> yy, vector <long double>& p, bool isEst);

    void diffVec();

    void pow2Vec();

    template <typename T, typename A> void log10Vec (vector<T> &f, vector<A> &flog);

    // part for DFA
    double calcAlfa(bool choose);
    void calculateResults();


 
public:
    // Constructor
    Polyfit(int fs, std::shared_ptr<const std::vector<int>>& RPeaks, int start = 4, int end = 64, int nDiv = 16)
    {
        *m_Input = *RPeaks;
        m_Fs = fs;
        m_Start = start;
        m_End = end;
        m_NDiv = nDiv;
        // debugging
//        cout << "constructor " << endl;
    }

    double returnAlfa(bool choose);
    vector <double> returnLogF();
    vector <double> returnLogN();
    int returnNDiv();

};


#endif //ECG_ANALYZER_SAMPLE_H
