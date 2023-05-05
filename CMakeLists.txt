cmake_minimum_required(VERSION 3.0.0)
project(CPMMessageStorage VERSION 0.1.0)
find_package( log4cxx REQUIRED )

file(GLOB CPP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/include/*.h)
file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(CPMMessageStorage ${CPP_FILES} ${CPP_HEADERS})
target_include_directories(CPMMessageStorage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
set_property(TARGET CPMMessageStorage PROPERTY CXX_STANDARD 20)
target_link_libraries(CPMMessageStorage log4cxx)

set(CMAKE_CXX_STANDARD 20)

find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()
	CXXTEST_ADD_TEST(unittest_debug tests/debugtestsuite.cc ${CMAKE_CURRENT_SOURCE_DIR}/tests/debugtestsuite.h)
    target_link_libraries(unittest_debug CPMMessageStorage)
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
