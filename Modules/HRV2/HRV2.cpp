//
// Created by Izabela Wilusz and Bartłomiej Ziółkowski on 08.12.2022.
//
#include "HRV2.h"
using namespace std;

HRV2::HRV2()
{
}


// HISTOGRAM //

void HRV2::SetPeakDistances(vector<int> peaks)
{
    peakDistances.clear();
    int value = 0;
    int prevValue = 0;
    int size = peaks.size();

    for (int i = 0; i <size; i++)
    {
        value = peaks.at(i);
        if(i!=0){peakDistances.push_back(value-prevValue);}
        prevValue = value;
    }
}

void HRV2::SetHistogram(int bins, vector<int> values)
{
    SetPeakDistances(values);

    double maxVal;
    double minVal;
    double interval;
    int maxBinNumber;
    vector<int> histogram;
    vector<double> borders;
    histogram.assign(bins, 0);
    int binSum;
    int upBin;
    int i;
    vector<int>::iterator it;

    maxVal = *max_element(this->peakDistances.begin(), this->peakDistances.end());
    minVal = *min_element(this->peakDistances.begin(), this->peakDistances.end());

    borders.push_back(minVal);

    double diff = maxVal-minVal;
    interval = static_cast<double>(diff/bins);
    this->OurHistogramData.interval= interval;

    for (i = 0; i<bins; i++)
    {
        binSum = 0;
        upBin = minVal+(interval*(i+1));
        borders.push_back(upBin);

        for (it = this->peakDistances.begin(); it != this->peakDistances.end(); it++)
        {
            if((*it >= (minVal + i*interval)) && (*it < (minVal + (i+1)*interval))) {

                binSum++;
            }
            else {
                if (*it == upBin && *it == maxVal) {

                    binSum++;
                }
            }
        }

        histogram[i] = binSum;
    }

    maxBinNumber = distance(histogram.begin(), max_element(histogram.begin(), histogram.end())) +1;

    vector<int>::iterator maxBin = max_element(histogram.begin(), histogram.end());
    int maxBinSize = histogram[distance(histogram.begin(), maxBin)];

    this->OurHistogramData.histogram = histogram;
    this->OurHistogramData.bins = bins;
    this->OurHistogramData.binBorders = borders;
    this->OurHistogramData.maxBinNumber = maxBinNumber;
    this->OurHistogramData.maxBinSize = maxBinSize;
}

shared_ptr<HistogramData> HRV2::GetHistogram()
{
    shared_ptr<HistogramData> output = make_shared<HistogramData>(this->OurHistogramData);
    return output;
}




// TINN //

vector<double> HRV2::calcBinCenters(int binNumber, double binSize, vector<double> borders) {
    vector <double> binCenters;
    double center;
    for (int i =0; i<= binNumber; i++)
    {
        center = borders[i] + (0.5)*binSize;
        binCenters.push_back(center);
    }
    return binCenters;
}

int HRV2::calculateDistance(vector<double> pointsX, vector<double> pointsY)

{
    double xSum=0, x2Sum=0, ySum=0, xySum=0;
    vector <double> yFitted;
    vector <double> distances;
    int minDistanceIndex;

    int listSize = pointsX.size();
    for (int i=0; i<listSize; i++)
    {
        xSum = xSum + pointsX[i];
        ySum = ySum + pointsY[i];
        x2Sum = x2Sum + pow(pointsX[i],2);
        xySum = xySum + pointsX[i] * pointsY[i];
    }
    double a = (listSize * xySum - xSum * ySum)/(listSize * x2Sum - xSum * xSum);
    double b = (x2Sum * ySum - xSum * xySum)/(x2Sum * listSize - xSum * xSum);


    for (int i = 0; i < pointsX.size(); i++) {
        yFitted.push_back(a * pointsX[i] + b);
    }

    for (int i = 0; i < pointsX.size(); i++) {
        distances.push_back(abs(yFitted[i]-pointsY[i]));
    }
    minDistanceIndex = min_element(distances.begin(), distances.end()) - distances.begin();
    return minDistanceIndex;
}


void HRV2::setTinn(int maxHist, int maxBin, int bins, vector<double> borders, vector<double> binCenters, vector<int> histogram){

    //Współrzędne wierzcholka trojkata
    double xT = binCenters[maxHist - 1];
    int yT = maxBin;

    //Wektory punktów-kandydatów na punkt M (środki binów na lewo od maxBin), i na punkt N (środki binów na prawo od maxBin)
    vector <double> xVectorM, xVectorN;
    vector <double> yVectorM, yVectorN;

    for (int i = 0; i < maxHist; i++) {
        yVectorM.push_back(histogram[i]);
        xVectorM.push_back(binCenters[i]);
    }

    for (int i = (maxHist-1); i <= (bins-1); i++) {
        yVectorN.push_back(histogram[i]);
        xVectorN.push_back(binCenters[i]);
    }

    // Wyznaczenie indeksów punktów M i N dla których odległość od dopasowanej krzywej była najmniejsza
    int idxM, idxN;
    idxM= calculateDistance(xVectorM, yVectorM);
    idxN= calculateDistance(xVectorN, yVectorN) + (maxHist - 1) ;


    // Wartosci współrzędnych punktów M i N
    double xM = binCenters[idxM];
    double xN = binCenters[idxN];
    int yM = histogram[idxM];
    int yN = histogram[idxN];

    // Proste łączącze poszczególne punkty
    double a1 = (yN-yM)/(xN-xM);
    double b1 = yM - xM* a1;
    double a2 = (yT-yM)/(xT - xM);
    double b2 = yM - xM* a2;
    double a3 = (yN-yT)/(xN - xT);
    double b3 = yT - xT * a3;

    vector <double> xVectorMT, xVectorTN, xVectorMN;
    vector <double> yVectorMT, yVectorTN, yVectorMN;

    for (int i = 0; i <= (xT - xM); i++) {
        yVectorMT.push_back(xT + i);
        xVectorMT.push_back(a2*(xM+i)+b2);
    }

    for (int i = 0; i <= (xN-xM); i++) {
        yVectorMN.push_back(xM+i);
        xVectorMN.push_back(a1*(xM+i)+b1);
    }

    for (int i = 0; i <= (xN - xT); i++) {
        yVectorTN.push_back(xT + i);
        xVectorTN.push_back(a3*(xT + i) + b3);
    }

    // Wektory zawierające współrzędne prostych łączących wierzchołki trójkąta
    this->TinnResults.xVectorMT = xVectorMT;
    this->TinnResults.yVectorMT = yVectorMT;
    this->TinnResults.xVectorMN = xVectorMN;
    this->TinnResults.yVectorMN = yVectorMN;
    this->TinnResults.xVectorTN = xVectorTN;
    this->TinnResults.yVectorTN = yVectorTN;

    // Wartość liczbowa wskaźnika TINN: tinnLength [ms], jest to długość podstawy trójkąta
    double tinnLength = xN - xM;
    this->TinnResults.TinnLength = tinnLength;

}

shared_ptr<TinnParams> HRV2::getTinn() {

    shared_ptr<TinnParams> output = make_shared<TinnParams>(this->TinnResults);
    return output;
}




// Indeks trójkątny //

void HRV2::setTriangularIndex(vector<int> values) {

    double Y = *max_element(this->OurHistogramData.histogram.begin(), this->OurHistogramData.histogram.end());
    double numOfRR = values.size();
    double triangularIndex = numOfRR/Y;
    this->triangularIndex = triangularIndex;
}

shared_ptr<double> HRV2::getTriangularIndex() {
    shared_ptr<double> output = make_shared<double>(triangularIndex);
    return output;
}




// Poincare //

void HRV2::SetPoincareParams(vector<int> values)
{
    SetPeakDistances(values);
    double sd1 = 0;
    double sd2 = 0;
    int i;
    vector<double> tempVecRR;
    double temp;

    // Odchylenie standradowe odstepow RR
    double SDRR;
    double meanRR = accumulate(peakDistances.begin(), peakDistances.end(),0)/peakDistances.size();

    // Odchylenie liczone dla proby
    for (i =0; i<peakDistances.size(); i++)
    {
        temp = pow((peakDistances[i] - meanRR),2);
        tempVecRR.push_back(temp);
    }

    SDRR = sqrt(accumulate(tempVecRR.begin(), tempVecRR.end(), 0)/(peakDistances.size()-1));

    // Odchylenie standardowe kolejnych roznic pomiedzy sasiadujacymi odstepami RR
    double SDSD;
    vector<double> diff;
    vector<double> tempVecSD;

    for(i = 0; i<peakDistances.size()-1;i++)
    {
        temp = abs(peakDistances[i] - peakDistances[i+1]);
        diff.push_back(temp);
    }

    double mean_diff = accumulate(diff.begin(), diff.end(),0)/diff.size();

    for (i =0; i<diff.size(); i++)
    {
        temp = pow((diff[i] - mean_diff),2);
        tempVecSD.push_back(temp);
    }

    SDSD = sqrt(accumulate(tempVecSD.begin(), tempVecSD.end(), 0)/(tempVecSD.size()-1));

    sd1 = sqrt(pow(SDSD,2)/2);
    sd2 = sqrt( 2*pow(SDRR,2) - (pow(SDSD,2)/2));

    this->PoincareParams.sd1 = sd1;
    this->PoincareParams.sd2 = sd2;

}

shared_ptr<PoincareParameters> HRV2::GetPoincareParameters()
{
    shared_ptr<PoincareParameters> output = make_shared<PoincareParameters>(this->PoincareParams);
    return output;
};

void HRV2::setPoincareX(vector<int> values)
{
    SetPeakDistances(values);
    vector<int> pX = peakDistances;
    pX.pop_back();
    this->poincareX = pX;
}

void HRV2::setPoincareY(vector<int> values)
{
    SetPeakDistances(values);
    vector<int> pY = peakDistances;
    reverse(pY.begin(), pY.end());
    pY.pop_back();
    reverse(pY.begin(), pY.end());
    this->poincareY = pY;
}

shared_ptr<vector<int>> HRV2::getPoincareX()
{
    shared_ptr<vector<int>> output = make_shared<vector<int>>(this->poincareX);
    return output;
}

shared_ptr<vector<int>> HRV2::getPoincareY()
{
    shared_ptr<vector<int>> output = make_shared<vector<int>>(this->poincareY);
    return output;
}

void HRV2::setIdentityLine(vector<int> values)
{
    SetPeakDistances(values);
    vector<int> temp = peakDistances;
    temp.pop_back();
    this->identityLine = temp;
}

shared_ptr<vector<int>> HRV2::GetIdentityLine()
{
    shared_ptr<vector<int>> output = make_shared<vector<int>>(this->identityLine);
    return output;
}

void HRV2::setEllipse(vector<int> values)
{

    SetPeakDistances(values);
    vector<int>::iterator it;
    int i;

    //Środek elipsy
    double sumRR = accumulate(peakDistances.begin(), peakDistances.end(), 0.0);
    int center = sumRR / peakDistances.size();
    int centerIndex = 0;
    const double sd1Length = this->PoincareParams.sd1;
    const double sd2Length = this->PoincareParams.sd2;

    //Obliczanie srodka elipsy
    for (i = 0; i <= this->poincareX.size(); i++) {
        if ((this->poincareX[i] == center) && (identityLine[i] == center)) {
            centerIndex = i;
        }
    }

    //Obliczenie linii prostopadlej

    int p1x = *min_element(poincareX.begin(), poincareX.end());
    int p1y = *min_element(poincareY.begin(), poincareY.end());
    int p2x = *max_element(poincareX.begin(), poincareX.end());
    int p2y = *max_element(poincareY.begin(), poincareY.end());

    double slope = double(p2y - p1y) / (p2x - p1x);
    slope = -1 / slope;

    vector<int> perpendicular;
    int temp;

    for (it = poincareX.begin(); it < poincareX.end(); it++) {

        temp = int(slope * (*it - center) + center);
        perpendicular.push_back(temp);

    }

    // Koniec obliczania linii prostopadej

    // Obliczanie konca osi elipsy

    for(i = 0; i<this->identityLine.size(); i++){

        if ((identityLine[i] - identityLine[centerIndex]) <= sd2Length && identityLine[i] >= identityLine[centerIndex])
        {
            Ellipse.valuesSD2.push_back(identityLine[i]);
            Ellipse.indicesSD2.push_back(i);
        }

        if((perpendicular[i] - perpendicular[centerIndex]) <= sd1Length && perpendicular[i] >= perpendicular[centerIndex])
        {
            Ellipse.valuesSD1.push_back(perpendicular[i]);
            Ellipse.indicesSD1.push_back(i);
        }

    }

}