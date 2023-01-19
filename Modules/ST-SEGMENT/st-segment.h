#include <iostream>
#include <vector>

struct stseg {
    std::vector<int> Offsets;
    std::vector<int> j20tM;
    std::vector<int> j20tP;
    std::vector<int> j20tN;
    std::vector<int> TONpoints;
    std::vector<float> Slopes;
    std::vector<float> Slopes2;
    std::vector<float> J20tmdistance;
    std::vector<float> J20tONslopes;
    std::vector<float> J20tONslopes2;
    std::vector<float> J20tONmdistance;
    std::vector<int> J20tonN;
    std::vector<int> J20tonP;
    std::vector<int> J20tonM;
    std::vector<int> T1;
    std::vector<int> T2;
};


class STsegment{

    std::vector<int> J20points(std::vector <int> QRSend);
    std::vector<int> JXpoints(std::vector<int> J20points ,std::vector<int> Rpeaks);

    void T_OnSetPoints(std::shared_ptr<stseg> out, std::vector<int> J20Point, std::vector<float> FilteredSignal, std::vector<int> Tpeak);
    void J20Tdistance(std::shared_ptr<stseg> out, std::vector<int> J20points, std::vector<int> TONpoints, std::vector<float> filteredSignal);
    void Offset(std::shared_ptr<stseg> out,std::vector<float> filteredSignal, std::vector<int> QRSonset, std::vector<int> JXpoints);
    void STtype(std::shared_ptr<stseg> out);

public:
    STsegment();

    std::shared_ptr<stseg> finalSTfunction(std::vector <int> QRSend, std::vector <int> RPeaks, std::vector <float> FilteredSignal, std::vector <int> QRS_onset, std::vector <int> TPeak );

};




