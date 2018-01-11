#include "Scene.h"

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>

namespace
{
	const int cMinimumPolygonVertexCount = 3;
	
	const double cPointSize = 4.0;
	const QBrush cPointBrush = QBrush(Qt::black);
	
	const QBrush cPolygonBrush = QBrush(Qt::green);

	const double cLineWidth = 2.0;
	
	const QPen cLineSolidGreenPen = QPen(QBrush(Qt::green), cLineWidth);
	const QPen cLineSolidRedPen = QPen(QBrush(Qt::red), cLineWidth);
	const QPen cLineSolidBlackPen = QPen(QBrush(Qt::black), cLineWidth);
	
	const QPen cLineDashGreenPen = QPen(QBrush(Qt::green), cLineWidth, Qt::DashLine);
	const QPen cLineDashRedPen = QPen(QBrush(Qt::red), cLineWidth, Qt::DashLine);
	const QPen cLineDashBlackPen = QPen(QBrush(Qt::black), cLineWidth, Qt::DashLine);
	
	double _CalculateAreaOf(const QPolygonF& i_polygon)
	{
		Q_ASSERT(i_polygon.size() >= cMinimumPolygonVertexCount);
		
		double area = 0.0;
		for (int i = 0; i < i_polygon.size(); ++i)
		{
			const QPointF& current_point = i_polygon[i];

			bool is_last_point = i == i_polygon.size() - 1;
			const QPointF& next_point = is_last_point ? i_polygon[0] : i_polygon[i + 1];

			area += (next_point.x() * current_point.y() - next_point.y() * current_point.x()) / 2;
		}

		return qAbs(area);
	}
	
	bool _IsConvex(const QPolygonF& i_polygon)
	{
		Q_ASSERT(i_polygon.size() >= cMinimumPolygonVertexCount);
		
		bool is_first_product_positive = true;
		
		for (int i = 0; i < i_polygon.size(); ++i)
		{
			const QPointF& current_point = i_polygon[i];
			
			bool is_first_point = i == 0;
			const QPointF& prev_point = is_first_point ? i_polygon[i_polygon.size() - 1] : i_polygon[i - 1];
			
			bool is_last_point = i == i_polygon.size() - 1;
			const QPointF& next_point = is_last_point ? i_polygon[0] : i_polygon[i + 1];
			
			const QPointF first_vector = QPointF(current_point.x() - prev_point.x(),
												 current_point.y() - prev_point.y());
			
			const QPointF second_vector = QPointF(next_point.x() - current_point.x(),
												  next_point.y() - current_point.y());
			
			bool is_product_positive = (first_vector.x() * second_vector.y() - second_vector.x() * first_vector.y()) > 0;
			if (is_first_point)
				is_first_product_positive = is_product_positive;
			
			if (is_product_positive != is_first_product_positive)
				return false;
		}
		
		return true;
	}
}

Scene::Scene(Delegate* ip_delegate)
: mp_delegate(ip_delegate)
{
}

bool Scene::IsPossibleToComplete()
{
	return m_polygon.size() >= cMinimumPolygonVertexCount;
}

void Scene::Reset()
{
	clear();

	m_polygon.clear();
	m_lines.clear();
	
	mp_next_connection_line = nullptr;
	mp_last_connection_line = nullptr;
	
	if (mp_delegate)
		mp_delegate->OnPointsCountChanged(m_polygon.size());
}

Scene::Result Scene::Complete()
{
	mp_next_connection_line->hide();
	mp_last_connection_line->hide();
	
	addPolygon(m_polygon,
			   cLineSolidBlackPen,
			   cPolygonBrush);

	Result result;
	result.m_square = _CalculateAreaOf(m_polygon);
	result.m_is_convex = _IsConvex(m_polygon);
	
	return result;
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
		if (m_polygon.empty())
			mp_next_connection_line = addLine(QLineF(mouse_position, mouse_position), cLineDashBlackPen);
		else
			mp_last_connection_line = addLine(QLineF(m_polygon.front(), mouse_position), cLineDashGreenPen);
	}
	
	if (!m_polygon.empty())
		m_lines.push_back(addLine(QLineF(m_polygon.back(), mouse_position), cLineSolidBlackPen));
	
	m_polygon.push_back(mouse_position);
	
	if (mp_delegate)
		mp_delegate->OnPointsCountChanged(m_polygon.size());
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *ip_event)
{
	if (mp_delegate)
		mp_delegate->OnMouseMove(ip_event);

	if (!mp_next_connection_line)
		return;
	
	mp_next_connection_line->setLine(QLineF(m_polygon.back(), ip_event->scenePos()));
	
	if (!mp_last_connection_line)
		return;
	
	mp_last_connection_line->setLine(QLineF(m_polygon.front(), ip_event->scenePos()));
	
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

bool Scene::event(QEvent *ip_event)
{
	if (ip_event->type() == QEvent::Leave)
	{
		if (mp_next_connection_line)
			mp_next_connection_line->setLine(QLineF(m_polygon.last(), m_polygon.last()));
		
		if (mp_last_connection_line)
		{
			mp_last_connection_line->setLine(QLineF(m_polygon.first(), m_polygon.last()));
			mp_last_connection_line->setPen(cLineDashGreenPen);
		}
		
		return true;
	}
	
	return QGraphicsScene::event(ip_event);
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
