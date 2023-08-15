cmake_minimum_required(VERSION 3.23)
project(parser)

set(CMAKE_CXX_STANDARD 23)
#make o3
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(parser main.cpp PCAPStructs.h Parser.h PcapReader.cpp PcapReader.h PcapWriter.cpp PcapWriter.h)
