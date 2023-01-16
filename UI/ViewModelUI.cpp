//
// Created by Bartosz Łukasik on 16/11/2022.
//

#include "ViewModelUI.h"
#include <QPushButton>
#include "../Modules/Sample/Sample.h"
#include "textBrowserName.h"
//#include "chartCallout.h"

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


    Tabsgraphs *tabWidgetGraphs = new Tabsgraphs;

    QPushButton *pushButtonRemoveTab = new QPushButton(tr("Remove Tab"));
    pushButtonRemoveTab->setFixedHeight(30);
    pushButtonRemoveTab->setFixedWidth(140);
    pushButtonRemoveTab->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");
    QHBoxLayout *horizontalLayoutLabelAndRemoveTabButton = new QHBoxLayout;
    QLabel *labelGraph = new QLabel(tr("GRAPHS"));
    QLabel *labelParameters = new QLabel(tr("Parameters"));
    labelParameters->setAlignment(Qt::AlignCenter);
    labelParameters->setMinimumSize(QSize(110,30));
    labelGraph->setMinimumSize(QSize(1400,30));
    labelGraph->setStyleSheet("font-size: 11pt;");
    labelParameters->setStyleSheet("font-size: 11pt;");
    horizontalLayoutLabelAndRemoveTabButton ->addWidget(labelGraph);
    labelGraph->setAlignment(Qt::AlignCenter);
    horizontalLayoutLabelAndRemoveTabButton ->addWidget(pushButtonRemoveTab);
    QVBoxLayout *verticalLayoutGraphSection = new QVBoxLayout(this);
    QVBoxLayout *verticalLayoutParametersSection = new QVBoxLayout(this);
    verticalLayoutParametersSection->addWidget(labelParameters);
    verticalLayoutGraphSection->addLayout(horizontalLayoutLabelAndRemoveTabButton);
    verticalLayoutGraphSection->addWidget(tabWidgetGraphs);


    QScrollArea *scrollAreaParameters = new QScrollArea(this);
    QScrollBar *scrollBarParameters = new QScrollBar();
    scrollAreaParameters->setStyleSheet("QScrollArea {background-color:#E7F6F2;}");
    scrollAreaParameters->setVerticalScrollBar(scrollBarParameters);
    scrollAreaParameters->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    verticalLayoutParametersSection->addWidget(scrollAreaParameters);
    horizontalMainWindowLayout->addLayout(verticalLayoutGraphSection);
    scrollAreaParameters->setMinimumSize(QSize(110,200));

    horizontalMainWindowLayout->addLayout(verticalLayoutParametersSection);

    QWidget* scrollAreaCentralWidget = new QWidget( this );
    scrollAreaCentralWidget->setStyleSheet("QWidget {background-color:#E7F6F2;}");
    QVBoxLayout *verticalLayout_scrollAreaCentralWidget = new QVBoxLayout;
    scrollAreaCentralWidget->setLayout(verticalLayout_scrollAreaCentralWidget);

    //// Scroll Area Group Boxes

    //// Patient Group Box
    QLabel *labelAge = new QLabel(this);
    labelAge->setText("Age");
    labelAge->setMinimumSize(QSize(40,30));
    labelAge->setMaximumSize(QSize(50,30));


    textBrowserName *textBrowserAge = new textBrowserName;
    textBrowserAge->setMinimumSize(QSize(40,30));
    textBrowserAge->setMaximumSize(QSize(50,30));
    textBrowserAge->setStyleSheet("QTextBrowser {background-color:#ffffff; color:#000000;}");


    QLabel *labelGender = new QLabel(this);
    labelGender->setText("Gender");
    labelGender->setMinimumSize(QSize(40,30));
    labelGender->setMaximumSize(QSize(50,30));


    textBrowserName *textBrowserGender = new textBrowserName();
    textBrowserGender->setStyleSheet("QTextBrowser {background-color:#ffffff; color:#000000;}");
    textBrowserGender->setMinimumSize(QSize(40,30));
    textBrowserGender->setMaximumSize(QSize(50,30));

    QGroupBox *groupBoxPatientInfo = new QGroupBox(tr("Patient Info"));
    QGridLayout *gridLayoutPatientInfo = new QGridLayout;
    gridLayoutPatientInfo->addWidget(labelAge,0,0);
    gridLayoutPatientInfo->addWidget(textBrowserAge,0,1);
    gridLayoutPatientInfo->addWidget(labelGender,1,0);
    gridLayoutPatientInfo->addWidget(textBrowserGender,1,1);
    groupBoxPatientInfo->setLayout(gridLayoutPatientInfo);
    groupBoxPatientInfo->setStyleSheet("QGroupBox { border-color:#D7F2EB; border-style: solid; border-width: 2px;}");

    //// Filter Group Box
    QPushButton *pushButtonecgMA = new QPushButton(tr("ECG - MA Filter"));
    QPushButton *pushButtonecgButterworth = new QPushButton(tr("ECG - Butterworth Filter "));
    QPushButton *pushButtonecgLMS = new QPushButton(tr("ECG - LMS"));

    pushButtonecgMA->setFixedHeight(30);
    pushButtonecgMA->setFixedWidth(200);
    pushButtonecgMA->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");

    pushButtonecgButterworth->setFixedHeight(30);
    pushButtonecgButterworth->setFixedWidth(200);
    pushButtonecgButterworth->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");

    pushButtonecgLMS->setFixedHeight(30);
    pushButtonecgLMS->setFixedWidth(200);
    pushButtonecgLMS->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");


    QGridLayout *gridLayoutFilter = new QGridLayout(this);
    gridLayoutFilter->addWidget(pushButtonecgMA, 1,0);
    gridLayoutFilter->addWidget(pushButtonecgButterworth, 2, 0);
    gridLayoutFilter->addWidget(pushButtonecgLMS, 3, 0);


    QGroupBox *groupBoxFilter= new QGroupBox(tr("Filter"));
    groupBoxFilter->setLayout(gridLayoutFilter);
    groupBoxFilter->setStyleSheet("QGroupBox { border-color:#D7F2EB; border-style: solid; border-width: 2px;}");

    //// QRSPT Detection groupbox
    QPushButton *pushButtonRPeakDetectionPanTompkins = new QPushButton(tr("R Peaks Detection - Pan Tompkins"));
    pushButtonRPeakDetectionPanTompkins->setFixedHeight(30);
    pushButtonRPeakDetectionPanTompkins->setFixedWidth(200);
    pushButtonRPeakDetectionPanTompkins->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");

    QPushButton *pushButtonRPeaksDetectionHilbert = new QPushButton(tr("R Peaks Detection - Hilbert"));
    pushButtonRPeaksDetectionHilbert->setFixedHeight(30);
    pushButtonRPeaksDetectionHilbert->setFixedWidth(200);
    pushButtonRPeaksDetectionHilbert->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");


    QPushButton *pushButtonQRSandPTPeaksDetection = new QPushButton(tr("QRS and PT Detection"));
    pushButtonQRSandPTPeaksDetection->setFixedHeight(30);
    pushButtonQRSandPTPeaksDetection->setFixedWidth(200);
    pushButtonQRSandPTPeaksDetection->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");

    QGridLayout *gridLayoutQRSPTDetectionPanTompkins = new QGridLayout();
    gridLayoutQRSPTDetectionPanTompkins->addWidget(pushButtonRPeakDetectionPanTompkins, 0,0);
    gridLayoutQRSPTDetectionPanTompkins->addWidget(pushButtonRPeaksDetectionHilbert, 1,0);
    gridLayoutQRSPTDetectionPanTompkins->addWidget(pushButtonQRSandPTPeaksDetection,2, 0);

    QGroupBox *groupBoxQRSPTDetection= new QGroupBox(tr("QRSPT Detection"));
    groupBoxQRSPTDetection->setLayout(gridLayoutQRSPTDetectionPanTompkins);
    groupBoxQRSPTDetection->setStyleSheet("QGroupBox { border-color:#D7F2EB; border-style: solid; border-width: 2px;}");

    //// HRV Parameters Groupbox
    QGroupBox *groupBoxHRVDetection= new QGroupBox(tr("Heart Rate Variability Parameters"));
    QGridLayout *gridLayoutHRVDetection = new QGridLayout();

    QPushButton *pushButtonHRV = new QPushButton(tr("HRV"));
    pushButtonHRV->setFixedHeight(30);
    pushButtonHRV->setFixedWidth(200);
    pushButtonHRV->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");
    QLabel *labelTimeDomain = new QLabel(tr("Time Domain"));
    QLabel *labelTD_RRmean = new QLabel(tr("RR mean"));
    QLabel *labelTD_SDNN = new QLabel(tr("SDNN"));
    QLabel *labelTD_RMSSD = new QLabel(tr("RMSSD"));
    QLabel *labelTD_NN50 = new QLabel(tr("NN50"));
    QLabel *labelTD_pNN50 = new QLabel(tr("pNN50"));

    gridLayoutHRVDetection->addWidget(pushButtonHRV, 1,0);
    gridLayoutHRVDetection->setVerticalSpacing(10);
    gridLayoutHRVDetection->addWidget(labelTimeDomain, 2,0);
    gridLayoutHRVDetection->addWidget(labelTD_RRmean, 3, 0);
    labelTD_RRmean->setMinimumSize(QSize(40,30));
    labelTD_RRmean->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelTD_SDNN, 4, 0);
    labelTD_RRmean->setMinimumSize(QSize(40,30));
    labelTD_RRmean->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelTD_RMSSD, 5, 0);
    labelTD_RMSSD->setMinimumSize(QSize(40,30));
    labelTD_RMSSD->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelTD_NN50, 6, 0);
    labelTD_NN50->setMinimumSize(QSize(40,30));
    labelTD_NN50->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelTD_pNN50, 7, 0);
    labelTD_pNN50->setMinimumSize(QSize(40,30));
    labelTD_pNN50->setMaximumSize(QSize(50,30));

    textBrowserName *textBrowserRRmean = new textBrowserName();
    textBrowserName *textBrowserSDNN = new textBrowserName();
    textBrowserName *textBrowserRMSSD = new textBrowserName();
    textBrowserName *textBrowserNN50 = new textBrowserName();
    textBrowserName *textBrowserpNN50 = new textBrowserName();

    gridLayoutHRVDetection->addWidget(textBrowserRRmean, 3, 1);
    textBrowserRRmean->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserRRmean->setMinimumSize(QSize(40,30));
    textBrowserRRmean->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserSDNN, 4, 1);
    textBrowserSDNN->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserSDNN->setMinimumSize(QSize(40,30));
    textBrowserSDNN->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserRMSSD, 5, 1);
    textBrowserRMSSD->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserRMSSD->setMinimumSize(QSize(40,30));
    textBrowserRMSSD->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserNN50, 6, 1);
    textBrowserNN50->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserNN50->setMinimumSize(QSize(40,30));
    textBrowserNN50->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserpNN50, 7, 1);
    textBrowserpNN50->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserpNN50->setMinimumSize(QSize(40,30));
    textBrowserpNN50->setMaximumSize(QSize(50,30));

    QLabel *labelfrequencyDomain = new QLabel(tr("Frequency Domain"));
    QLabel *labelFD_TP = new QLabel(tr("TP"));
    QLabel *labelFD_HF = new QLabel(tr("HF"));
    QLabel *labelFD_LF = new QLabel(tr("LF"));
    QLabel *labelFD_VLF = new QLabel(tr("VLF"));
    QLabel *labelFD_ULF = new QLabel(tr("ULF"));
    QLabel *labelFD_LFHF = new QLabel(tr("LFHF"));

    gridLayoutHRVDetection->addWidget(labelfrequencyDomain, 2, 2);
    gridLayoutHRVDetection->addWidget(labelFD_TP, 3, 2);
    labelFD_TP->setMinimumSize(QSize(40,30));
    labelFD_TP->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelFD_HF, 4,2);
    labelFD_HF->setMinimumSize(QSize(40,30));
    labelFD_HF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelFD_LF, 5, 2);
    labelFD_LF->setMinimumSize(QSize(40,30));
    labelFD_LF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelFD_VLF, 6, 2);
    labelFD_VLF->setMinimumSize(QSize(40,30));
    labelFD_VLF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelFD_ULF, 7, 2);
    labelFD_ULF->setMinimumSize(QSize(40,30));
    labelFD_ULF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelFD_LFHF, 8, 2);
    labelFD_LFHF->setMinimumSize(QSize(40,30));
    labelFD_LFHF->setMaximumSize(QSize(50,30));

    textBrowserName *textBrowserTP = new textBrowserName();
    textBrowserName *textBrowserHF = new textBrowserName();
    textBrowserName *textBrowserLF = new textBrowserName();
    textBrowserName *textBrowserVLF = new textBrowserName();
    textBrowserName *textBrowserULF = new textBrowserName();
    textBrowserName *textBrowserLFHF = new textBrowserName();

    gridLayoutHRVDetection->addWidget(textBrowserTP, 3, 3);
    textBrowserTP->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserTP->setMinimumSize(QSize(40,30));
    textBrowserTP->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserHF, 4, 3);
    textBrowserHF->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserHF->setMinimumSize(QSize(40,30));
    textBrowserHF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserLF, 5, 3);
    textBrowserLF->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserLF->setMinimumSize(QSize(40,30));
    textBrowserLF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserVLF, 6, 3);
    textBrowserVLF->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserVLF->setMinimumSize(QSize(40,30));
    textBrowserVLF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserULF, 7, 3);
    textBrowserULF->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserULF->setMinimumSize(QSize(40,30));
    textBrowserULF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserLFHF, 8, 3);
    textBrowserLFHF->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserLFHF->setMinimumSize(QSize(40,30));
    textBrowserLFHF->setMaximumSize(QSize(50,30));

    groupBoxHRVDetection->setLayout(gridLayoutHRVDetection);
    groupBoxHRVDetection->setStyleSheet("QGroupBox { border-color:#D7F2EB; border-style: solid; border-width: 2px;}");

    //// Scroll Area parameters

    verticalLayout_scrollAreaCentralWidget->setSizeConstraint(QLayout::SetMinimumSize);
    verticalLayout_scrollAreaCentralWidget->addWidget(groupBoxPatientInfo);
    verticalLayout_scrollAreaCentralWidget->addSpacing(20);
    verticalLayout_scrollAreaCentralWidget->addWidget(groupBoxFilter);
    verticalLayout_scrollAreaCentralWidget->addSpacing(20);
    verticalLayout_scrollAreaCentralWidget->addWidget(groupBoxQRSPTDetection);
    verticalLayout_scrollAreaCentralWidget->addSpacing(20);
    verticalLayout_scrollAreaCentralWidget->addWidget(groupBoxHRVDetection);
    verticalLayout_scrollAreaCentralWidget->addSpacing(20);
    scrollAreaParameters->setWidget(scrollAreaCentralWidget);


    // Layouts added to Main Layout
    mainLayout->addLayout(horizontalMainWindowLayout);
    centralWidget->setLayout(mainLayout);
    centralWidget->setStyleSheet("QWidget {background-color: #F1FAEE;}");
    setCentralWidget(centralWidget);


    ////SLOTS
    //Buttons
    connect(pushButtonRemoveTab, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(RemoveTab()));
    connect(pushButtonecgMA, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(addtab_ECGBaselineMA()));
    connect(pushButtonecgLMS, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(addtab_ECGBaselineLMS()));
    connect(pushButtonecgButterworth, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(addtab_ECGBaselineButterworth()));

    connect(pushButtonRPeakDetectionPanTompkins, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(addtab_RPeaksDetectionPanTompkins()));
    connect(pushButtonRPeaksDetectionHilbert, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(addtab_RPeaksDetectionHilbert()));
    connect(pushButtonQRSandPTPeaksDetection, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(addtab_QRSandPTDetection()));
    connect(pushButtonHRV, SIGNAL(clicked(bool)), textBrowserRRmean, SLOT(handleTextBrowserRRmean()));
    connect(pushButtonHRV, SIGNAL(clicked(bool)), textBrowserSDNN, SLOT(handleTextBrowserSDNN()));
    connect(pushButtonHRV, SIGNAL(clicked(bool)), textBrowserRMSSD, SLOT(handleTextBrowserRMSSD()));
    connect(pushButtonHRV, SIGNAL(clicked(bool)), textBrowserNN50, SLOT(handleTextBrowserNN50()));
    connect(pushButtonHRV, SIGNAL(clicked(bool)), textBrowserpNN50, SLOT(handleTextBrowserpNN50()));
    connect(pushButtonHRV, SIGNAL(clicked(bool)), textBrowserTP, SLOT(handleTextBrowserTP()));
    connect(pushButtonHRV, SIGNAL(clicked(bool)), textBrowserHF, SLOT(handleTextBrowserHF()));
    connect(pushButtonHRV, SIGNAL(clicked(bool)), textBrowserLF, SLOT(handleTextBrowserLF()));
    connect(pushButtonHRV, SIGNAL(clicked(bool)), textBrowserVLF, SLOT(handleTextBrowserVLF()));
    connect(pushButtonHRV, SIGNAL(clicked(bool)), textBrowserULF, SLOT(handleTextBrowserULF()));
    connect(pushButtonHRV, SIGNAL(clicked(bool)), textBrowserLFHF, SLOT(handleTextBrowserLFHF()));
//    connect(radioButtonLMS, SIGNAL(clicked(bool)), pushButtonecgBaseline, SLOT(setEnabled(bool)));
//    connect(radioButtonButterworth, SIGNAL(clicked(bool)), pushButtonecgBaseline, SLOT(setEnabled(bool)));
//    connect(radioButtonSaviztkyGolay, SIGNAL(clicked(bool)), pushButtonecgBaseline, SLOT(setEnabled(bool)));
//    connect(radioButtonMovingAverage, SIGNAL(clicked(bool)), pushButtonecgBaseline, SLOT(setEnabled(bool)));

    // Menu Bar Options
    //Open Action
    connect(openAction, SIGNAL(triggered()), this, SLOT(handleOpenActionClicked()));
    connect(openAction, SIGNAL(triggered()), textBrowserAge, SLOT(handleTextBrowserAge()));
    connect(openAction, SIGNAL(triggered()), textBrowserGender, SLOT(handleTextBrowserGender()));

//    checked(radioButtonLMS);
//    checked(radioButtonMovingAverage);
//    checked(radioButtonSaviztkyGolay);
//    checked(radioButtonButterworth);

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

    return cstr_filePath;
}




