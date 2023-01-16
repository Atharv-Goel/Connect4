cmake_minimum_required(VERSION 3.5)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 20)

project(Connect4)

add_compile_options(-Og -g3 -ggdb -fsanitize=address)
add_link_options(-Og -g3 -ggdb -fsanitize=address)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIRS})
link_libraries(${SDL2_LIBRARY} ${SDL2_TTF_LIBRARIES})

file(GLOB_RECURSE source src/*.cpp)
add_executable(Connect4 ${source})