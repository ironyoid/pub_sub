add_executable(p3_server main.cpp tcp_server/tcp_server.cpp)
target_include_directories(p3_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tcp_server)
target_link_libraries(p3_server LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(p3_server LINK_PUBLIC Utils)