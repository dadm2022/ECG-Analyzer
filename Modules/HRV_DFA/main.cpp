#include "DFA.h"

using namespace std;



int main()
{    

    // this code should stay in main program of application in proper module

    //this is vector with data from R-peak module - number of sample where R-peak is
    vector<int> input;
    // this is value from io module - frequency of sampling from header
    int fs = 360;

    // vector of natural numbers from start to end, used for DFA
    vector<int> n;
    inputData inp;

    // need to choose the range of n
    // Fabian agreed to those
    int start = 4;
    int end = 64;
    n = inp.createNarray(start, end);


    // debugging
    cout << "Part1: " << endl;
    
    // create vector for polyfit; reworked input vector
    vector <long double> yk;
    yk = inp.readInput(input, fs);


    // debugging
    cout << "Part2: " << endl;

    Polyfit poly;


    // vector of F(n)
    vector <double> f;

    // vectors of F(n) and n, but in log values
    vector <double> flog;
    vector <int> nlog;


    poly.loopPoly(yk, n);
    f = poly.returnF();


    // debugging
    cout << "Part3: " << endl;

    // value for n, whre alfa1 ends end alfa2 starts
    // agreed with Fabian
    int nDiv = 16;
    double alfa1, alfa2;

    // 0 (false) for alfa1, 1 (true) for alfa2
    alfa1 = poly.calcAlfa(nDiv, 0);
    alfa2 = poly.calcAlfa(nDiv, 1);



    // for debugging
    // delete before use in main application
    cout << "Alfa1: " << alfa1 << endl;
    cout << "Alfa2: " << alfa2 << endl;

    cout << "Skonczona petla " << endl;
    
    return 0;
}
