//
// Created by Piotr on 16.01.2023.
//

#ifndef ECG_ANALYZER_ECG_CHARTCALLOUT_H
#define ECG_ANALYZER_ECG_CHARTCALLOUT_H

#include <QWidget>
#include <QtCharts>
#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QScrollBar>

using namespace QtCharts;

class ECGSignal : public QChartView {
Q_OBJECT

public:
    explicit ECGSignal(std::vector<float> &FilteredECGSignal);

private:
    QChart *chart;
    QLineSeries *series{};
    QValueAxis *defXaxis;
    QValueAxis *defYaxis;

    std::vector <float> FilteredECGSignal;

    void createSeries();

    void addDataToQSeries(int startIndex);

    void addSeriesToChart();


};

class LMSFilteredSignal : public ECGSignal {
Q_OBJECT

public:
    explicit LMSFilteredSignal(std::vector<float> &LMSSignal);
private:
    std::vector<float> LMSSignal;
};

class ButterworthFilteredSignal : public ECGSignal {
Q_OBJECT

public:
    explicit ButterworthFilteredSignal(std::vector<float> &ButterworthSignal);
private:
    std::vector<float> ButterworthSignal;
};

class MovingAverageFilteredSignal : public ECGSignal {
Q_OBJECT

public:
    explicit MovingAverageFilteredSignal(std::vector<float> &MovingAverageSignal);
private:
    std::vector<float> MovingAverageSignal;
};


#endif //ECG_ANALYZER_ECG_CHARTCALLOUT_H
