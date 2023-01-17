//
// Created by Bartosz Łukasik on 17/01/2023.
//
#include "DataController.h"

DataController::DataController(std::string pathToFile)
{
    dataAccessor.load(pathToFile);
    if (dataAccessor.isLoaded())
    {
        m_signalDetails.numberOfSignals = dataAccessor.signalCountGet();
        m_signalDetails.sex = dataAccessor.sexGet();
        m_signalDetails.age = dataAccessor.ageGet();
        m_rawSignal = dataAccessor.at(0).data;
        ecgBaseline = ECG_Baseline(std::make_shared<std::vector<float>>(m_rawSignal));
    }
}

const std::vector<float> DataController::getLMSFilteredSignal()
{
    if(m_LMSFilteredSignal.empty())
        m_LMSFilteredSignal = ecgBaseline.GetFilteredSignalLMSFilter();

    return m_LMSFilteredSignal;
}
const std::vector<float> DataController::getButterworthFilteredSignal()
{
    if(m_ButterworthFilteredSignal.empty())
        m_ButterworthFilteredSignal = ecgBaseline.GetFilteredSignalButterworthFilter();

    return m_ButterworthFilteredSignal;
}
const std::vector<float> DataController::getMovingAverageFilteredSignal()
{
    if(m_MovingAverageFilteredSignal.empty())
        m_MovingAverageFilteredSignal = ecgBaseline.GetFilteredSignalMovingAverageFilter();

    return m_MovingAverageFilteredSignal;
}

const std::vector<int> DataController::getHilbertRPeaks(std::shared_ptr<std::vector<float>> filteredSignal)
{
    if(hilbertTransformRPeaks.empty())
    {
        auto handler = HilbertTransform();
        hilbertTransformRPeaks = handler.GetPeaks(filteredSignal);
    }

    return hilbertTransformRPeaks;
}

const std::vector<int> DataController::getPanTompkinsRPeaks(std::shared_ptr<std::vector<float>> filteredSignal)
{
    if(panTompkinsRPeaks.empty())
    {
        auto handler = PanTompkins();
        panTompkinsRPeaks = handler.GetPeaks(filteredSignal);
    }

    return panTompkinsRPeaks;
}

const Waves DataController::getWaves(std::vector<float> filteredSignal, std::vector<int> rPeaks)
{
    if (waves.QRSonset.empty() && waves.QRSend.empty() && waves.Tend.empty() && waves.Ponset.empty() && waves.Pend.empty())
    {
        auto handler = WavesDetector(filteredSignal, rPeaks);
        waves.QRSonset = handler.getQRSonset();
        waves.QRSend = handler.getQRSend();
        waves.Tend = handler.getTend();
        waves.Ponset = handler.getPonset();
        waves.Pend = handler.getPend();
    }
    return waves;
}


