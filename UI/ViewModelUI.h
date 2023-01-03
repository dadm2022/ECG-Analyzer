//
// Created by Bartosz Łukasik on 16/11/2022.
//

#ifndef ECG_ANALYZER_VIEWMODELUI_H
#define ECG_ANALYZER_VIEWMODELUI_H

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


class MainWindow : public QMainWindow
{
    Q_OBJECT;

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        std::shared_ptr<QPushButton> pushbuttonOpenFile;
    public slots:
        void handleButton();
    private:
//        QPushButton *pushbuttonOpenFile;
        std::shared_ptr<QPushButton> m_button;
//        std::shared_ptr<QPushButton> pushbuttonOpenFile;

        std::shared_ptr<QHBoxLayout> horizontalLayoutAge;
        std::shared_ptr<QHBoxLayout> horizontalLayoutGender;



};

#endif //ECG_ANALYZER_VIEWMODELUI_H
