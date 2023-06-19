cmake_minimum_required(VERSION 3.0.0)
project(CPMMessageStorage VERSION 0.1.0)

find_package( log4cxx REQUIRED )

set(CMAKE_CXX_STANDARD 17)

file(GLOB CPP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/example/include/*.h)
file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/example/*.cpp)

add_library(CPMMessageStorage ${CPP_FILES} ${CPP_HEADERS})
target_include_directories(CPMMessageStorage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include ${CMAKE_CURRENT_SOURCE_DIR}/example/include)
target_link_libraries(CPMMessageStorage log4cxx)

find_package(CxxTest )
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()

    file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/*.h)
    set(TEST_REPORTS ${CMAKE_CURRENT_BINARY_DIR}/test/reports)
    file(MAKE_DIRECTORY ${TEST_REPORTS})
    # Generate the test runner
    foreach(TEST_SOURCE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        set(TEST_RUNNER test/${TEST_NAME}.cc)
        set(CXXTEST_TESTGEN_ARGS --xunit-printer --xunit-file=${TEST_REPORTS}/${TEST_NAME}.xml)
        CXXTEST_ADD_TEST(${TEST_NAME} ${TEST_RUNNER} ${TEST_SOURCE})
        target_link_libraries(${TEST_NAME} CPMMessageStorage)

    endforeach()
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
