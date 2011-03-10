####################################
# CMake script for tiodbc

cmake_minimum_required (VERSION 2.6)
project (tiodbc)

set (tiodbc_VERSION_MAJOR 1)
set (tiodbc_VERSION_MINOR 0)
 
# Target library
add_library(tiodbc SHARED tiodbc.cpp)

# Install target
install (TARGETS tiodbc DESTINATION lib)
install (FILES tiodbc.hpp DESTINATION include)

