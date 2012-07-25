cmake_minimum_required(VERSION 2.8)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  set(PatchComparison_includes ${QtHelpers_includes} ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
endif()

# Enable c++11
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
list(APPEND compileflags "USE_QT_PARALLEL")

add_library(QtHelpers QtHelpers.cpp)
target_link_libraries(QtHelpers ${QT_LIBRARIES})

SET(QtHelpers_BuildTests OFF CACHE BOOL "QtHelpers_BuildTests")
if(QtHelpers_BuildTests)
  add_subdirectory(Tests)
endif()
