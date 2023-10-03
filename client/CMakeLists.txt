add_executable(p3_client main.cpp)
set(INCLUDES ${PROJECT_SOURCE_DIR}/client/parser.hpp ${PROJECT_SOURCE_DIR}/client/commands.hpp)
target_include_directories(p3_client PUBLIC ${INCLUDES})
target_link_libraries(p3_client LINK_PUBLIC ${Boost_LIBRARIES})