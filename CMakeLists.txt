cmake_minimum_required (VERSION 3.1.0)
project(main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-L/usr/local/include -L/usr/local/lib -Wall -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/memory/*.cpp ${CMAKE_SOURCE_DIR}/cpu/*.cpp ${CMAKE_SOURCE_DIR}/utils/*.cpp ${CMAKE_SOURCE_DIR}/display/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} main.cpp)
target_link_libraries(${PROJECT_NAME} kvm)
