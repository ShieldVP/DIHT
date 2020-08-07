cmake_minimum_required(VERSION 3.10)
project(TheMostOriginalName)

SET(PATH ${CMAKE_SOURCE_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PATH}/lib)

add_library(Blib SHARED lib.h)
