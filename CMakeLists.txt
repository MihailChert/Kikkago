cmake_minimum_required(VERSION 3.0)

project(kikka VERSION 1.0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -o1 -g -fsanitize=address -fsanitize=undefined")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -o3")
set(CMAKE_CXX_FLAGS_RELEASELTO_INIT "-Wall -o3 -flto")

set(HEADERS headers/better_enum.hpp headers/interpretator-flags.hpp headers/interpretator-args.hpp headers/interpretator-maht.hpp headers/interpretate-line.hpp)

set(SOURCES sources/interpretator-flags.cpp sources/interpretator-args.cpp sources/interpretator-math.cpp sources/interpretate-line.cpp kikka.cpp)
#set_target_properties(kikka PROPERTIES
#	WIN32_EXECUTABLE TRUE
#)

add_executable(kikka ${SOURCES})