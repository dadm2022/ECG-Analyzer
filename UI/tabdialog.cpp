//
// Created by Piotr on 05.01.2023.
//

#include "tabdialog.h"
#include <QtWidgets>
#include "RPEAKS/rpeakstab.h"


TabDialog::TabDialog(const QString &fileName, QWidget *parent)
        : QDialog(parent) {
    QString filepath = QFileDialog::getOpenFileName(this, tr("Open file with signal"), "/home",
                                                    tr("Signal Files (*.dat)"));

    if (filepath.isEmpty()) {
        QMessageBox::warning(this, "WARNING", "Select file.");
    }

    ECG ecg;
    ecg.loadSignal(const_cast<char *>( filepath.toStdString().c_str()));
    ecg.lowFiltering();
    ecg.highFiltering();

    R_peaks r_peaks;
    int fs = 360;
    const std::vector<float> &filteredSignal = ecg.getFilteredSignal();
    auto detectedRPeaks = r_peaks.DetekcjaZalamkowR(filteredSignal, fs);
    const std::shared_ptr<std::vector<float>> &filteredSignalPtr = std::make_shared<std::vector<float>>(filteredSignal);
    auto rIndeksyPtr = std::make_shared<std::vector<int>>(detectedRPeaks->RIndeksy);
    auto rOdstepyPtr = std::make_shared<std::vector<int>>(detectedRPeaks->ROdstepy);
    // wywołanie getWaves od modułu WAVES
    std::shared_ptr<WavesDetector> wavesDetectorPtr(new WavesDetector());
    wavesDetectorPtr->naiveTdetection(filteredSignalPtr, rIndeksyPtr, rOdstepyPtr);
    wavesDetectorPtr->naivePdetection(filteredSignalPtr, rIndeksyPtr, rOdstepyPtr);


    std::shared_ptr<HRV2> hrv2Ptr = createHrv2Ptr(detectedRPeaks);

    tabWidget = new QTabWidget;
    tabWidget->addTab(new RPeaksTab(filteredSignal, detectedRPeaks), tr("R peaks"));
    tabWidget->addTab(new HRVTab(detectedRPeaks), tr("HRV1"));
    tabWidget->addTab(new HRV2Tab(detectedRPeaks, hrv2Ptr), tr("HRV2"));

    tabWidget->addTab(new WavesTab(filteredSignalPtr, wavesDetectorPtr),
                      tr("WAVES"));

    tabWidget->addTab(new STSegmentTab(std::make_shared<std::vector<float>>(filteredSignal), detectedRPeaks),
                      tr("ST segment"));
    tabWidget->addTab(new TWAPeaksTab(filteredSignalPtr, wavesDetectorPtr), tr("T Wave Alt"));
    tabWidget->addTab(new HeartClassTab(filteredSignalPtr, rIndeksyPtr, wavesDetectorPtr), tr("Heart class"));


    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    QDesktopWidget dw;
    int x = dw.width()*0.7;
    int y = dw.height()*0.7;

    setWindowTitle(tr("Analiza EKG"));
    setMinimumSize(1600, 1000);
    setFixedSize(x, y);
}

std::shared_ptr<HRV2> TabDialog::createHrv2Ptr(const std::shared_ptr<R_peaks::R_Peaks_Pakiet> &detectedRPeaks) {
    int bins = 30;
    std::shared_ptr<HRV2> hrv2Ptr(new HRV2());
    hrv2Ptr->SetHistogram(bins, detectedRPeaks->RSinusOdstepy);
    hrv2Ptr->setHRVi(detectedRPeaks->RSinusOdstepy);

    hrv2Ptr->setSD1SD2Params(detectedRPeaks->RSinusOdstepy);

    hrv2Ptr->setPoincare_x(detectedRPeaks->RSinusOdstepy);
    hrv2Ptr->setPoincare_y(detectedRPeaks->RSinusOdstepy);

    hrv2Ptr->setIdentityLine(detectedRPeaks->RSinusOdstepy);
    hrv2Ptr->setAxes(detectedRPeaks->RSinusOdstepy);

    auto hrv2Histogram = hrv2Ptr->GetHistogram();
    auto binsBorders = hrv2Histogram->bins_borders;
    bins = hrv2Histogram->bins;
    auto interval = hrv2Histogram->interval;
    auto max_hist = hrv2Histogram->max_hist;
    auto max_bin = hrv2Histogram->max_bin;

    std::vector<double> bin_centers = hrv2Ptr->calcBinCenters(bins, interval, binsBorders);
    hrv2Ptr->setTinn(max_hist, max_bin, binsBorders, bin_centers, hrv2Histogram->histogram);

    return hrv2Ptr;
}
