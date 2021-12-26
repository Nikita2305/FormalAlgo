cmake_minimum_required(VERSION 3.20)
project(FormalAlgo)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(sources
    include/Algo.h
    include/Configuration.h
    include/ConfigurationSet.h
    include/Earley.h
    include/Grammar.h
    include/Rule.h
    include/Situation.h
    include/ParserLR.h
    include/NodeLR.h
    include/SituationLR.h
    include/Instruction.h
    src/Earley.cpp
    src/Grammar.cpp
    src/ConfigurationSet.cpp
    src/ParserLR.cpp
    src/Instruction.cpp
    src/NodeLR.cpp)

add_executable(FormalAlgo main.cpp)

target_link_libraries(FormalAlgo sources)
