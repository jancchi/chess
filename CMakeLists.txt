cmake_minimum_required(VERSION 3.10)
project(Chess)

set(SFML_DIR "/opt/sfml2/lib/cmake/SFML/")

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(MyApp src/main.cpp)

target_link_libraries(MyApp sfml-graphics sfml-window sfml-system)
