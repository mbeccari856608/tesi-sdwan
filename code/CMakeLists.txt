cmake_minimum_required(VERSION 3.13..3.13)

# minimum compiler versions
set(AppleClang_MinVersion 11.0.0)
set(Clang_MinVersion 10.0.0)
set(GNU_MinVersion 9.1.0)

# set the project name and version
project(Simulator VERSION 1.0)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/ns-allinone-3.42/ns-3.42/)



add_executable(Simulator 
    main.cpp
    DeviceApplication.h
    DeviceApplication.cpp
    ApplicationSenderHelper.h
    ApplicationSenderHelper.cpp
    SenderApplication.h
    SenderApplication.cpp
    ReceiverApplicationHelper.h
    ReceiverApplicationHelper.cpp
    ReceiverApplication.h
    ReceiverApplication.cpp
    Utils.h
    Utils.cpp
    ISPInterface.h
    ISPInterface.cpp)


target_link_libraries(Simulator
    ${libpoint-to-point}
    ${libinternet}
    ${libhelper}
    ${libcsma}
    ${libapplications}
    ${libcore}
    ${libapplications})


set_target_properties(Simulator PROPERTIES
            CXX_STANDARD 20)

