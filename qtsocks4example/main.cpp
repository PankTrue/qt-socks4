#include <QCoreApplication>
#include <QObject>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "qt-socks4.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSocks4 *client = new QSocks4(new QNetworkProxy,"84.47.135.132",4145);


    QNetworkRequest request(QUrl("http://httpbin.org/get"));

    QNetworkAccessManager m;
    m.setProxy(*client->get_proxy());

    QNetworkReply* reply = m.get(request);

    QObject::connect( reply, &QNetworkReply::finished, [&](){
        qDebug() << reply->readAll();
        client->deleteLater();
        a.quit();
    });




    return a.exec();
}
