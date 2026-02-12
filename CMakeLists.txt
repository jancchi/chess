cmake_minimum_required(VERSION 3.10)
project(Chess)

set(SFML_DIR "/opt/sfml2/lib/cmake/SFML/")

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

set(SOURCES
    src/main.cpp
    src/Backend.cpp
)

add_executable(MyApp ${SOURCES})

target_link_libraries(MyApp sfml-graphics sfml-window sfml-system)
