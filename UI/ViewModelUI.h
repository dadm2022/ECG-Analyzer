//
// Created by Bartosz Łukasik on 16/11/2022.
//

#ifndef ECG_ANALYZER_VIEWMODELUI_H
#define ECG_ANALYZER_VIEWMODELUI_H
//#include <QtCharts>
#include "Tabsgraphs.h"

#include <QPushButton>
#include <QMainWindow>
#include <QLabel>
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QGridlayout>
#include <QTabWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QMenubar>
#include <QMenu>
#include <QGroupBox>
#include <QDebug>
#include <QObject>
#include <QFileDialog>
#include <QString>
#include <QFileInfo>
#include <QRadioButton>
#include <QListWidget>
#include <QButtonGroup>
#include <QRadioButton>
#include <QEvent>
#include "../DataController/DataController.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT;

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        friend class string;

    private slots:
        std::string handleOpenActionClicked();
    private:
        void initializeDataController();
        std::string pathToFile;
        std::shared_ptr<DataController> datacontroller = std::make_shared<DataController>(DataController());
};

#endif //ECG_ANALYZER_VIEWMODELUI_H
