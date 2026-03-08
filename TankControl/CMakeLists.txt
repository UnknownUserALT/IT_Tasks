cmake_minimum_required(VERSION 3.16)

project(TankControl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(TankControl
    main.cpp
)

target_link_libraries(TankControl
    Qt6::Core
    Qt6::Widgets
)
