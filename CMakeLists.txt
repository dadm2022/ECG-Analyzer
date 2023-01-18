cmake_minimum_required(VERSION 3.23)
project(ECG_Analyzer)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_COMPILER /usr/bin/g++)
#set(CMAKE_C_COMPILER  /usr/bin/gcc)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Comment the following line on Windows (uncomment on Mac)
# DO NOT COMMIT THIS CHANGE
#set(CMAKE_PREFIX_PATH "E:/Qt/5.15.11/mingw81_64/lib/cmake/Qt5")

add_subdirectory(Modules)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Charts
        REQUIRED)

add_executable(ECG_Analyzer
        main.cpp
        Modules/Sample/Sample.h
        Modules/Sample/Sample.cpp
        UI/ViewModelUI.cpp
        UI/ViewModelUI.h
        UI/tabdialog.h
        UI/chartCallout.h
        UI/chartCallout.cpp
        UI/generaltab.cpp
        UI/textBrowserName.cpp
        UI/Tabsgraphs.cpp UI/Visualization/ECG_chartCallout.cpp UI/Visualization/Peaks_chartCallout.cpp UI/Visualization/SignalUtils.cpp)

target_link_libraries(
        ECG_Analyzer
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Charts
)

target_link_libraries(ECG_Analyzer)