//
// Created by DiLLu on 07.01.2023.
//
#include <QTextBrowser>

#ifndef ECG_ANALYZER_TEXTBROWSERNAME_H
#define ECG_ANALYZER_TEXTBROWSERNAME_H


class textBrowserName : public QTextBrowser
{
    Q_OBJECT;

    public:
    explicit textBrowserName(QTextBrowser *parent = nullptr);


    public slots:
    void handleTextBrowserAge();
    void handleTextBrowserGender();
    void handleTextBrowserRRmean();
    void handleTextBrowserSDNN();
    void handleTextBrowserRMSSD();
    void handleTextBrowserNN50();
    void handleTextBrowserpNN50();
    void handleTextBrowserTP();
    void handleTextBrowserHF();
    void handleTextBrowserLF();
    void handleTextBrowserVLF();
    void handleTextBrowserULF();
    void handleTextBrowserLFHF();
    void handleTextBrowserTINN();
    void handleTextBrowserHRV();
    void handleTextBrowserSD1();
    void handleTextBrowserSD2();
    void handleTextBrowserALFA1();
    void handleTextBrowserALFA2();
    void handleTextBrowserSV();
    void handleTextBrowserV();
    void handleTextBrowserOTHER();
    void handleTextBrowserARTIFACT();
    void handleTextBrowserAMPLITUDEALT();
    void handleTextBrowserTHRESHOLD();
    void handleTextBrowserDISEASE();
    void handleTextBrowserCASEDISEASE();

    private:

};


#endif //ECG_ANALYZER_TEXTBROWSERNAME_H
