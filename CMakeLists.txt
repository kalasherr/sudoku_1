cmake_minimum_required(VERSION 3.10)

project(MySFMLProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(my_sfml_app src/main.cpp)

set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")
list(APPEND CMAKE_PREFIX_PATH "C:/SFML-2.6.1")

find_package(SFML 2.6.1 COMPONENTS system window graphics REQUIRED)

target_link_libraries(my_sfml_app sfml-system sfml-window sfml-graphics)