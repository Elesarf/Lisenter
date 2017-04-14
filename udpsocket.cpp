#include "udpsocket.h"

#include <QByteArray>

UDPSocket::UDPSocket(QObject* parent) : QObject(parent) {
  qDebug() << "socket start";
  _state = S_START;

  _jpg.append(static_cast<char>(0xff));
  _jpg.append(static_cast<char>(0xd8));
  _jpg.append(static_cast<char>(0xff));
  _jpg.append(static_cast<char>(0xd9));

  _socket = new QUdpSocket(this);

  if (_ip.isNull()) {
    _socket->bind(QHostAddress::AnyIPv4, 1234, QUdpSocket::ShareAddress);
  } else {
    _socket->bind(_ip, 1234, QUdpSocket::ShareAddress);
  }

  QObject::connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

}

void UDPSocket::onReadyRead() {
  qint32 indx = 0;
  _buffer.clear();
  while (_socket->hasPendingDatagrams()) {
    _buffer.resize(_socket->pendingDatagramSize());
    _socket->readDatagram(_buffer.data(), _buffer.size(), &_sender,
                          &_senderPort);
  }
  qDebug() << _sender.toString() << " " << _senderPort;
  switch (_state) {
    case S_START: {
      for (int i = 1; i < _buffer.length(); ++i) {
        if (_buffer.at(i - 1) == _jpg.at(0) && _buffer.at(i) == _jpg.at(1)) {
          _place = _buffer.at(i - 7);
          _pack = _buffer.at(i - 6);
          char c_to_ui[2] = {_buffer.at(i - 2), _buffer.at(i - 3)};
          _size = *(uint16_t*)&c_to_ui;
          qDebug() << "get frame";
          qDebug() << "place #" << _place << " pack #" << _pack;
          emit talking(QString("get frame from place #%1, pack #%2")
                           .arg(_place)
                           .arg(_pack));
          indx = i;
        }
      }
      if (indx) {
        _frame.clear();
        _frame = _buffer.mid(indx - 1, -1);
        _state = S_READ;
      }

    }

    break;
    case S_READ: {
      for (qint32 i = 1; i < _buffer.length(); ++i) {
        if ((_buffer.at(i - 1) == _jpg.at(2)) &&
            (_buffer.at(i) == _jpg.at(3))) {
          qDebug() << "get frame";
          indx = i;
        }
      }

      if (!indx) {
        _frame.append(_buffer);
      } else {
        _frame.append(_buffer.left(indx + 1));
        qDebug() << "frame recieve full";
        emit talking(QString("frame recieve end"));
        emit imgComplete(_frame, _place, _pack, _size);
        _state = S_START;
      }

    } break;
  }
}
