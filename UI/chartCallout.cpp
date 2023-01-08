//
// Created by DiLLu on 07.01.2023.
//

#include "chartCallout.h"
#include "../Modules/Sample/Sample.h"
#include <QtWidgets>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCore/QtMath>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <utility>


chartCallout::chartCallout(const std::vector<float> &filteredSignal, std::vector<int> &detectedRPeaks):
        QtCharts::QChartView() {

    this->filteredSignal = filteredSignal;
    this->detectedRPeaks = detectedRPeaks;

    createSeries();
    addDataToQSeries(0);

    chart = new QtCharts::QChart();
    chart->setAcceptHoverEvents(true);
    chart->legend()->hide();

    defXaxis = new QValueAxis();
    defYaxis = new QValueAxis();
    defXTimeAxis = new QValueAxis();
    defXaxis->setRange(0, 200);
    defYaxis->setRange(0, 2000);
    defXTimeAxis->setRange(0, double(3000) / 360);

    chart->addAxis(defXaxis, Qt::AlignBottom);
    chart->addAxis(defYaxis, Qt::AlignLeft);
    chart->addAxis(defXTimeAxis, Qt::AlignBottom);

    addSeriesToChart();

    defXaxis->hide();

    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(Qt::black);
    chart->setTitle("EKG");




    auto * chartview = new QChartView(chart);
    auto *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(chartview);
    chart->axes(Qt::Horizontal).back()->setTitleText("Czas [s]");
    chart->axes(Qt::Vertical).back()->setTitleText("Amplituda [mV]");

    setLayout(mainLayout);
    setRenderHint(QPainter::Antialiasing);

    chartview->setRenderHint(QPainter::Antialiasing);


}

void chartCallout::createSeries() {
    series = new QtCharts::QLineSeries();
//    rPeaks = new QtCharts::QScatterSeries();
//
//    rPeaks->setName("R peaks");
//    rPeaks->setMarkerShape(QScatterSeries::MarkerShapeCircle);
//    rPeaks->setMarkerSize(10.0);
//    rPeaks->setPointLabelsVisible(true);

    QPen pen(QRgb(0x000000));
    pen.setWidth(1);
    series->setPen(pen);
}


void chartCallout::addDataToQSeries(int startIndex) {
    float signalValue;
    for (int i = startIndex; i < startIndex + 3000; i++) {
        if (i >= filteredSignal.size()) {
            break;
        }
        signalValue = filteredSignal[i];
        series->append(i, signalValue);
    }
}

void chartCallout::addSeriesToChart() {


    chart->addSeries(series);
    series->attachAxis(defXaxis);
    series->attachAxis(defYaxis);

}