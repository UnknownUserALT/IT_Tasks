// OpenLibraryClient — сетевой клиент для работы с OpenLibrary API.
// Живёт в отдельном потоке (moveToThread), чтобы не блокировать UI.
// Связь с главным окном — только через сигналы и слоты (Qt::QueuedConnection).
// QNetworkAccessManager создаётся внутри клиента (в его потоке), а не передаётся извне,
// потому что QNAM должен жить в том же потоке, где обрабатываются его ответы.

#pragma once
#include <QObject>
#include <QVariantMap>

class QNetworkAccessManager;
class QNetworkReply;
class QTimer;

class OpenLibraryClient : public QObject
{
    Q_OBJECT
public:
    explicit OpenLibraryClient(QObject *parent = nullptr);

public slots:
    /// Запрашивает данные о книге по ISBN из OpenLibrary Books API.
    /// Вызывается из UI-потока через QMetaObject::invokeMethod (QueuedConnection).
    void fetchByIsbn(const QString &isbn);

    /// Отменяет текущий запрос (вызывается при повторном нажатии кнопки «Найти»)
    void cancelRequest();

    /// Скачивает обложку книги с covers.openlibrary.org и сохраняет в cachePath.
    /// По завершении эмитит сигнал coverReady.
    void fetchCover(const QString &isbn, const QString &cachePath);

signals:
    /// Успешно получены данные о книге (title, author, isbn, pages, genre, publish_date)
    void finished(const QVariantMap &data);

    /// Ошибка при запросе (сеть, парсинг, книга не найдена)
    void failed(const QString &message);

    /// Обложка успешно скачана и сохранена по cachePath
    void coverReady(const QString &isbn, const QString &cachePath);

private slots:
    /// Обрабатывает ответ на запрос данных о книге (парсинг JSON)
    void onReplyFinished(QNetworkReply *reply);

    /// Срабатывает при истечении таймаута (15 сек) — отменяет запрос
    void onTimeout();

private:
    QNetworkAccessManager *m_nam = nullptr;          // Менеджер сетевых запросов (один на клиента)
    QNetworkReply         *m_currentReply = nullptr;  // Текущий активный запрос (для отмены)
    QTimer                *m_timeoutTimer = nullptr;  // Таймер автоматической отмены запроса
};
