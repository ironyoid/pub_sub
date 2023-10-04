add_executable(p3_server main.cpp)
target_include_directories(p3_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(p3_server LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(p3_server LINK_PUBLIC Utils)