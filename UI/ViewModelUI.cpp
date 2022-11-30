//
// Created by Bartosz Łukasik on 16/11/2022.
//

#include "ViewModelUI.h"
#include <QPushButton>
#include "../Modules/Sample/Sample.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto sampleData = Sample();
    m_button = std::make_shared<QPushButton>(sampleData.getText().c_str(), this);
    m_button->resize(200, 100);
}
