//
// Created by Izabela Wilusz and Bartłomiej Ziółkowski on 08.12.2022.
//

#ifndef ECG_ANALYZER_HRV2_H
#define ECG_ANALYZER_HRV2_H

#include <algorithm>
#include <memory>

/* Example usage:

    HRV2 hrv2;
    hrv2.SetPeakDistances(mSharedPtr);

    int bins = 16;
    hrv2.SetHistogram(bins);
    HistogramData histogramData = hrv2.GetHistogram();
    vector <int> histogram = histogramData.histogram;
    std::vector<double> binBorders = histogramData.binBorders;
    double interval = histogramData.interval;
    int maxBinSize = histogramData.maxBinSize;
    int maxBinNumber = histogramData.maxBinNumber;
    std::vector<double> binCenters = hrv2.calcBinCenters(bins, interval, binBorders);

    hrv2.setTinn(maxBinNumber, maxBinSize, bins, binBorders, binCenters, histogram);
    TinnParams tinnParams = hrv2.getTinn();
    std::vector<double> xVectorMT = tinnParams.xVectorMT;
    std::vector<double> yVectorMT = tinnParams.yVectorMT;
    double TinnLength = tinnParams.TinnLength;

    hrv2.setTriangularIndex();
    hrv2.getTriangularIndex();
    double triangularIndex = hrv2.triangularIndex;

    hrv2.SetPoincareParams();
    PoincareParameters poincareParameters = hrv2.GetPoincareParameters();
    double sd1 = poincareParameters.sd1;
    hrv2.setPoincareX();
    std::vector<int> poincareX = hrv2.getPoincareX();
    hrv2.setIdentityLine();
    hrv2.setPoincareY();
    std::vector<int> poincareY = hrv2.getPoincareY();
    std::vector<int> identityLine = hrv2.GetIdentityLine();

    hrv2.setEllipse();
    EllipseParameters ellipseParameters = hrv2.GetEllipse();
    std::vector<int> indicesSD1 = ellipseParameters.indicesSD1;
    std::vector<int> valuesSD1 = ellipseParameters.valuesSD1;
*/


struct HistogramData
{
    int bins;
    std::vector<int> histogram;
    std::vector<double> binBorders;
    double interval;
    int maxBinSize;
    int maxBinNumber;
};

struct TinnParams{

    std::vector<double> xVectorMT;
    std::vector<double> yVectorMT;
    std::vector<double> xVectorMN;
    std::vector<double> yVectorMN;
    std::vector<double> xVectorTN;
    std::vector<double> yVectorTN;
    double TinnLength;

};

struct PoincareParameters
{
    double sd1;
    double sd2;
};

struct EllipseParameters
{
    std::vector<int> valuesSD1;
    std::vector<int> valuesSD2;
    std::vector<int> indicesSD1;
    std::vector<int> indicesSD2;
};

class HRV2 {
private:

    HistogramData OurHistogramData;
    std::vector<int> peakDistances;

    TinnParams TinnResults;

    PoincareParameters PoincareParams;
    std::vector<int> poincareX;
    std::vector<int> poincareY;
    std::vector<int> identityLine;

    EllipseParameters Ellipse;

public:

    HRV2();

    //Histogram

    void SetPeakDistances(std::shared_ptr<std::vector<int>> rPeaks);
    // This function takes the R-peaks locations and calculates the RR intervals on which all geometric analysis of the HRV2 modulus is based

    void SetHistogram(int bins);
    // This function prepares all the data needed to create a histogram and assigns it to the HistogramData structure
    // Input:
    //      int bins - number of bins we want on the histogram
    // Output: -
    // From the structure we can get:
    //      std::vector<int> histogram - histogram
    //      std::vector<double> binBorders - bin borders
    //      double interval - width of bins
    //      int maxBinSize - the highest value of counts in histogram
    //      int maxBinNumber - number of bin with the highest value of counts is achieved

    HistogramData GetHistogram();


    //Tinn

    std::vector <double> calcBinCenters(int binNum, double binSize, std::vector <double> borders);
    //This function calculates centers of the bins
    // Input:
    //      int binNum - number of bins
    //      double binSize - size of the bins
    //      std::vector <double> borders - borders of the bins
    // Output:
    //      std::vector <double> binCenters - centers of the bins

    int calculateDistance(std::vector<double> pointsX, std::vector<double> pointsY);
    // Input:
    //      std::vector<double> pointsX - vector of x-coordinates of points to which we wanted to fit line
    //      std::vector<double> pointsY - vector of y-coordinates of points to which we wanted to fit line
    // Output:
    //          int minDistanceIndex - index of element with the smallest distance from the line fitted to the group of points

    void setTinn(int max_hist, int max_bin, int bins, std::vector <double> borders, std::vector <double> binsCenters, std::vector <int> histogram);
    // This function prepares all the data needed to create TINN (triangular interpolation of NN intervals) and assigns it to the TinnParams structure
    // Input:
    //      int max_hist - the largest histogram value
    //      int max_bin - bin in which the highest value of the histogram is achieved
    //      std::vector <double> borders - borders of the bins
    //      std::vector <double> binCenters - centers of the bins
    //      std::vector <int> histogram - histogram calculated in the histogram section
    // Output: -
    // From structure we get vectors containing the coordinates of the lines connecting the vertices of the triangle:
    //      std::vector<double> xVectorMT - vector of x-coordinates of MT segment
    //      std::vector<double> yVectorMT - vector of y-coordinates of MT segment
    //      std::vector<double> xVectorMN - vector of x-coordinates of MN segment
    //      std::vector<double> yVectorMN - vector of y-coordinates of MN segment
    //      std::vector<double> xVectorTN - vector of x-coordinates of TN segment
    //      std::vector<double> yVectorTN - vector of y-coordinates of TN segment
    // and :
    //       double TinnLength - numerical value of the TINN (length of the base of the triangle)

    TinnParams getTinn();


    // Triangular index

    void setTriangularIndex();
    // This function set triangular index
    double getTriangularIndex();
    double triangularIndex;


    //Poincare

    void SetPoincareParams();
    // This function prepares and assigns data it to the PoincareParameters structure
    // From structure we get:
    //     double sd1 - sd1 parameter value
    //     double sd2 - sd2 parameter value
    PoincareParameters GetPoincareParameters();

    void setPoincareX();
    // This function set values of the vector:
    //         std::vector<int> poincareX - vector of the x-coordinates of the Poincare graph

    void setPoincareY();
    // This function set values of the vector:
    //          std::vector<int> poincareY vector of the y-coordinates of the Poincare graph

    std::vector<int> getPoincareX();
    std::vector<int> getPoincareY();


    void setIdentityLine();
    // This function set values of the vector:
    //          std::vector<int> identityLine - vector of values of identity of the Poincare graph

    std::vector<int> GetIdentityLine();

    void setEllipse();
    // This function prepares all the data needed to create Ellipse and assigns it to the EllipseParameters structure
    // From structure we get:
    //         std::vector<int> valuesSD1 - vector of y-coordinates of SD1 segment
    //         std::vector<int> valuesSD2 - vector of y-coordinates of SD2 segment
    //         std::vector<int> indicesSD1 - vector of x-coordinates of SD1 segment
    //         std::vector<int> indicesSD2 - vector of x-coordinates of SD2 segment

    EllipseParameters GetEllipse();



};

#endif //ECG_ANALYZER_HRV2_H
