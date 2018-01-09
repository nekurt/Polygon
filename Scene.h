#pragma once

#include <QGraphicsScene>

class Scene : public QGraphicsScene
{
public:
	struct Result
	{
		double m_square;
		bool m_isConvex;
	};
	
	class Delegate
	{
	public:
		virtual void onMouseMove(const QGraphicsSceneMouseEvent* ip_event) = 0;
	};
	
	explicit Scene(Delegate* ip_delegate = nullptr);
	
	bool IsPossibleToComplete();
	
	void Reset();
	Result Complete();
	
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* ip_event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* ip_event) override;
	
	bool _LinesHaveCollisions();
	
private:
	Delegate* mp_delegate;
	
	std::vector<QPointF> m_points;
	std::vector<QGraphicsLineItem*> m_lines;
	
	QGraphicsLineItem* mp_next_connection_line = nullptr;
	QGraphicsLineItem* mp_last_connection_line = nullptr;
};
