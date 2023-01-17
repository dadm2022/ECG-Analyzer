//
// Created by Weronika on 16.01.2023.
//

#include "HRV2_chartCallout.h"
#include "../../Modules/HRV2/HRV2.h"

//HRV2HistogramCallout(std::shared_ptr<HRV2>histogram);
//HRV2HistogramCallout(std::vector<int> HRV2::peakDistances,
//                     std::vector<int> HRV2::poincareX,
//                     std::vector<int> HRV2::poincareY,
//                     std::vector<int> HRV2::identityLine,
//                     std::vector<int> HistogramData::histogram,
//                     std::vector<double> HistogramData::binBorders,
//                     std::vector<int> HistogramData::maxBinSize,
//                     std::vector<int> HistogramData::maxBinNumber,
//                     std::vector<double> HRV2::TinnParams::xVectorMT,
//                     std::vector<double> HRV2::TinnParams::yVectorMT,
//                     std::vector<double> HRV2::triangularIndex,
//                     std::vector<int> EllipseParameters::indicesSD1,
//                     std::vector<int> EllipseParameters::valuesSD1);
//HRV2HistogramCallout(std::vector<double> binsBorders, std::vector<int> histogram, int maxHistogramValueIndex,
//                     std::shared_ptr<HRV2> hrv2);
HRV2HistogramCallout::HRV2HistogramCallout(std::shared_ptr<HRV2>PoincareParameters,
                                            std::shared_ptr<HRV2>HistogramData,
                                            std::shared_ptr<HRV2>TinnParams,
                                            std::shared_ptr<HRV2>EllipseParameters){
    //auto histogramHRV2 = HistogramData::histogram;
    this->HistogramData->peakDistances;
    std::vector<int> histogram = histogramHRV2->histogram;
}
 //   auto abc = GetHistogram (bins);
//    int bins = 16;
//    hrv2.SetHistogram(bins);
//    HistogramData histogramData = hrv2.GetHistogram();
//    std::vector <int> histogram = histogramData.histogram;
//    std::vector<double> binBorders = histogramData.binBorders;
//    double interval = histogramData.interval;
//    int maxBinSize = histogramData.maxBinSize;
//    int maxBinNumber = histogramData.maxBinNumber;
//    std::vector<double> binCenters = hrv2.calcBinCenters(bins, interval, binBorders);
//
//    hrv2.setTinn(maxBinNumber, maxBinSize, bins, binBorders, binCenters, histogram);
//    TinnParams tinnParams = hrv2.getTinn();
//    std::vector<double> xVectorMT = tinnParams.xVectorMT;
//    std::vector<double> yVectorMT = tinnParams.yVectorMT;
//    double TinnLength = tinnParams.TinnLength;
//
//    hrv2.setTriangularIndex();
//    hrv2.getTriangularIndex();
//    double triangularIndex = hrv2.triangularIndex;
//
//    PoincareParameters poincareParameters = hrv2.GetPoincareParameters();
//    double sd1 = poincareParameters.sd1;
//    std::vector<int> poincareX = hrv2.getPoincareX();
//    std::vector<int> poincareY = hrv2.getPoincareY();
//    std::vector<int> identityLine = hrv2.GetIdentityLine();
//
//    EllipseParameters ellipseParameters = hrv2.GetEllipse();
//    std::vector<int> indicesSD1 = ellipseParameters.indicesSD1;
//    std::vector<int> valuesSD1 = ellipseParameters.valuesSD1;

