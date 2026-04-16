cmake_minimum_required(VERSION 3.16)
project(Lab3QPainter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Widgets Gui REQUIRED)

set(SOURCES
    main.cpp
    mainwindow.cpp
    shapescene.cpp
    shapeview.cpp
    shapeitem.cpp
    clockitem.cpp
    commands.cpp
)

set(HEADERS
    mainwindow.h
    shapescene.h
    shapeview.h
    shapeitem.h
    clockitem.h
    commands.h
)

add_executable(Lab3QPainter ${SOURCES} ${HEADERS})
target_link_libraries(Lab3QPainter Qt6::Core Qt6::Widgets Qt6::Gui)
