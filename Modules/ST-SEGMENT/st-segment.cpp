#include <memory>
#include "st-segment.h"
#include <cmath>
#include <algorithm>





STsegment::STsegment(){}


std::vector<int> STsegment::J20points(std::vector<int> QRSend) {
// ta funkcja oblicza punkty J20, czyli punkt przesunięty o 20 względem końca załamka QRS
    std::vector<int> J20points;

    std::for_each(QRSend.begin(), QRSend.end(), [&J20points](int &i){
        J20points.push_back(i + 20);
    });
    return J20points;
}

std::vector<int> STsegment::JXpoints(std::vector<int> J20points , std::vector<int> Rpeaks){
// ta funkcja oblicza punkty JX, czyli punkty przesuniete o X wzgledem punktu J20
    std::vector <int> JXpoints ;
    int hr;

    for(int i = 0; i< J20points.size(); i++){
        hr =  60 / (Rpeaks[i] != 0 ? (360 * Rpeaks[i]) : 1);
        if(hr <= 100){
            JXpoints.push_back(J20points[i] + 28.8);}
        else if (hr> 100 && hr <= 110){
            JXpoints.push_back(J20points[i] + 25.92);}
        else if (hr > 110 && hr <= 120){
            JXpoints.push_back(J20points[i] + 23.04);}
        else if (hr > 120){
            JXpoints.push_back(J20points[i] + 21.6);
        }}
    return JXpoints;
}

void STsegment::Offset( std::shared_ptr<stseg> out, std::vector<float> filteredSignal, std::vector<int> QRSonset, std::vector<int> JXpoints){

    float offset;
    int lower = 0;
    int higher = 1;
    int normal = 2;
    float K1 = -0.1;
    float K2 = 0.1;
    for(int i=0;i<JXpoints.size();i++){
        // y(JX) - y(ISO)
        offset = filteredSignal[JXpoints[i]] - filteredSignal[QRSonset[i]];
        if(offset < K1){
            out->Offsets.push_back(lower);}
        else if (offset > K2){
            out->Offsets.push_back(higher);}
        else{
            out->Offsets.push_back(normal);}
    }
}

void STsegment::T_OnSetPoints(std::shared_ptr<stseg> out, std::vector<int> J20Point, std::vector<float> FilteredSignal, std::vector<int> Tpeak){

int  pnt, j20 = 0;
float signal_j20, dist, distance, slope = 0.0;

for (int i=0;i<J20Point.size();i++){
    slope = (FilteredSignal[Tpeak[i]]-FilteredSignal[j20])/ (float(Tpeak[i]) - float(J20Point[i]));
out->Slopes.push_back(slope);
out->Slopes2.push_back(FilteredSignal[Tpeak[i]]-slope*(Tpeak[i] - J20Point[i]));
}
for (int i=0; i<J20Point.size(); i++){
int P,M,N,j = 0;
float line, signal = 0.0;
int point = J20Point[i];

    for (int k = J20Point[i]+3; k < Tpeak[i]; k++){
    float slope = out->Slopes2[i];
    line = slope * (float)(k - point) + out->Slopes2[i];
    signal = FilteredSignal[k];
    dist = sqrt((line- signal)*(line - signal));
    if(dist > distance){
        pnt = k;
        distance = dist;}
    if(line > signal){
        M=M+1;
        N=N+1;}
    else if (line < signal){
        P=P + 1;
        M=M + 1;}else{
    M=M+1;}
j=j+1;
}

out->TONpoints.push_back(pnt);
out->J20tmdistance.push_back(distance);
out->j20tN.push_back(N);
out->j20tP.push_back(P);
out->j20tM.push_back(M);

distance = 0.0;
pnt = 0;
}
}

void STsegment::J20Tdistance(std::shared_ptr<stseg> out, std::vector<int> J20points, std::vector<int> TONpoints, std::vector<float> filteredSignal){

    float distance, dist, slope = 0.0;
    for (int i=0; i < J20points.size(); i++){
        slope = (filteredSignal[TONpoints[i]]-filteredSignal[J20points[i]])/ float(TONpoints[i] - J20points[i]);
        out->J20tONslopes.push_back(slope);
        out->J20tONslopes2.push_back(filteredSignal[TONpoints[i]]- slope * (TONpoints[i] - J20points[i]));
    }

    for (int i=0; i <  J20points.size(); i++){
        int M,P,N,j = 0;
        int probka0 = J20points[i];
        for (int p= J20points[i]; p <  TONpoints[i]; p++){
            float slope = out->J20tONslopes[i];
            float val_line = slope * (float)(p - probka0) / 360 + out->J20tONslopes2[i];
            dist = sqrt((val_line - filteredSignal[p])*(val_line - filteredSignal[p]));
            if(dist > distance){
                distance = dist;}
            if(val_line > filteredSignal[p]){
                N=N+1;
                M=M + 1;}
            else if (val_line < filteredSignal[p]){
                M=M+1;
                P=P+1;}
            else{
                M = M + 1;}
            j = j + 1;}

        out->J20tONmdistance.push_back(distance);
        out->J20tonM.push_back(M);
        out->J20tonN.push_back(N);
        out->J20tonP.push_back(P);

        distance = 0.0;
        j = 0;
    }
}


void STsegment::STtype( std::shared_ptr<stseg> out){
    int prosty = 0;
    int dol = 1;
    int gora = 2;
    int poziomy = 3;
    int zakrzywiony = 4;
    int wypukly = 5;
    int wklesly = 6;
    int nieokreslony = 7;
    float max_distance, slope;
    for(int i=0; i < out->Offsets.size() ;i++) {
        if (out->Offsets[i] == 0){
            max_distance = out->J20tmdistance[i];
            slope = out->Slopes[i];
        }else{
            max_distance = out->J20tONmdistance[i];
            slope = out->J20tONslopes[i];
        }if(max_distance < 0.15){
            out->T1.push_back(0);
            float k1 = 0.15;
            float k2 = -0.15;
            if(slope < k2){
                out->T2.push_back(dol);}
            else if(slope > k1){
                out->T2.push_back(gora);}
            else{
                out->T2.push_back(poziomy);}} else{
            out->T1.push_back(zakrzywiony);
            int N,P,M;
            if (out->Offsets[i] == 0){
                N = out->j20tN[i];
                P = out->j20tP[i];
                M = out->j20tM[i];
            }else{
                N = out->J20tonN[i];
                P = out->J20tonP[i];
                M = out->J20tonM[i];
            }if( P/M > 0.7){
                out->T2.push_back(wypukly);}
            else if (N/M > 0.7){
                out->T2.push_back(wklesly);}
            else{
                out->T2.push_back(nieokreslony);}
        }
    }

}

std::shared_ptr<stseg>  STsegment::finalSTfunction(std::vector <int> QRSend,std::vector <int> RPeaks, std::vector <float> FilteredSignal, std::vector <int> QRS_onset, std::vector <int> TPeak  ){

    std::shared_ptr<stseg> out(new stseg);
//    auto st = STsegment();
    std::vector <int> J20point = this->J20points(QRSend);
    std::vector <int> jpoints = this->JXpoints(J20point , RPeaks);
    this->Offset(out, FilteredSignal,QRS_onset, jpoints);
    this->T_OnSetPoints(out, J20point, FilteredSignal, TPeak);

    this->J20Tdistance(out, J20point, TPeak, FilteredSignal);
    //this->J20Tdistance(out,J20point, FilteredSignal);
    this->STtype(out);

    return out;
}















