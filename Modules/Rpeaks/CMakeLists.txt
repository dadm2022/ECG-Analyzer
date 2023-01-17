# GSL configuration
set(GCC_COVERAGE_COMPILE_FLAGS "-fexceptions -DGSL_COMPLEX_LEGACY")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
find_package(Qt5 COMPONENTS Widgets)
add_library(R_PEAKS HilbertTransform.h HilbertTransform.cpp PanTompkins.h PanTompkins.cpp)
find_package(GSL REQUIRED)
target_link_libraries(R_PEAKS
        GSL::gsl
        GSL::gslcblas
        )