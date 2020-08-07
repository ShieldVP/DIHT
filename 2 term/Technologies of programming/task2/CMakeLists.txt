cmake_minimum_required(VERSION 3.10)
project(OriginalName)

SET(PATH ${CMAKE_SOURCE_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PATH}/bin)
SET(OriginalName C)

find_package(GTest REQUIRED)

include_directories(${PATH})
include_directories(${GTEST_INCLUDE_DIRS})

add_custom_command(
    OUTPUT ${PATH}/A/index.h
    COMMAND python3 ${PATH}/A/preparing.py ${PATH}/A/index.h
    DEPENDS ${PATH}/A/preparing.py
)

add_executable(OriginalName ${PATH}/C/main.cpp)
add_subdirectory(${PATH}/B)
target_link_libraries(OriginalName Blib)

add_executable(CTest ${PATH}/C/main_test.cpp)
target_link_libraries(CTest ${GTEST_LIBRARIES} pthread)

add_custom_target(Check DEPENDS ${PATH}/A/index.h)
add_dependencies(OriginalName Check)

set_target_properties(Blib PROPERTIES LINKER_LANGUAGE CXX)
