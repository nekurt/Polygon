#include "Scene.h"

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>

namespace
{
	const double cPointSize = 4.0;
	const QBrush cPointBrush = QBrush(Qt::black);

	const double cLineWidth = 2.0;
	
	const QPen cLineSolidGreenPen = QPen(QBrush(Qt::green), cLineWidth);
	const QPen cLineSolidRedPen = QPen(QBrush(Qt::red), cLineWidth);
	
	const QPen cLineDashGreenPen = QPen(QBrush(Qt::green), cLineWidth, Qt::DashLine);
	const QPen cLineDashRedPen = QPen(QBrush(Qt::red), cLineWidth, Qt::DashLine);
	const QPen cLineDashBlackPen = QPen(QBrush(Qt::black), cLineWidth, Qt::DashLine);
	
	const QPen cLineSolidBlackPen = QPen(QBrush(Qt::black), cLineWidth);
}

Scene::Scene(Delegate* ip_delegate)
: mp_delegate(ip_delegate)
{
}

void Scene::Reset()
{
	m_points.clear();
	clear();
	mp_next_connection_line = nullptr;
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
		mp_next_connection_line = addLine(QLineF(mouse_position, mouse_position), cLineDashBlackPen);
	else
	{
		mp_next_connection_line->setPen(cLineSolidGreenPen);
		addLine(QLineF(m_points.back(), mouse_position), cLineSolidBlackPen);
	}
	
	m_points.push_back(mouse_position);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *ip_event)
{
	if (mp_delegate)
		mp_delegate->onMouseMove(ip_event);
	
	if (mp_next_connection_line)
		mp_next_connection_line->setLine(QLineF(m_points.back(), ip_event->scenePos()));
}
