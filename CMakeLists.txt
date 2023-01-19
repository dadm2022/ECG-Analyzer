cmake_minimum_required(VERSION 3.23)
project(ECG_Analyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_C_COMPILER  /usr/bin/gcc)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Comment the following line on Windows (uncomment on Mac)
# DO NOT COMMIT THIS CHANGE
set(CMAKE_PREFIX_PATH "/Users/bartosz/Qt5.12.12/5.12.12/clang_64/lib/cmake/Qt5")
set(CMAKE_PREFIX_PATH "/Users/bartosz/Qt5.12.12/5.12.12/clang_64/lib/cmake/Qt5Charts")

add_subdirectory(Modules)
find_package(Qt5 COMPONENTS Widgets)
FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Charts
        REQUIRED)

add_executable(ECG_Analyzer main.cpp UI/ViewModelUI.cpp
        UI/ViewModelUI.h DataAccessor/DataAccessor.h
        DataAccessor/DataAccessor.cpp DataAccessor/wfdb.h DataAccessor/wfdb.cpp Modules/TWaveAlt/TWaveAlt.cpp
        DataController/DataController.h DataController/DataController.cpp Modules/ECG-Baseline/ECG_Baseline.h
        Modules/ECG-Baseline/ECG_Baseline.cpp Modules/WAVES/waves.h Modules/WAVES/waves.cpp
        Modules/HRV_DFA/DFA.h Modules/HRV_DFA/DFA.cpp
        Modules/HRV2/HRV2.h Modules/HRV2/HRV2.cpp Modules/HEART_CLASS/heart_class.h Modules/HEART_CLASS/heart_class.cpp
        Modules/ST-SEGMENT/st-segment.cpp Modules/ST-SEGMENT/st-segment.h
        UI/ViewModelUI.h UI/ViewModelUI.cpp
        UI/Tabsgraphs.h UI/Tabsgraphs.cpp
        UI/chartCallout.h UI/chartCallout.cpp
        UI/Visualization/ECG_chartCallout.h UI/Visualization/ECG_chartCallout.cpp
        UI/Visualization/Peaks_chartCallout.h UI/Visualization/Peaks_chartCallout.cpp
        UI/Visualization/SignalUtils.h UI/Visualization/SignalUtils.cpp
        UI/textBrowserName.h UI/textBrowserName.cpp)

target_link_libraries(ECG_Analyzer
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Charts
        ${Boost_LIBRARIES}
        )

target_link_libraries(ECG_Analyzer R_PEAKS)


