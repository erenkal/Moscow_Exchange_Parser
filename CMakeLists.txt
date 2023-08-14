cmake_minimum_required(VERSION 3.25)
project(parser)

set(CMAKE_CXX_STANDARD 23)

add_executable(parser main.cpp PCAPStructs.h Parser.h PcapReader.cpp PcapReader.h)
