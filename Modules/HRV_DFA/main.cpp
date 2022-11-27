#include "DFA.h"

using namespace std;



int main()
{    
    vector<double> data;
    ifstream ifile ("C://Users//Natalia//Desktop//dadm projekt//ECG-Analyzer//Modules//HRV_DFA//nsr001.txt");
    double num = 0.0;

    if (!ifile.is_open()) {
            std::cerr << "There was a problem opening the input file!\n";
            exit(1);//exit or do additional error checking
        }

    while (ifile >> num) {
            data.push_back(num);
        };

	//długość inputa:
    int length = 106460;
    
    // wybrać, jakie wartości n:
    int start = 4;
    int end = 64;
    int size = end-start+1;
    vector<int> n;
    double temp;
    
    cout << "size: " << size << endl;
    
    // input - dane z poprzedniego modułu, RR-intervals
    vector <double> input;
    vector <double> y;
    vector <double> yk;
    double xmean=0;
    
    // stworzenie tablicy, w której są wartości długości przedziałów
    for(int i = 0; i<size; i++)
    {
        n.push_back(i+start);
		
//		cout << n[ i ] << "  ";
	}
    
    // zakomentować później - tworzenie wartości inputa
//    while ( int (input.size()) < length/2)
//    {
//		input.push_back(0.70);
//	}

//    while ( int (input.size()) < length)
//    {
//        input.push_back(0.60);
//    }
    input = data;
    
    for(int i = 0; i<length; i++)
		xmean += input.at(i);
	
	xmean = xmean/length;
	cout << "xmean: " << xmean << endl;


//    cout << "xmean: " << input.at(100)-xmean << endl;
	
	temp = 0.0;
	
    for( size_t i = 0; i < input.size(); i++ )
    {
		temp = input.at(i)-xmean;
		y.push_back(temp);

	}
	

    temp = 0.0;

//    for( size_t i = 0; i < y.size(); i++ )
//    {
//        temp += y.at(i);
//        yk.push_back(temp);
//    }

    sumVec(y, yk);

 
    int j;
    int mod;
    vector <double> y_cut;
    int k;
    vector <int> x;
    vector <double> yy;
    vector <double> p;
    vector <double> y_temp;
    vector <double> yf;
    vector <double> diff;
    vector <double> diff2;
    double sum = 0.0;
    vector <double> f;
    polyfit poly;
    int m;
    double yy_temp;
    double sq = 0.0;
    double mean_sqr;
    vector <double> flog;
    vector <int> nlog;

  
    for (int i = 0; i<size; i++)
//    for (int i = 0; i<1; i++)
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
//        while (k < 5 )
//        while (k < 1 )
        {
            x.clear();
            yy.clear();
            p.clear();
            y_temp.clear();

            m = k;
            while (m < k+j)
            {
                x.push_back(m+1);
                yy_temp = y_cut.at( m );
                yy.push_back(yy_temp);
                m++;
            }
            k = k+j;
//            cout << "xsize: " << int(x.size()) << endl;
            poly.takeInput(x, yy);
            poly.sizeOfData(x);
            poly.calculateCoefficient(x);
            poly.calculateConstantTerm(x);
            poly.returnP(p);
				
            poly.polyval(p, x, y_temp);
            yf.insert(yf.end(), y_temp.begin(), y_temp.end() );

        }
        diffVec(y_cut, yf, diff);
        pow2Vec(diff, diff2);
        sumDouble(diff2, sum);
        mean_sqr = sum/(int (y_cut.size()));
        sq = sqrt(mean_sqr);
        f.push_back(sq);


        cout << "Test dzialania: " << j << endl;
    }
    
    log10Vec(f, flog);
    log10Vec(n, nlog);

    vector <double> fa1, fa2;
    vector <int> na1, na2;

    int n_pod = 16;

    fa1.insert(fa1.begin(), f.begin(), f.begin()+n_pod);
    fa2.insert(fa2.begin(), f.begin()+n_pod, f.end());

    na1.insert(na1.begin(), f.begin(), f.begin()+n_pod);
    na2.insert(na2.begin(), f.begin()+n_pod, f.end());

    p.clear();
    poly.takeInput(na1, fa1);
    poly.sizeOfData(na1);
    poly.calculateCoefficient(na1);
    poly.calculateConstantTerm(na1);
    poly.returnP(p);

    cout << "Alfa1: " << p.at(0) << endl;

    p.clear();
    poly.takeInput(na2, fa2);
    poly.sizeOfData(na2);
    poly.calculateCoefficient(na2);
    poly.calculateConstantTerm(na2);
    poly.returnP(p);

    cout << "Alfa2: " << p.at(0) << endl;

    cout << "Skonczona petla " << endl;
    
    return 0;
}
