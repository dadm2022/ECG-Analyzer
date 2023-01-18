//
// Created by DiLLu on 07.01.2023.
//

#include "textBrowserName.h"

textBrowserName::textBrowserName(QTextBrowser *parent) : QTextBrowser(parent) {

}
void textBrowserName::handleTextBrowserAge()
{   // wywloanie funkcji modułu dataAcessor
    this->setText("12");
}
void textBrowserName::handleTextBrowserGender()
{
    this->setText("Male");
}
void textBrowserName::handleTextBrowserRRmean()
{
    this->setText("1.2");
}
void textBrowserName::handleTextBrowserSDNN()
{
    this->setText("12");
}
void textBrowserName::handleTextBrowserRMSSD()
{
    this->setText("342");
}
void textBrowserName::handleTextBrowserNN50()
{
    this->setText("567.8");
}
void textBrowserName::handleTextBrowserpNN50()
{
    this->setText("456.1");
}
void textBrowserName::handleTextBrowserTP()
{
    this->setText("234");
}
void textBrowserName::handleTextBrowserHF()
{
    this->setText("21");
}
void textBrowserName::handleTextBrowserLF()
{
    this->setText("42");
}
void textBrowserName::handleTextBrowserVLF()
{
    this->setText("16");
}
void textBrowserName::handleTextBrowserULF()
{
    this->setText("9.2");
}
void textBrowserName::handleTextBrowserLFHF()
{
    this->setText("8.2");
}

// hrv - GEOMETRIC ANALYSIS
void textBrowserName::handleTextBrowserTINN()
{
    this->setText("16.2");
}
void textBrowserName::handleTextBrowserHRV()
{
    this->setText("24.2");
}
void textBrowserName::handleTextBrowserSD1()
{
    this->setText("40.2");
}
void textBrowserName::handleTextBrowserSD2()
{
    this->setText("49.2");
}
void textBrowserName::handleTextBrowserALFA1()
{
    this->setText("77");
}
void textBrowserName::handleTextBrowserALFA2()
{
    this->setText("23");
}
void textBrowserName::handleTextBrowserSV()
{
    this->setText("0.99");
}
void textBrowserName::handleTextBrowserV()
{
    this->setText("4.63");
}
void textBrowserName::handleTextBrowserOTHER()
{
    this->setText("71.59");
}
void textBrowserName::handleTextBrowserARTIFACT()
{
    this->setText("18.18");
}
void textBrowserName::handleTextBrowserAMPLITUDEALT()
{
    this->setText("45");
}
void textBrowserName::handleTextBrowserTHRESHOLD()
{
    this->setText("Tak"); // boolowska wartosc zawiadamiajaca o przekroczeniu progu
}
void textBrowserName::handleTextBrowserDISEASE()
{
    this->setText("Nie");
}
void textBrowserName::handleTextBrowserCASEDISEASE()
{
    this->setText("Ostra przypadłość choroby.");
}