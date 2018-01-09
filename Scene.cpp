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
	const QPen cLineSolidBlackPen = QPen(QBrush(Qt::black), cLineWidth);
	
	const QPen cLineDashGreenPen = QPen(QBrush(Qt::green), cLineWidth, Qt::DashLine);
	const QPen cLineDashRedPen = QPen(QBrush(Qt::red), cLineWidth, Qt::DashLine);
	const QPen cLineDashBlackPen = QPen(QBrush(Qt::black), cLineWidth, Qt::DashLine);
}

Scene::Scene(Delegate* ip_delegate)
: mp_delegate(ip_delegate)
{
}

void Scene::Reset()
{
	clear();

	m_points.clear();
	m_lines.clear();
	
	mp_next_connection_line = nullptr;
	mp_last_connection_line = nullptr;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent* ip_event)
{
	if (_LinesHaveCollisions())
		return;
	
	const QPointF mouse_position = ip_event->scenePos();
	
	addEllipse(mouse_position.x() - cPointSize / 2,
			   mouse_position.y() - cPointSize / 2,
			   cPointSize,
			   cPointSize,
			   QPen(),
			   cPointBrush);
	
	if (m_lines.empty())
	{
		if (m_points.empty())
			mp_next_connection_line = addLine(QLineF(mouse_position, mouse_position), cLineDashBlackPen);
		else
			mp_last_connection_line = addLine(QLineF(m_points.front(), mouse_position), cLineDashGreenPen);
	}
	
	if (!m_points.empty())
		m_lines.push_back(addLine(QLineF(m_points.back(), mouse_position), cLineSolidBlackPen));
	
	m_points.push_back(mouse_position);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *ip_event)
{
	if (mp_delegate)
		mp_delegate->onMouseMove(ip_event);

	if (!mp_next_connection_line)
		return;
	
	mp_next_connection_line->setLine(QLineF(m_points.back(), ip_event->scenePos()));
	
	if (!mp_last_connection_line)
		return;
	
	mp_last_connection_line->setLine(QLineF(m_points.front(), ip_event->scenePos()));
	
	if (_LinesHaveCollisions())
	{
		mp_next_connection_line->setPen(cLineSolidRedPen);
		mp_last_connection_line->setPen(cLineDashRedPen);
	}
	else
	{
		mp_next_connection_line->setPen(cLineSolidGreenPen);
		mp_last_connection_line->setPen(cLineDashGreenPen);
	}
}

bool Scene::_LinesHaveCollisions()
{
	if (!mp_next_connection_line || !mp_last_connection_line)
		return false;
	
	for (QGraphicsLineItem* p_line : m_lines)
	{
		// next connection line always has collision with the last one added line
		if (p_line == m_lines.back())
			continue;
		
		if (mp_next_connection_line->collidesWithItem(p_line))
			return true;
	}
	
	for (QGraphicsLineItem* p_line : m_lines)
	{
		// last connection line always has collision with the first one line
		if (p_line == m_lines.front())
			continue;
		
		if (mp_last_connection_line->collidesWithItem(p_line))
			return true;
	}
	
	return false;
}

