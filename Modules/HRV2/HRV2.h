//
// Created by Izabela Wilusz and Bartłomiej Ziółkowski on 08.12.2022.
//

#ifndef ECG_ANALYZER_HRV2_H
#define ECG_ANALYZER_HRV2_H

#include <complex>
#include <iostream>
#include <valarray>
#include <vector>
#include <cmath>
#include <numeric>
#include <cstdlib>
#include <algorithm>
#include <memory>
using namespace std;

struct HistogramData
{
    int bins;
    vector<int> histogram;
    vector<double> binBorders;
    double interval;
    int maxBinSize;
    int maxBinNumber;
};

struct TinnParams{

    vector<double> xVectorMT;
    vector<double> yVectorMT;
    vector<double> xVectorMN;
    vector<double> yVectorMN;
    vector<double> xVectorTN;
    vector<double> yVectorTN;
    double TinnLength;

};

struct PoincareParameters
{
    double sd1;
    double sd2;
};

struct EllipseParameters
{
    vector<int> valuesSD1;
    vector<int> valuesSD2;
    vector<int> indicesSD1;
    vector<int> indicesSD2;
};

class HRV2 {
private:

    HistogramData OurHistogramData;
    vector<int> peakDistances;
    void SetPeakDistances(vector<int> values);

    TinnParams TinnResults;

    PoincareParameters PoincareParams;
    vector<int> poincareX;
    vector<int> poincareY;
    vector<int> identityLine;

    EllipseParameters Ellipse;
public:
    HRV2();

    //Histogram

    void SetHistogram(int bins, vector<int> values);
    shared_ptr<HistogramData>  GetHistogram();

    //Tinn
    void setTinn(int max_hist, int max_bin, int bins, vector <double> borders, vector <double> binsCenters, vector <int> histogram);
    vector <double> calcBinCenters(int binNum, double binSize, vector <double> borders);
    shared_ptr<TinnParams> getTinn();
    int calculateDistance(vector<double> pointsX, vector<double> pointsY);

    //Indeks trojkatny
    void setTriangularIndex(vector<int> values);
    shared_ptr<double> getTriangularIndex();

    //Wykres Poincare
    void SetPoincareParams(vector<int>);
    shared_ptr<PoincareParameters> GetPoincareParameters();

    void setPoincareX(vector<int> values);
    void setPoincareY(vector<int> values);

    shared_ptr<vector<int>> getPoincareX();
    shared_ptr<vector<int>> getPoincareY();


    void setIdentityLine(vector<int> values);
    shared_ptr<vector<int>> GetIdentityLine();


    void setEllipse(vector<int> values);


    double triangularIndex;
};

#endif //ECG_ANALYZER_HRV2_H
