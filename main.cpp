#include <QApplication>
#include "UI/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto window = MainWindow();
    window.show();
    return a.exec();
}
