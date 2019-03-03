#ifndef QTSOCKS4CLIENT_H
#define QTSOCKS4CLIENT_H


#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QTcpServer>

#include <QMutableListIterator>
#include <QDebug>


#include "socks5server_impl.h"
#include "socks4client_impl.h"


class QSocks4 : public QObject
{

public:
    QSocks4(QObject *parent = NULL);
    QSocks4(QNetworkProxy *proxy, QString proxy_addr, quint16 proxy_port, QObject *parent = NULL);
    ~QSocks4();

    void start();


    bool isStarted();


    uint16_t get_listen_port()              { return _listen_socket.serverPort(); }
    QNetworkProxy *get_proxy()              { return _proxy; }
    QPair<quint32,quint16> get_proxy_host() { return qMakePair(_proxy_addr,_proxy_port); }


    void set_proxy_host(QString addr, quint16 port);
    void set_proxy(QNetworkProxy *proxy);

private slots:
    void onNewIncomingConnection();


private:
    QTcpServer _listen_socket;

    quint32 _proxy_addr;
    quint16 _proxy_port;

    QNetworkProxy *_proxy;

};

#endif // QTSOCKS4CLIENT_H
