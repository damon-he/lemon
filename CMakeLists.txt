cmake_minimum_required(VERSION 2.8)
project (lemon)

#SET (CMAKE_SYSTEM_NAME Linux)
#SET (CMAKE_C_COMPILER "arm-hisiv300-linux-gcc")
#SET (CMAKE_CXX_COMPILER "arm-hisiv300-linux-g++")



add_subdirectory (lemon/base)
add_subdirectory (lemon/thread)
add_subdirectory (lemon/net)
add_subdirectory (lemon/crypt)

