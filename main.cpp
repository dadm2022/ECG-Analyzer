#include <QApplication>
#include "UI/ViewModelUI.h"
#include <iostream>

//#include "./DataAccessor/DataAccessor.h"



//int main()
int main(int argc, char *argv[])
{
//    std::cout << "Hello world" < <std:: endl;
    QApplication a(argc, argv);
    auto window = new MainWindow();
    window->show();
    return a.exec();
//    return 0;

//    auto da = DataAccessor();
//    da.load("/Users/marcelpikula/Desktop/Data/233.dat");
//    auto a = da.signalCountGet();
//
//    auto vec = da.at(0);
}
