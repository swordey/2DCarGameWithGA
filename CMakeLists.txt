cmake_minimum_required(VERSION 3.5)

project(2DCarGameWithGA LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets LinguistTools )

add_executable(2DCarGameWithGA
main.cpp
ui/widget.cpp
ui/widget.h
ui/widget.ui
game/game.h
algo/genetic_algo.h
algo/simple_brain.h
objects/track.h
objects/car.h
)

target_link_libraries(2DCarGameWithGA 
                        PUBLIC 
                        Qt5::Core
                        Qt5::Gui
                        Qt5::Widgets)