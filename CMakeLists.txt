cmake_minimum_required(VERSION 3.5)
project(kamio)

add_subdirectory(external/CLI11)
add_subdirectory(external/json)

add_library(kamio_lib src/lib.cpp)
target_include_directories(kamio_lib PUBLIC lib)

add_executable(kamio src/main.cpp)

target_link_libraries(kamio_lib PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(kamio PRIVATE CLI11::CLI11)
target_link_libraries(kamio PRIVATE kamio_lib)