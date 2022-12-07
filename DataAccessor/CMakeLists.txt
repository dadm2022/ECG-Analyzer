add_library(DATA_ACCESSOR DataAccessor.cpp wfdb.cpp)

target_include_directories(DATA_ACCESSOR PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
