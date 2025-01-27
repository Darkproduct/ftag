cmake_minimum_required(VERSION 3.16)
project(ftag LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(sqlite3 REQUIRED)
add_subdirectory(vendor/argparse)

include_directories(include)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/database.cpp
)

target_link_libraries(${PROJECT_NAME}
  argparse
  sqlite3
)

