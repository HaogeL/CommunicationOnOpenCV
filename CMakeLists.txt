cmake_minimum_required(VERSION 3.8)
project(ComOpenCV)
find_package(OpenCV REQUIRED)
find_package(PythonLibs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ComOpenCV main.cpp)
target_include_directories(ComOpenCV PRIVATE ${PYTHON_INCLUDE_DIRS})

#message(STATUS ${PYTHON_INCLUDE_DIRS})
#message(STATUS ${OpenCV_LIBS})
target_link_libraries(ComOpenCV ${PYTHON_LIBRARIES})
target_link_libraries(ComOpenCV ${OpenCV_LIBS})

message("-- CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER_ID}")
message("-- CMAKE_C_COMPILER: ${CMAKE_C_COMPILER_ID}")
message("-- CMAKE_C_COMPILER_VERSION: ${CMAKE_C_COMPILER_VERSION}")
message("-- CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
