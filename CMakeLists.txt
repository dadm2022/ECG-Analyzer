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
set(CMAKE_PREFIX_PATH "/Users/marcelpikula/Qt/6.4.1/macos/lib/cmake/Qt6")


FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_subdirectory(Modules)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(ECG_Analyzer main.cpp Modules/Sample/Sample.h Modules/Sample/Sample.cpp UI/ViewModelUI.cpp
        UI/ViewModelUI.h DataAccessor/DataAccessor.h
        DataAccessor/DataAccessor.cpp DataAccessor/wfdb.h DataAccessor/wfdb.cpp Modules/HRV1/HRV1.cpp Modules/HRV1/HRV1.h
        Modules/HRV1/HRV1.cpp Modules/HRV1/HRV1.h Modules/ECG-Baseline/ECG_Baseline.cpp Modules/ECG-Baseline/ECG_Baseline.h
        )


target_link_libraries(ECG_Analyzer
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        ${Boost_LIBRARIES}
        )

target_link_libraries(ECG_Analyzer R_PEAKS)