//
// Created by Weronika Wójcik and Piotr Pietranis on 16.01.2023.
//

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include "HRV2_chartCallout.h"
#include "SignalUtils.h"
#include<QLineF>

#include "../../Modules/HRV2/HRV2.h"


using SignalUtils::getRangeFromTo;

HRV2HistogramCallout::HRV2HistogramCallout(int &bins, std::vector<float> &highBins, std::vector<float> &binsCenter,
                                           float &binsWidth, int&xM, int&yM, int &xT, int &yT, int &xN, int &yN):
        QtCharts::QChartView(){

        this->bins = bins;
        this->highBins = highBins;
        this->binsCenter = binsCenter;
        this->binsWidth = binsWidth;
        this->xT = xT;
        this->yT = yT;
        this->xM = xM;
        this->yM = yM;
        this->xN = xN;
        this->yN = yN;


    createSeries();
    addDataToBarQSeries(0);
    chart = new QtCharts::QChart();
    chart->setAcceptHoverEvents(true);

    auto [fromX, toX] = getRangeFromTo(binsCenter);
    auto [fromY, toY] = getRangeFromTo(highBins);

    defXaxis = new QValueAxis();
    defYaxis = new QValueAxis();
    defXaxis->setRange(0, toX + 50);
    defYaxis->setRange(0, toY + 50);

    chart->addAxis(defXaxis, Qt::AlignBottom);
    chart->addAxis(defYaxis, Qt::AlignLeft);

    addSeriesToChart();

    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(Qt::black);
    chart->setTitle("Histogram interwałów RR i indeks trójkątny");

    auto *chartView = new QChartView(chart);
    auto *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(chartView);
    chart->axes(Qt::Horizontal).back()->setTitleText("Wartości przedziałów [ms]");
    chart->axes(Qt::Vertical).back()->setTitleText("Liczba zliczeń");
    setLayout(mainLayout);
    setRenderHint(QPainter::Antialiasing);

    chartView->setRenderHint(QPainter::Antialiasing);

}


void HRV2HistogramCallout::createSeries() {

    lineseries = new QtCharts::QLineSeries;
    linehistogram = new QtCharts::QLineSeries;


    ParamM = new QtCharts::QScatterSeries();
    ParamT = new QtCharts::QScatterSeries();
    ParamN = new QtCharts::QScatterSeries();

    ParamM->setName("M");
    ParamM->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    ParamM->setMarkerSize(10);
    ParamM->setColor(QRgb(0xFF0000));

    ParamT->setName("T");
    ParamT->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    ParamT->setMarkerSize(10);
    ParamT->setColor(QRgb(0xBD2EBD));

    ParamN->setName("N");
    ParamN->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    ParamN->setMarkerSize(10);
    ParamN->setColor(QRgb(0x9900FF));


    QPen pen(QRgb(0x0000FF));
    pen.setWidth(1);
    lineseries->setPen(pen);

}

void HRV2HistogramCallout::addDataToBarQSeries(int startIndex) {


    for(int i = 0; i < bins; i++){

        double Ax = (binsCenter[i] - 0.5*binsWidth);
        double Bx = Ax;
        double By = highBins[i];
        double Cx = (binsCenter[i] + 0.5*binsWidth);
        double Cy = By;
        double Dx = Cx;
        linehistogram->append(Ax, 0);
        linehistogram->append(Bx, By);
        linehistogram->append(Cx, Cy);
        linehistogram->append(Dx, 0);

    }

    lineseries->append(xM,yM);
    lineseries->append(xN,yN);
    lineseries->append(xT, yT);
    lineseries->append(xM, yM);

    ParamM->append(xM, yM);
    ParamT->append(xT, yT);
    ParamN->append(xN, yN);

}

void HRV2HistogramCallout::addSeriesToChart() {

//    chart->addSeries(barseries);
    chart->addSeries(linehistogram);
    chart->addSeries(lineseries);

    chart->addSeries(ParamM);
    chart->addSeries(ParamT);
    chart->addSeries(ParamN);

    linehistogram->attachAxis(defXaxis);
    linehistogram->attachAxis(defYaxis);

    lineseries->attachAxis(defXaxis);
    lineseries->attachAxis(defYaxis);

    ParamM->attachAxis(defXaxis);
    ParamM->attachAxis(defYaxis);

    ParamT->attachAxis(defXaxis);
    ParamT->attachAxis(defYaxis);

    ParamN->attachAxis(defXaxis);
    ParamN->attachAxis(defYaxis);

}
