//
// Created by Bartosz Łukasik on 16/11/2022.
//

#include "ViewModelUI.h"
#include <QPushButton>
#include "../Modules/Sample/Sample.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto sampleData = Sample(1, 2, 3);
    int numberOfRoots = sampleData.getNumberOfRoots();
    std::cout<<numberOfRoots;
    m_button = std::make_shared<QPushButton>("The number of roots was displayed in the console", this);
    m_button->resize(200, 100);
}
