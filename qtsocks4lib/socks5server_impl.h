#ifndef SOCKS5SERVER_IMPL_H
#define SOCKS5SERVER_IMPL_H

#include <QObject>
#include <QTcpSocket>
#include <QtEndian>
#include <QByteArray>
#include <QHostInfo>




class Socks5Server_Impl : public QObject
{
    Q_OBJECT
public:
    explicit Socks5Server_Impl(QTcpSocket *socket, QObject *parent = nullptr);
    ~Socks5Server_Impl();

    void start();

    QPair<quint32, quint16> dest_info() { return _dest_info; }
    void set_dest_host(QPair<quint32, quint16> dest_info) { _dest_info = dest_info; emit dest_infoChanged(_dest_info); }


signals:
    void have_data(QByteArray);
    void dest_infoChanged(QPair<quint32, quint16>);


public slots:
    void handleReadyRead();
    void send(QByteArray);



private:
    QTcpSocket *_socket;

    QByteArray _dest_domain;
    QHostInfo _dest_host;

    QPair<quint32, quint16> _dest_info;

    enum{
        greeting,
        auth,

        finally
    } cur_step;

};

#endif // SOCKS5SERVER_IMPL_H
