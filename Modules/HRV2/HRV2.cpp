//
// Created by Izabela Wilusz and Bartłomiej Ziółkowski on 08.12.2022.
//
#include "HRV2.h"
#include <iostream>
#include <valarray>
#include <vector>
#include <cmath>
#include <numeric>
#include <cstdlib>


HRV2::HRV2(std::shared_ptr<std::vector<int>> rPeaks)
{
    SetPeakDistances(rPeaks);
    SetPoincareParams();
    setPoincareX();
    setPoincareY();
    setIdentityLine();
    setEllipse();
}


// HISTOGRAM //

void HRV2::SetPeakDistances(std::shared_ptr<std::vector<int>> rPeaks)
{
    peakDistances.clear();
    int value = 0;
    int prevValue = 0;
    int size = rPeaks->size();

    for (int i = 0; i <size; i++)
    {
        value = rPeaks->at(i);
        if(i!=0){peakDistances.push_back(value-prevValue);}
        prevValue = value;
    }
}

void HRV2::SetHistogram(int bins)
{
    double maxVal;
    double minVal;
    double interval;
    int maxBinNumber;
    std::vector<int> histogram;
    std::vector<double> borders;
    histogram.assign(bins, 0);
    int binSum;
    int upBin;
    int i;
    std::vector<int>::iterator it;

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

    maxBinNumber = std::distance(histogram.begin(), max_element(histogram.begin(), histogram.end())) +1;

    std::vector<int>::iterator maxBin = max_element(histogram.begin(), histogram.end());
    int maxBinSize = histogram[std::distance(histogram.begin(), maxBin)];

    this->OurHistogramData.histogram = histogram;
    this->OurHistogramData.bins = bins;
    this->OurHistogramData.binBorders = borders;
    this->OurHistogramData.maxBinNumber = maxBinNumber;
    this->OurHistogramData.maxBinSize = maxBinSize;
}

HistogramData HRV2::GetHistogram()
{
    HistogramData output = HistogramData(this->OurHistogramData);
    return output;
}




// TINN //

std::vector<double> HRV2::calcBinCenters(int binNumber, double binSize, std::vector<double> borders) {
    std::vector <double> binCenters;
    double center;
    for (int i =0; i<= binNumber; i++)
    {
        center = borders[i] + (0.5)*binSize;
        binCenters.push_back(center);
    }
    return binCenters;
}

int HRV2::calculateDistance(std::vector<double> pointsX, std::vector<double> pointsY)

{
    double xSum=0, x2Sum=0, ySum=0, xySum=0;
    std::vector <double> yFitted;
    std::vector <double> distances;
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


void HRV2::setTinn(int maxHist, int maxBin, int bins, std::vector<double> borders, std::vector<double> binCenters, std::vector<int> histogram){

    //Coordinates of the apex of the triangle
    double xT = binCenters[maxHist - 1];
    int yT = maxBin;

    //Point vectors - they are candidates for point M (centers of bins to the left of maxBin), and for point N (centers of bins to the right of maxBin)
    std::vector <double> xVectorM, xVectorN;
    std::vector <double> yVectorM, yVectorN;

    for (int i = 0; i < maxHist; i++) {
        yVectorM.push_back(histogram[i]);
        xVectorM.push_back(binCenters[i]);
    }

    for (int i = (maxHist-1); i <= (bins-1); i++) {
        yVectorN.push_back(histogram[i]);
        xVectorN.push_back(binCenters[i]);
    }

    // Determination of the indices of points M and N for which the distance from the fitted curve was the smallest
    int idxM, idxN;
    idxM= calculateDistance(xVectorM, yVectorM);
    idxN= calculateDistance(xVectorN, yVectorN) + (maxHist - 1) ;


    // Coordinate values of points M and N
    double xM = binCenters[idxM];
    double xN = binCenters[idxN];
    int yM = histogram[idxM];
    int yN = histogram[idxN];

    // Lines connecting the points
    double a1 = (yN-yM)/(xN-xM);
    double b1 = yM - xM* a1;
    double a2 = (yT-yM)/(xT - xM);
    double b2 = yM - xM* a2;
    double a3 = (yN-yT)/(xN - xT);
    double b3 = yT - xT * a3;

    std::vector <double> xVectorMT, xVectorTN, xVectorMN;
    std::vector <double> yVectorMT, yVectorTN, yVectorMN;

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

    // Vectors containing the coordinates of the lines connecting the vertices of the triangle
    this->TinnResults.xVectorMT = xVectorMT;
    this->TinnResults.yVectorMT = yVectorMT;
    this->TinnResults.xVectorMN = xVectorMN;
    this->TinnResults.yVectorMN = yVectorMN;
    this->TinnResults.xVectorTN = xVectorTN;
    this->TinnResults.yVectorTN = yVectorTN;

    // Numerical value of the TINN: tinnLength [ms], this is the length of the base of the triangle
    double tinnLength = xN - xM;
    this->TinnResults.TinnLength = tinnLength;

}

TinnParams HRV2::getTinn() {

    TinnParams output = TinnParams(this->TinnResults);
    return output;
}




// Triangular index

void HRV2::setTriangularIndex() {

    double Y = *max_element(this->OurHistogramData.histogram.begin(), this->OurHistogramData.histogram.end());
    double numOfRR = peakDistances.size();
    double triangularIndex = numOfRR/Y;
    this->triangularIndex = triangularIndex;
}

double HRV2::getTriangularIndex() {
    double output = double(triangularIndex);
    return output;
}




// Poincare //

void HRV2::SetPoincareParams()
{
    double sd1 = 0;
    double sd2 = 0;
    int i;
    std::vector<double> tempVecRR;
    double temp;

    // Standard deviation of RR outliers
    double SDRR;
    double meanRR = accumulate(peakDistances.begin(), peakDistances.end(),0)/peakDistances.size();

    // Deviation calculated for the probe
    for (i =0; i<peakDistances.size(); i++)
    {
        temp = pow((peakDistances[i] - meanRR),2);
        tempVecRR.push_back(temp);
    }

    SDRR = sqrt(accumulate(tempVecRR.begin(), tempVecRR.end(), 0)/(peakDistances.size()-1));

    // Standard deviation of consecutive differences between adjacent RR intervals
    double SDSD;
    std::vector<double> diff;
    std::vector<double> tempVecSD;

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

PoincareParameters HRV2::GetPoincareParameters()
{
    PoincareParameters output = PoincareParameters(this->PoincareParams);
    return output;
};

void HRV2::setPoincareX()
{
    std::vector<int> pX = peakDistances;
    pX.pop_back();
    this->poincareX = pX;
}

void HRV2::setPoincareY()
{
    std::vector<int> pY = peakDistances;
    reverse(pY.begin(), pY.end());
    pY.pop_back();
    reverse(pY.begin(), pY.end());
    this->poincareY = pY;
}

std::vector<int> HRV2::getPoincareX()
{
    std::vector<int> output = std::vector<int>(this->poincareX);
    return output;
}

std::vector<int> HRV2::getPoincareY()
{
    std::vector<int> output = std::vector<int>(this->poincareY);
    return output;
}

void HRV2::setIdentityLine()
{
    std::vector<int> temp = peakDistances;
    temp.pop_back();
    this->identityLine = temp;
}

std::vector<int> HRV2::GetIdentityLine()
{
    std::vector<int> output = std::vector<int>(this->identityLine);
    return output;
}

void HRV2::setEllipse()
{
    std::vector<int>::iterator it;
    int i;

    //The center of the ellipse
    double sumRR = accumulate(peakDistances.begin(), peakDistances.end(), 0.0);
    int center = sumRR / peakDistances.size();
    int centerIndex = 0;
    const double sd1Length = this->PoincareParams.sd1;
    const double sd2Length = this->PoincareParams.sd2;

    //Calculating the center of an ellipse
    for (i = 0; i <= this->poincareX.size(); i++) {
        if ((this->poincareX[i] == center) && (identityLine[i] == center)) {
            centerIndex = i;
        }
    }

    //Calculating the perpendicular line

    int p1x = *min_element(poincareX.begin(), poincareX.end());
    int p1y = *min_element(poincareY.begin(), poincareY.end());
    int p2x = *max_element(poincareX.begin(), poincareX.end());
    int p2y = *max_element(poincareY.begin(), poincareY.end());

    double slope = double((p2y - p1y) / (p2x - p1x));
    slope = -1 / slope;

    std::vector<int> perpendicular;
    int temp;

    for (it = poincareX.begin(); it < poincareX.end(); it++) {

        temp = int(slope * (*it - center) + center);
        perpendicular.push_back(temp);

    }


    // Calculating the end of the ellipse axis

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

EllipseParameters HRV2::GetEllipse(){

    EllipseParameters output = EllipseParameters(this->Ellipse);
    return output;
}