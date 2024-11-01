cmake_minimum_required(VERSION 3.13..3.13)

# minimum compiler versions
set(AppleClang_MinVersion 11.0.0)
set(Clang_MinVersion 10.0.0)
set(GNU_MinVersion 9.1.0)

# set the project name and version
project(Simulator VERSION 1.0)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/ns-allinone-3.42/ns-3.42/)
include_directories(/home/michele/Scrivania/tesi-sdwan/openflowlib/openflow/include)



add_executable(Simulator 
    main.cpp
    DeviceApplication.h
    DeviceApplication.cpp
    ApplicationSenderHelper.h
    ApplicationSenderHelper.cpp
    SenderApplication.h
    SenderApplication.cpp)


target_link_libraries(Simulator
    ${libpoint-to-point}
    ${libopenflow}
    ${libinternet}
    ${libhelper}
    ${libcsma}
    ${libapplications}
    ${libcore}
    ${libapplications})


set_target_properties(Simulator PROPERTIES
            CXX_STANDARD 20)

