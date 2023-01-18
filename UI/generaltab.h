#ifndef GENERALTAB_H
#define GENERALTAB_H

#include <QMainWindow>
#include <QWidget>
#include <QFileInfo>

//class GeneralTab : public QMainWindow
//{
//    Q_OBJECT
//public:
//    explicit GeneralTab(QWidget *parent = nullptr);

//signals:

//};

class GeneralTab : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralTab(const QFileInfo &fileInfo, QWidget *parent = nullptr);
};
#endif // GENERALTAB_H

