//
// Created by Piotr on 05.01.2023.
//

#ifndef ECG_ANALYZER_TABDIALOG_H
#define ECG_ANALYZER_TABDIALOG_H

#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <QDialog>

#include "./../Modules/Rpeaks/HilbertTransform.h"
#include "./../Modules/Rpeaks/PanTompkins.h"

class QTabWidget;

class QDialogButtonBox;

class TabDialog : public QDialog {
Q_OBJECT
public:
    explicit TabDialog(const QString &fileName, QWidget *parent = nullptr);

private:
//    std::shared_ptr<HRV2> createHrv2Ptr(const std::shared_ptr<R_peaks::R_Peaks_Pakiet>& detectedRPeaks);
    QTabWidget *tabWidget;
    QDialogButtonBox *buttonBox;
};

#endif // TABDIALOG_H


#endif //ECG_ANALYZER_TABDIALOG_H
