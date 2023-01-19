cmake_minimum_required(VERSION 3.5)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 20)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
project(Connect4)

add_compile_options(-Og -g3 -ggdb -fsanitize=address)
add_link_options(-Og -g3 -ggdb -fsanitize=address)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Boost REQUIRED serialization)
find_package(Boost COMPONENTS system thread REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
link_libraries(${SDL2_LIBRARY} ${SDL2_TTF_LIBRARIES} ${Boost_LIBRARIES} ${Boost_SYSTEM_LIBRARY} Boost::boost Boost::serialization)

file(GLOB_RECURSE source src/*.cpp)
add_executable(Connect4 ${source})