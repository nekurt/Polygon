#include "Scene.h"

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>

namespace
{
	const double cPointSize = 4.0;
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
	
	if (m_points.empty())
		mp_connection_line = addLine(QLineF(mouse_position, mouse_position));
	else
		addLine(QLineF(m_points.back(), mouse_position));
	
	m_points.push_back(mouse_position);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *ip_event)
{
	if (mp_delegate)
		mp_delegate->onMouseMove(ip_event);
	
	if (mp_connection_line)
		mp_connection_line->setLine(QLineF(m_points.back(), ip_event->scenePos()));
}
