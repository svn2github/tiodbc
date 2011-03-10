####################################
# CMake script for tiodbc

cmake_minimum_required (VERSION 2.6)
project (tiodbc)

set (tiodbc_VERSION_MAJOR 1)
set (tiodbc_VERSION_MINOR 0)
 
# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#  "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
#  "${PROJECT_BINARY_DIR}/TutorialConfig.h"
#  )
 
# add the executable
add_library(tiodbc SHARED tiodbc.cpp)
