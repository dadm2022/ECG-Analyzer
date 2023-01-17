//
// Created by Weronika on 16.01.2023.
//

#ifndef ECG_ANALYZER_HRV2_CHARTCALLOUT_H
#define ECG_ANALYZER_HRV2_CHARTCALLOUT_H
#include <QWidget>
#include <QtCharts>
#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QScrollBar>
#include "../../Modules/HRV2/HRV2.h"
using namespace QtCharts;

class HRV2HistogramCallout : public QChartView {
    Q_OBJECT

public:
//   // explicit HRV2HistogramCallout(std::shared_ptr<HRV2>histogram);
//   explicit HRV2HistogramCallout(std::vector<int> HRV2::peakDistances,
//                                 std::vector<int> HRV2::poincareX,
//                                 std::vector<int> HRV2::poincareY,
//                                 std::vector<int> HRV2::identityLine,
//                                 std::vector<int> HistogramData::histogram,
//                                 std::vector<double> HistogramData::binBorders,
//                                 std::vector<int> HistogramData::maxBinSize,
//                                 std::vector<int> HistogramData::maxBinNumber,
//                                 std::vector<double> HRV2::TinnParams::xVectorMT,
//                                 std::vector<double> HRV2::TinnParams::yVectorMT,
//                                 std::vector<double> HRV2::triangularIndex,
//                                 std::vector<int> EllipseParameters::indicesSD1,
//                                 std::vector<int> EllipseParameters::valuesSD1);

//
//    explicit HRV2HistogramCallout(std::vector<double> binsBorders, std::vector<int> histogram, int maxHistogramValueIndex,
//                                  const std::shared_ptr<HRV2> hrv2Ptr,
//                                  );

    explicit HRV2HistogramCallout( std::shared_ptr<HRV2>PoincareParameters,
                                        std::shared_ptr<HRV2>HistogramData,
                                        std::shared_ptr<HRV2>TinnParams,
                                        std::shared_ptr<HRV2>EllipseParameters);

private :
    std::shared_ptr<HRV2> HistogramData;
//    QScrollBar *scrollbar;
//    QChart *chart;
//    QLineSeries *series;
//    QScatterSeries *rPeaks;
//    QValueAxis *defXaxis;
//    QValueAxis *defYaxis;
//    QValueAxis *defXTimeAxis;
//    QBarSeries *bins;


};
#endif //ECG_ANALYZER_HRV2_CHARTCALLOUT_H
