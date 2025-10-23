#!/bin/bash

# Проверяем, передан ли аргумент
if [ $# -eq 0 ]; then
    echo "Ошибка: Необходимо передать имя процесса в качестве аргумента"
    echo "Пример использования: ./kill_process.sh process_name"
    exit 1
fi

process_name="$1"

echo "Поиск процесса: $process_name"

# Проверяем, запущен ли процесс с помощью pgrep
if pgrep "$process_name" > /dev/null 2>&1; then
    echo "Процесс '$process_name' найден. Завершение..."
    
    # Завершаем процесс с помощью pkill
    if pkill "$process_name"; then
        echo "Процесс '$process_name' успешно завершен"
    else
        echo "Ошибка: Не удалось завершить процесс '$process_name'"
        exit 1
    fi
else
    echo "Процесс '$process_name' не запущен"
    exit 1
fi