cmake_minimum_required(VERSION 3.16)
project(dht_ipfs VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Include paths ──────────────────────────────────────────────────────────────
include_directories(${PROJECT_SOURCE_DIR}/include)

# ── Main executable ────────────────────────────────────────────────────────────
add_executable(dht_ipfs src/main.cpp)
target_include_directories(dht_ipfs PRIVATE ${PROJECT_SOURCE_DIR}/include)

# ── Tests (Catch2 via FetchContent) ────────────────────────────────────────────
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.10
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(dht_tests tests/test_dht.cpp)
target_include_directories(dht_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(dht_tests PRIVATE Catch2::Catch2)

include(CTest)
add_test(NAME dht_tests COMMAND dht_tests)
