#include "DFA.h"

using namespace std;

// creating array with natural numbers for dfa algorhitm
vector<int> inputData::createNarray(int start, int end)
{
    vector <int> n;
    int size = end-start+1;
    for(int i = 0; i<size; i++)
    {
        n.push_back(i+start);
    }

    return n;
}

// create vector with proper data
// input comes from higher level module as number of R-peak sample
vector<long double> inputData::readInput(vector<int>& input, int fs)
{
    // this only for debugging
    // delete before use in application
    ifstream ifile ("C://Users//Natalia//Desktop//dadm projekt//ECG-Analyzer//Modules//HRV_DFA//nsr001.txt");
    double num = 0.0;

    if (!ifile.is_open()) {
            std::cerr << "There was a problem opening the input file!\n";
            exit(1);//exit or do additional error checking
        }

    while (ifile >> num) {
            data.push_back(num);
        };

    // for proper application
    // uncomment this before use in application
//    int len = int (input.size());
//    int k = 0;
//    int temp;

//    while (k < len-1)
//    {
//        temp = input.at(k+1) - input.at(k);
//        data.push_back( temp/fs );
//        k++;
//    }

    length = int(data.size());

    for(int i = 0; i<length; i++)
        xmean += data.at(i);

    xmean = xmean/length;

    for( size_t i = 0; i < data.size(); i++ )
    {
        temp = data.at(i)-xmean;
        y.push_back(temp);
    }

    for( size_t i = 0; i < y.size(); i++ )
    {
        temp += y.at(i);
        yk.push_back(temp);
    }

    return yk;
}




/////////////////////////////////////////////////////////////////

// proper function for all polyfitting for DFA calculation
// y (means yk)
// y_cut is data from yk, but last few data is cut off, as y_cut mast be divided by n value
// whole x is for the number of data in input
// n value say, how large x pieces will be
// for n=4, x will be: <1, 2, 3, 4>, <5, 6, 7, 8> and so on
// with those x and short y_cut pieces, the algorithm of actual polyfit is calculated
// yf is vector of estimated values, needed for calculating difference between estimated and real value
// next are calculations for F(n)
// f is the output vector with F(n)
void Polyfit::loopPoly(vector<long double>& yk, vector<int>& n)
{
    int size = int (n.size());
    for (int i = 0; i<size; i++)
    // for debugging
//        for (int i = 1; i<2; i++)
    {
        j = n[i];
        mod = int (yk.size()) % j;
        y_cut.assign(yk.begin(), yk.end()-mod);

        k = 0;
        m = 0;
        yf.clear();
        diff.clear();
        diff2.clear();

        while (k < int (y_cut.size()) )
        // for debugging
//        while (k < 9 )
//        while (k < 1 )
        {
            x.clear();
            yy.clear();
            p.clear();
            y_est.clear();

            m = k;
            while (m < k+j)
            {
                x.push_back(m+1);
                yy.push_back(y_cut.at( m ));
                m++;
            }

            takeInput(x, yy);
            calculateCoeff(x,p, y_est);

            yf.insert(yf.end(), y_est.begin(), y_est.end() );
            k = k+j;

        }
        diffVec(y_cut, yf, diff);
        pow2Vec(diff, diff2);
        sum = sumDouble(diff2, sum);
        mean_sqr = sum/(int (y_cut.size()));
        sq = sqrt(mean_sqr);
        // f is F(n), the result of DFA
        f.push_back(sq);

        // for debugging
        cout << "Test dzialania: " << j << endl;
    }
}

// return F(n) - result of DFA
vector<double> Polyfit:: returnF()
{
    return f;
}

// calculate coeff a and b of function y=ax+b
// y_est is y-value estimated from a and b values
void Polyfit::calculateCoeff(vector<int>& x, vector<double>& p, vector<long double>& y_est)
    {
        a=0.0;
        b=0.0;
        double N = x.size();


        // to do - change for matrix algorhitm or gsl library
        b = (N * sumXY - sumY * sumX)/(N*sumXsquare - sumX * sumX);
        a = (sumY - b*sumX)/N;

        p.push_back(a);
        p.push_back(b);

        for( size_t i = 0; i < x.size(); i++ )
        {
            value = a*x.at(i) + b;
            y_est.push_back(value);
        }

        sumY = 0;
        sumYsquare = 0;
        sumXsquare = 0;
        sumX = 0;
        sumXY = 0;

    }

// function for taking input data (x and y) and return parameters for polyfit algorithm
void Polyfit::takeInput(vector<int>& x, vector<long double>& y)
    {
        tempx = x.front();
        tempy = y.front();

        for (size_t i = 0; i < x.size(); i++) 
        {
            double xi = 0.0;
            double yi = 0.0;
            xi = x.at(i)-tempx;
            yi = y.at(i)-tempy;
            sumXY += xi * yi;
            sumX += xi;
            sumY += yi;
            sumXsquare += xi * xi;
            sumYsquare += yi * yi;
        }
    }

// difference between real y and estimated y
void Polyfit::diffVec(vector<long double>& y_cut, vector<long double>& yf, vector<double>& diff)
    {
        int k = 0;
        double temp=0.0;

        while (k < int (yf.size()) )
        {
            temp = y_cut.at(k) - yf.at(k);
            diff.push_back(temp);
            k++;
        }
    }

// squared value of difference
 void Polyfit::pow2Vec(vector<double>& diff, vector<double>& diff2)
    {
        int k = 0;
        double temp=0.0;

        while (k < int (diff.size()) )
        {
            temp = diff.at(k)*diff.at(k);
            diff2.push_back(temp);
            k++;
        }
    }

// sum of squared values
 double Polyfit::sumDouble(vector<double>& diff2, double &sum)
    {
        sum = 0.0;
        for_each(diff2.begin(), diff2.end(), [&] (double val)
        {
          sum += val;
        });
        return sum;

    }

// log10 for polyfit after DFA
 template <typename T> T* Polyfit::log10Vec (vector<T> &f, vector<T> &flog)
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
 double Polyfit::calcAlfa(int nDiv, bool choose)
    {
        vector <double> pDfa;
        vector <long double> yEstDfa;
        if (choose == 0)
        {
            fa.insert(fa.begin(), f.begin(), f.begin()+nDiv);
            na.insert(na.begin(), f.begin(), f.begin()+nDiv);

        }
        else
        {
            fa.insert(fa.begin(), f.begin()+nDiv, f.end());
            na.insert(na.begin(), f.begin()+nDiv, f.end());
        }


        takeInput(na, fa);
        calculateCoeff(na, pDfa, yEstDfa);

        return pDfa.at(0);
    }


/////////////////////////////////////////////////////////////////////////////////////////////
 // if unused before 16.12, needs to be deleted
 static double CalcDeterminant(vector<vector<double>> Matrix)
    {
         //this function is written in c++ to calculate the determinant of matrix
         // it's a recursive function that can handle matrix of any dimension
         int det = 0; // the determinant value will be stored here
         if (Matrix.size() == 1)
         {
             return Matrix[0][0]; // no calculation needed
         }
         else if (Matrix.size() == 2)
         {
             //in this case we calculate the determinant of a 2-dimensional matrix in a
             //default procedure
             det = (Matrix[0][0] * Matrix[1][1] - Matrix[0][1] * Matrix[1][0]);
             return det;
         }
         else
         {
             //in this case we calculate the determinant of a squared matrix that have
             // for example 3x3 order greater than 2
             for (int p = 0; p < int (Matrix[0].size()) ; p++)
             {
                 //this loop iterate on each elements of the first row in the matrix.
                 //at each element we cancel the row and column it exist in
                 //and form a matrix from the rest of the elements in the matrix
                 vector<vector<double>> TempMatrix; // to hold the shaped matrix;
                 for (int i = 1; i < int ( Matrix.size() ) ; i++)
                 {
                     // iteration will start from row one cancelling the first row values
                     vector<double> TempRow;
                     for (int j = 0; j < int (Matrix[i].size()); j++)
                     {
                         // iteration will pass all cells of the i row excluding the j
                         //value that match p column
                         if (j != p)
                         {
                            TempRow.push_back(Matrix[i][j]);//add current cell to TempRow
                         }
                     }
                     if (TempRow.size() > 0)
                         TempMatrix.push_back(TempRow);
                     //after adding each row of the new matrix to the vector tempx
                     //we add it to the vector temp which is the vector where the new
                     //matrix will be formed
                 }
                 det = det + Matrix[0][p] * pow(-1, p) * CalcDeterminant(TempMatrix);
                 //then we calculate the value of determinant by using a recursive way
                 //where we re-call the function by passing to it the new formed matrix
                 //we keep doing this until we get our determinant
             }
             return det;
         }
     }
