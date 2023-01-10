#include <iostream>
#include <vector>
#include <complex>
#include <iostream>
#include <valarray>
#include <cmath>
#include <numeric>
#include <cstdlib>
#include <algorithm>
#include <memory>
#include <string>


using namespace std;
class STsegment{
public:
    STsegment();

    vector<int> J20points(std::vector <int> QRSend);
    std::tuple<vector<float>, vector<int>,vector<int>,vector<int>,vector<int>,vector<float>  >T_OnSetPoints(vector<int> J20Point, vector<float> FilteredSignal, vector<int> Tpeak);

    std::tuple<vector<float>, vector<int>, vector<int>,vector<int>,vector<float>  >calculateJ20_TON_distance(vector<int> J20points, vector<int> TONpoints, vector<float> filteredSignal);

    vector<int> calculateJpoints(vector<int> J20points ,vector<int> Rpeaks);

    vector<string> calculateOffsets(vector<float> filteredSignal, vector<int> QRSonset, vector<int> JXpoints);

    void determineSTtype(vector <string> Offsets, vector<float> Slopes, vector<float> J20_Tpeak_max_dist,
                         vector<float> J20_TON_max_dist, vector<float> J20_TON_slopes, vector<int> J20_Tpeak_N,
                         vector<int> J20_Tpeak_M, vector<int> J20_Tpeak_P, vector<int> J20_TON_N,
                         vector<int> J20_TON_P, vector<int> J20_TON_M);


    };




