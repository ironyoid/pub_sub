add_library(Utils utils/utils.cpp)
target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils ${CMAKE_CURRENT_SOURCE_DIR}/parser ${CMAKE_CURRENT_SOURCE_DIR}/logs)