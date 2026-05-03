cmake_minimum_required(VERSION 3.16)
project(LibraryCatalog LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматическая обработка мета-объектов, UI-форм и ресурсов Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.4 REQUIRED
    COMPONENTS Core Gui Widgets Sql Network Concurrent)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/databasemanager.cpp
    src/databasemanager.h
    src/openlibraryclient.cpp
    src/openlibraryclient.h
    src/bookdialog.cpp
    src/bookdialog.h
    src/bookdialog.ui
    src/bookfilterproxymodel.cpp
    src/bookfilterproxymodel.h
    src/statusdelegate.cpp
    src/statusdelegate.h
    src/statisticswidget.cpp
    src/statisticswidget.h
    resources/icons.qrc
)

qt_add_executable(library_catalog ${SOURCES})

target_link_libraries(library_catalog PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Sql Qt6::Network Qt6::Concurrent)
