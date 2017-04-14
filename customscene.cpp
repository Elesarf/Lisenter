#include "customscene.h"
#include <QDebug>

CustomScene::CustomScene(QObject *parent) :
    QGraphicsScene()
{
    Q_UNUSED(parent);
}

CustomScene::~CustomScene()
{

}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit customMouseEvent(event);
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit customMouseEvent(event);
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit customMouseEvent(event);
}
