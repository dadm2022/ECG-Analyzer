cmake_minimum_required(VERSION 3.23)
project(ECG_Analyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Comment the following line on Windows (uncomment on Mac)
# DO NOT COMMIT THIS CHANGE
set(CMAKE_PREFIX_PATH "C:\Qt\5.15.11\mingw81_64\lib\cmake\Qt5")


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(ECG_Analyzer main.cpp Modules/Sample/Sample.h Modules/Sample/Sample.cpp UI/ViewModelUI.cpp UI/ViewModelUI.h)
target_link_libraries(ECG_Analyzer
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

