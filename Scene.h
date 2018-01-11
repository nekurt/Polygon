#pragma once

#include <QGraphicsScene>

class Scene : public QGraphicsScene
{
public:
	struct Result
	{
		double m_square = 0.0;
		bool m_is_convex = false;
	};
	
	class Delegate
	{
	public:
		virtual void OnMouseMove(const QGraphicsSceneMouseEvent* ip_event) = 0;
		virtual void OnPointsCountChanged(int i_count) = 0;
	};
	
	explicit Scene(Delegate* ip_delegate = nullptr);
	
	bool IsPossibleToComplete();
	
	void Reset();
	Result Complete();
	
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* ip_event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* ip_event) override;
	bool event(QEvent* ip_event) override;
	
	bool _LinesHaveCollisions();
	
private:
	Delegate* mp_delegate;
	
	QPolygonF m_polygon;
	std::vector<QGraphicsLineItem*> m_lines;
	
	QGraphicsLineItem* mp_next_connection_line = nullptr;
	QGraphicsLineItem* mp_last_connection_line = nullptr;
};
