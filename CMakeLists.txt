cmake_minimum_required(VERSION 3.14)

project(FirstTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV)
find_package(CURL REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${CURL_INCLUDE_DIR})

add_executable(${PROJECT_NAME}
  main.cpp
  BSlogger/src/BSlogger.hpp
)

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} ${CURL_LIBRARIES})


