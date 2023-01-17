//
// Created by Weronika on 16.01.2023.
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
#include<QLineF>
#include "../../Modules/HRV2/HRV2.h"

//HRV2HistogramCallout(std::shared_ptr<HRV2>histogram);
//HRV2HistogramCallout(std::vector<int> HRV2::peakDistances,
//                     std::vector<int> HRV2::poincareX,
//                     std::vector<int> HRV2::poincareY,
//                     std::vector<int> HRV2::identityLine,
//                     std::vector<int> HistogramData::histogram,
//                     std::vector<double> HistogramData::binBorders,
//                     std::vector<int> HistogramData::maxBinSize,
//                     std::vector<int> HistogramData::maxBinNumber,
//                     std::vector<double> HRV2::TinnParams::xVectorMT,
//                     std::vector<double> HRV2::TinnParams::yVectorMT,
//                     std::vector<double> HRV2::triangularIndex,
//                     std::vector<int> EllipseParameters::indicesSD1,
//                     std::vector<int> EllipseParameters::valuesSD1);
//HRV2HistogramCallout(std::vector<double> binsBorders, std::vector<int> histogram, int maxHistogramValueIndex,
//                     std::shared_ptr<HRV2> hrv2);
HRV2HistogramCallout::HRV2HistogramCallout(std::vector<int> &bins, std::vector<int> &dupa,
                                           int&xM, int&yM, int &xT, int &yT,  int &xN, int &yN):
        QtCharts::QChartView(){

        this->dupa = dupa;
        this->bins = bins;
        this->xT = xT;
        this->yT = yT;
        this->xM = xM;
        this->yM = yM;
        this->xN = xN;
        this->yN = yN;

//    this->peakDistances = peakDistances;
//    this->poincareX = poincareX;
//    this->poincareY = poincareY;
//    this->identityLine = identityLine;
//    this->histogram = histogram;
//    this->binBorders = binBorders;
//    this->maxBinSize = maxBinSize;
//    this->maxBinNumber = maxBinNumber;
//    this->xVectorMT = xVectorMT;
//    this->yVectorMT = yVectorMT;
//    this->xVectorMT = xVectorMN;
//    this->yVectorMT = yVectorMN;
//    this->xVectorMT = xVectorTN;
//    this->yVectorMT = yVectorTN;
//    this->triangularIndex = triangularIndex;
//    this->indicesSD1 = indicesSD1;
//    this->valuesSD1 = valuesSD1;

    createSeries();
    addDataToBarQSeries(0);
    chart = new QtCharts::QChart();
    chart->setAcceptHoverEvents(true);

    defXaxis = new QValueAxis();
    defYaxis = new QValueAxis();
    defXaxis->setRange(0, 30);
    defYaxis->setRange(0, 30);

    chart->addAxis(defXaxis, Qt::AlignBottom);
    chart->addAxis(defYaxis, Qt::AlignLeft);

    addSeriesToChart();

    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(Qt::black);
    chart->setTitle("histogram");

    auto *chartView = new QChartView(chart);
    auto *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(chartView);
    setLayout(mainLayout);
    setRenderHint(QPainter::Antialiasing);

    chartView->setRenderHint(QPainter::Antialiasing);

}


void HRV2HistogramCallout::createSeries() {

    lineseries = new QtCharts::QLineSeries;

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


    barseries = new QtCharts::QBarSeries;
    set = new QtCharts::QBarSet("BINS");
    barseries->setName("dane bar");

    QPen pen(QRgb(0x0000FF));
    pen.setWidth(1);
    lineseries->setPen(pen);

}

void HRV2HistogramCallout::addDataToBarQSeries(int startIndex) {


    int signalValue;

//    for (int i = startIndex; xM - xT; i++) {
//
//        signalValue = dupa[i];
//        lineseries->append(i, signalValue);
//    }
    lineseries->append(xM,yM);
    lineseries->append(xN,yN);
    lineseries->append(xT, yT);
    lineseries->append(xM, yM);

    ParamM->append(xM, yM);
    ParamT->append(xT, yT);
    ParamN->append(xN, yN);


    int barValue;

    for (int i = startIndex; i < startIndex + 25; i++) {
        if (i >= bins.size()) {
            break;
        }
        barValue = bins[i];
        set->append(barValue);
    }
    barseries->append(set);

}

void HRV2HistogramCallout::addSeriesToChart() {

    chart->addSeries(barseries);
    chart->addSeries(lineseries);

    chart->addSeries(ParamM);
    chart->addSeries(ParamT);
    chart->addSeries(ParamN);


    chart->setTitle("Line and barchart example");


    lineseries->attachAxis(defXaxis);
    lineseries->attachAxis(defYaxis);

    ParamM->attachAxis(defXaxis);
    ParamM->attachAxis(defYaxis);

    ParamT->attachAxis(defXaxis);
    ParamT->attachAxis(defYaxis);

    ParamN->attachAxis(defXaxis);
    ParamN->attachAxis(defYaxis);

    QStringList label;
    label << "X";
    axisX = new QBarCategoryAxis();
    axisX->append(label);
    barseries->attachAxis(axisX);
}



//    auto histogramHRV2 = HistogramData::histogram;
//    HistogramData::histogram
//
//    this->peakDistances =
//
//    this->HistogramData->peakDistances;
//    std::vector<int> histogram = histogramHRV2->histogram;

 //   auto abc = GetHistogram (bins);
//    int bins = 16;
//    hrv2.SetHistogram(bins);
//    HistogramData histogramData = hrv2.GetHistogram();
//    std::vector <int> histogram = histogramData.histogram;
//    std::vector<double> binBorders = histogramData.binBorders;
//    double interval = histogramData.interval;
//    int maxBinSize = histogramData.maxBinSize;
//    int maxBinNumber = histogramData.maxBinNumber;
//    std::vector<double> binCenters = hrv2.calcBinCenters(bins, interval, binBorders);
//
//    hrv2.setTinn(maxBinNumber, maxBinSize, bins, binBorders, binCenters, histogram);
//    TinnParams tinnParams = hrv2.getTinn();
//    std::vector<double> xVectorMT = tinnParams.xVectorMT;
//    std::vector<double> yVectorMT = tinnParams.yVectorMT;
//    double TinnLength = tinnParams.TinnLength;
//
//    hrv2.setTriangularIndex();
//    hrv2.getTriangularIndex();
//    double triangularIndex = hrv2.triangularIndex;
//
//    PoincareParameters poincareParameters = hrv2.GetPoincareParameters();
//    double sd1 = poincareParameters.sd1;
//    std::vector<int> poincareX = hrv2.getPoincareX();
//    std::vector<int> poincareY = hrv2.getPoincareY();
//    std::vector<int> identityLine = hrv2.GetIdentityLine();
//
//    EllipseParameters ellipseParameters = hrv2.GetEllipse();
//    std::vector<int> indicesSD1 = ellipseParameters.indicesSD1;
//    std::vector<int> valuesSD1 = ellipseParameters.valuesSD1;

