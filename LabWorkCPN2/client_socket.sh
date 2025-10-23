#!/bin/bash

# Клиентский сокет с использованием netcat
SERVER_HOST="localhost"
SERVER_PORT="54321"
MESSAGE="Привет, сервер!"

echo "Клиентский сокет: подключение к серверу $SERVER_HOST:$SERVER_PORT"

# Проверяем доступность netcat
if ! command -v nc &> /dev/null; then
    echo "Ошибка: netcat (nc) не установлен"
    echo "Установите его: sudo apt-get install netcat"
    exit 1
fi

# Отправляем сообщение и получаем ответ
echo "Отправка сообщения: $MESSAGE"
echo "----------------------------------------"

response=$(echo "$MESSAGE" | nc -w 5 "$SERVER_HOST" "$SERVER_PORT")

if [ $? -eq 0 ]; then
    echo "Ответ от сервера: $response"
else
    echo "Ошибка: Не удалось подключиться к серверу $SERVER_HOST:$SERVER_PORT"
    echo "Убедитесь, что сервер запущен и доступен"
fi