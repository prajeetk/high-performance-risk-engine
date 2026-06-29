cmake_minimum_required(VERSION 3.20)
project(high-perf-risk-engine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.0.2
)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1
)

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Use external fmt" FORCE)
FetchContent_MakeAvailable(fmt spdlog Catch2)

add_library(risk_engine_lib
    src/core/Order.cpp
    src/core/Trade.cpp
    src/core/Position.cpp
    src/risk/RiskEngine.cpp
    src/risk/PositionLimit.cpp
    src/risk/ExposureLimit.cpp
    src/risk/FatFingerCheck.cpp
    src/risk/PnL.cpp
)

target_include_directories(risk_engine_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(risk_engine_lib PUBLIC fmt::fmt spdlog::spdlog)

add_executable(risk_engine src/main.cpp)
target_link_libraries(risk_engine PRIVATE risk_engine_lib)

enable_testing()
add_executable(risk_engine_tests
    tests/test_order.cpp
    tests/test_position.cpp
    tests/test_risk_checks.cpp
    tests/test_risk_engine.cpp
    tests/test_pnl.cpp
)
target_link_libraries(risk_engine_tests PRIVATE risk_engine_lib Catch2::Catch2WithMain)
include(Catch)
catch_discover_tests(risk_engine_tests)
