cmake_minimum_required(VERSION 3.16)
project(LabRabota4)

set(CMAKE_CXX_STANDARD 20)

# Определяем префикс Homebrew
if(APPLE)
    if(EXISTS "/opt/homebrew")
        set(HOMEBREW_PREFIX "/opt/homebrew")
    else()
        set(HOMEBREW_PREFIX "/usr/local")
    endif()
endif()

# Расширенный поиск PostgreSQL заголовков
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

# Расширенный поиск PostgreSQL библиотеки
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

# Отладочная информация
message(STATUS "Searching for PostgreSQL in: ${HOMEBREW_PREFIX}")
message(STATUS "PostgreSQL_INCLUDE_DIR: ${PostgreSQL_INCLUDE_DIR}")
message(STATUS "PostgreSQL_LIBRARY: ${PostgreSQL_LIBRARY}")

# Проверяем, что PostgreSQL найден
if(NOT PostgreSQL_INCLUDE_DIR)
    message(STATUS "Checking common PostgreSQL locations...")
    execute_process(COMMAND find ${HOMEBREW_PREFIX} -name "libpq-fe.h" 2>/dev/null
            OUTPUT_VARIABLE FOUND_HEADERS
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(FOUND_HEADERS)
        message(STATUS "Found PostgreSQL headers at: ${FOUND_HEADERS}")
    endif()
    message(FATAL_ERROR "PostgreSQL headers not found. Please install PostgreSQL with: brew install postgresql")
endif()

if(NOT PostgreSQL_LIBRARY)
    message(STATUS "Checking common PostgreSQL library locations...")
    execute_process(COMMAND find ${HOMEBREW_PREFIX} -name "libpq*" 2>/dev/null
            OUTPUT_VARIABLE FOUND_LIBS
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(FOUND_LIBS)
        message(STATUS "Found PostgreSQL libraries at: ${FOUND_LIBS}")
    endif()
    message(FATAL_ERROR "PostgreSQL library not found. Please install PostgreSQL with: brew install postgresql")
endif()

# Устанавливаем переменные для PostgreSQL
set(PostgreSQL_INCLUDE_DIRS ${PostgreSQL_INCLUDE_DIR})
set(PostgreSQL_LIBRARIES ${PostgreSQL_LIBRARY})
set(PostgreSQL_FOUND TRUE)

# Ищем libpqxx заголовки
find_path(LIBPQXX_INCLUDE_DIR pqxx/pqxx
        PATHS
        ${HOMEBREW_PREFIX}/include
        /usr/local/include
        /opt/homebrew/include
        NO_DEFAULT_PATH
)

# Ищем libpqxx библиотеку
find_library(LIBPQXX_LIBRARY
        NAMES pqxx libpqxx
        PATHS
        ${HOMEBREW_PREFIX}/lib
        /usr/local/lib
        /opt/homebrew/lib
        NO_DEFAULT_PATH
)

# Проверяем, что libpqxx найден
if(NOT LIBPQXX_INCLUDE_DIR)
    message(FATAL_ERROR "libpqxx headers not found. Please install libpqxx with: brew install libpqxx")
endif()

if(NOT LIBPQXX_LIBRARY)
    message(FATAL_ERROR "libpqxx library not found. Please install libpqxx with: brew install libpqxx")
endif()

# Создаем исполняемый файл
add_executable(LabRabota4 main.cpp
        main.cpp
        Task.cpp
        Task.h
        User.cpp
        User.h
        TaskStatus.cpp
        TaskStatus.h
)

# Добавляем директории заголовочных файлов
target_include_directories(LabRabota4 PRIVATE
        ${PostgreSQL_INCLUDE_DIRS}
        ${LIBPQXX_INCLUDE_DIR}
)

# Линкуем библиотеки
target_link_libraries(LabRabota4
        ${PostgreSQL_LIBRARIES}
        ${LIBPQXX_LIBRARY}
)

# Добавляем флаги компиляции
target_compile_options(LabRabota4 PRIVATE -std=c++20)

# Выводим информацию для отладки
message(STATUS "Homebrew prefix: ${HOMEBREW_PREFIX}")
message(STATUS "PostgreSQL include dir: ${PostgreSQL_INCLUDE_DIRS}")
message(STATUS "PostgreSQL library: ${PostgreSQL_LIBRARIES}")
message(STATUS "libpqxx include dir: ${LIBPQXX_INCLUDE_DIR}")
message(STATUS "libpqxx library: ${LIBPQXX_LIBRARY}")