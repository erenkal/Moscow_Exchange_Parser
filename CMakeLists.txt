cmake_minimum_required(VERSION 3.23)
project(parser)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../parser)

#make o3
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o3")

add_executable(parser
        main.cpp
        pcapSrc/PCAPStructs.h Parser.h pcapSrc/PcapReader.cpp pcapSrc/PcapReader.h pcapSrc/PcapWriter.cpp
        pcapSrc/PcapWriter.h simbaSrc/SimbaStructs.h simbaSrc/SimbaReader.cpp simbaSrc/SimbaReader.h simbaSrc/SimbaWriter.cpp simbaSrc/SimbaWriter.h simbaSrc/SBEStructs.h
        simbaSrc/OrderUpdate.cpp simbaSrc/OrderUpdate.h simbaSrc/OrderExecution.cpp simbaSrc/OrderExecution.h 3rdParty/NanoLog.h 3rdParty/NanoLog.cpp 3rdParty/readerwriterqueue.h simbaSrc/SnapShot.h)
