#include "Scene.h"

#include <QGraphicsSceneMouseEvent>

namespace
{
	const double cPointSize = 6.0;
	const QBrush cPointBrush = QBrush(Qt::black);
}

Scene::Scene(Delegate* ip_delegate)
: mp_delegate(ip_delegate)
{
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* ip_event)
{
	const QPointF mouse_position = ip_event->scenePos();
	
	addEllipse(mouse_position.x() - cPointSize / 2,
			   mouse_position.y() - cPointSize / 2,
			   cPointSize,
			   cPointSize,
			   QPen(),
			   cPointBrush);
	
	m_points.push_back(mouse_position.toPoint());
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *ip_event)
{
if (mp_delegate)
	mp_delegate->onMouseMove(ip_event);
}
