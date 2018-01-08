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
	
private:
	Delegate* mp_delegate;
	
	std::vector<QPoint> m_points;	
};
