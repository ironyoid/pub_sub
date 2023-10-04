add_executable(p3_client main.cpp utils.cpp)
set(INCLUDES parser.hpp commands.hpp tcp_client.hpp utils.hpp)
target_include_directories(p3_client PUBLIC ${INCLUDES})
target_link_libraries(p3_client LINK_PUBLIC ${Boost_LIBRARIES})