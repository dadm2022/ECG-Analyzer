#include <QWidget>
#include <QtCharts>
#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QScrollBar>
//#include "../Modules/Sample/Sample.h"


using namespace QtCharts;

class chartCallout : public QChartView {
Q_OBJECT
public:

    explicit chartCallout(const std::vector<float> &filteredSignal, std::vector<int> &detectedRPeaks);


private:

    // QScrollBar *scrollbar;
    QChart *chart;
    QLineSeries *series{};
    // QScatterSeries *rPeaks;
    QValueAxis *defXaxis;
    QValueAxis *defYaxis;
    QValueAxis *defXTimeAxis;

    std::vector <float> filteredSignal;
    std::vector <int> detectedRPeaks;

    void createSeries();

    void addDataToQSeries(int startIndex);

    void addSeriesToChart();



};