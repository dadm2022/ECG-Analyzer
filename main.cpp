#include <QApplication>
#include "UI/ViewModelUI.h"
#include "DataController/DataController.h"

#include "./DataAccessor/DataAccessor.h"

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    auto window = MainWindow();
//    window.show();
//    return a.exec();

//    auto da = DataAccessor();
//    da.load("/Users/marcelpikula/Desktop/Data/233.dat");
//    auto a = da.signalCountGet();
//
//    auto vec = da.at(0);

    auto dataController = DataController("/Users/bartosz/downloads/100.dat");
    auto filtered = dataController.getButterworthFilteredSignal();
    auto filtered1 = dataController.getLMSFilteredSignal();
    auto filtered2 = dataController.getMovingAverageFilteredSignal();

//    auto rpks = dataController.getHilbertRPeaks(std::make_shared<std::vector<float>>(filtered));
    auto rpks1 = dataController.getPanTompkinsRPeaks(std::make_shared<std::vector<float>>(filtered));

    auto waves = dataController.getWaves(filtered, rpks1);

    auto wavespointer = std::make_shared<const std::vector<int>>(waves.Tend);
    auto filteredpointer = std::make_shared<const std::vector<float>>(filtered);

    auto alternans = dataController.getAlternans(wavespointer, filteredpointer);

    auto alpha1 = dataController.getAlpha1(std::make_shared<std::vector<int>>(rpks1));
    auto alpha2 = dataController.getAlpha2(std::make_shared<std::vector<int>>(rpks1));

    auto tinnTriangleCorners = dataController.getTinnTriangleCorners(std::make_shared<std::vector<int>>(rpks1));
    auto hrv2Histogram = dataController.getHRV2Histogram(std::make_shared<std::vector<int>>(rpks1));

    auto heartActivationsClassification = dataController.getHeartActivationClassification(rpks1, waves.Ponset,waves.QRSend, waves.QRSonset);

    auto stSegment = dataController.getSTSegment(waves.QRSend, rpks1, filtered, waves.QRSonset, waves.Tend);
}
