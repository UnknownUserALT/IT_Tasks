#!/bin/bash

# Проверяем существование файла important.txt
if [ -f "important.txt" ]; then
    # Создаем резервную копию
    cp "important.txt" "important backup.txt"
    # Выводим сообщение о создании резервной копии
    echo "Резервная копия создана"
fi