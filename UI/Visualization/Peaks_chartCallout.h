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
    PeaksChartCallout(std::vector<float> &filteredSignal, std::vector<int> &detectedRPeaks);

private:
    QChart *chart;
    QLineSeries *series{};
    QScatterSeries *rPeaks{};

    QValueAxis *defXaxis;
    QValueAxis *defYaxis;
    QValueAxis *defXTimeAxis;

    std::vector <float> filteredSignal;
    std::vector <int> detectedRPeaks;


    void createSeries();

    void addDataToQSeries(int startIndex);

    void addSeriesToChart();

};

#endif //ECG_ANALYZER_PEAKS_CHARTCALLOUT_H
