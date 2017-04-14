#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QTime>
#include <QGraphicsSceneMouseEvent>

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer;
    timer->setSingleShot(true);

    bar	= 0;
    count_loc		= 0;
    count_gl		= 0;
    count_img_good	= 1;
    count_img_bad	= 0;
    _tcpSocket		= 0;
    dr_state		= MB_RELEASE;
    _rect.setRect(0, 0, 0, 0);
    _pause = true;


    QPalette p = ui->progressBar->palette();
    p.setColor(QPalette::Highlight, Qt::gray);
    ui->progressBar->setPalette(p);
    ui->progressBar->show();

    map			= new QPixmap();
    itm			= new QGraphicsPixmapItem;
    scene		= new CustomScene;
    rectItem	= new QGraphicsRectItem;
    painter		= new QPainter;

    QPen pen(Qt::green, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    rectItem->setPen(pen);
    scene->addItem(itm);
    scene->addItem(rectItem);
    ui->graphicsView->setScene(scene);

    connect( scene, SIGNAL(customMouseEvent(QGraphicsSceneMouseEvent *)), this, SLOT(mouseEvent(QGraphicsSceneMouseEvent *)));

    connect(timer, &QTimer::timeout, [this](){
        bar++;
        timer->start(500);
        if ( bar >= 120 )
            bar = 60;
        ui->progressBar->setValue(bar);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::imgShow(const QByteArray & frame, char place,  char pack,  uint16_t size)
{
    if ( _pause ) {
        if ( _place == pack ) {
            count_loc++;
        } else {
            count_loc = 1;
        }

        ui->lcdNumber->display(count_loc);
        ui->lcdNumber_2->display(++count_gl);

        _place = pack;

        qDebug() << "imgShow";

        ui->label_2->setText(
            QString("place #%1\ncam  #%2\nsize %3").arg(place).arg(pack).arg(size) );

        if ( map->loadFromData(frame) ) {
            ++count_img_good;
        } else {
            ++count_img_bad;
        }

        double tmp = ( count_img_bad / (count_img_bad + count_img_good) ) * 100;
        ui->lcdNumber_4->display(count_img_good);
        ui->lcdNumber_3->display(count_img_bad);
        ui->lcdNumber_5->display(tmp);

        itm->setPixmap(*map);
        scene->setSceneRect( 0, 0, map->width(), map->height() );
        ui->graphicsView->fitInView(itm, Qt::AspectRatioMode::IgnoreAspectRatio);
        ui->graphicsView->update();

        bar = 0;
    }
}   // MainWindow::imgShow

void MainWindow::on_pushButton_clicked()
{
    _tcpSocket = new My_TCPSocket(1235);

    connect( _tcpSocket, SIGNAL(talking(const QString&)), this, SLOT(talking(const QString&)));
    connect( _tcpSocket, SIGNAL(recieveGoOn(quint32)), this, SLOT(process(quint32)));
    connect( _tcpSocket, SIGNAL(imgComplete(const QByteArray&,char,char,uint16_t)), this, SLOT(imgShow(const QByteArray&,char,char,uint16_t)));

    connect(_tcpSocket,  &My_TCPSocket::startrecieve, [this](quint32 size){
        ui->progressBar->setMaximum( static_cast<int>(size) );
    });

    ui->label->setText("up TCP server");
}

void MainWindow::talking(const QString& s)
{
    ui->label->setText(s);
}

void MainWindow::mouseEvent(QGraphicsSceneMouseEvent * e)
{
    switch ( e->type() ) {
        case QEvent::GraphicsSceneMousePress: {
            ui->lineEdit->setText( QString("x= %1 \n y=%2").arg( e->scenePos().x() ).arg( e->scenePos().y() ) );
            _coord_start.setX( e->scenePos().x() );
            _coord_start.setY( e->scenePos().y() );
        }
        break;
        case QEvent::GraphicsSceneMouseRelease: {
            ui->lineEdit->setText( QString("%1,%2,%3,%4").arg( static_cast<int>( _coord_start.x() ) ).arg( static_cast<int>( _coord_start.y() ) ).arg( static_cast<int>( e->scenePos()
              .x() - _coord_start.x() ) ).arg( static_cast<int>( e->scenePos().y() - _coord_start.y() ) ) );
        }
        break;
        case QEvent::GraphicsSceneMouseMove: {
            _rect.setRect( _coord_start.x(), _coord_start.y(), e->scenePos().x() - _coord_start.x(), e->scenePos().y() - _coord_start.y() );
            rectItem->setRect(_rect);
            scene->update();
        }
        break;
        default:
            break;
    }
}

void MainWindow::process(quint32 size)
{
    ui->progressBar->setValue( ui->progressBar->maximum() -  static_cast<int>(size) );
}

void MainWindow::on_pushButton_2_clicked()
{
    _tcpSocket->sendToSocket( ui->spinBox->value() );
}

void MainWindow::on_pushButton_3_clicked()
{
    if ( _pause ) {
        ui->pushButton_3->setText("go");
        _pause = false;
    } else {
        ui->pushButton_3->setText("pause");
        _pause = true;
    }
}

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
