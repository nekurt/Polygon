#include "MainWindow.h"

#include "ui_MainWindow.h"

#include <QGraphicsView>
#include <QGraphicsSceneEvent>

namespace
{
	const QString kStatusBarMessageCoords 			= QString("%1; %2");
	const QString kStatusBarMessagePolygonResult 	= QString("Built a polygon. It %1 convex, it's area = %2");
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, mp_ui(new Ui::MainWindow)
{
	mp_ui->setupUi(this);
	
	mp_scene = std::make_shared<Scene>(this);

	mp_ui->mp_graphics_view->setScene(mp_scene.get());

	connect(mp_ui->mp_button_reset, &QPushButton::pressed, [this]() {
		
		mp_scene->Reset();
		
		mp_ui->mp_graphics_view->setInteractive(true);
	});
	
	connect(mp_ui->mp_button_complete, &QPushButton::pressed, [this](){
		
		mp_ui->mp_button_complete->setEnabled(false);
		mp_ui->mp_graphics_view->setInteractive(false);
		
		Scene::Result result = mp_scene->Complete();
		
		QString final_message = kStatusBarMessagePolygonResult.arg(result.m_is_convex ? "is" : "is not").arg(result.m_square);
		mp_ui->mp_status_bar->showMessage(final_message);
	});
}

MainWindow::~MainWindow()
{
	delete mp_ui;
}

void MainWindow::OnMouseMove(const QGraphicsSceneMouseEvent* ip_event)
{
	QPointF mouse_position_on_scene = ip_event->scenePos();
	
	QString message = kStatusBarMessageCoords.arg(mouse_position_on_scene.x()).arg(mouse_position_on_scene.y());
	
	mp_ui->mp_status_bar->showMessage(message);
}

void MainWindow::OnPointsCountChanged(int)
{
	mp_ui->mp_button_complete->setEnabled(mp_scene->IsPossibleToComplete());
}

