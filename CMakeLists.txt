cmake_minimum_required(VERSION 3.5)
project(kamio)

add_subdirectory(CLI11)

add_executable(kamio src/main.cpp)
target_link_libraries(kamio PRIVATE CLI11::CLI11)