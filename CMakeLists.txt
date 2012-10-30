cmake_minimum_required(VERSION 2.8)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Enable c++11
if(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif(UNIX)

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
list(APPEND compileflags "USE_QT_PARALLEL")

add_library(QtHelpers QtHelpers.cpp)
target_link_libraries(QtHelpers ${QT_LIBRARIES})
set(QtHelpers_libraries ${QtHelpers_libraries} QtHelpers)

option(QtHelpers_BuildTests "QtHelpers_BuildTests")
if(QtHelpers_BuildTests)
  add_subdirectory(Tests)
endif()

# Allow this project to be detected and used as a submodule
set(QtHelpers_include_dirs ${QtHelpers_include_dirs} ${CMAKE_CURRENT_SOURCE_DIR})
set_property(GLOBAL PROPERTY QtHelpersIncludeDirs ${QtHelpers_include_dirs})
set_property(GLOBAL PROPERTY QtHelpersLibs ${QtHelpers_libraries})
