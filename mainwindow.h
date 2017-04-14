#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QTimer>
#include "my_tcpsocket.h"
#include "customscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();

    enum DRAW_STATE {
        MB_PRESS,
        MB_RELEASE,
        DRAWING
    };

public slots:
    void imgShow(const QByteArray &, char, char, uint16_t);
    void mouseEvent(QGraphicsSceneMouseEvent *);
    void process(quint32);

private slots:
    void on_pushButton_clicked();
    void talking(const QString &);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow * ui;
    CustomScene * scene;
    QGraphicsPixmapItem * itm;
    QGraphicsRectItem * rectItem;
    QPixmap * map;
    QPainter * painter;
    My_TCPSocket * _tcpSocket;
    QTimer * timer;
    quint16 bar;
    DRAW_STATE dr_state;
    QPointF _coord_start;
    QRectF _rect;

    int count_loc;
    int count_gl;
    bool _pause;
    double count_img_good, count_img_bad;
    char _place;
};

#endif  // MAINWINDOW_H
