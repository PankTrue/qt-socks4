#include "socks4client_impl.h"

Socks4Client_Impl::Socks4Client_Impl(quint32 proxy_addr, quint32 proxy_port, QObject *parent)
    : QObject(parent), _proxy_addr(proxy_addr), _proxy_port(proxy_port), cur_step(greeting), _connected(false)
{

}

Socks4Client_Impl::~Socks4Client_Impl() {}

void Socks4Client_Impl::start()
{
    connect(&_socket,&QTcpSocket::readyRead,
            this,&Socks4Client_Impl::handleReadyRead);

    connect(this,&Socks4Client_Impl::connected,
            this,&Socks4Client_Impl::onConnected);

    connect_to_proxy();
    send_request();
}

void Socks4Client_Impl::set_dest_info(QPair<quint32, quint16> dest_info)
{
    _dest_addr = dest_info.first;
    _dest_port = dest_info.second;
    start();
}

void Socks4Client_Impl::handleReadyRead()
{
    QByteArray buf = _socket.readAll();


    switch (cur_step)
    {
    case greeting:
        if(buf.at(0) != 0x00 ||
           buf.at(1) != 0x5a ) {this->deleteLater();}

        emit connected();

        cur_step=finally;
        break;
    case finally:
        emit have_data(buf);

        break;
    default:
        break;
    }
}

void Socks4Client_Impl::onConnected()
{
    _connected=true;
    QMutableListIterator<QByteArray> it(_packet_queue);
    while (it.hasNext())
    {
        _socket.write(it.next());
        it.remove();
    }
}

void Socks4Client_Impl::send(QByteArray data)
{
    if(_connected)
        _socket.write(data);
    else
        _packet_queue.push_back(data);
}

void Socks4Client_Impl::connect_to_proxy()
{
    _socket.connectToHost(QHostAddress(_proxy_addr),_proxy_port);
}

void Socks4Client_Impl::send_request()
{
    char buf[9];

    buf[0] = 0x04;
    buf[1] = 0x01;

    qToBigEndian<quint16>(_dest_port,buf+2);
    qToBigEndian<quint32>(_dest_addr,buf+4);

    buf[8] = '\0';

    if(_socket.write(buf,sizeof(buf)) <= 0) {qWarning() << "socks4 write <= 0"; this->deleteLater();}
}
