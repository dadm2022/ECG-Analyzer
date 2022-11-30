#include "DFA.h"

using namespace std;



int main()
{    

    // this code should stay in main program of application in proper module

    //this is vector with data from R-peak module
    vector<int> input;
    // this is value from io module
    int fs = 360;


    vector<int> n;
    inputData inp;

    // choose the range of n
    int start = 4;
    int end = 64;
    n = inp.createNarray(start, end);
    
    cout << "Part1: " << endl;
    
    // create vector for polyfit
    vector <long double> yk;

    yk = inp.readInput(input, fs);

    cout << "Part2: " << endl;

    polyfit poly;
 
    vector <double> f;

    vector <double> flog;
    vector <int> nlog;


    poly.loopPoly(yk, n);
    f = poly.returnF();

    cout << "Part3: " << endl;

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
