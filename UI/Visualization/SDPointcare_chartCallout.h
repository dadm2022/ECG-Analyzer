//
// Created by Piotr on 18.01.2023.
//

#ifndef ECG_ANALYZER_SDPOINTCARE_CHARTCALLOUT_H
#define ECG_ANALYZER_SDPOINTCARE_CHARTCALLOUT_H
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

class SDParamsPoincareChartCallout : public QChartView {
Q_OBJECT
public:

    explicit SDParamsPoincareChartCallout(std::vector<int> &identityLine, std::vector<int> &indicesSD1,
                                          std::vector<int> &valuesSD1, std::vector<int> &indicesSD2, std::vector<int> &valuesSD2,
                                          std::vector<int> &PointcareX, std::vector<int> &PointcareY);

private:

    QChart *chart;
    QLineSeries *identityL{};
    QLineSeries *lineSD1{};
    QLineSeries *lineSD2{};
    QScatterSeries *Pointcare{};

    QValueAxis *defXaxis;
    QValueAxis *defYaxis;
    QValueAxis *defXTimeAxis{};
    QBarCategoryAxis *axisX{};


    std::vector<int> indicesSD1;
    std::vector<int> valuesSD1;
    std::vector<int> indicesSD2;
    std::vector<int> valuesSD2;
    std::vector<int> PointcareX;
    std::vector<int> PointcareY;
    std::vector<int> identityLine;

    void createSeries();

    void addDataQSeries();

    void addSeriesToChart();

};
#endif //ECG_ANALYZER_SDPOINTCARE_CHARTCALLOUT_H
