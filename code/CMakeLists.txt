cmake_minimum_required(VERSION 3.13..3.13)

# minimum compiler versions
set(AppleClang_MinVersion 11.0.0)
set(Clang_MinVersion 10.0.0)
set(GNU_MinVersion 9.1.0)

# set the project name and version
project(Simulator VERSION 1.0)

SET(BUILD_DEPS ON)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/or-tools/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/ns-allinone-3.42/ns-3.42/)



add_executable(Simulator 
    main.cpp
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
    ISPInterface.cpp
    SDWanApplication.h
    SDWanApplication.cpp
    SDWanStaticApplication.h
    SDWanStaticApplication.cpp
    Strategy.h
    Strategy.cpp
    LinearStrategy.h
    LinearStrategy.cpp
    StrategyTypes.h)


target_link_libraries(Simulator
    ${libpoint-to-point}
    ${libinternet}
    ${libhelper}
    ${libcsma}
    ${libapplications}
    ${libcore}
    ${libapplications}
    ortools)


set_target_properties(Simulator PROPERTIES
            CXX_STANDARD 20)

