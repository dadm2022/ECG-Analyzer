//
// Created by DiLLu on 07.01.2023.
//

#include "textBrowserName.h"
#include <sstream>

textBrowserName::textBrowserName(QTextBrowser *parent, std::shared_ptr<DataController> datacontroller) : QTextBrowser(parent) {
    dataController = datacontroller;
}
void textBrowserName::handleTextBrowserAge()
{
    std::stringstream age;
    age << dataController->getAge();
    this->setText(QString::fromStdString(age.str()));
}
void textBrowserName::handleTextBrowserGender()
{   auto sex = dataController->getSex();
    this->setText(QString(sex));
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
    auto filteredSignal = dataController->getButterworthFilteredSignal();
    auto rPeaks = dataController->getPanTompkinsRPeaks(std::make_shared<std::vector<float>>(filteredSignal));
    auto ALFA1 = dataController->getAlpha1(std::make_shared<std::vector<int>>(rPeaks));
    std::stringstream alfa1;
    alfa1 << ALFA1;
    this->setText(QString::fromStdString(alfa1.str()));
}
void textBrowserName::handleTextBrowserALFA2()
{
    auto filteredSignal = dataController->getButterworthFilteredSignal();
    auto rPeaks = dataController->getPanTompkinsRPeaks(std::make_shared<std::vector<float>>(filteredSignal));
    auto ALFA2 = dataController->getAlpha2(std::make_shared<std::vector<int>>(rPeaks));
    std::stringstream alfa2;
    alfa2 << ALFA2;
    this->setText(QString::fromStdString(alfa2.str()));
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
    auto filteredSignal = dataController->getLMSFilteredSignal();
    auto rpeaks = dataController->getPanTompkinsRPeaks(std::make_shared<std::vector<float>>(filteredSignal));
    auto waves = dataController->getWaves(filteredSignal, rpeaks);
    auto dupa = std::make_shared<const std::vector<int>>(waves.Tend);
    auto siusiak = std::make_shared<const std::vector<float>>(filteredSignal);
    auto value = dataController->getAlternansValue(dupa, siusiak);
    std::stringstream strValue;
    strValue << value;
    this->setText(QString::fromStdString(strValue.str()));
}
void textBrowserName::handleTextBrowserTHRESHOLD()
{
    auto filteredSignal = dataController->getLMSFilteredSignal();
    auto rpeaks = dataController->getPanTompkinsRPeaks(std::make_shared<std::vector<float>>(filteredSignal));
    auto waves = dataController->getWaves(filteredSignal, rpeaks);
    auto dupa = std::make_shared<const std::vector<int>>(waves.Tend);
    auto siusiak = std::make_shared<const std::vector<float>>(filteredSignal);
    auto value = dataController->getAlternans(dupa, siusiak);
    this->setText(value ? "Tak" : "Nie");
}
void textBrowserName::handleTextBrowserDISEASE()
{
    this->setText("Nie");
}
void textBrowserName::handleTextBrowserCASEDISEASE()
{
    this->setText("Ostra przypadłość choroby.");
}
