//
// Created by Weronika on 16.01.2023.
//

#ifndef ECG_ANALYZER_HRV2_CHARTCALLOUT_H
#define ECG_ANALYZER_HRV2_CHARTCALLOUT_H
#include <QWidget>
#include <QtCharts>
#include <QDialog>
#include <QLineF>
#include <QFileDialog>
#include <QString>
#include <QScrollBar>
#include "../../Modules/HRV2/HRV2.h"
using namespace QtCharts;

class HRV2HistogramCallout : public QChartView {
    Q_OBJECT

public:
//   // explicit HRV2HistogramCallout(std::shared_ptr<HRV2>histogram);
//   explicit HRV2HistogramCallout(std::vector<int> &peakDistances,
//                                 std::vector<int> &poincareX,
//                                 std::vector<int> &poincareY,
//                                 std::vector<int> &identityLine,
//                                 std::vector<int> &histogram,
//                                 std::vector<double> &binBorders,
//                                 std::vector<int> &maxBinSize,
//                                 std::vector<int> &maxBinNumber,
//                                 std::vector<double> &xVectorMT,
//                                 std::vector<double> &yVectorMT,
//                                 std::vector<double> &triangularIndex,
//                                 std::vector<int> &indicesSD1,
//                                 std::vector<int> &valuesSD1);

//
//    explicit HRV2HistogramCallout(std::vector<double> binsBorders, std::vector<int> histogram, int maxHistogramValueIndex,
//                                  const std::shared_ptr<HRV2> hrv2Ptr,
//                                  );

//    std::vector<int> &peakDistances,
//    std::vector<int> &poincareX,
//    std::vector<int> &poincareY,
//    std::vector<int> &identityLine,
//    std::vector<int> &histogram,
//    std::vector<double> &binBorders,
//    std::vector<int> &maxBinSize,
//    std::vector<int> &maxBinNumber,
//    std::vector<double> &xVectorMT,
//    std::vector<double> &yVectorMT,
//    std::vector<double> &xVectorMN,
//    std::vector<double> &yVectorMN,
//    std::vector<double> &xVectorTN,
//    std::vector<double> &yVectorTN,
//    std::vector<double> &triangularIndex,
//    std::vector<int> &indicesSD1,
//    std::vector<int> &valuesSD1

    explicit HRV2HistogramCallout(std::vector<int> &bins, std::vector<int> &dupa,
                                  int &xT, int &yT, int&xM, int&yM, int &xN, int &yN);

private :

    //std::shared_ptr<HRV2> HistogramData;
    QScrollBar *scrollbar;
    QChart *chart;
    QBarSeries *barseries;
    QLineSeries *lineseries;
    QLineSeries *lineseriesMT;
    QLineSeries *lineseriesTN;
    QLineSeries *lineseriesMN;
    QValueAxis *defXaxis;
    QValueAxis *defYaxis;
    QValueAxis *defXTimeAxis;
    QBarSet *set;
    QBarCategoryAxis *axisX;
    QScatterSeries *ParamM;
    QScatterSeries *ParamT;
    QScatterSeries *ParamN;

    std::vector<int> peakDistances;
    std::vector<int> poincareX;
    std::vector<int> poincareY;
    std::vector<int> identityLine;
    std::vector<int> histogram;
    std::vector<double> binBorders;
    std::vector<int> maxBinSize;
    std::vector<int> maxBinNumber;
    std::vector<double> xVectorMT;
    std::vector<double> yVectorMT;
    std::vector<double> xVectorMN;
    std::vector<double> yVectorMN;
    std::vector<double> xVectorTN;
    std::vector<double> yVectorTN;
    std::vector<double> triangularIndex;
    std::vector<int> indicesSD1;
    std::vector<int> valuesSD1;
    std::vector<int> bins;
    std::vector<int> dupa;
    int xT;
    int yT;
    int xM;
    int yM;
    int xN;
    int yN;

    void createSeries();

    void addDataToBarQSeries(int startIndex);

    void addSeriesToChart();




};
#endif //ECG_ANALYZER_HRV2_CHARTCALLOUT_H
