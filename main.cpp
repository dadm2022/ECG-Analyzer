#include <QApplication>
#include "UI/ViewModelUI.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto window = MainWindow();
    window.show();
    return a.exec();
}
