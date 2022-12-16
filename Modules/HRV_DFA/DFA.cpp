#include "DFA.h"

using namespace std;

// creating array with natural numbers for dfa algorithm
void Polyfit::createNarray()
{

    int m_Size = m_End-m_Start+1;
    for(int i = 0; i<m_Size; i++)
    {
        m_N.push_back(i+m_Start);
    }

}

// create vector with proper data
// input comes from higher level module as number of R-peak sample
void Polyfit::readInput()
{
    // this only for debugging
    // delete before use in application
    ifstream ifile ("C://Users//Natalia//Desktop//dadm projekt//ECG-Analyzer//Modules//HRV_DFA//nsr001.txt");
    double m_Num = 0.0;

    vector <double> m_Data;


    if (!ifile.is_open()) {
            std::cerr << "There was a problem opening the input file!\n";
            exit(1);//exit or do additional error checking
        }

    while (ifile >> m_Num) {
            m_Data.push_back(m_Num);
        };

    // for proper application
    // uncomment this before use in application
//    int m_Len = int (m_Input->size());
//    int k = 0;
//    int m_Temp;

//    while (k < m_Len-1)
//    {
//        m_Temp = m_Input->at(k+1) - m_Input->at(k);
//        m_Data.push_back( m_Temp/m_Fs );
//        k++;
//    }

    int m_Length = int(m_Data.size());
    double m_Xmean = 0.0;
    double m_Temp = 0.0;
    vector <long double> m_Y;

    for(int i = 0; i<m_Length; i++)
        m_Xmean += m_Data.at(i);

    m_Xmean = m_Xmean / m_Length;

    for(size_t i = 0; i < m_Data.size(); i++ )
    {
        m_Temp = m_Data.at(i) - m_Xmean;
        m_Y.push_back(m_Temp);
    }

    m_Temp = 0.0;

//    std::for_each( m_Y.begin(), m_Y.end(), [](int& i)
//    {
//        m_Temp += m_Y.at(i);
//        m_Yk.push_back(m_Temp);
//    }
//    );

    for(size_t i = 0; i < m_Y.size(); i++ )
    {
        m_Temp += m_Y.at(i);
        m_Yk.push_back(m_Temp);
    }
}


// proper function for all polyfitting for DFA calculation
// y (means m_Yk)
// y_cut is data from m_Yk, but last few data is cut off, as y_cut mast be divided by n value
// whole x is for the number of data in input
// n value say, how large x pieces will be
// for n=4, x will be: <1, 2, 3, 4>, <5, 6, 7, 8> and so on
// with those x and short y_cut pieces, the algorithm of actual polyfit is calculated
// yf is vector of estimated values, needed for calculating difference between estimated and real value
// next are calculations for F(n)
// f is the output vector with F(n)
void Polyfit::loopPoly()
{
    vector <int> m_X;
    vector <long double> m_Yy;
    vector <long double> m_P;
    int size = int (m_N.size());
    int j, mod = 0;
    int k, m;
    double sum = 0.0;
    double mean_sqr = 0.0;
    double sq = 0.0;

    for (int i = 0; i < size; i++)
    // for debugging
//        for (int i = 1; i<2; i++)
    {
        j = m_N[i];
        mod = int (m_Yk.size()) % j;
        m_YCut.assign(m_Yk.begin(), m_Yk.end()-mod);

        k = 0;
        m = 0;
        m_Yf.clear();
        m_Diff.clear();
        m_Diff2.clear();

        while (k < int (m_YCut.size()) )
        // for debugging
//        while (k < 9 )
//        while (k < 1 )
        {
            m_X.clear();
            m_Yy.clear();
            m_P.clear();
            m_Yest.clear();

            m = k;
            while (m < k+j)
            {
                m_X.push_back(m+1);
                m_Yy.push_back(m_YCut.at( m ));
                m++;
            }

            calculateCoeff(m_X, m_Yy, m_P, true);

            m_Yf.insert(m_Yf.end(), m_Yest.begin(), m_Yest.end() );
            k = k+j;

        }
        diffVec();
        pow2Vec();
        sum = std::accumulate(begin(m_Diff2), end(m_Diff2), 0);
        mean_sqr = sum/(int (m_YCut.size()));
        sq = sqrt(mean_sqr);
        // f is F(n), the result of DFA
        m_F.push_back(sq);

        // for debugging
//        cout << "Test dzialania: " << j << endl;
    }
}

// calculate coeff a and b of function y=ax+b
// y_est is y-value estimated from a and b values

template <typename T> void Polyfit::calculateCoeff(vector<T>& x, vector <long double> yy, vector <long double>& p, bool isEst)
{
    long double value = 0.0;
    int N = int(x.size());

    const vector<double> constX(x.begin(), x.end());
    const vector<double> constY(yy.begin(), yy.end());

    double beta0, beta1, cov00, cov01, cov11, sumsq;

    gsl_fit_linear(constX.data(), 1, constY.data(), 1, N, &beta0, &beta1,
                   &cov00, &cov01, &cov11, &sumsq);


    p.push_back(beta1);
    p.push_back(beta0);
    if (isEst)
    {
        for( size_t i = 0; i < x.size(); i++ )
        {
            value = beta1*x.at(i) + beta0;
            m_Yest.push_back(value);
        }
    }
//    debugging
//    if (!isEst)
//        cout << p.at(0) << endl;


}


// difference between real y and estimated y
void Polyfit::diffVec()
{
    int k = 0;
    double temp=0.0;

    while (k < int (m_Yf.size()) )
    {
        temp = m_YCut.at(k) - m_Yf.at(k);
        m_Diff.push_back(temp);
        k++;
    }
}

// squared value of difference
 void Polyfit::pow2Vec()
 {
    int k = 0;
    double temp=0.0;

    while (k < int (m_Diff.size()) )
    {
        temp = m_Diff.at(k) * m_Diff.at(k);
        m_Diff2.push_back(temp);
        k++;
    }
 }


// log10 for polyfit after DFA
 template <typename T, typename A> void Polyfit::log10Vec (vector<T> &f, vector<A> &flog)
 {
    for (auto it = f.begin(); it != f.end(); it++)
    {
        flog.push_back( log10(*it) );
    }
 }

 // calculating alfa1 and alfa2 parameters - outputs of this module
 // nDiv tells, for which n the lower n is for alfa1 and higher is for alfa2
 // choose is for choosing which alfa calculate
 // 0 (false) for alfa1, 1 (true) for alfa2
 // fa is cuted f log vector, na is cuted n log vector
 // calculateCoeff gives 3 values, but only pDFA is important
 // pDFA.at(0) is alfa value
 double Polyfit::calcAlfa(bool choose)
    {
        vector <long double> pDfa;
        vector <long double> yEstDfa;
        if (choose == 0)
        {
            m_Fa.insert(m_Fa.begin(), m_FLog.begin(), m_FLog.begin()+m_NDiv);
            m_Na.insert(m_Na.begin(), m_NLog.begin(), m_NLog.begin()+m_NDiv);

        }
        else
        {
            m_Fa.insert(m_Fa.begin(), m_FLog.begin()+m_NDiv, m_FLog.end());
            m_Na.insert(m_Na.begin(), m_NLog.begin()+m_NDiv, m_NLog.end());
        }


        calculateCoeff(m_Na, m_Fa, pDfa, false);

        // debugging
//        cout << pDfa.at(0) << endl;

        return pDfa.at(0);
    }

void Polyfit::calculateResults()
{

    readInput();
    createNarray();
    loopPoly();
    log10Vec(m_N, m_NLog);
    log10Vec(m_F, m_FLog);
}

double Polyfit::returnAlfa(bool choose)
{
    if (!choose)
    {
        calculateResults();
    }
    double alfa = 0.0;
    // can we improve results of alfa1?
    alfa = calcAlfa(choose);
    return alfa;
}

// return log10F(n) - result of DFA
vector<double> Polyfit::returnLogF()
{
    return m_FLog;
}
// return log10n - result of DFA
vector<double> Polyfit::returnLogN()
{
    return m_NLog;
}

int Polyfit::returnNDiv()
{
    return m_NDiv;
}