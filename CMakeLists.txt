cmake_minimum_required(VERSION 3.16)
project(ftag LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SQLite3 REQUIRED)

add_subdirectory(vendor/argparse)

include_directories(
  include
  vendor/argparse/include
  ${SQLite3_INCLUDE_DIRS}
)

file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
  argparse
  SQLite::SQLite3
)

