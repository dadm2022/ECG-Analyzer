cmake_minimum_required(VERSION 3.23)
project(ECG_Analyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
# GSL configuration
set(GCC_COVERAGE_COMPILE_FLAGS "-fexceptions -DGSL_COMPLEX_LEGACY")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

# Comment the following line on Windows (uncomment on Mac)
# DO NOT COMMIT THIS CHANGE
set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt@5/5.15.7/lib/cmake/Qt5")

add_subdirectory(DataAccessor)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
find_package(GSL REQUIRED)

add_executable(ECG_Analyzer main.cpp Modules/Sample/Sample.h Modules/Sample/Sample.cpp UI/ViewModelUI.cpp UI/ViewModelUI.h Modules/Rpeaks/HilbertTransform.h Modules/Rpeaks/HilbertTransform.cpp Modules/Rpeaks/PanTompkins.h Modules/Rpeaks/PanTompkins.cpp )
target_link_libraries(ECG_Analyzer
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        GSL::gsl
        GSL::gslcblas
        )

target_link_libraries(ECG_Analyzer DATA_ACCESSOR)
