//
// Created by Piotr on 18.01.2023.
//

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include "SignalUtils.h"
#include<QLineF>
#include "../../Modules/HRV2/HRV2.h"
#include "SDPointcare_chartCallout.h"

SDParamsPoincareChartCallout::SDParamsPoincareChartCallout(std::vector<int> &identityLine, std::vector<int> &indicesSD1,
                                                           std::vector<int> &valuesSD1, std::vector<int> &indicesSD2, std::vector<int> &valuesSD2,
                                                           std::vector<int> &PointcareX, std::vector<int> &PointcareY):
        QtCharts::QChartView() {

    this->identityLine = identityLine;
    this->indicesSD1 = indicesSD1;
    this->valuesSD1 = valuesSD1;
    this->indicesSD2 = indicesSD2;
    this->valuesSD2 = valuesSD2;
    this->PointcareX = PointcareX;
    this->PointcareY = PointcareY;

    createSeries();
    addDataQSeries();
    chart = new QtCharts::QChart();
    chart->setAcceptHoverEvents(true);

//    auto [fromX, toX] = getRangeFromTo();
//    auto [fromY, toY] = getRangeFromTo();

    defXaxis = new QValueAxis();
    defYaxis = new QValueAxis();
//    defXaxis->setRange(0, toX + 50);
//    defYaxis->setRange(0, toY + 50);

    chart->addAxis(defXaxis, Qt::AlignBottom);
    chart->addAxis(defYaxis, Qt::AlignLeft);

    addSeriesToChart();

    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(Qt::black);
    chart->setTitle("Pointcare i Parametry SD");

    auto *chartView = new QChartView(chart);
    auto *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(chartView);
//    chart->axes(Qt::Horizontal).back()->setTitleText("Wartości przedziałów [ms]");
//    chart->axes(Qt::Vertical).back()->setTitleText("Liczba zliczeń");
    setLayout(mainLayout);
    setRenderHint(QPainter::Antialiasing);

    chartView->setRenderHint(QPainter::Antialiasing);

}

void SDParamsPoincareChartCallout::createSeries(){

    identityL = new QtCharts::QLineSeries;
    lineSD1 = new QtCharts::QLineSeries;
    lineSD2 = new QtCharts::QLineSeries;
    Pointcare = new QtCharts::QScatterSeries();

    Pointcare->setName("Pointcare");
    Pointcare->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    Pointcare->setMarkerSize(5);
    Pointcare->setColor(QRgb(0xFF0000));



}

void SDParamsPoincareChartCallout::addDataQSeries(){

    for(int i = 0; i < identityLine.size(); i++){

        identityL->append(identityLine[i], identityLine[i]);

    }

    for(int i = 0; i < indicesSD1.size(); i++){

        lineSD1->append(indicesSD1[i], valuesSD1[i]);
    }

    for(int i = 0; i < indicesSD2.size(); i++){

        lineSD2->append(indicesSD2[i], valuesSD2[i]);
    }

    for(int i = 0; i < PointcareX.size(); i++){

        Pointcare->append(PointcareX[i], PointcareY[i]);
    }

}

void SDParamsPoincareChartCallout::addSeriesToChart(){

    chart->addSeries(identityL);
    chart->addSeries(lineSD1);
    chart->addSeries(lineSD2);
    chart->addSeries(Pointcare);

    identityL->attachAxis(defXaxis);
    identityL->attachAxis(defYaxis);

    lineSD1->attachAxis(defXaxis);
    lineSD1->attachAxis(defYaxis);

    lineSD2->attachAxis(defXaxis);
    lineSD2->attachAxis(defYaxis);

    Pointcare->attachAxis(defXaxis);
    Pointcare->attachAxis(defYaxis);

}