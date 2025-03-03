cmake_minimum_required(VERSION 3.18)

# minimum compiler versions
set(AppleClang_MinVersion 11.0.0)
set(Clang_MinVersion 10.0.0)
set(GNU_MinVersion 9.1.0)

# set the project name and version
project(Simulator VERSION 1.0)

SET(BUILD_DEPS ON)

Set(FETCHCONTENT_QUIET FALSE)


find_package(ortools)

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
    StrategyTypes.h
    PI.h
    SinGenerator.h
    SinGenerator.cpp
    ReceivedPacketInfo.h
    ReceivedPacketInfo.cpp
    SendPacketInfo.h
    SendPacketInfo.cpp
    SinApplication.h
    SinApplication.cpp
    RandomStrategy.h
    RandomStrategy.cpp
    RoundRobinStrategy.h
    RoundRobinStrategy.cpp
    ReactiveStrategy.h
    ReactiveStrategy.cpp
    ComputeOptimizationResult.h
    ComputeOptimizationResult.cpp)


target_link_libraries(Simulator
    ${libpoint-to-point}
    ${libinternet}
    ${libhelper}
    ${libcsma}
    ${libapplications}
    ${libcore}
    ${libapplications}
    ortools::ortools)


set_target_properties(Simulator PROPERTIES
            CXX_STANDARD 20)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()

add_executable(
  hello_test
  tests/hello_test.cc
)
target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)