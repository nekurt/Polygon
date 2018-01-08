#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, mp_ui(new Ui::MainWindow)
{
	mp_ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete mp_ui;
}
