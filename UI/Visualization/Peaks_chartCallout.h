//
// Created by Piotr on 16.01.2023.
//

#ifndef ECG_ANALYZER_PEAKS_CHARTCALLOUT_H
#define ECG_ANALYZER_PEAKS_CHARTCALLOUT_H

#include <QWidget>
#include <QtCharts>
#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QScrollBar>

using namespace QtCharts;

class PeaksChartCallout : public QChartView {
Q_OBJECT

public:
    PeaksChartCallout(std::vector<float> &filteredSignal, std::vector<int> &detectedRPeaks,
                      std::vector<int> &detectedMqrsOnesetPoints,
                      std::vector<int> &detectedMqrsEndPoints, std::vector<int> &detectedMTPoints,
                      std::vector<int> &detectedMTEndPoints, std::vector<int> &detectedPonesetPoints,
                      std::vector<int> &detectedPendPoints, std::vector<int> &detectedPpointPoints);

private:
    QChart *chart;
    QLineSeries *series{};
    QScatterSeries *rPeaks{};
    QScatterSeries *mQrsOnesetPoints{};
    QScatterSeries *mQrsEndPoints{};
    QScatterSeries *mTPoints{};
    QScatterSeries *mTEndPoints{};
    QScatterSeries *mPOnesetPoints{};
    QScatterSeries *mPEndPoints{};
    QScatterSeries *mPpointPoints{};
    QScatterSeries *mPendPoints2{};
    QValueAxis *defXaxis;
    QValueAxis *defYaxis;
    QValueAxis *defXTimeAxis;

    std::vector <float> filteredSignal;
    std::vector <int> detectedRPeaks;
    std::vector <int> detectedMqrsOnesetPoints;
    std::vector <int> detectedMqrsEndPoints;
    std::vector <int> detectedMTPoints;
    std::vector <int> detectedMTEndPoints;
    std::vector <int> detectedPonesetPoints;
    std::vector <int> detectedPendPoints;
    std::vector <int> detectedPpointPoints;

    void createSeries();

    void addDataToQSeries(int startIndex);

    void addSeriesToChart();

};

#endif //ECG_ANALYZER_PEAKS_CHARTCALLOUT_H
