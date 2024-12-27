cmake_minimum_required(VERSION 3.0)

project(TermuxPowerControl)

file(GLOB SOURCES "*.c" "*.h")

# # add -lc
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc")

add_executable(TermuxPowerControl ${SOURCES})