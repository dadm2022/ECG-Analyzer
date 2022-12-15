#include <QApplication>
#include "UI/ViewModelUI.h"

#include "./DataAccessor/DataAccessor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto window = MainWindow();
    window.show();
    return a.exec();

//    auto da = DataAccessor();
//    da.load("/Users/marcelpikula/Desktop/Data/233.dat");
//    auto a = da.signalCountGet();
//
//    auto vec = da.at(0);
}
