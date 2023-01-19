//
// Created by Piotr on 16.01.2023.
//

#include <QtWidgets>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCore/QtMath>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include "ECG_chartCallout.h"

ECGSignal::ECGSignal(std::vector<float> &FilteredECGSignal):
        QtCharts::QChartView(){

    this->FilteredECGSignal  = FilteredECGSignal;
    createSeries();
    addDataToQSeries(0);

    chart = new QtCharts::QChart();
    chart->setAcceptHoverEvents(true);
//    chart->legend()->hide();

    defXaxis = new QValueAxis();
    defYaxis = new QValueAxis();
    defXaxis->setRange(0, 70);
    defYaxis->setRange(-10, 10);

    chart->addAxis(defXaxis, Qt::AlignBottom);
    chart->addAxis(defYaxis, Qt::AlignLeft);

    addSeriesToChart();

    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(Qt::black);
    chart->setTitle("Sygnał EKG po filtracji");

    auto *chartView = new QChartView(chart);
    auto *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(chartView);
    chart->axes(Qt::Horizontal).back()->setTitleText("Ilość próbek sygnału");
    chart->axes(Qt::Vertical).back()->setTitleText("Amplituda [mV]");

    setLayout(mainLayout);
    setRenderHint(QPainter::Antialiasing);

    chartView->setRenderHint(QPainter::Antialiasing);

}

void ECGSignal::createSeries(){

    series = new QtCharts::QLineSeries;
    series->setName("dane EKG");

    QPen pen(QRgb(0x0000FF));
    pen.setWidth(1);
    series->setPen(pen);
}

void ECGSignal::addDataToQSeries(int startIndex) {

    float signalValue;
    for (int i = startIndex; i < startIndex + 7000; i++) {
        if (i >= FilteredECGSignal.size()) {
            break;
        }
        signalValue = FilteredECGSignal[i];
        series->append(i, signalValue);
    }

}

void ECGSignal::addSeriesToChart(){

    chart->addSeries(series);
    series->attachAxis(defXaxis);
    series->attachAxis(defYaxis);
}

LMSFilteredSignal::LMSFilteredSignal(std::vector<float> &LMSSignal) : ECGSignal(LMSSignal) {

}

ButterworthFilteredSignal::ButterworthFilteredSignal(std::vector<float> &LMSSignal) : ECGSignal(LMSSignal) {

}

MovingAverageFilteredSignal::MovingAverageFilteredSignal(std::vector<float> &LMSSignal) : ECGSignal(LMSSignal) {

}
