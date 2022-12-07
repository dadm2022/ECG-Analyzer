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
set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt@5/5.15.7/lib/cmake/Qt5")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(ECG_Analyzer main.cpp Modules/Sample/Sample.h Modules/Sample/Sample.cpp UI/ViewModelUI.cpp UI/ViewModelUI.h DataAccessor/DataAccessor.h DataAccessor/DataAccessor.cpp DataAccessor/wfdb.h DataAccessor/wfdb.cpp)
target_link_libraries(ECG_Analyzer
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )
