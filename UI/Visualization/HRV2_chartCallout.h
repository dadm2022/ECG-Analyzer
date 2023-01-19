//
// Created by Weronika Wójcik and Piotr Pietranis on 16.01.2023.
//

#ifndef ECG_ANALYZER_HRV2_CHARTCALLOUT_H
#define ECG_ANALYZER_HRV2_CHARTCALLOUT_H
#include <QWidget>
#include <QPainter>
#include <QRectF>
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


    explicit HRV2HistogramCallout(int &bins, std::vector<float> &highBins, std::vector<float> &binsCenter,
                                  float &binsWidth, int&xM, int&yM, int &xT, int &yT, int &xN, int &yN);

private :

    QChart *chart;
    QLineSeries *lineseries;
    QLineSeries *linehistogram;
    QValueAxis *defXaxis;
    QValueAxis *defYaxis;
    QValueAxis *defXTimeAxis;
    QScatterSeries *ParamM;
    QScatterSeries *ParamT;
    QScatterSeries *ParamN;

    int bins;
    std::vector<float> highBins;
    std::vector<float> binsCenter;
    float binsWidth;
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