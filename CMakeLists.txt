cmake_minimum_required(VERSION 3.17)
project(solar VERSION 0.1 LANGUAGES CXX C)

option(RELEASE "Whether the program should be build for release" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

if (RELEASE)
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /O2")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -s")
    endif()
else()
    if (NOT MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
    endif()
endif()

file(COPY "data" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB_RECURSE sources src/*.cpp extern/*.c)

add_executable(solar ${sources})

find_package(glm REQUIRED)
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(solar PUBLIC glfw)
target_include_directories(solar PUBLIC extern)
target_include_directories(solar PUBLIC ${GLM_INCLUDE_DIRS})
