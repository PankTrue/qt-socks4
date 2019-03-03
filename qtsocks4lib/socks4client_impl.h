#ifndef SOCKS4CLIENT_IMPL_H
#define SOCKS4CLIENT_IMPL_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QtEndian>
#include <QHostAddress>
#include <QQueue>

#include <memory>



class Socks4Client_Impl : public QObject
{
    Q_OBJECT
public:
    explicit Socks4Client_Impl(quint32 proxy_addr, quint32 proxy_port,QObject *parent = nullptr);
    ~Socks4Client_Impl();


    void start();

    void set_dest_info(QPair<quint32,quint16> dest_info);


public slots:
    void send(QByteArray);


private slots:
    void handleReadyRead();
    void onConnected();


signals:
    void have_data(QByteArray);
    void connected();



private:
    void connect_to_proxy();
    void send_request();


    quint32 _dest_addr;
    quint16 _dest_port;

    quint32 _proxy_addr;
    quint16 _proxy_port;


    bool _connected;

    QQueue<QByteArray> _packet_queue;

    QTcpSocket _socket;

    enum{
        greeting,
        finally
    } cur_step;
};

#endif // SOCKS4CLIENT_IMPL_H
