cmake_minimum_required(VERSION 3.10)
project(dwh_loader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Для macOS: поиск libpq с учетом Homebrew
if(APPLE)
    # Homebrew устанавливает в /opt/homebrew или /usr/local
    set(HOMEBREW_PREFIX "/opt/homebrew")
    if(NOT EXISTS "${HOMEBREW_PREFIX}")
        set(HOMEBREW_PREFIX "/usr/local")
    endif()

    # Ищем заголовки
    find_path(POSTGRESQL_INCLUDE_DIR
            NAMES libpq-fe.h
            PATHS
            "${HOMEBREW_PREFIX}/include"
            "${HOMEBREW_PREFIX}/opt/libpq/include"
            /usr/local/include
            /usr/include
            /usr/include/postgresql
            PATH_SUFFIXES postgresql
    )

    # Ищем библиотеку
    find_library(POSTGRESQL_LIBRARY
            NAMES pq libpq
            PATHS
            "${HOMEBREW_PREFIX}/lib"
            "${HOMEBREW_PREFIX}/opt/libpq/lib"
            /usr/local/lib
            /usr/lib
    )
else()
    # Для Linux
    find_package(PostgreSQL REQUIRED)
    set(POSTGRESQL_INCLUDE_DIR ${PostgreSQL_INCLUDE_DIRS})
    set(POSTGRESQL_LIBRARY ${PostgreSQL_LIBRARIES})
endif()

if(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARY)
    message(STATUS "Found libpq:")
    message(STATUS "  Include dir: ${POSTGRESQL_INCLUDE_DIR}")
    message(STATUS "  Library: ${POSTGRESQL_LIBRARY}")
else()
    message(FATAL_ERROR "libpq not found! Install with: brew install libpq")
endif()

# Создание исполняемого файла
add_executable(${PROJECT_NAME} main.cpp)

# Подключение заголовков
target_include_directories(${PROJECT_NAME} PRIVATE ${POSTGRESQL_INCLUDE_DIR})

# Подключение библиотеки
target_link_libraries(${PROJECT_NAME} PRIVATE ${POSTGRESQL_LIBRARY})

# Для macOS может потребоваться фреймворки
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            "-framework Security"
            "-framework CoreFoundation"
    )
endif()