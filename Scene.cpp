#include "Scene.h"

#include <QGraphicsSceneMouseEvent>

Scene::Scene(Delegate* ip_delegate)
: mp_delegate(ip_delegate)
{
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* ip_event)
{
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *ip_event)
{
if (mp_delegate)
	mp_delegate->onMouseMove(ip_event);
}
