#include "MainWindow.h"

#include "ui_MainWindow.h"

#include <QGraphicsView>
#include <QGraphicsSceneEvent>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, mp_ui(new Ui::MainWindow)
{
	mp_ui->setupUi(this);
	
	mp_scene = std::make_shared<Scene>(this);

	mp_ui->mp_graphics_view->setScene(mp_scene.get());

	connect(mp_ui->mp_button_reset, &QPushButton::pressed, mp_scene.get(), &Scene::Reset);
}

MainWindow::~MainWindow()
{
	delete mp_ui;
}

void MainWindow::onMouseMove(const QGraphicsSceneMouseEvent* ip_event)
{
	QPointF mouse_position_on_scene = ip_event->scenePos();
	
	QString message = QString("%1; %2").arg(mouse_position_on_scene.x()).arg(mouse_position_on_scene.y());
	
	mp_ui->mp_status_bar->showMessage(message);
}
