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
#include "SignalUtils.h"

using SignalUtils::getRangeFromTo;

PeaksChartCallout::PeaksChartCallout(std::vector<float> &filteredSignal, std::vector<int> &detectedRPeaks,
                                     std::vector<int> &detectedMqrsOnesetPoints,
                                     std::vector<int> &detectedMqrsEndPoints, std::vector<int> &detectedMTPoints,
                                     std::vector<int> &detectedMTEndPoints,std::vector<int> &detectedPonesetPoints,
                                     std::vector<int> &detectedPendPoints, std::vector<int> &detectedPpointPoints):
        QtCharts::QChartView(){


    this->filteredSignal  = filteredSignal;
    this->detectedRPeaks = detectedRPeaks;
    this->detectedMqrsOnesetPoints = detectedMqrsOnesetPoints;
    this->detectedMqrsEndPoints = detectedMqrsEndPoints;
    this->detectedMTPoints = detectedMTPoints;
    this->detectedMTEndPoints = detectedMTEndPoints;
    this->detectedPonesetPoints = detectedPonesetPoints;
    this->detectedPendPoints = detectedPendPoints;
    this->detectedPpointPoints = detectedPpointPoints;

    auto [from, to] = getRangeFromTo(filteredSignal);
    //auto maxXElement = sizeof(&filteredSignal) / sizeof(int);

    createSeries();
    addDataToQSeries(0);
    chart = new QtCharts::QChart();
    chart->setAcceptHoverEvents(true);
//    chart->legend()->hide();
    defXaxis = new QValueAxis();
    defYaxis = new QValueAxis();
    defXTimeAxis = new QValueAxis();
    defXaxis->setRange(0, 10000);
    defYaxis->setRange(from, to);
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
    mQrsOnesetPoints = new QtCharts::QScatterSeries();
    mQrsEndPoints = new QtCharts::QScatterSeries();
    mTPoints = new QtCharts::QScatterSeries();
    mTEndPoints = new QtCharts::QScatterSeries();
    mPOnesetPoints = new QtCharts::QScatterSeries();
    mPEndPoints = new QtCharts::QScatterSeries();
    mPpointPoints = new QtCharts::QScatterSeries();

    series->setName("dane EKG");
    rPeaks->setName("R peaks");

    rPeaks->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    rPeaks->setMarkerSize(10.0);
    rPeaks->setColor(QRgb(0xFF0000));

    mQrsOnesetPoints->setName("qrs Oneset Points");
    mQrsOnesetPoints->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    mQrsOnesetPoints->setMarkerSize(10.0);
    mQrsOnesetPoints->setColor(QRgb(0xBD2EBD));
//    rPeaks->setPointLabelsVisible(true);
    mQrsEndPoints->setName("qrs End Points");
    mQrsEndPoints->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    mQrsEndPoints->setMarkerSize(10.0);
    mQrsEndPoints->setColor(QRgb(0x9900FF));

    //mTPoints
    mTPoints->setName("T Points");
    mTPoints->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    mTPoints->setMarkerSize(10.0);
    mTPoints->setColor(QRgb(0xFF9900));
    //T End
    mTEndPoints->setName("T End Points");
    mTEndPoints->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    mTEndPoints->setMarkerSize(10.0);
    mTEndPoints->setColor(QRgb(0xFFFF00));
    //P oneset
    mPOnesetPoints->setName("P oneset Points");
    mPOnesetPoints->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    mPOnesetPoints->setMarkerSize(10.0);
    mPOnesetPoints->setColor(QRgb(0x60914A));
    // P End
    mPEndPoints->setName("P end Points");
    mPEndPoints->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    mPEndPoints->setMarkerSize(10.0);
    mPEndPoints->setColor(QRgb(0x19320C));
    // P point
    mPpointPoints->setName("P Points");
    mPpointPoints->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    mPpointPoints->setMarkerSize(10.0);
    mPpointPoints->setColor(QRgb(0x274E13));

    QPen pen(QRgb(0x0000FF));
    pen.setWidth(1);
    series->setPen(pen);
}

void PeaksChartCallout::addDataToQSeries(int startIndex) {
    float signalValue;
    for (int i = startIndex; i < startIndex + filteredSignal.size(); i++) {
        if (i >= filteredSignal.size()) {
            break;
        }
        signalValue = filteredSignal[i];
        series->append(i, signalValue);
    }


    for (int detectedRPeak : detectedRPeaks){

        rPeaks->append(detectedRPeak, filteredSignal[detectedRPeak]);

    }


    for (int detectedMqrsOnesetPoint : detectedMqrsOnesetPoints){
        mQrsOnesetPoints->append(detectedMqrsOnesetPoint, filteredSignal[detectedMqrsOnesetPoint]);
    }


    for (int detectedMqrsEndPoint : detectedMqrsEndPoints){
        mQrsEndPoints->append(detectedMqrsEndPoint, filteredSignal[detectedMqrsEndPoint]);
    }

    for (int detectedMTPoint : detectedMTPoints){
        mTPoints->append(detectedMTPoint, filteredSignal[detectedMTPoint]);
    }

    //T End

    for (int detectedMTEndPoint : detectedMTEndPoints){
        mTEndPoints->append(detectedMTEndPoint, filteredSignal[detectedMTEndPoint]);
    }

    //P oneset
    std::vector<int> &indexesPoneset = detectedPonesetPoints;
    for (int detectedPonesetPoint : detectedPonesetPoints) {
        mPOnesetPoints->append(detectedPonesetPoint, filteredSignal[detectedPonesetPoint]);
    }
    // P End

    for (int detectedPendPoint : detectedPendPoints) {
        mPEndPoints->append(detectedPendPoint, filteredSignal[detectedPendPoint]);
    }
    // P point

    for (int detectedPpointPoint : detectedPpointPoints) {
        mPpointPoints->append(detectedPpointPoint, filteredSignal[detectedPpointPoint]);
    }
}

void PeaksChartCallout::addSeriesToChart(){
    chart->addSeries(series);
    chart->addSeries(rPeaks);
    chart->addSeries(mQrsOnesetPoints);
    chart->addSeries(mQrsEndPoints);
    chart->addSeries(mTPoints);
    chart->addSeries(mTEndPoints);
    chart->addSeries(mPOnesetPoints);
    chart->addSeries(mPEndPoints);
    chart->addSeries(mPpointPoints);

    series->attachAxis(defXaxis);
    series->attachAxis(defYaxis);
    rPeaks->attachAxis(defXaxis);
    rPeaks->attachAxis(defYaxis);
    mQrsOnesetPoints->attachAxis(defXaxis);
    mQrsOnesetPoints->attachAxis(defYaxis);
    mQrsEndPoints->attachAxis(defXaxis);
    mQrsEndPoints->attachAxis(defYaxis);
    mTPoints->attachAxis(defXaxis);
    mTPoints->attachAxis(defYaxis);
    mTEndPoints->attachAxis(defXaxis);
    mTEndPoints->attachAxis(defYaxis);
    mPOnesetPoints->attachAxis(defXaxis);
    mPOnesetPoints->attachAxis(defYaxis);
    mPEndPoints->attachAxis(defXaxis);
    mPEndPoints->attachAxis(defYaxis);
    mPpointPoints->attachAxis(defXaxis);
    mPpointPoints->attachAxis(defYaxis);
}