cmake_minimum_required(VERSION 3.0.0)
project(MandelbrotPlot)

set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(SFML COMPONENTS graphics window system REQUIRED)

set(RENDER Render)
add_executable(${PROJECT_NAME} src/main.cpp)
add_executable(${RENDER} src/render.cpp)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
target_link_libraries(${RENDER} sfml-graphics sfml-window sfml-system)