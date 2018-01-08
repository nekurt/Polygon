#pragma once

#include "Scene.h"

#include <QMainWindow>

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow, public Scene::Delegate
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void onMouseMove(const QGraphicsSceneMouseEvent* ip_event) override;
		
private:
	Ui::MainWindow* mp_ui;
	
	std::shared_ptr<Scene> mp_scene;
};
