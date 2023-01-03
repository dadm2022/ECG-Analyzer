//
// Created by Bartosz Łukasik on 16/11/2022.
//

#include "ViewModelUI.h"
#include <QPushButton>
#include "../Modules/Sample/Sample.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
//    auto sampleData = Sample();
//    m_button = std::make_shared<QPushButton>(sampleData.getText().c_str(), this);
//    m_button->setGeometry(500,20,50,20);


    QWidget *centralWidget = new QWidget(parent);
    QGridLayout *gridLayout = new QGridLayout;
    QHBoxLayout *horizontalMainWindowLayout = new QHBoxLayout;
    pushbuttonOpenFile = std::make_shared<QPushButton>();
//    QPushButton *pushbuttonOpenFile = new QPushButton(this);
    pushbuttonOpenFile->setText("& Open File");



    // TO DO - Menu Bar
//    QMenuBar *menubar = new QMenuBar();
//    QMenu *menuOptionFile = new QMenu("&File");
//    menuOptionFile->addMenu("Open");
//    menuOptionFile->addMenu("Close");
//
//    menubar->addMenu(menuOptionFile);

//    QHBoxLayout *layoutAge = new QHBoxLayout;
//    QHBoxLayout *layoutGender = new QHBoxLayout;

    QLabel *labelAge = new QLabel(this);
    labelAge->setText("Age");
    labelAge->setGeometry(500,20,50,30);
//    QLayoutItem *labelAgeItem = new QWidgetItem(labelAge);
    QLabel *labelGender = new QLabel(this);
    labelGender->setText("Gender");
    labelGender->setGeometry(200,20,40,30);
//    QLayoutItem *labelGenderItem = new QWidgetItem(labelGender);

    QTextBrowser *textBrowserAge = new QTextBrowser(this);
    textBrowserAge->setGeometry(50, 50, 30, 10);
//    QLayoutItem *textBrowserAgeItem = new QWidgetItem(textBrowserAge);

    QTextBrowser *textBrowserGender = new QTextBrowser(this);
    textBrowserGender->setGeometry(250, 65, 30, 10);
//    QLayoutItem *textBrowserGenderItem = new QWidgetItem(textBrowserGender);
//
//    layoutAge->addItem(labelAgeItem);
//    layoutAge->addWidget(textBrowserAge);
//
//    layoutGender->addItem(labelGenderItem);
//    layoutGender->addItem(textBrowserGenderItem);

    QTabWidget *tabWidgetGraphs = new QTabWidget(this);
    tabWidgetGraphs->setGeometry(30,30,1200,300);
//    tabWidgetGraphs->setMinimumSize(QSize(1200,200));
    QScrollArea *scrollAreaParameters = new QScrollArea(this);
//
    scrollAreaParameters ->setGeometry(tabWidgetGraphs->x()+100, tabWidgetGraphs->y(), 300, tabWidgetGraphs->height());
    QScrollBar *scrollBarParams = new QScrollBar();
    scrollAreaParameters->setVerticalScrollBar(scrollBarParams);
    scrollAreaParameters->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //    scrollAreaParameters ->setMinimumSize(250,tabWidgetGraphs->height());
//    scrollAreaParameters ->setMaximumSize(300,tabWidgetGraphs->height());
    horizontalMainWindowLayout->addWidget(tabWidgetGraphs);
    horizontalMainWindowLayout->addWidget(scrollAreaParameters);
    horizontalMainWindowLayout->addWidget(pushbuttonOpenFile);

//    gridLayout->addWidget(tabWidgetGraphs, 0, 0, 0, 0);
//    gridLayout->addWidget(scrollAreaParameters,0,1);

    QGroupBox *groupBoxPatientInfo = new QGroupBox();
    QVBoxLayout *PatientInfo  = new QVBoxLayout();
    PatientInfo->addWidget(labelAge);
    PatientInfo->addWidget(labelGender);
    PatientInfo->addWidget(textBrowserAge);
    PatientInfo->addWidget(textBrowserGender);
    groupBoxPatientInfo->setLayout(PatientInfo);

    scrollAreaParameters->setWidget(groupBoxPatientInfo);

    gridLayout->addLayout(horizontalMainWindowLayout,0,0,0,0);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);





}
void MainWindow::handleButton()
{
    // change the text
    pushbuttonOpenFile->setText("Example");
    // resize button
    pushbuttonOpenFile->resize(100,100);
    std::cout<<"dupa";
}