// Точка входа в приложение «Каталог электронной библиотеки».
// Создаёт QApplication, открывает базу данных и запускает главное окно.

#include <QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Имя организации и приложения используются QSettings для хранения настроек
    QCoreApplication::setOrganizationName("BMSTU");
    QCoreApplication::setApplicationName("LibraryCatalog");

    // Инициализация базы данных (SQLite, файл в AppDataLocation)
    DatabaseManager db;
    if (!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Ошибка"),
            QObject::tr("Не удалось открыть базу данных:\n%1").arg(db.lastError()));
        return 1;
    }

    // Создание и отображение главного окна; передаём указатель на менеджер БД
    MainWindow w(&db);
    w.show();

    return app.exec();
}
