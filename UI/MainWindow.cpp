//
// Created by Bartosz Łukasik on 16/11/2022.
//

#include "MainWindow.h"
#include <QPushButton>
//#include "../Modules/Sample/Sample.h"
#include "ViewmodelUI.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto Viewmodel = std::make_unique<ViewmodelUI>();

    unsigned int numberOfRoots = Viewmodel->getNumberOfRoots();
    std::cout<<numberOfRoots;

    m_button = std::make_shared<QPushButton>("The number of roots was displayed in the console", this);
    m_button->resize(400, 200);
}
