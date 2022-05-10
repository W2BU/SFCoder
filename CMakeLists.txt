cmake_minimum_required(VERSION 3.19)
project(SFCoder)

set(
    CMAKE_CXX_STANDARD 17
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    gtest_force_shared_crt ON CACHE BOOL "" FORCE
)


include(
    FetchContent
)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# compiling test list
add_executable(
    test_SFCoder
    test/test_SFCoder.cpp
)

# compiling example code
add_executable(
    demo
    demo/demo.cpp
)

# adding include path for exampleCode
target_include_directories(
    demo PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# linking test list and gtest main fuction
target_link_libraries(
    test_SFCoder
    gtest_main
)

# declaring src as include directory for test list
target_include_directories(
    test_SFCoder PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# enabling cmake's test runner to discover the tests
include(
    GoogleTest
)
gtest_discover_tests(test_SFCoder)