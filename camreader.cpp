#include "camreader.h"

#include <QSerialPortInfo>
#include <QDebug>

#include <iostream>

static const char INIT_STR[] = "3\r";

CamReader::CamReader(QObject* parent)
    : QObject(parent),
      state(S_START)

{
  connect(&serial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void CamReader::onReadyRead() {
  QByteArray jpg;
  jpg.append(static_cast<char>(0xff));
  jpg.append(static_cast<char>(0xd8));

  jpg.append(static_cast<char>(0xff));
  jpg.append(static_cast<char>(0xd9));

  switch (state) {
    case S_START: {
      buffer.clear();
      buffer = serial.readAll();
      qint32 indx = 0;
      for (qint32 i = 1; i < buffer.length(); ++i) {
        try {
          if ((buffer.at(i - 1) == jpg.at(0)) && (buffer.at(i) == jpg.at(1))) {
            place = buffer.at(i - 7);
            pack = buffer.at(i - 6);
            char c_to_ui[2] = {buffer.at(i - 2), buffer.at(i - 3)};
            size = *(uint16_t*)&c_to_ui;
            qDebug() << "get frame";
            qDebug() << "place #" << place << " pack #" << pack;
            emit talking(QString("get frame from place #%1, pack #%2")
                             .arg(place)
                             .arg(pack));
            indx = i;
          }
        } catch (...) {
          qDebug() << "Error: some wrong";
        }
      }

      if (indx) {
        frame.clear();
        frame = buffer.mid(indx - 1, -1);
        state = S_READ;
      }
    } break;

    case S_READ: {
      buffer.clear();
      buffer = serial.readAll();
      qint32 indx = 0;
      for (qint32 i = 1; i < buffer.length(); ++i) {
        if ((buffer.at(i - 1) == jpg.at(2)) && (buffer.at(i) == jpg.at(3))) {
          qDebug() << "get frame";
          indx = i;
        }
      }

      if (!indx) {
        frame.append(buffer);
      } else {
        frame.append(buffer.left(indx + 1));
        qDebug() << "frame recieve full";
        emit talking(QString("frame recieve end"));
        emit imgComplete(frame, place, pack, size);
        state = S_START;
      }

    } break;
  }

  buffer.clear();
}

bool CamReader::getImage() {
  auto written = serial.write(INIT_STR);
  qDebug() << "getImage";
  qDebug() << "INIT: Written: " << written;

  if (!serial.waitForBytesWritten(100)) {
    qWarning() << "INIT: I/O error: " << serial.errorString();
  }
  buffer.clear();
  state = S_START;
  return false;
}

bool CamReader::open(int numPort) {
  qDebug() << "COM Port connector v1b";
  emit talking(QString("COM Port connector v1b"));

  QSerialPortInfo info[2];
  int i = 0;

  foreach (const QSerialPortInfo& infport, QSerialPortInfo::availablePorts()) {
    info[i] = infport;

    qDebug() << i << " is : \n";
    qDebug() << "   Name :          " << info[i].portName() << "\n";
    qDebug() << "   Description :   " << info[i].description() << "\n";
    qDebug() << "   Manufacturer:   " << info[i].manufacturer() << "\n";

    ++i;
  }

  serial.setPortName(info[numPort].portName());

  qDebug() << "open port => " << info[numPort].portName() << "\n";

  if (!serial.open(QIODevice::ReadWrite)) {
    qWarning() << "error open port";
    emit talking(QString("error open port"));
    return false;
  }

  serial.setBaudRate(115200);
  serial.setDataBits(QSerialPort::Data8);

  qDebug() << "Port already open ";
  emit talking(QString("Port already open"));
  return true;
}

void CamReader::reset() {}

void CamReader::changeChannel(int ch) {
  QByteArray str = "\r";
  str.prepend(QByteArray::number(ch));
  auto written = serial.write(str.data());
  qDebug() << "INIT: Written: " << written;

  if (!serial.waitForBytesWritten(100)) {
    qWarning() << "INIT: I/O error: " << serial.errorString();
  }
  buffer.clear();
  state = S_START;
}
