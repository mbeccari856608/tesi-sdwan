cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(Simulator VERSION 1.0)


include_directories("./external/ns-allinone-3.42/ns-3.42/src/core/model")
include_directories("./external/ns-allinone-3.42/ns-3.42/build/include")

add_executable(Simulator main.cpp)

