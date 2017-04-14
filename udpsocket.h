#ifndef UDPSOCET_H
#define UDPSOCET_H

#include <QObject>
#include <QUdpSocket>

class UDPSocket: public QObject
{
    Q_OBJECT
public:
    explicit UDPSocket(QObject *parent = 0);

    enum STATE {
        S_START,
        S_READ
    };

    void setPort(quint16 p){ _port = p;}
    void setIp(QString s){ _ip = s;}
    bool exist(){return true;}

signals:
        void imgComplete(const QByteArray &, char, char, uint16_t);
        void talking(const QString &);
    
public slots:

    void onReadyRead();
    
private:
    QUdpSocket *_socket;
    quint16 _port;
    quint16 _senderPort;
    QHostAddress _ip;
    QHostAddress _sender;

    QByteArray _buffer;
    QByteArray _jpg;
    QByteArray _frame;



    char _place;
    char _pack;
    uint16_t _size;

    STATE _state;
};

#endif // UDPSOCET_H
