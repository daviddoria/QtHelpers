# Allow headers in tests to be included like
# #include "QtHelpers.h" rather than needing
# #include "QtHelpers/QtHelpers.h"
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(Tests Tests.cpp)
target_link_libraries(Tests libQtHelpers)
