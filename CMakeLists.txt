cmake_minimum_required (VERSION 3.1.0)
project(main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -g -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/memory/*.cpp ${CMAKE_SOURCE_DIR}/cpu/*.cpp ${CMAKE_SOURCE_DIR}/utils/*.cpp ${CMAKE_SOURCE_DIR}/display/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} main.cpp)
