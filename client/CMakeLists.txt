add_executable(p3_client main.cpp tcp_client/tcp_client.cpp)
target_include_directories(p3_client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tcp_client ${CMAKE_CURRENT_SOURCE_DIR}/commands)
target_link_libraries(p3_client LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(p3_client LINK_PUBLIC Utils)