cmake_minimum_required(VERSION 3.10)

project(kikka VERSION 1.0.3 LANGUAGES CXX)

add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUGSANITIZE_INIT "-Wall -o1 -g -fsanitize=address -fsanitize=undefined")
set(CMAKE_CXX_FLAGS_DEBUGLTOSANITIZE_INIT "-Wall -o1 -g -fsanitize=address -fsanitize=undefined -flto")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -o1 -g")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -o3")
set(CMAKE_CXX_FLAGS_RELEASELTO_INIT "-Wall -o3 -flto")

set(HEADERS headers/interpretator-flags.hpp headers/interpretator-args.hpp headers/interpretator-math.hpp headers/interpretate-line.hpp)
include_directories(headers)

set(SOURCES sources/interpretator-flags.cpp sources/interpretator-args.cpp sources/interpretator-math.cpp sources/interpretate-line.cpp kikka.cpp)
#set_target_properties(kikka PROPERTIES
#	WIN32_EXECUTABLE TRUE
#)

add_executable(kikka ${SOURCES} ${HEADERS})