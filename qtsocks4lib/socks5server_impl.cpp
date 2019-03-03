#include "socks5server_impl.h"

Socks5Server_Impl::Socks5Server_Impl(QTcpSocket *socket, QObject *parent) : QObject(parent), _socket(socket), cur_step(greeting) {}

Socks5Server_Impl::~Socks5Server_Impl() {}

void Socks5Server_Impl::start()
{
    connect(_socket,&QTcpSocket::readyRead,
            this,&Socks5Server_Impl::handleReadyRead);
}

void Socks5Server_Impl::handleReadyRead()
{
    QByteArray buf = _socket->readAll();


    switch (cur_step)
    {
        case greeting:
            if( buf.at(0) != 5 ||
                buf.at(1) != 1 ||
                buf.at(2) != 0) { qWarning() << "error in step:" << cur_step; this->deleteLater();}

            buf.clear();

            buf.append((char)5)
               .append((char)0);

            _socket->write(buf);

            cur_step = auth;
            break;


        case auth:
            if( buf.at(0) != 5 ||
                buf.at(1) != 1 ||
                buf.at(2) != 0 ){ qWarning() << "error in step:" << cur_step; this->deleteLater(); }

            switch (buf.at(3))
            {
                case 1: //ipv4

                    set_dest_host(qMakePair(
                                  qFromBigEndian<quint32>(buf.data()+4),
                                  qFromBigEndian<quint16>(buf.data()+8) ));

                    break;
                case 3: //domain
                    _dest_domain.resize(buf.at(4));

                    memcpy(_dest_domain.data(),buf.data()+5,_dest_domain.size());

                    _dest_host = QHostInfo::fromName(_dest_domain);

                    if(_dest_host.addresses().count() <= 0)
                    {
                        qWarning() << "unable to resolve domain" << _dest_domain;
                        this->deleteLater();
                        return;
                    }

                    set_dest_host(qMakePair(
                                      _dest_host.addresses().at(0).toIPv4Address(),
                                      qFromBigEndian<quint16>(buf.data() + 4 + _dest_domain.size() + 1) ));

                    break;

                default: qWarning() << "address type isn't support"; break;
            }

            buf.clear();

            buf.append((char)0x05)
               .append((char)0x00)
               .append((char)0x00)
               .append((char)0x01)
               .append(6,(char)0x00);

            _socket->write(buf);
            cur_step = finally;
            break;
        case finally:
            emit have_data(buf);
            break;
        default: ;break;
    }
}

void Socks5Server_Impl::send(QByteArray data)
{
    _socket->write(data);
}
