#ifndef FAKEWEBVIEW_H
#define FAKEWEBVIEW_H

#include <QtNetwork>
#include <QWebView>

class FakeWebView : public QWebView
{
    Q_OBJECT

public:
    explicit FakeWebView(QWidget* parent = 0);

    void load(const QUrl& url);
    void load(const QNetworkRequest& request, QNetworkAccessManager::Operation operation = QNetworkAccessManager::GetOperation, const QByteArray &body = QByteArray());
    void setUrl(const QUrl &url);

};

#endif // FAKEWEBVIEW_H