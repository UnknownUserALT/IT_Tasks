// Реализация OpenLibraryClient.
// Запрос к API: GET https://openlibrary.org/api/books?bibkeys=ISBN:<isbn>&format=json&jscmd=data
// Ответ — JSON, где ключ "ISBN:<isbn>", значение — объект с полями title, authors, publish_date и др.

#include "openlibraryclient.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QTimer>
#include <QFile>

OpenLibraryClient::OpenLibraryClient(QObject *parent)
    : QObject(parent)
{
    // QNAM создаётся здесь, а не в UI-потоке.
    // После moveToThread этот конструктор уже вызван, но QNAM будет обрабатывать
    // ответы в event loop того потока, куда перемещён клиент.
    m_nam = new QNetworkAccessManager(this);

    // Таймер для автоматической отмены запроса при отсутствии ответа (15 сек)
    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    m_timeoutTimer->setInterval(15000);
    connect(m_timeoutTimer, &QTimer::timeout,
            this, &OpenLibraryClient::onTimeout);
}

void OpenLibraryClient::fetchByIsbn(const QString &isbn)
{
    // Если предыдущий запрос ещё активен — отменяем его
    if (m_currentReply) {
        m_currentReply->abort();
        m_currentReply = nullptr;
    }

    const QString url =
        QStringLiteral("https://openlibrary.org/api/books?"
                       "bibkeys=ISBN:%1&format=json&jscmd=data").arg(isbn);
    QNetworkRequest req((QUrl(url)));
    req.setRawHeader("User-Agent", "BMSTU-LibraryCatalog/1.0");
    m_currentReply = m_nam->get(req);

    // Подключаем обработчик к конкретному reply, а не к глобальному QNAM::finished,
    // чтобы ответы на обложки не попадали в onReplyFinished
    connect(m_currentReply, &QNetworkReply::finished,
            this, [this]() { onReplyFinished(m_currentReply); });
    m_timeoutTimer->start();
}

void OpenLibraryClient::cancelRequest()
{
    m_timeoutTimer->stop();
    if (m_currentReply) {
        m_currentReply->abort();
        // abort() вызовет finished() → onReplyFinished() → emit failed()
    }
}

void OpenLibraryClient::onTimeout()
{
    // Таймаут — отменяем запрос, пользователь получит сообщение об ошибке
    if (m_currentReply) {
        m_currentReply->abort();
    }
}

void OpenLibraryClient::onReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();    // Обязательно! Иначе утечка памяти.
    m_timeoutTimer->stop();
    m_currentReply = nullptr;

    // Проверяем сетевую ошибку (нет интернета, таймаут, отмена и т.д.)
    if (reply->error() != QNetworkReply::NoError) {
        emit failed(tr("Ошибка сети: %1").arg(reply->errorString()));
        return;
    }

    // Парсим JSON-ответ
    const QByteArray data = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        emit failed(tr("Ошибка разбора JSON: %1").arg(parseError.errorString()));
        return;
    }

    // Ответ — объект {"ISBN:1234...": {...}}. Если пустой — книга не найдена.
    QJsonObject root = doc.object();
    if (root.isEmpty()) {
        emit failed(tr("Книга с таким ISBN не найдена в OpenLibrary."));
        return;
    }

    // Берём первый (и единственный) ключ — "ISBN:<номер>"
    const QString key = root.keys().first();
    QJsonObject bookObj = root.value(key).toObject();

    // Собираем QVariantMap для передачи в UI через сигнал
    QVariantMap result;
    result["title"] = bookObj.value("title").toString();

    // Авторы — массив объектов [{name: "..."}, ...], склеиваем через запятую
    QJsonArray authors = bookObj.value("authors").toArray();
    QStringList authorNames;
    for (const QJsonValue &a : authors)
        authorNames.append(a.toObject().value("name").toString());
    result["author"] = authorNames.join(", ");

    result["publish_date"] = bookObj.value("publish_date").toString();
    result["pages"] = bookObj.value("number_of_pages").toInt();

    // Жанр — берём первый элемент из массива subjects
    QJsonArray subjects = bookObj.value("subjects").toArray();
    if (!subjects.isEmpty())
        result["genre"] = subjects.first().toObject().value("name").toString();

    // ISBN извлекаем из ключа, убирая префикс "ISBN:"
    result["isbn"] = key.mid(5);

    emit finished(result);
}

void OpenLibraryClient::fetchCover(const QString &isbn, const QString &cachePath)
{
    // Загрузка обложки с covers.openlibrary.org (размер M — средний)
    QString coverUrl = QStringLiteral("https://covers.openlibrary.org/b/isbn/%1-M.jpg").arg(isbn);
    QNetworkRequest req((QUrl(coverUrl)));
    req.setRawHeader("User-Agent", "BMSTU-LibraryCatalog/1.0");

    QNetworkReply *reply = m_nam->get(req);
    // Обработчик подключён к конкретному reply через лямбду
    connect(reply, &QNetworkReply::finished, this, [this, reply, isbn, cachePath]() {
        reply->deleteLater();
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray imgData = reply->readAll();
            // Сохраняем только если данные не пустые (OpenLibrary может вернуть пустой ответ)
            if (!imgData.isEmpty()) {
                QFile f(cachePath);
                if (f.open(QIODevice::WriteOnly)) {
                    f.write(imgData);
                    f.close();
                }
                emit coverReady(isbn, cachePath);
            }
        }
    });
}
