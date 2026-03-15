cmake_minimum_required(VERSION 3.16)
project(WindowCannotClose)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(WindowCannotClose
        main.cpp
        MainWindow.cpp
        MainWindow.h
        Phase1Widget.cpp
        Phase1Widget.h
        Phase2Widget.cpp
        Phase2Widget.h
        Phase3Widget.cpp
        Phase3Widget.h
        CloneWindow.cpp
        CloneWindow.h
        Phase4Widget.cpp
        Phase4Widget.h
)
target_link_libraries(WindowCannotClose
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

