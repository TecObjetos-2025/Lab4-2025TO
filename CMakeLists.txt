# Versión mínima de CMake requerida.
cmake_minimum_required(VERSION 3.10)

project(CalculoTrapecio CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(trapecio src/trapecio.cpp)

# Libreria para manejo de hilos
find_package(Threads REQUIRED)
target_link_libraries(trapecio PRIVATE Threads::Threads)