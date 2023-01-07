//
// Created by Bartosz Łukasik on 16/11/2022.
//
#include <QTabWidget>
#include "ViewModelUI.h"
#include <QPushButton>
#include "tabdialog.h"
#include "generaltab.h"
#include "../Modules/Sample/Sample.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //Main Window Layout
    QWidget *centralWidget = new QWidget(parent);
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // Manu Bar
    QMenuBar *menuBar = new QMenuBar();
    mainLayout->setMenuBar(menuBar);
    QMenu *fileMenu = menuBar->addMenu("File");
    // Options
    QAction *openAction = fileMenu->addAction("Open");
    QAction *saveAction = fileMenu->addAction("Close");
    // Stylesheet
    menuBar->setStyleSheet("QMenuBar {background-color:#A0E4CB; font-size: 11pt; font-family:Microsoft JhengHei Light;}");
    fileMenu->setStyleSheet("QMenu {background-color:#A0E4CB; font-family:Microsoft JhengHei Light;}");

    // Divide Main Layout into graph section and parameter section
    QHBoxLayout *horizontalMainWindowLayout = new QHBoxLayout;
    QTabWidget *tabWidgetGraphs = new QTabWidget(this);
    QScrollArea *scrollAreaParameters = new QScrollArea(this);
    QScrollBar *scrollBarParameters = new QScrollBar();
    scrollAreaParameters->setStyleSheet("QScrollArea {background-color:#E7F6F2;}");
    scrollAreaParameters->setVerticalScrollBar(scrollBarParameters);
    scrollAreaParameters->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    horizontalMainWindowLayout->addWidget(tabWidgetGraphs);
    tabWidgetGraphs->setMinimumSize(QSize(1400,200));
    horizontalMainWindowLayout->addWidget(scrollAreaParameters);

    // Scroll Area Group Boxes

    // Patient Group Box
    QHBoxLayout *hLayoutAge = new QHBoxLayout();
    QLabel *labelAge = new QLabel(this);
    labelAge->setText("Age");
    labelAge->setBaseSize(QSize(40,2));


    QTextBrowser *textBrowserAge = new QTextBrowser(this);


    textBrowserAge->setBaseSize(QSize(40,2));
    textBrowserAge->setStyleSheet("QTextBrowser {background-color:#ffffff; color:#000000;}");
    hLayoutAge->addWidget(labelAge);
    hLayoutAge->addWidget(textBrowserAge);

    QHBoxLayout *hLayoutGender = new QHBoxLayout();

    QLabel *labelGender = new QLabel(this);
    labelGender->setText("Gender");
    labelGender->setBaseSize(QSize(40,2));


    QTextBrowser *textBrowserGender = new QTextBrowser(this);
    textBrowserGender->setStyleSheet("QTextBrowser {background-color:#ffffff; color:#000000;}");
    textBrowserGender->setBaseSize(QSize(40,2));
    hLayoutGender->addWidget(labelGender);
    hLayoutGender->addWidget(textBrowserGender);


    QGroupBox *groupBoxPatientInfo = new QGroupBox(tr("Patient Info"));;
    QGridLayout *PatientInfo  = new QGridLayout();
    PatientInfo->addWidget(labelAge,1,0);
    PatientInfo->addWidget(textBrowserAge,1,1);
    PatientInfo->addWidget(labelGender,2,0);
    PatientInfo->addWidget(textBrowserGender,2,1);

    groupBoxPatientInfo->setLayout(PatientInfo);
    groupBoxPatientInfo->setStyleSheet("QGroupBox { border-color:#D7F2EB; border-style: solid; border-width: 2px;}");
    scrollAreaParameters->setWidget(groupBoxPatientInfo);
//    scrollAreaParameters->setWidgetResizable(true);

    // Layouts addded to Main Layout
    mainLayout->addLayout(horizontalMainWindowLayout);
    centralWidget->setLayout(mainLayout);
    centralWidget->setStyleSheet("QWidget {background-color: #F1FAEE;}");
    setCentralWidget(centralWidget);

    // Angelika
//    QFileInfo fileInfo(path);
//    tabWidget = new QTabWidget;
//    tabWidget->addTab(new GeneralTab(fileInfo), tr("General"));
//    tabWidget->addTab(new PermissionsTab(fileInfo), tr("Permissions"));
//    tabWidget->addTab(new ApplicationsTab(fileInfo), tr("Applications"));
    //    tabWidget->addTab(new GeneralTab(this));

//    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
//                                         | QDialogButtonBox::Cancel);

//        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
//        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    //QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(tabWidget);
       // mainLayout->addWidget(buttonBox);
        setLayout(mainLayout);

        setWindowTitle(tr("Tab Dialog"));

    //SLOTS
    connect(openAction, SIGNAL(triggered()), this, SLOT(handleOpenActionClicked()));

}
std::string MainWindow::handleOpenActionClicked() {
    QString filePath = QFileDialog::getOpenFileName(
            this,
            "Open file",
            QDir::homePath(), // ścieżka startowa
            "file (*.dat)" // filtr plików

    );
    std::string str = filePath.toStdString();
    const char *cstr_filePath = str.c_str();
    std::cout << cstr_filePath;
    path = filePath;
    return cstr_filePath;
}
