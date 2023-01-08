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