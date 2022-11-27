#include "DFA.h"

using namespace std;

void polyfit::calculateCoefficient(vector<int>& x)
    {
        double N = x.size();
        double numerator
            = (N * sum_xy - sum_x * sum_y);
        double denominator
            = (N * sum_x_square - sum_x * sum_x);
        coeff = numerator / denominator;
    }

void polyfit::calculateConstantTerm(vector<int>& x)
    {
        double N = x.size();
        double numerator
            = (sum_y * sum_x_square - sum_x * sum_xy);
        double denominator
            = (N * sum_x_square - sum_x * sum_x);
        constTerm = numerator / denominator;
    }

int polyfit::sizeOfData(vector<int>& x)
    {
        return x.size();
    }

double polyfit::coefficient(vector<int>& x)
    {
        if (coeff == 0)
            calculateCoefficient(x);
        return coeff;
    }

double polyfit::constant(vector<int>& x)
    {
        if (constTerm == 0)
            calculateConstantTerm(x);
        return constTerm;
    }

void polyfit::takeInput(vector<int>& x, vector<double>& yy)
    {
        for (size_t i = 0; i < x.size(); i++) 
        {
            double xi;
            double yi;
            xi = x.at(i);
            yi = yy.at(i);
            sum_xy += xi * yi;
            sum_x += xi;
            sum_y += yi;
            sum_x_square += xi * xi;
            sum_y_square += yi * yi;
        }
    }

void polyfit::returnP (vector<double>& p)
    {
        p.push_back(coeff);
        p.push_back(constTerm);
	}

void polyfit::polyval(vector<double>& p, vector<int>& x, vector<double>& y_temp)
    {
        for( size_t i = 0; i < x.size(); i++ )
        {
            value = p.at(0)*x.at(i) + p.at(1);
            y_temp.push_back(value);
//            cout << "Test dzialania" << endl;
        }
    }

void diffVec(vector<double>& y_cut, vector<double>& yf, vector<double>& diff)
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

 void pow2Vec(vector<double>& diff, vector<double>& diff2)
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


 void sumVec(vector<double>& y, vector<double>& yk)
    {
        double temp=0.0;

        for( size_t i = 0; i < y.size(); i++ )
        {
            temp += y.at(i);
            yk.push_back(temp);
        }
    }


 void sumDouble(vector<double>& diff2, double &sum)
    {
//        double temp = 0.0;
        sum = 0.0;
        for( size_t i = 0; i < diff2.size(); i++ )
        {
            sum += diff2.at(i);
//            temp = diff2.at(i);
        }

    }

 void log10Vec(vector<double>& f, vector<double>& flog)
    {
        double temp = 0.0;
        for( size_t i = 0; i < f.size(); i++ )
        {
            temp = log10 ( f.at(i) );
            flog.push_back(temp);
        }
    }

 void log10Vec(vector<int>& n, vector<int>& nlog)
    {
        double temp = 0.0;
        for( size_t i = 0; i < n.size(); i++ )
        {
            temp = log10 ( n.at(i) );
            nlog.push_back(temp);
        }
    }
