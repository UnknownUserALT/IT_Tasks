/**
 * @file main.cpp
 * @brief Точка входа в приложение "Окно, которое нельзя закрыть"
 *
 * Игра-головоломка из 4 фаз. Каждая фаза - уникальный способ
 * сопротивления закрытию окна.
 */

#include "MainWindow.h"
#include "Phase1Widget.h"
#include <QApplication>

/**
 * @brief Главная функция приложения
 * @param argc Количество аргументов командной строки
 * @param argv Массив аргументов командной строки
 * @return Код завершения приложения
 */
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Предотвращаем закрытие приложения при закрытии всех окон
    // (важно для фазы 3 с множеством клонов)
    a.setQuitOnLastWindowClosed(false);

    // Создаём и показываем главное окно
    MainWindow window;
    window.show();

    return QApplication::exec();
}
