#include <iostream>
#include <vector>
#include <fstream>
#include "./heart_class.h"

/* Dlugosc odcinka RS */
#define R_S_INTERVAL            60

/* Ilosc interwalow uzywanych do dysocjacji AV */
#define NUM_OF_INTERVALS        10

/* Odchylenie standarowe (AVDissociation) */
#define STD_TH                  10

int SamplesToMiliseconds(int value, int fs)
{
    return value*1000/fs;
}

enum type heart_class::CheckAVDissociation(std::vector<int> rPeaks, std::vector<int> P, int currR, std::vector<int> QRSonset1) {

    std::vector<int> intervals;
    std::vector<int> QRSonset = QRSonset1;

    /* Dlugosc odcinka PQ */
    auto itQRS = std::upper_bound(QRSonset.begin(), QRSonset.end(), currR);
    auto itP = std::upper_bound(P.begin(), P.end(), currR);
    int currInterval;

    for(uint8_t i = 0; i < NUM_OF_INTERVALS; i++)
    {
        /* Sprawdzenie czy jest koniec sygnalu */
        if(itQRS == QRSonset.end() || itP == P.end())
            return DIFF_DISEASE;

        currInterval = *itQRS++ - *itP++;
        intervals.push_back(currInterval);
    }

    /* Obliczenie odchylenia standardowego dla danych interwalow */
    int sum = std::accumulate(intervals.begin(), intervals.end(), 0.0);
    int mean = sum/intervals.size();
    int stdDev = 0;
    int temp;
    for(const auto &el : intervals)
    {
        temp = (el-mean)*(el-mean);
        stdDev += temp;
    }
    stdDev /= intervals.size();
    stdDev = std::sqrt(stdDev);

    if(stdDev < STD_TH)
        return DIFF_DISEASE;
    else
        return VENTRICULAR;
}

void heart_class::Classify_Type(std::vector<int> rPeaks, std::vector<int> P, std::vector<int> QRSend, std::vector<int> QRSonset, int fs)
    {
    activations_t nextClassification;
    auto S = QRSend;

    for(const auto &r : rPeaks)
    {
        /* Szukamy zalamka Q */
        auto it_q = std::lower_bound(m_QRSonset.begin(), m_QRSonset.end(), r);
        it_q -= 1;

        /* Szukamy zalamka S */
        auto it_s = std::upper_bound(S.begin(), S.end(), r); 
        
        if(it_s != S.end())
        {
            int intervalRS = SamplesToMiliseconds(*it_s - r , fs);
            int QRS = *it_s - *it_q;
            int QRSms = SamplesToMiliseconds(QRS, fs);

            if(QRSms < 180 && QRSms > 50)  /* sprawdzenie czy to artefakt czy QRS */
            {
                if(QRSms >= 120)
                {
                    /* szeroki QRS -> VR albo inna choroba */
                    if(intervalRS < R_S_INTERVAL )
                    {
                        /* RS jest krotki
                         * Sprawdzamy czy jest dysocjacja */
                        nextClassification.actType = CheckAVDissociation(rPeaks, P, r, m_QRSonset);
                        if(nextClassification.actType == 1)
                        {
                            /* jest dysocjacja -> VT */
                            nextClassification.actType = VENTRICULAR;
                        }
                        else 
                        {
                            /* nie ma dysocjacji -> inna choroba niemozliwa do okreslenia */
                            nextClassification.actType = DIFF_DISEASE;
                        }
                    }
                    else
                    {
                        /* RS jest dlugi -> VT */
                        nextClassification.actType = VENTRICULAR;
                    }
                }
                else
                {
                    /* krotki QRS -> SVT */
                    nextClassification.actType = SUPRAVENTRICULAR;
                }
            } else
            {
                nextClassification.actType = ARTIFACT;
            }
            nextClassification.Ridx = r;
            this->activations.push_back(nextClassification);
        }
    }
}