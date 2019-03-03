#include "qt-socks4.h"



QSocks4::QSocks4(QObject *parent) : QObject(parent){}

QSocks4::QSocks4(QNetworkProxy *proxy, QString address, quint16 port, QObject *parent) :
    QSocks4(parent)
{
    set_proxy(proxy);
    set_proxy_host(address,port);
    start();
}

QSocks4::~QSocks4()
{
    delete _proxy;
}


void QSocks4::start()
{
    if(!_listen_socket.listen(QHostAddress::Any,0))
    {
        qWarning() << "failed to listen on " << _listen_socket.serverAddress()
                   << ":" << _listen_socket.serverPort();
        this->deleteLater();
    }

    qInfo() << "socks5 server listen on:" << _listen_socket.serverPort();

    _proxy->setHostName(_listen_socket.serverAddress().toString());
    _proxy->setPort(_listen_socket.serverPort());
    _proxy->setType(QNetworkProxy::Socks5Proxy);

    connect(&_listen_socket,&QTcpServer::newConnection,
            this,&QSocks4::onNewIncomingConnection);
}

void QSocks4::set_proxy_host(QString addr, quint16 port)
{
    _proxy_addr = QHostAddress(addr).toIPv4Address();
    _proxy_port = port;
}

void QSocks4::set_proxy(QNetworkProxy *proxy)
{
    _proxy = proxy;
}


void QSocks4::onNewIncomingConnection()
{
    while(_listen_socket.hasPendingConnections())
    {
        QTcpSocket *s = _listen_socket.nextPendingConnection();


        auto socks5 = new Socks5Server_Impl(s,this);
        auto socks4 = new Socks4Client_Impl(_proxy_addr,_proxy_port,this);

        connect(socks4,&Socks4Client_Impl::destroyed,
                socks5,&Socks5Server_Impl::deleteLater);

        connect(socks5,&Socks5Server_Impl::destroyed,
                socks4,&Socks4Client_Impl::deleteLater);

        connect(socks5,&Socks5Server_Impl::dest_infoChanged,
                socks4,&Socks4Client_Impl::set_dest_info);

        connect(socks4,&Socks4Client_Impl::have_data,
                socks5,&Socks5Server_Impl::send);

        connect(socks5,&Socks5Server_Impl::have_data,
                socks4,&Socks4Client_Impl::send);


        socks5->start();
    }
}
