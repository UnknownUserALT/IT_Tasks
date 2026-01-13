cmake_minimum_required(VERSION 3.16)
project(ExamTaskShop)

set(CMAKE_CXX_STANDARD 20)

# Для Windows используем find_package и переменные окружения
if(WIN32)
    # Поиск PostgreSQL через переменные окружения или стандартные пути
    if(DEFINED ENV{POSTGRESQL_DIR})
        set(POSTGRESQL_ROOT_DIR "$ENV{POSTGRESQL_DIR}")
        message(STATUS "Using PostgreSQL from POSTGRESQL_DIR: ${POSTGRESQL_ROOT_DIR}")
    elseif(DEFINED ENV{POSTGRESQL_ROOT})
        set(POSTGRESQL_ROOT_DIR "$ENV{POSTGRESQL_ROOT}")
        message(STATUS "Using PostgreSQL from POSTGRESQL_ROOT: ${POSTGRESQL_ROOT_DIR}")
    endif()

    # Поиск заголовков PostgreSQL
    find_path(PostgreSQL_INCLUDE_DIR libpq-fe.h
            HINTS
            ${POSTGRESQL_ROOT_DIR}
            "$ENV{ProgramFiles}/PostgreSQL/*/include"
            "$ENV{ProgramFiles\(x86\)}/PostgreSQL/*/include"
            "C:/Program Files/PostgreSQL/*/include"
            "C:/Program Files (x86)/PostgreSQL/*/include"
            PATHS
            "$ENV{POSTGRESQL_INCLUDE_DIR}"
            "$ENV{PG_INCLUDE_DIR}"
    )

    # Поиск библиотеки PostgreSQL
    find_library(PostgreSQL_LIBRARY
            NAMES pq libpq libpq.lib
            HINTS
            ${POSTGRESQL_ROOT_DIR}
            "$ENV{ProgramFiles}/PostgreSQL/*/lib"
            "$ENV{ProgramFiles\(x86\)}/PostgreSQL/*/lib"
            "C:/Program Files/PostgreSQL/*/lib"
            "C:/Program Files (x86)/PostgreSQL/*/lib"
            PATHS
            "$ENV{POSTGRESQL_LIB_DIR}"
            "$ENV{PG_LIB_DIR}"
    )

    # Поиск libpqxx через vcpkg или стандартные пути
    if(DEFINED ENV{VCPKG_ROOT})
        set(VCPKG_ROOT "$ENV{VCPKG_ROOT}")
        message(STATUS "Using vcpkg from: ${VCPKG_ROOT}")
    endif()

    # Поиск заголовков libpqxx
    find_path(LIBPQXX_INCLUDE_DIR pqxx/pqxx
            HINTS
            ${VCPKG_ROOT}/installed/*/include
            "C:/libpqxx/include"
            "C:/Program Files/libpqxx/include"
            "C:/Program Files (x86)/libpqxx/include"
            "$ENV{LIBPQXX_DIR}/include"
            PATHS
            "$ENV{LIBPQXX_INCLUDE_DIR}"
    )

    # Поиск библиотеки libpqxx
    find_library(LIBPQXX_LIBRARY
            NAMES pqxx libpqxx
            HINTS
            ${VCPKG_ROOT}/installed/*/lib
            ${VCPKG_ROOT}/installed/*/debug/lib
            "C:/libpqxx/lib"
            "C:/Program Files/libpqxx/lib"
            "C:/Program Files (x86)/libpqxx/lib"
            "$ENV{LIBPQXX_DIR}/lib"
            PATHS
            "$ENV{LIBPQXX_LIB_DIR}"
    )

    # Альтернативный способ через find_package для libpqxx (если установлен через vcpkg)
    find_package(libpqxx QUIET)

else()
    # Оригинальный код для macOS/Linux
    if(APPLE)
        if(EXISTS "/opt/homebrew")
            set(HOMEBREW_PREFIX "/opt/homebrew")
        else()
            set(HOMEBREW_PREFIX "/usr/local")
        endif()
    endif()

    # Поиск PostgreSQL для macOS/Linux
    find_path(PostgreSQL_INCLUDE_DIR libpq-fe.h
            PATHS
            ${HOMEBREW_PREFIX}/include
            ${HOMEBREW_PREFIX}/include/postgresql
            ${HOMEBREW_PREFIX}/opt/postgresql/include
            ${HOMEBREW_PREFIX}/opt/postgresql@15/include
            ${HOMEBREW_PREFIX}/opt/postgresql@14/include
            ${HOMEBREW_PREFIX}/opt/postgresql@13/include
            /usr/local/include
            /usr/local/include/postgresql
            /usr/local/opt/postgresql/include
            /opt/homebrew/include
            /opt/homebrew/include/postgresql
            /opt/homebrew/opt/postgresql/include
            /opt/homebrew/opt/postgresql@15/include
            /opt/homebrew/opt/postgresql@14/include
            /opt/homebrew/opt/postgresql@13/include
            NO_DEFAULT_PATH
    )

    find_library(PostgreSQL_LIBRARY
            NAMES pq libpq postgresql libpostgresql
            PATHS
            ${HOMEBREW_PREFIX}/lib
            ${HOMEBREW_PREFIX}/opt/postgresql/lib
            ${HOMEBREW_PREFIX}/opt/postgresql@15/lib
            ${HOMEBREW_PREFIX}/opt/postgresql@14/lib
            ${HOMEBREW_PREFIX}/opt/postgresql@13/lib
            /usr/local/lib
            /usr/local/opt/postgresql/lib
            /opt/homebrew/lib
            /opt/homebrew/Cellar/postgresql@14/14.20
            /opt/homebrew/opt/postgresql/lib
            /opt/homebrew/opt/postgresql@15/lib
            /opt/homebrew/opt/postgresql@14/lib
            /opt/homebrew/opt/postgresql@13/lib
            NO_DEFAULT_PATH
    )

    # Поиск libpqxx для macOS/Linux
    find_path(LIBPQXX_INCLUDE_DIR pqxx/pqxx
            PATHS
            ${HOMEBREW_PREFIX}/include
            /usr/local/include
            /opt/homebrew/include
            NO_DEFAULT_PATH
    )

    find_library(LIBPQXX_LIBRARY
            NAMES pqxx libpqxx
            PATHS
            ${HOMEBREW_PREFIX}/lib
            /usr/local/lib
            /opt/homebrew/lib
            NO_DEFAULT_PATH
    )
endif()

# Проверка PostgreSQL
if(NOT PostgreSQL_INCLUDE_DIR)
    if(WIN32)
        message(FATAL_ERROR "PostgreSQL headers not found. Please:\n"
                "1. Install PostgreSQL from https://www.postgresql.org/download/windows/\n"
                "2. Set POSTGRESQL_DIR environment variable to PostgreSQL installation directory\n"
                "3. Or specify -DPOSTGRESQL_ROOT_DIR=... in CMake command line")
    else()
        message(FATAL_ERROR "PostgreSQL headers not found. Please install PostgreSQL with: brew install postgresql")
    endif()
endif()

if(NOT PostgreSQL_LIBRARY)
    if(WIN32)
        message(FATAL_ERROR "PostgreSQL library not found. Please check PostgreSQL installation.")
    else()
        message(FATAL_ERROR "PostgreSQL library not found. Please install PostgreSQL with: brew install postgresql")
    endif()
endif()

# Проверка libpqxx
if(NOT LIBPQXX_INCLUDE_DIR)
    if(WIN32)
        message(FATAL_ERROR "libpqxx headers not found. Please:\n"
                "1. Install libpqxx via vcpkg: vcpkg install libpqxx\n"
                "2. Or download from https://github.com/jtv/libpqxx\n"
                "3. Or set LIBPQXX_DIR environment variable")
    else()
        message(FATAL_ERROR "libpqxx headers not found. Please install libpqxx with: brew install libpqxx")
    endif()
endif()

if(NOT LIBPQXX_LIBRARY AND NOT libpqxx_FOUND)
    if(WIN32)
        message(FATAL_ERROR "libpqxx library not found. Please check libpqxx installation.")
    else()
        message(FATAL_ERROR "libpqxx library not found. Please install libpqxx with: brew install libpqxx")
    endif()
endif()

# Установка переменных
set(PostgreSQL_INCLUDE_DIRS ${PostgreSQL_INCLUDE_DIR})
set(PostgreSQL_LIBRARIES ${PostgreSQL_LIBRARY})
set(PostgreSQL_FOUND TRUE)

# Создание исполняемого файла
add_executable(TaskExamShop
        main.cpp
        DatabaseConnection.hpp
        User.cpp
        Admin.cpp
        Manager.cpp
        Customer.cpp
        Order.cpp
        OrderItem.cpp
        Payment.cpp
        init/database_setup.cpp
        init/stored_procedure.cpp
        init/triggers.cpp
        init/functions.cpp
        init/audit_system.cpp
        init/business_logic.cpp
        init/order_operations.cpp
        init/database_setup.hpp
        init/stored_procedure.hpp
        init/triggers.hpp
        init/functions.hpp
        init/audit_system.hpp
        init/business_logic.hpp
        init/order_operations.hpp
)

# Добавление директорий с заголовками
target_include_directories(TaskExamShop PRIVATE
        ${PostgreSQL_INCLUDE_DIRS}
        ${LIBPQXX_INCLUDE_DIR}
)

# Линковка библиотек
if(libpqxx_FOUND)
    # Используем find_package если найдено
    target_link_libraries(TaskExamShop
            ${PostgreSQL_LIBRARIES}
            libpqxx::pqxx
    )
else()
    target_link_libraries(TaskExamShop
            ${PostgreSQL_LIBRARIES}
            ${LIBPQXX_LIBRARY}
    )
endif()

# Флаги компиляции
target_compile_options(TaskExamShop PRIVATE -std=c++20)

# Для Windows добавляем определение для экспорта DLL если нужно
if(WIN32)
    target_compile_definitions(TaskExamShop PRIVATE
            -DNOMINMAX
            -D_USE_MATH_DEFINES
    )

    # Копирование DLL библиотек в выходную директорию
    if(PostgreSQL_LIBRARY)
        get_filename_component(POSTGRESQL_DLL_DIR "${PostgreSQL_LIBRARY}" DIRECTORY)
        get_filename_component(POSTGRESQL_DLL_DIR "${POSTGRESQL_DLL_DIR}/../bin" ABSOLUTE)
        file(GLOB POSTGRESQL_DLLS "${POSTGRESQL_DLL_DIR}/libpq.dll")
        if(POSTGRESQL_DLLS)
            add_custom_command(TARGET TaskExamShop POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${POSTGRESQL_DLLS}
                    $<TARGET_FILE_DIR:TaskExamShop>
            )
        endif()
    endif()
endif()

# Отладочная информация
message(STATUS "PostgreSQL include dir: ${PostgreSQL_INCLUDE_DIRS}")
message(STATUS "PostgreSQL library: ${PostgreSQL_LIBRARIES}")
message(STATUS "libpqxx include dir: ${LIBPQXX_INCLUDE_DIR}")
message(STATUS "libpqxx library: ${LIBPQXX_LIBRARY}")