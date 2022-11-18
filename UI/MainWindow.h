//
// Created by Bartosz Łukasik on 16/11/2022.
//

#ifndef ECG_ANALYZER_MAINWINDOW_H
#define ECG_ANALYZER_MAINWINDOW_H

#include <QPushButton>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT;

    public:
        explicit MainWindow(QWidget *parent = nullptr);

    private:
        std::shared_ptr<QPushButton> m_button;
};

#endif //ECG_ANALYZER_MAINWINDOW_H
