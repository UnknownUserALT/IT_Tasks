#!/bin/bash

# Проверяем аргументы
if [ $# -eq 0 ]; then
    echo "Ошибка: Укажите имя файла"
    echo "Пример: ./file_info.sh filename.txt"
    exit 1
fi

if [ ! -f "$1" ]; then
    echo "Ошибка: Файл '$1' не существует или не является обычным файлом"
    exit 1
fi

echo "Информация о файле: $1"
echo "================================"

# Получаем всю информацию одной командой ls -l
file_info=$(ls -l "$1")

# Извлекаем нужные данные
permissions=$(echo "$file_info" | awk '{print $1}')
size=$(echo "$file_info" | awk '{print $5}')
date=$(echo "$file_info" | awk '{print $6, $7, $8}')

echo "Права доступа: $permissions"
echo "Размер: $size байт"
echo "Дата модификации: $date"