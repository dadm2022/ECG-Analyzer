#include <QApplication>
#include "UI/ViewModelUI.h"

#include "./DataAccessor/DataAccessor.h"
#include "./Modules/ECG-Baseline/ECG_Baseline.h"
#include "./Modules/Rpeaks/PanTompkins.h"
#include "./Modules/HRV1/HRV1.h"


int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    auto window = MainWindow();
//    window.show();
//    return a.exec();

    auto da = DataAccessor();
    da.load("/Users/marcelpikula/Desktop/Data/233.dat");
    auto a = da.signalCountGet();

    auto raw = da.at(0);


    auto filtered = std::make_shared<std::vector<float>>(ECG_Baseline(std::make_shared<std::vector<float>>(raw.data)).GetFilteredSignalLMSFilter());

    auto rpeaks = std::make_shared<std::vector<int>>(PanTompkins().GetPeaks(filtered));

    auto hrv1Params = HRV1(rpeaks).getCalculatedStatisticParams();

}
