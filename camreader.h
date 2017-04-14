#ifndef CAMREADER_H
#define CAMREADER_H

#include <QObject>

#include <QtSerialPort>


class CamReader : public QObject
{
    Q_OBJECT
public:
    enum State {
        S_START,
        S_READ
    };


    explicit CamReader(QObject *parent = 0);

signals:
    void imgComplete(const QByteArray &, char, char, uint16_t);
    void talking(const QString &);

public slots:
    bool open(int portNo);
    bool getImage();
    void reset();
    void changeChannel(int);

private slots:
    void onReadyRead();

private:
    struct Header {
        char id;
        char id2;
        uint32_t dataSize;
        int16_t jpg1[2];
    };

    QSerialPort serial;
    Header hdr;
    QByteArray buffer;
    QTimer resetTimer;
    QTimer watchDogTimer;
    QByteArray frame;

    char place;
    char pack;
    uint16_t size;
    State state;
};

#endif // CAMREADER_H
