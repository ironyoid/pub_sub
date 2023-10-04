add_executable(p3_client main.cpp)
target_include_directories(p3_client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(p3_client LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(p3_client LINK_PUBLIC Utils)