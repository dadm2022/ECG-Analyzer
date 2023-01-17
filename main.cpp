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

}
