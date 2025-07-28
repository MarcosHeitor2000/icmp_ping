cmake_minimum_required(VERSION 3.14)
project(icmp_ping)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(icmp STATIC
  src/icmp_message.cc
)
target_include_directories(icmp PUBLIC include)

add_executable(icmp_message_test tests/icmp_message_test.cc)
target_link_libraries(icmp_message_test gtest_main icmp)

include(GoogleTest)
gtest_discover_tests(icmp_message_test)
