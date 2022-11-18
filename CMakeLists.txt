cmake_minimum_required(VERSION 3.23)
project(ECG_Analyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Comment the following line on Windows (uncomment on Mac)
# DO NOT COMMIT THIS CHANGE
set(CMAKE_PREFIX_PATH "/Users/marcelpikula/Qt/6.4.1/macos/lib/cmake/Qt6")


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(ECG_Analyzer main.cpp
        Modules/Sample/Sample.h
        Modules/Sample/Sample.cpp
        UI/MainWindow.cpp
        UI/MainWindow.h
        UI/ViewmodelUI.cpp)

target_link_libraries(ECG_Analyzer
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        )

