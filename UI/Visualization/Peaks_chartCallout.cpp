//
// Created by Piotr on 16.01.2023.
//

#include "Peaks_chartCallout.h"
#include <QtWidgets>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCore/QtMath>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>


PeaksChartCallout::PeaksChartCallout(std::vector<float> &filteredSignal, std::vector<int> &detectedRPeaks):
        QtCharts::QChartView(){


    this->filteredSignal  = filteredSignal;
    this->detectedRPeaks = detectedRPeaks;


    createSeries();
    addDataToQSeries(0);
    chart = new QtCharts::QChart();
    chart->setAcceptHoverEvents(true);
//    chart->legend()->hide();
    defXaxis = new QValueAxis();
    defYaxis = new QValueAxis();
    defXTimeAxis = new QValueAxis();
    defXaxis->setRange(0, 50000);
    defYaxis->setRange(-1, 1);
//    defXTimeAxis->setRange(0, double(7000) / 360);

    chart->addAxis(defXaxis, Qt::AlignBottom);
    chart->addAxis(defYaxis, Qt::AlignLeft);
//    chart->addAxis(defXTimeAxis, Qt::AlignBottom);

    addSeriesToChart();

//    defXaxis->hide();

    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(Qt::black);
    chart->setTitle("EKG");

    auto *chartView = new QChartView(chart);
    auto *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(chartView);
//    chart->axes(Qt::Horizontal).back()->setTitleText("Czas [s]");
    chart->axes(Qt::Horizontal).back()->setTitleText("Ilość próbek sygnału");
    chart->axes(Qt::Vertical).back()->setTitleText("Amplituda [mV]");

    setLayout(mainLayout);
    setRenderHint(QPainter::Antialiasing);

    chartView->setRenderHint(QPainter::Antialiasing);


}

void PeaksChartCallout::createSeries(){

    series = new QtCharts::QLineSeries;
    rPeaks = new QtCharts::QScatterSeries();

    series->setName("dane EKG");
    rPeaks->setName("R peaks");

    rPeaks->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    rPeaks->setMarkerSize(10.0);
    rPeaks->setColor(QRgb(0xFF0000));

//    rPeaks->setPointLabelsVisible(true);


    QPen pen(QRgb(0x0000FF));
    pen.setWidth(1);
    series->setPen(pen);
}

void PeaksChartCallout::addDataToQSeries(int startIndex) {
    float signalValue;
    for (int i = startIndex; i < startIndex + 7000; i++) {
        if (i >= filteredSignal.size()) {
            break;
        }
        signalValue = filteredSignal[i];
        series->append(i, signalValue);
    }


    for (int detectedRPeak : detectedRPeaks){

        rPeaks->append(detectedRPeak, filteredSignal[detectedRPeak]);

    }

}

void PeaksChartCallout::addSeriesToChart(){
    chart->addSeries(series);
    chart->addSeries(rPeaks);

    series->attachAxis(defXaxis);
    series->attachAxis(defYaxis);
    rPeaks->attachAxis(defXaxis);
    rPeaks->attachAxis(defYaxis);

}