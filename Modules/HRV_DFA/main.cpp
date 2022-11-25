#include "DFA.h"

using namespace std;


int main()
{    
	//długość inputa:
    int length = 106460;
    
    // wybrać, jakie wartości n:
    int start = 4;
    int end = 64;
    int size = end-start+1;
    int n[size];
    double temp;
    
    cout << "size: " << size << endl;
    
    // input - dane z poprzedniego modułu, RR-intervals
    vector <double> input;
    vector <double> y;
    vector <double> yk;
    double xmean;
    
    // stworzenie tablicy, w której są wartości długości przedziałów
    for(int i = 0; i<size; i++)
    {
		if (i-1<0)
		{
			n[i] = start;
		}
		else
		{
			n[i] = n[i-1]+1;
		}
		
		cout << n[ i ] << "  ";
	}
    
    // zakomentować później - tworzenie wartości inputa
    while ( int (input.size()) < length)
    {
		input.push_back(0.70);
	}
    
    
    for(int i = 0; i<length; i++)
		xmean += input.at(i);
	
	xmean = xmean/length;
	cout << "xmean: " << xmean << endl;
	
	for( size_t i = 0; i < input.size(); i++ )
    {
		temp = input.at(i)-xmean;
		y.push_back(temp);
	}
	
	yk.push_back(y.at(0));
    
	for( size_t i = 1; i < y.size(); i++ )
    {
		temp = y.at(i-1) + y.at(i);
		yk.push_back(temp);
	}
    
 
    int j;
    int mod;
    int len;
    vector <double> y_cut;
    int k;
    vector <int> x;
    vector <double> yy;
    vector <double> p;
    vector <double> y_temp;
    vector <double> yf;
    vector <double> diff;
    vector <double> sum;
    vector <double> f;
    polyfit poly;
  
  
	for (int i = 0; i<size; i++)
	{
		j = n[i];
		mod = int (yk.size()) % j;
		len = int (yk.size()) - mod;
		y_cut.assign(yk.begin(), yk.end()-mod);
		//cout << "Size: " << int (y_cut.size()) << '\n';
		k = 0;
		
		while (k < int (y_cut.size()) )
		{
			vector <double> y_t;
			for( int i = k; i < k+j-1; i++ )
			{
				x.push_back(k);
				yy[i] = y_cut[i];
				k = k+j;
				poly.takeInput(x, yy);
				poly.sizeOfData(x);
				poly.calculateCoefficient(x);
				poly.calculateConstantTerm(x);
				poly.returnP(p);
			}
		}
		
		
	}
    
    cout << "Test dzialania" << endl;
    
    return 0;
}
