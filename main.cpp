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

}
