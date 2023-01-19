//
// Created by Bartosz Łukasik on 16/11/2022.
//
#include <QTabWidget>
#include "Tabsgraphs.h"
#include "ViewModelUI.h"
#include <QPushButton>
#include "textBrowserName.h"

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
    labelGraph->setMinimumSize(QSize(800,30));
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
    gridLayoutPatientInfo->addWidget(labelAge,0,0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    gridLayoutPatientInfo->addWidget(textBrowserAge,0,1,1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    gridLayoutPatientInfo->addWidget(labelGender,1,0,1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    gridLayoutPatientInfo->addWidget(textBrowserGender,1,1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
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

    QLabel * labelempty = new QLabel(tr("      "));
    QGridLayout *gridLayoutFilter = new QGridLayout(this);
    gridLayoutFilter->addWidget(labelempty,1,0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft} );
    gridLayoutFilter->addWidget(pushButtonecgMA, 2,0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    gridLayoutFilter->addWidget(pushButtonecgButterworth, 3, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    gridLayoutFilter->addWidget(pushButtonecgLMS, 4, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});


    QGroupBox *groupBoxFilter= new QGroupBox(tr("Filter"));
    groupBoxFilter->setLayout(gridLayoutFilter);
    groupBoxFilter->setStyleSheet("QGroupBox { border-color:#D7F2EB; border-style: solid; border-width: 2px;}");

    //// QRSPT Detection groupbox
    QPushButton *pushButtonRPeakDetectionPanTompkins = new QPushButton(tr("QRSPT - Pan Tompkins"));
    pushButtonRPeakDetectionPanTompkins->setFixedHeight(30);
    pushButtonRPeakDetectionPanTompkins->setFixedWidth(200);
    pushButtonRPeakDetectionPanTompkins->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");

    QPushButton *pushButtonRPeaksDetectionHilbert = new QPushButton(tr("QRSPT - Hilbert"));
    pushButtonRPeaksDetectionHilbert->setFixedHeight(30);
    pushButtonRPeaksDetectionHilbert->setFixedWidth(200);
    pushButtonRPeaksDetectionHilbert->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");

    QLabel * labelemptyQRSPT = new QLabel(tr("      "));
    QGridLayout *gridLayoutQRSPTDetectionPanTompkins = new QGridLayout();
    gridLayoutQRSPTDetectionPanTompkins->addWidget(labelemptyQRSPT, 0,0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    gridLayoutQRSPTDetectionPanTompkins->addWidget(pushButtonRPeakDetectionPanTompkins, 1,0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    gridLayoutQRSPTDetectionPanTompkins->addWidget(pushButtonRPeaksDetectionHilbert, 2,0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});


    QGroupBox *groupBoxQRSPTDetection= new QGroupBox(tr("QRSPT Detection"));
    groupBoxQRSPTDetection->setLayout(gridLayoutQRSPTDetectionPanTompkins);
    groupBoxQRSPTDetection->setStyleSheet("QGroupBox { border-color:#D7F2EB; border-style: solid; border-width: 2px;}");

    //// HRV Parameters Groupbox
    QGroupBox *groupBoxHRVDetection= new QGroupBox(tr("Heart Rate Variability Parameters"));
    QGridLayout *gridLayoutHRVDetection = new QGridLayout();

    QLabel * labelemptyHRVParams = new QLabel(tr("      "));
    QPushButton *pushButtonHRV = new QPushButton(tr("HRV"));
    pushButtonHRV->setFixedHeight(30);
    pushButtonHRV->setFixedWidth(100);
    pushButtonHRV->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");
    QLabel *labelTimeDomain = new QLabel(tr("Time Domain"));
    QLabel *labelTD_RRmean = new QLabel(tr("RR mean"));
    QLabel *labelTD_SDNN = new QLabel(tr("SDNN"));
    QLabel *labelTD_RMSSD = new QLabel(tr("RMSSD"));
    QLabel *labelTD_NN50 = new QLabel(tr("NN50"));
    QLabel *labelTD_pNN50 = new QLabel(tr("pNN50"));

    gridLayoutHRVDetection->addWidget(labelemptyHRVParams, 0,0,1,1,{Qt::AlignTop, Qt::AlignLeft});
    gridLayoutHRVDetection->addWidget(pushButtonHRV, 1,0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    gridLayoutHRVDetection->setVerticalSpacing(10);
    gridLayoutHRVDetection->addWidget(labelTimeDomain, 2,0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    gridLayoutHRVDetection->addWidget(labelTD_RRmean, 3, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelTD_RRmean->setMinimumSize(QSize(40,30));
    labelTD_RRmean->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelTD_SDNN, 4, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelTD_RRmean->setMinimumSize(QSize(40,30));
    labelTD_RRmean->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelTD_RMSSD, 5, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelTD_RMSSD->setMinimumSize(QSize(40,30));
    labelTD_RMSSD->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelTD_NN50, 6, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelTD_NN50->setMinimumSize(QSize(40,30));
    labelTD_NN50->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelTD_pNN50, 7, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelTD_pNN50->setMinimumSize(QSize(40,30));
    labelTD_pNN50->setMaximumSize(QSize(50,30));

    textBrowserName *textBrowserRRmean = new textBrowserName();
    textBrowserName *textBrowserSDNN = new textBrowserName();
    textBrowserName *textBrowserRMSSD = new textBrowserName();
    textBrowserName *textBrowserNN50 = new textBrowserName();
    textBrowserName *textBrowserpNN50 = new textBrowserName();

    gridLayoutHRVDetection->addWidget(textBrowserRRmean, 3, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserRRmean->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserRRmean->setMinimumSize(QSize(40,30));
    textBrowserRRmean->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserSDNN, 4, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserSDNN->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserSDNN->setMinimumSize(QSize(40,30));
    textBrowserSDNN->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserRMSSD, 5, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserRMSSD->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserRMSSD->setMinimumSize(QSize(40,30));
    textBrowserRMSSD->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserNN50, 6, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserNN50->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserNN50->setMinimumSize(QSize(40,30));
    textBrowserNN50->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserpNN50, 7, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
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

    gridLayoutHRVDetection->addWidget(labelfrequencyDomain, 2, 2, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    gridLayoutHRVDetection->addWidget(labelFD_TP, 3, 2, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelFD_TP->setMinimumSize(QSize(40,30));
    labelFD_TP->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelFD_HF, 4,2, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelFD_HF->setMinimumSize(QSize(40,30));
    labelFD_HF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelFD_LF, 5, 2, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelFD_LF->setMinimumSize(QSize(40,30));
    labelFD_LF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelFD_VLF, 6, 2, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelFD_VLF->setMinimumSize(QSize(40,30));
    labelFD_VLF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelFD_ULF, 7, 2, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelFD_ULF->setMinimumSize(QSize(40,30));
    labelFD_ULF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(labelFD_LFHF, 8, 2, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelFD_LFHF->setMinimumSize(QSize(40,30));
    labelFD_LFHF->setMaximumSize(QSize(50,30));

    textBrowserName *textBrowserTP = new textBrowserName();
    textBrowserName *textBrowserHF = new textBrowserName();
    textBrowserName *textBrowserLF = new textBrowserName();
    textBrowserName *textBrowserVLF = new textBrowserName();
    textBrowserName *textBrowserULF = new textBrowserName();
    textBrowserName *textBrowserLFHF = new textBrowserName();

    gridLayoutHRVDetection->addWidget(textBrowserTP, 3, 3, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserTP->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserTP->setMinimumSize(QSize(40,30));
    textBrowserTP->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserHF, 4, 3, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserHF->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserHF->setMinimumSize(QSize(40,30));
    textBrowserHF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserLF, 5, 3, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserLF->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserLF->setMinimumSize(QSize(40,30));
    textBrowserLF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserVLF, 6, 3, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserVLF->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserVLF->setMinimumSize(QSize(40,30));
    textBrowserVLF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserULF, 7, 3, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserULF->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserULF->setMinimumSize(QSize(40,30));
    textBrowserULF->setMaximumSize(QSize(50,30));
    gridLayoutHRVDetection->addWidget(textBrowserLFHF, 8, 3, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserLFHF->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserLFHF->setMinimumSize(QSize(40,30));
    textBrowserLFHF->setMaximumSize(QSize(50,30));

    groupBoxHRVDetection->setLayout(gridLayoutHRVDetection);
    groupBoxHRVDetection->setStyleSheet("QGroupBox { border-color:#D7F2EB; border-style: solid; border-width: 2px;}");


//// HRV - Geometric Analysis Detection groupbox
    QPushButton *pushButtonHRVGeometricAnalysisDetection = new QPushButton(tr("HRV2"));
    pushButtonHRVGeometricAnalysisDetection->setFixedHeight(30);
    pushButtonHRVGeometricAnalysisDetection->setFixedWidth(140);
    pushButtonHRVGeometricAnalysisDetection->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");


    QGridLayout *gridLayoutHRVGeometricAnalysisDetection = new QGridLayout();

    //LABELS
    QLabel *labelemptyHRVGeomAnalysis = new QLabel(tr("     "));
    gridLayoutHRVGeometricAnalysisDetection->addWidget(labelemptyHRVGeomAnalysis, 0,0,1,1,{Qt::AlignTop, Qt::AlignLeft});

    QLabel *labelTINN = new QLabel(tr("TINN"));
    gridLayoutHRVGeometricAnalysisDetection->addWidget(labelTINN, 3, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelTINN->setMinimumSize(QSize(40,30));
    labelTINN->setMaximumSize(QSize(50,30));

    QLabel *labelHRV = new QLabel(tr("HRV"));
    gridLayoutHRVGeometricAnalysisDetection->addWidget(labelHRV, 4, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelHRV->setMinimumSize(QSize(40,30));
    labelHRV->setMaximumSize(QSize(50,30));

    QLabel *labelSD1 = new QLabel(tr("SD1"));
    gridLayoutHRVGeometricAnalysisDetection->addWidget(labelSD1, 5, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelSD1->setMinimumSize(QSize(40,30));
    labelSD1->setMaximumSize(QSize(50,30));

    QLabel *labelSD2 = new QLabel(tr("SD2"));
    gridLayoutHRVGeometricAnalysisDetection->addWidget(labelSD2, 6, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelSD2->setMinimumSize(QSize(40,30));
    labelSD2->setMaximumSize(QSize(50,30));

    textBrowserName *textBrowserTINN = new textBrowserName();
    textBrowserName *textBrowserHRV = new textBrowserName();
    textBrowserName *textBrowserSD1 = new textBrowserName();
    textBrowserName *textBrowserSD2 = new textBrowserName();

    gridLayoutHRVGeometricAnalysisDetection->addWidget(textBrowserTINN, 3, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserTINN->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserTINN->setMinimumSize(QSize(40,30));
    textBrowserTINN->setMaximumSize(QSize(50,30));
    gridLayoutHRVGeometricAnalysisDetection->addWidget(textBrowserHRV, 4, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserHRV->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserHRV->setMinimumSize(QSize(40,30));
    textBrowserHRV->setMaximumSize(QSize(50,30));
    gridLayoutHRVGeometricAnalysisDetection->addWidget(textBrowserSD1, 5, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserSD1->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserSD1->setMinimumSize(QSize(40,30));
    textBrowserSD1->setMaximumSize(QSize(50,30));
    gridLayoutHRVGeometricAnalysisDetection->addWidget(textBrowserSD2, 6, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserSD2->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserSD2->setMinimumSize(QSize(40,30));
    textBrowserSD2->setMaximumSize(QSize(50,30));


    gridLayoutHRVGeometricAnalysisDetection->setVerticalSpacing(10);
    gridLayoutHRVGeometricAnalysisDetection->addWidget(pushButtonHRVGeometricAnalysisDetection, 1, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});


    QGroupBox *groupBoxHRVGeometricAnalysisDetection= new QGroupBox(tr("HRV - Geometric Analysis"));
    groupBoxHRVGeometricAnalysisDetection->setLayout(gridLayoutHRVGeometricAnalysisDetection);
    groupBoxHRVGeometricAnalysisDetection->setStyleSheet("QGroupBox { border-color:#D7F2EB; border-style: solid; border-width: 2px;}");


//// Detrended Fluctuation Analysis
    QPushButton *pushButtonDetrendedFluctuationAnalysis = new QPushButton(tr("DFA"));
    pushButtonDetrendedFluctuationAnalysis->setFixedHeight(30);
    pushButtonDetrendedFluctuationAnalysis->setFixedWidth(200);
    pushButtonDetrendedFluctuationAnalysis->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");

    QGridLayout *gridLayoutDetrendedFluctuationAnalysis = new QGridLayout();

    QLabel *labelemptyDFA = new QLabel(tr("     "));
    gridLayoutDetrendedFluctuationAnalysis->addWidget(labelemptyDFA,0,0,1,1,{Qt::AlignTop, Qt::AlignLeft});

    QLabel *labelALFA1 = new QLabel(tr("ALFA1"));
    gridLayoutDetrendedFluctuationAnalysis->addWidget(labelALFA1, 3, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelALFA1->setMinimumSize(QSize(40,30));
    labelALFA1->setMaximumSize(QSize(50,30));

    QLabel *labelALFA2 = new QLabel(tr("ALFA2"));
    gridLayoutDetrendedFluctuationAnalysis->addWidget(labelALFA2, 4, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelALFA2->setMinimumSize(QSize(40,30));
    labelALFA2->setMaximumSize(QSize(50,30));

    textBrowserName *textBrowserALFA1 = new textBrowserName();
    textBrowserName *textBrowserALFA2 = new textBrowserName();

    gridLayoutDetrendedFluctuationAnalysis->addWidget(textBrowserALFA1, 3, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserALFA1->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserALFA1->setMinimumSize(QSize(40,30));
    textBrowserALFA1->setMaximumSize(QSize(50,30));

    gridLayoutDetrendedFluctuationAnalysis->addWidget(textBrowserALFA2, 4, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserALFA2->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserALFA2->setMinimumSize(QSize(40,30));
    textBrowserALFA2->setMaximumSize(QSize(50,30));

    QGroupBox *groupBoxDetrendedFluctuationAnalysis = new QGroupBox(tr("Detrended Fluctuation Analysis"));
    groupBoxDetrendedFluctuationAnalysis->setLayout(gridLayoutDetrendedFluctuationAnalysis);
    groupBoxDetrendedFluctuationAnalysis->setStyleSheet("QGroupBox { border-color:#D7F2EB; border-style: solid; border-width: 2px;}");

    gridLayoutDetrendedFluctuationAnalysis->setVerticalSpacing(10);
    gridLayoutDetrendedFluctuationAnalysis->addWidget(pushButtonDetrendedFluctuationAnalysis, 2, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});

//// QRS Classification
    QPushButton *pushButtonQRSClassification = new QPushButton(tr("QRS Classification"));
    pushButtonQRSClassification->setFixedHeight(30);
    pushButtonQRSClassification->setFixedWidth(200);
    pushButtonQRSClassification->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");

    QGridLayout *gridLayoutQRSClassification = new QGridLayout();
    QLabel *labelemptyQRS = new QLabel(tr("     "));
    gridLayoutQRSClassification->addWidget(labelemptyQRS,0,0,1,1,{ Qt::AlignTop, Qt::AlignLeft});

    QLabel *labelSV = new QLabel(tr("SV"));
    gridLayoutQRSClassification->addWidget(labelSV, 3, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelSV->setMinimumSize(QSize(40,30));
    labelSV->setMaximumSize(QSize(50,30));

    QLabel *labelV= new QLabel(tr("V"));
    gridLayoutQRSClassification->addWidget(labelV, 4, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelV->setMinimumSize(QSize(40,30));
    labelV->setMaximumSize(QSize(50,30));

    QLabel *labelOTHER = new QLabel(tr("Other"));
    gridLayoutQRSClassification->addWidget(labelOTHER, 5, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelOTHER->setMinimumSize(QSize(40,30));
    labelOTHER->setMaximumSize(QSize(50,30));

    QLabel *labelARTIFACT = new QLabel(tr("Artifact"));
    gridLayoutQRSClassification->addWidget(labelARTIFACT, 6, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelARTIFACT->setMinimumSize(QSize(40,30));
    labelARTIFACT->setMaximumSize(QSize(60,30));

    textBrowserName *textBrowserSV = new textBrowserName();
    textBrowserName *textBrowserV = new textBrowserName();
    textBrowserName *textBrowserOTHER = new textBrowserName();
    textBrowserName *textBrowserARTIFACT = new textBrowserName();

    gridLayoutQRSClassification->addWidget(textBrowserSV, 3, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserSV->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserSV->setMinimumSize(QSize(40,30));
    textBrowserSV->setMaximumSize(QSize(50,30));

    gridLayoutQRSClassification->addWidget(textBrowserV, 4, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserV->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserV->setMinimumSize(QSize(40,30));
    textBrowserV->setMaximumSize(QSize(50,30));

    gridLayoutQRSClassification->addWidget(textBrowserOTHER, 5, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserOTHER->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserOTHER->setMinimumSize(QSize(40,30));
    textBrowserOTHER->setMaximumSize(QSize(50,30));

    gridLayoutQRSClassification->addWidget(textBrowserARTIFACT, 6, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserARTIFACT->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserARTIFACT->setMinimumSize(QSize(40,30));
    textBrowserARTIFACT->setMaximumSize(QSize(50,30));

    QGroupBox *groupBoxQRSClassification = new QGroupBox(tr("QRS Classification"));
    groupBoxQRSClassification->setLayout(gridLayoutQRSClassification);
    groupBoxQRSClassification->setStyleSheet("QGroupBox { border-color:#D7F2EB; border-style: solid; border-width: 2px;}");

    gridLayoutQRSClassification->setVerticalSpacing(10);
    gridLayoutQRSClassification->addWidget(pushButtonQRSClassification, 2, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});

//// ST Segment
    QPushButton *pushButtonSTSegment = new QPushButton(tr("ST Segment"));
    pushButtonSTSegment->setFixedHeight(30);
    pushButtonSTSegment->setFixedWidth(200);
    pushButtonSTSegment->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");

    QGridLayout *gridLayoutSTSegment = new QGridLayout();
    QLabel * labelemptyST = new QLabel(tr("     "));
    gridLayoutSTSegment->addWidget(labelemptyST,0,0,1,1,{ Qt::AlignTop, Qt::AlignLeft});

    QLabel *labelDISEASE = new QLabel(tr("Healthy/Unhealthy"));
    gridLayoutSTSegment->addWidget(labelDISEASE, 3, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelDISEASE->setMinimumSize(QSize(90,30));
    labelDISEASE->setMaximumSize(QSize(100,30));

    QLabel *labelCASEDISEASE = new QLabel(tr("Diagnosis"));
    gridLayoutSTSegment->addWidget(labelCASEDISEASE, 4, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelCASEDISEASE->setMinimumSize(QSize(40,30));
    labelCASEDISEASE->setMaximumSize(QSize(100,30));

    textBrowserName *textBrowserDISEASE = new textBrowserName();
    textBrowserName *textBrowserCASEDISEASE = new textBrowserName();

    gridLayoutSTSegment->addWidget(textBrowserDISEASE, 3, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserDISEASE->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserDISEASE->setMinimumSize(QSize(40,30));
    textBrowserDISEASE->setMaximumSize(QSize(50,30));

    gridLayoutSTSegment->addWidget(textBrowserCASEDISEASE, 4, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserCASEDISEASE->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserCASEDISEASE->setMinimumSize(QSize(60,50));
    textBrowserCASEDISEASE->setMaximumSize(QSize(100,70));

    QGroupBox *groupBoxSTSegment = new QGroupBox(tr("ST Segment"));
    groupBoxSTSegment->setLayout(gridLayoutSTSegment);
    groupBoxSTSegment->setStyleSheet("QGroupBox { border-color:#D7F2EB; border-style: solid; border-width: 2px;}");

    gridLayoutSTSegment->setVerticalSpacing(10);
    gridLayoutSTSegment->addWidget(pushButtonSTSegment, 2, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});

//// T Wave Alternans
    QPushButton *pushButtonTWaveAlternans = new QPushButton(tr("TWA"));
    pushButtonTWaveAlternans->setFixedHeight(30);
    pushButtonTWaveAlternans->setFixedWidth(200);
    pushButtonTWaveAlternans->setStyleSheet("background-color:#D7F2EB; border: 1px solid #59C1BD;");

    QGridLayout *gridLayoutTWaveAlternans = new QGridLayout();

    QLabel *labelemptyTwave = new QLabel(tr("     "));
    gridLayoutTWaveAlternans->addWidget(labelemptyTwave, 0,0,1,1,{ Qt::AlignTop, Qt::AlignLeft});

    QLabel *labelAMPLITUDEALT = new QLabel(tr("Amplitude ALT"));
    gridLayoutTWaveAlternans->addWidget(labelAMPLITUDEALT, 3, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelAMPLITUDEALT->setMinimumSize(QSize(90,30));
    labelAMPLITUDEALT->setMaximumSize(QSize(100,30));

    QLabel *labelTHRESHOLD = new QLabel(tr("Threshold"));
    gridLayoutTWaveAlternans->addWidget(labelTHRESHOLD, 4, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    labelTHRESHOLD->setMinimumSize(QSize(40,30));
    labelTHRESHOLD->setMaximumSize(QSize(70,30));

    textBrowserName *textBrowserAMPLITUDEALT = new textBrowserName();
    textBrowserName *textBrowserTHRESHOLD = new textBrowserName();

    gridLayoutTWaveAlternans->addWidget(textBrowserAMPLITUDEALT, 3, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserAMPLITUDEALT->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserAMPLITUDEALT->setMinimumSize(QSize(40,30));
    textBrowserAMPLITUDEALT->setMaximumSize(QSize(50,30));

    gridLayoutTWaveAlternans->addWidget(textBrowserTHRESHOLD, 4, 1, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});
    textBrowserTHRESHOLD->setStyleSheet("QTextBrowser {background-color:#ffffff;}");
    textBrowserTHRESHOLD->setMinimumSize(QSize(40,30));
    textBrowserTHRESHOLD->setMaximumSize(QSize(50,30));

    QGroupBox *groupBoxTWaveAlternans = new QGroupBox(tr("T Wave Alternans"));
    groupBoxTWaveAlternans->setLayout(gridLayoutTWaveAlternans);
    groupBoxTWaveAlternans->setStyleSheet("QGroupBox { border-color:#D7F2EB; border-style: solid; border-width: 2px;}");

    gridLayoutTWaveAlternans->setVerticalSpacing(10);
    gridLayoutTWaveAlternans->addWidget(pushButtonTWaveAlternans, 2, 0, 1, 1,{ Qt::AlignTop, Qt::AlignLeft});

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
    verticalLayout_scrollAreaCentralWidget->addWidget(groupBoxHRVGeometricAnalysisDetection);
    verticalLayout_scrollAreaCentralWidget->addSpacing(20);
    verticalLayout_scrollAreaCentralWidget->addWidget(groupBoxDetrendedFluctuationAnalysis);
    verticalLayout_scrollAreaCentralWidget->addSpacing(20);
    verticalLayout_scrollAreaCentralWidget->addWidget(groupBoxQRSClassification);
    verticalLayout_scrollAreaCentralWidget->addSpacing(20);
    verticalLayout_scrollAreaCentralWidget->addWidget(groupBoxTWaveAlternans);
    verticalLayout_scrollAreaCentralWidget->addSpacing(20);
    verticalLayout_scrollAreaCentralWidget->addWidget(groupBoxSTSegment);

    scrollAreaParameters->setWidget(scrollAreaCentralWidget);


    // Layouts added to Main Layout
    mainLayout->addLayout(horizontalMainWindowLayout);
    centralWidget->setLayout(mainLayout);
    centralWidget->setStyleSheet("QWidget {background-color: #F1FAEE;}");
    setCentralWidget(centralWidget);

        setLayout(mainLayout);

        setWindowTitle(tr("ECG - Analyzer"));

    ////SLOTS
    //Buttons
    connect(pushButtonRemoveTab, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(RemoveTab()));
    connect(pushButtonecgMA, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(addtab_ECGBaselineMA()));
    connect(pushButtonecgLMS, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(addtab_ECGBaselineLMS()));
    connect(pushButtonecgButterworth, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(addtab_ECGBaselineButterworth()));

    connect(pushButtonRPeakDetectionPanTompkins, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(addtab_RPeaksDetectionPanTompkins()));
    connect(pushButtonRPeaksDetectionHilbert, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(addtab_RPeaksDetectionHilbert()));

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

    connect(pushButtonHRVGeometricAnalysisDetection, SIGNAL(clicked(bool)), textBrowserTINN, SLOT(handleTextBrowserTINN()));
    connect(pushButtonHRVGeometricAnalysisDetection, SIGNAL(clicked(bool)), textBrowserHRV, SLOT(handleTextBrowserHRV()));
    connect(pushButtonHRVGeometricAnalysisDetection, SIGNAL(clicked(bool)), textBrowserSD1, SLOT(handleTextBrowserSD1()));
    connect(pushButtonHRVGeometricAnalysisDetection, SIGNAL(clicked(bool)), textBrowserSD2, SLOT(handleTextBrowserSD2()));
    connect(pushButtonHRVGeometricAnalysisDetection, SIGNAL(clicked(bool)), tabWidgetGraphs, SLOT(addtab_HRV2()));

    connect(pushButtonDetrendedFluctuationAnalysis, SIGNAL(clicked(bool)), textBrowserALFA1, SLOT(handleTextBrowserALFA1()));
    connect(pushButtonDetrendedFluctuationAnalysis, SIGNAL(clicked(bool)), textBrowserALFA2, SLOT(handleTextBrowserALFA2()));

    connect(pushButtonQRSClassification, SIGNAL(clicked(bool)), textBrowserSV, SLOT(handleTextBrowserSV()));
    connect(pushButtonQRSClassification, SIGNAL(clicked(bool)), textBrowserV, SLOT(handleTextBrowserV()));
    connect(pushButtonQRSClassification, SIGNAL(clicked(bool)), textBrowserOTHER, SLOT(handleTextBrowserOTHER()));
    connect(pushButtonQRSClassification, SIGNAL(clicked(bool)), textBrowserARTIFACT, SLOT(handleTextBrowserARTIFACT()));

    connect(pushButtonTWaveAlternans, SIGNAL(clicked(bool)), textBrowserAMPLITUDEALT, SLOT(handleTextBrowserAMPLITUDEALT()));
    connect(pushButtonTWaveAlternans, SIGNAL(clicked(bool)), textBrowserTHRESHOLD, SLOT(handleTextBrowserTHRESHOLD()));

    connect(pushButtonSTSegment, SIGNAL(clicked(bool)), textBrowserCASEDISEASE, SLOT(handleTextBrowserCASEDISEASE()));
    connect(pushButtonSTSegment, SIGNAL(clicked(bool)), textBrowserDISEASE, SLOT(handleTextBrowserDISEASE()));

    // Menu Bar Options
    //Open Action
    connect(openAction, SIGNAL(triggered()), this, SLOT(handleOpenActionClicked()));
    connect(openAction, SIGNAL(triggered()), textBrowserAge, SLOT(handleTextBrowserAge()));
    connect(openAction, SIGNAL(triggered()), textBrowserGender, SLOT(handleTextBrowserGender()));

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
