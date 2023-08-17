cmake_minimum_required(VERSION 3.23)
project(parser)

set(CMAKE_CXX_STANDARD 23)
#make o3
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o2")

add_executable(parser main.cpp pcapSrc/PCAPStructs.h Parser.h pcapSrc/PcapReader.cpp pcapSrc/PcapReader.h pcapSrc/PcapWriter.cpp pcapSrc/PcapWriter.h SimbaStructs.h SimbaReader.cpp SimbaReader.h SimbaWriter.cpp SimbaWriter.h SBEStructs.h OrderUpdate.cpp OrderUpdate.h OrderExecution.cpp OrderExecution.h)
