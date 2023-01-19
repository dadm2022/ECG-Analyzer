/**
 * Created by Natalia Kowalska (10.01.2022)
 *
 * Header file for DFA class
 */


#include <vector>
#include <cmath>
#include <numeric>

#include <gsl/gsl_fit.h>


#ifndef ECG_ANALYZER_DFA_H
#define ECG_ANALYZER_DFA_H

/**
 * Example of usage:
 * @code
 * std::shared_ptr<const std::vector<int>> RPeaks (from another module)
 * auto obiekt = Polyfit(fs, RPeaks, start, end, nDiv);
 * or
 * auto obiekt = Polyfit(fs, RPeaks);
 *
 * long double alfa1, alfa2;
 * vector <double> logFn;
 * vector <double> logN;
 * int div;
 *
 * alfa1 = obiekt.returnAlfa1();
 * alfa2 = obiekt.returnAlfa2();
 * logFn = obiekt.returnLogF();
 * logN = obiekt.returnLogN();
 * div = obiekt.returnNDiv();
 *
 * @endcode
 */


/**
 * Implementation of DFA algorithm in EKG from R peaks data
 */
class Polyfit {
private:

    std::shared_ptr<const std::vector<int>> m_Input;
    int m_Fs;

    std::vector <long double> m_Y;
    int m_Start, m_End;
    std::vector <int> m_N;

    std::vector <long double> m_YCut;

    std::vector <long double> m_Yf;
    std::vector <long double> m_Yest;

    std::vector <long double> m_Diff;
    std::vector <long double> m_Diff2;

    std::vector <long double> m_F;
    std::vector <double> m_FLog;
    std::vector <double> m_NLog;

    std::vector <long double> m_Fa;
    std::vector <long double> m_Na;
    int m_NDiv;
    long double alfa1 = 0.0l;
    long double alfa2 = 0.0l;

    /**
     * private functions
     */
    /**
     * @brief creates vector of natural values
     * @param m_Start   first value
     * @param m_End     last value
     * @return m_N      vector with natural numbers from start to end values
     */
    void createNarray();

    /**
     * @brief reads input and creates vector for further calculations
     *
     * Reads the data from higher module kept in RPeaks vector. Calculates
     * difference between each sample divided by sampling frequency
     * (gets dt), than counts integral - cumulative sum minus mean value.
     *
     * @param   RPeaks from higher module
     * @return  m_Y vector of long doubles for calculation of DFA algorithm
     */
    void readInput();

    /**
     * @brief main function calculating linear models and errors
     *
     * Function mainly in for loop. Takes processed input data, divides it for
     * sections of n length, removes last section if it's shorter than n. Next creates
     * vector of xs which consists of consecutive natural numbers
     * (ex. 1,2,3,4 than 5, 6, 7, 8 etc.) in the n length. For every short section of
     * y and x, linear regression is made (@see calculateCoeff) and error of estimated
     * y value is calculated. For every n value error values are calculated,
     * than the errors are squared and the sum of them is calculated. At the end root mean
     * square of sum is calculated giving n-long vector m_F (F(n))
     * @param m_Y      processed input data (@see readInput)
     * @param m_N       contains values of how long sections of x and y are
     * @return m_F      values of root mean square fluctuation for every n value
     */
    void loopPoly();

    /**
     * @brief Calculates the coefficients of linear regressions a and b:
     * @f$y = a*x + b@f$
     *
     * Function do linear regression using GSL library (@see gsl_fit_linear).
     * It calculates a and be coefficients and stores them in vector.
     * If isEst is true, than using a and b algorithm calculates:
     * @f$y_est = a*x + b@f$
     *
     * @tparam T    type of data, might be int or long double
     * @param x     vector of values representing x
     * @param yy    vector of values representing y
     * @param p     vector of 2 values, a and b
     * @param isEst bool for choosing, if estimated y is calculated
     * @return      p with a and b coefficients
     * @return      if isEst, returns values of estimated y for consecutives x values
     */
    template <typename T> void calculateCoeff(std::vector<T>& x, std::vector <long double> yy, std::vector <long double>& p, bool isEst);

    /**
     * @brief Calculates difference between 2 vectors
     *
     * calculates difference between corresponding values in 2 vectors
     * consisting of real data (y) and estimated data (y_est)
     * @f$diff = y - y_est@f$
     *
     * @param m_Yf      vector of estimated values
     * @param m_YCut    vector of real values
     * @return m_Diff   vector with difference between corresponding elements
     */
    void diffVec();

    /**
     * @brief calculates squared values of values in input vector
     * @param m_Diff    vector of errors
     * @return m_Diff2  vector of squared errors
     */
    void pow2Vec();

    /**
     * @brief calculates vector of decimal logarithm
     * @tparam T    type of data, might be int or long double
     * @param f     input vector
     * @param flog  vector if decimal logarithm
     * @return      vector with values of decimal logarithm
     */
    template <typename T> void log10Vec (std::vector<T> &f, std::vector<double> &flog);

    /**
     * @brief calculates "a" coefficient of linear regression of log10(F(n)) (@see loopPoly)
     *
     * calculates linear regression for function with argument-value:
     * x: log10n
     * y: log10( F(n) )
     * Regression is calculated only for part of n before and after division number
     * (@see m_NDiv). It calculates slopes of one of two sections
     *
     * @param whichAlfa     for false calculates alfa1, for true - alfa2
     * @param m_NDiv        the value of n which divides line into 2 for less and greater n
     * @return              alfa1 or alfa2
     */
    long double calcAlfa(bool whichAlfa);

    /**
     * @brief calculates all parameters for output of module
     *
     * Combines all of the function for computing DFA algorithm, beginning from
     * processing input data to DFA algorithm. It calculates all of the output
     * data stored in class fields
     *
     * @return alfa1, alfa2, m_NDiv, m_NLog, m_FLog
     */
    void calculateResults();



 
public:
    // Constructor
    Polyfit(std::shared_ptr<const std::vector<int>> &RPeaks);

    /**
     *
     * @return alfa1    parameter for less than or equal to m_NDiv values of n
     */
    [[nodiscard]] long double returnAlfa1() const;

    /**
     *
     * @return alfa2    parameter for greater than or equal to m_NDiv values of n
     */
    [[nodiscard]] long double returnAlfa2() const;

    /**
     *
     * @return m_FLog   vector of doubles of log10( F(n) ) (@see log10Vec @see loopPoly)
     */
    [[nodiscard]] std::vector <double> returnLogF() const;

    /**
     *
     * @return m_NLog   vector of doubles of log10( n ) (@see log10Vec @see createNArray)
     */
    [[nodiscard]] std::vector <double> returnLogN() const;

    /**
     *
     * @return m_NDiv   natural value which divides inputs for calculating alfa for 2 sections
     * (below and above m_NDiv)
     */
    [[nodiscard]] int returnNDiv() const;

};


#endif //ECG_ANALYZER_DFA_H
