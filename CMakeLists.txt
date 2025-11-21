cmake_minimum_required(VERSION 3.15)
project(OOP_LAB5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(googletest)
foreach(target gtest gtest_main gmock gmock_main)
    if(TARGET ${target})
        target_compile_options(${target} PRIVATE -Wno-error)
    endif()
endforeach()


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_library(lab5 STATIC
    src/memory_pool_resourse.cpp
)

target_include_directories(lab5 PUBLIC includes)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE lab5)

add_subdirectory(tests)
