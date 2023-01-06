#include "DFA.h"


void Polyfit::createNarray()
{
    int m_Size = m_End-m_Start+1;
    m_N.resize(m_Size);
    std::iota(m_N.begin(), m_N.end(), m_Start);
}

void Polyfit::readInput()
{
    // this only for debugging
    // delete before merge
    std::ifstream ifile ("C://Users//Natalia//Desktop//dadm projekt//ECG-Analyzer//Modules//HRV_DFA//nsr001.txt");
    double m_Num = 0.0;

    std::vector <double> m_Data;


    if (!ifile.is_open()) {
            std::cerr << "There was a problem opening the input file!\n";
            exit(1);//exit or do additional error checking
        }

    while (ifile >> m_Num) {
            m_Data.push_back(m_Num);
        }

    // for proper application
    // uncomment before merge
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
    auto m_Xmean = 0.0l;
    auto m_Temp = 0.0l;
    std::vector <long double> m_Y;

    m_Xmean = std::accumulate(begin(m_Data), end(m_Data), 0.0l);

    m_Xmean = m_Xmean / m_Length;

    for(double i : m_Data)
    {
        m_Temp = i - m_Xmean;
        m_Y.push_back(m_Temp);
    }

    m_Temp = 0.0l;

    for(long double i : m_Y)
    {
        m_Temp += i;
        m_Yk.push_back(m_Temp);
    }
}

void Polyfit::loopPoly()
{
    std::vector <int> m_X;
    std::vector <long double> m_Yy;
    std::vector <long double> m_P;
    auto size = int (m_N.size());
    auto j = 0, k = 0, m = 0, mod = 0;
    auto sum = 0.0l , mean_sqr = 0.0l, sq = 0.0l;

    for (int i = 0; i < size; i++)
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
        sum = std::accumulate(begin(m_Diff2), end(m_Diff2), 0.0l);
        mean_sqr = sum/(int (m_YCut.size()));
        sq = sqrt(mean_sqr);
        // m_F is F(n), the result of DFA
        m_F.push_back(sq);
    }
}


template <typename T> void Polyfit::calculateCoeff(std::vector<T>& x, std::vector <long double> yy, std::vector <long double>& p, bool isEst)
{
    long double value = 0.0;
    auto N = int(x.size());

    const std::vector<double> constX(x.begin(), x.end());
    const std::vector<double> constY(yy.begin(), yy.end());

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
}


void Polyfit::diffVec()
{
    int k = 0;
    auto temp=0.0l;

    while (k < int (m_Yf.size()) )
    {
        temp = m_YCut.at(k) - m_Yf.at(k);
        m_Diff.push_back(temp);
        k++;
    }
}

void Polyfit::pow2Vec()
{
    int k = 0;
    auto temp=0.0l;

    while (k < int (m_Diff.size()) )
    {
        temp = m_Diff.at(k) * m_Diff.at(k);
        m_Diff2.push_back(temp);
        k++;
    }
}


template <typename T> void Polyfit::log10Vec (std::vector<T> &f, std::vector<double> &flog)
{
    for (auto it = f.begin(); it != f.end(); it++)
    {
        flog.push_back( log10(*it) );
    }
}

long double Polyfit::calcAlfa(bool whichAlfa)
{
    std::vector <long double> pDfa;
    if (whichAlfa == false)
    {
        m_Fa.insert(m_Fa.begin(), m_FLog.begin(), m_FLog.begin()+m_NDiv);
        m_Na.insert(m_Na.begin(), m_NLog.begin(), m_NLog.begin()+m_NDiv);

    }
    else
    {
        m_Fa.insert(m_Fa.begin(), m_FLog.begin()+m_NDiv-1, m_FLog.end());
        m_Na.insert(m_Na.begin(), m_NLog.begin()+m_NDiv-1, m_NLog.end());
    }


    calculateCoeff(m_Na, m_Fa, pDfa, false);

    return pDfa.at(0);
}

void Polyfit::calculateResults()
{
    readInput();
    createNarray();
    loopPoly();
    log10Vec(m_N, m_NLog);
    log10Vec(m_F, m_FLog);
    m_Fa.clear();
    m_Na.clear();
    alfa1 = calcAlfa(false);
    m_Fa.clear();
    m_Na.clear();
    alfa2 = calcAlfa(true);
}

long double Polyfit::returnAlfa1() const
{
    return alfa1;
}

long double Polyfit::returnAlfa2() const
{
    return alfa2;
}

std::vector<double> Polyfit::returnLogF()
{
    return m_FLog;
}

std::vector<double> Polyfit::returnLogN()
{
    return m_NLog;
}

int Polyfit::returnNDiv() const
{
    return m_NDiv;
}