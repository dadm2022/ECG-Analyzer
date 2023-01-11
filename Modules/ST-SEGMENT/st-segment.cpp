#include <memory>
#include "st-segment.h"
#include <iostream>
#include <string>


#define FREQUENCY_360 360
using namespace std;


STsegment::STsegment(){}



vector<int> STsegment::J20points(vector<int> QRSend) {
// ta funkcja oblicza punkty J20, czyli punkt przesunięty o 20 względem końca załamka QRS
    vector<int> J20points;
    for(int i=0; i<QRSend.size(); i++) {
         J20points.push_back(QRSend[i] + 20);

    }
    return J20points;
}

vector<int> STsegment::calculateJpoints(vector<int> J20points , vector<int> Rpeaks){
// ta funkcja oblicza punkty JX, czyli punkty przesuniete o X wzgledem punktu J20
    vector<int> JXpoints;
    int heart_rate,point;
    float X80 = FREQUENCY_360 * 0.08;
    float X72 = FREQUENCY_360 * 0.072;
    float X64 = FREQUENCY_360 * 0.064;
    float X60 = FREQUENCY_360 * 0.06;


    for(int licznik = 0; licznik < J20points.size(); licznik++){
        heart_rate =  60 / (Rpeaks[licznik] != 0 ? (FREQUENCY_360 * Rpeaks[licznik]) : 1);
        point = J20points[licznik];
        if(heart_rate <= 100){
            JXpoints.push_back(point + X80);
        }
        else if (heart_rate> 100 && heart_rate <= 110){
            JXpoints.push_back(point + X72);
        }
        else if (heart_rate > 110 && heart_rate <= 120){
            JXpoints.push_back(point + X64);
        }
        else if (heart_rate > 120){
            JXpoints.push_back(point + X60);
        }}
    return JXpoints;
}

vector <string> STsegment::calculateOffsets(vector<float> filteredSignal, vector<int> QRSonset, vector<int> JXpoints){
    float offset;
    vector <string> Offsets ;
    for(int i=0;i<JXpoints.size();i++){
        offset = filteredSignal[JXpoints[i]] - filteredSignal[QRSonset[i]];
        cout << offset;
        if(offset < -0.1){
            Offsets.push_back("Obnizony");}
        else if (offset > 0.1){
            Offsets.push_back("Podwyzszony");}
        else{
            Offsets.push_back("Normalny");}
    }
    return Offsets;
}





std::tuple<vector<float>, vector<int>,vector<int>,vector<int>,vector<int>, vector<float> >STsegment::T_OnSetPoints(vector<int> J20Point, vector<float> FilteredSignal, vector<int> Tpeak){

int tpeak, no_probka,j20 = 0;
float signal_j20, signal_l, signal_t, dystans_temp, dystans, slope = 0.0;
vector<int> J20_Tpeak_M;
vector<int> J20_Tpeak_P;
vector<int> J20_Tpeak_N;
vector<int> TONpoints;
vector<float> Slopes;
vector<float> Intercept;
vector<float> J20_Tpeak_max_dist;

for (int licznik=0; licznik < J20Point.size(); licznik++){
tpeak = Tpeak[licznik];
signal_t = FilteredSignal[tpeak];
j20 = J20Point[licznik];
signal_j20 = FilteredSignal[j20];
slope = (signal_t-signal_j20)/ (float(tpeak) - float(j20));

Slopes.push_back(slope);
Intercept.push_back(signal_t - slope * (tpeak - j20));
}

for (int licznik=0; licznik < J20Point.size(); licznik++){
int n = 0;
int p = 0;
int m = 0;
float wart_prosta = 0.0;
float wart_signal = 0.0;
int i = 0;
int probka1 = J20Point[licznik];

for (int probka= J20Point[licznik]+3; probka < Tpeak[licznik]; probka++){


float slope = Slopes[licznik];
wart_prosta = slope * (float)(probka - probka1) + Intercept[licznik];
wart_signal = FilteredSignal[probka];
dystans_temp = sqrt((wart_prosta - wart_signal)*(wart_prosta - wart_signal));
if(dystans_temp > dystans)
{
no_probka = probka;
dystans = dystans_temp;
}
if(wart_prosta > wart_signal)
{
//Tu sygnal jest pod prosta
n = n + 1;
m = m + 1;
}
else if (wart_prosta < wart_signal)
{
//Tu sygnal jest nad prosta
p = p + 1;
m = m + 1;
}
else
{
//Sygnal jest na prostej
m = m + 1;
}
i = i + 1;
}

TONpoints.push_back(no_probka);


J20_Tpeak_max_dist.push_back(dystans);


J20_Tpeak_N.push_back(n);
J20_Tpeak_P.push_back(p);
J20_Tpeak_M.push_back(m);

dystans = 0.0;
no_probka = 0;
}
return std::make_tuple(Slopes, TONpoints, J20_Tpeak_N,J20_Tpeak_P,J20_Tpeak_M,J20_Tpeak_max_dist );

}

std::tuple<vector<float>, vector<int>, vector<int>,vector<int>, vector<float> >STsegment::calculateJ20_TON_distance(vector<int> J20points, vector<int> TONpoints, vector<float> filteredSignal){

    float dystans , dystans_temp, signal_ton, signal_j20, slope = 0.0;
    int TON, j20 = 0;
    vector<float> J20_TON_slopes;
    vector<float> J20_TON_intercepts;
    vector<float> J20_TON_max_dist;
    vector<int> J20_TON_N;
    vector<int> J20_TON_P;
    vector<int> J20_TON_M;

    for (int licznik=0; licznik < J20points.size(); licznik++){
        TON = TONpoints[licznik];
        signal_ton = filteredSignal[TON];
        j20 = J20points[licznik];
        signal_j20 = filteredSignal[j20];
        slope = (signal_ton-signal_j20)/ float(TON - j20);

        J20_TON_slopes.push_back(slope);
        J20_TON_intercepts.push_back(signal_ton - slope * (TON - j20));
    }

    for (int licznik=0; licznik <  J20points.size(); licznik++){
        int n = 0;
        int p = 0;
        int m = 0;
        float wart_prosta;
        float wart_signal;
        int i = 0;
        int probka0 = J20points[licznik];
        for (int probka=  J20points[licznik]; probka <  TONpoints[licznik]; probka++){

            float slope = J20_TON_slopes[licznik];
            wart_prosta = slope * (float)(probka - probka0) / FREQUENCY_360 + J20_TON_intercepts[licznik];
            wart_signal = filteredSignal[probka];
            dystans_temp = sqrt((wart_prosta - wart_signal)*(wart_prosta - wart_signal));
            if(dystans_temp > dystans){
                dystans = dystans_temp;
            }

            if(wart_prosta > wart_signal){
                //Tu sygnal jest pod prosta
                n = n + 1;
                m = m + 1;
            }
            else if (wart_prosta < wart_signal){
                //Tu sygnal jest nad prosta
                p = p + 1;
                m = m + 1;
            }
            else{
                //Sygnal jest na prostej
                m = m + 1;
            }
            i = i + 1;
        }

        J20_TON_max_dist.push_back(dystans);
        J20_TON_N.push_back(n);
        J20_TON_P.push_back(p);
        J20_TON_M.push_back(m);
        dystans = 0.0;
        i = 0;
    }
    return std::make_tuple(J20_TON_max_dist,J20_TON_N,J20_TON_P,J20_TON_M,J20_TON_slopes );
}

void STsegment::determineSTtype(vector <string> Offsets, vector<float> Slopes, vector<float> J20_Tpeak_max_dist,
                                vector<float> J20_TON_max_dist, vector<float> J20_TON_slopes, vector<int> J20_Tpeak_N,
                                vector<int> J20_Tpeak_M, vector<int> J20_Tpeak_P, vector<int> J20_TON_N,
                                vector<int> J20_TON_P, vector<int> J20_TON_M){

    float max_dystans;
    float slope;
    vector <string> Types1, Types2;
    for(int licznik=0; licznik < Offsets.size() ;licznik++) {
        if (Offsets[licznik] == "Podwyzszony"){
            max_dystans = J20_Tpeak_max_dist[licznik];
            slope = Slopes[licznik];
        }
        else{
            max_dystans = J20_TON_max_dist[licznik];
            slope = J20_TON_slopes[licznik];
        }
        if(max_dystans < 0.15){
            //Odcinek jest prosty, wyznaczamy czy jest nachylony w gore/w dol/prosty
            Types1.push_back("Prosty");
            float k1 = 0.15;
            float k2 = -0.15;
            if(slope < k2){
                Types2.push_back("N. w dol");
            }
            else if(slope > k1){
                Types2.push_back("N. w gore");
            }
            else{
                Types2.push_back("Poziomy");
            }
        }
        else{
            Types1.push_back("Zakrzywiony");
            int N;
            int P;
            int M;
            if (Offsets[licznik] == "Podwyzszony")
            {
                N = J20_Tpeak_N[licznik];
                P = J20_Tpeak_P[licznik];
                M = J20_Tpeak_M[licznik];

            }
            else
            {
                N = J20_TON_N[licznik];
                P = J20_TON_P[licznik];
                M = J20_TON_M[licznik];
            }

            if( P/M > 0.7){
                Types2.push_back("Wypukly");
            }
            else if (N/M > 0.7){
                Types2.push_back("Wklesly");
            }
            else{
                Types2.push_back("Nieokreslony");
            }

        }

    }
    cout << "Typ: " << Types2[0] << endl;
}









