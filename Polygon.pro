#-------------------------------------------------
#
# Project created by QtCreator 2018-01-06T19:01:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Polygon

unix {
TEMPLATE = app
}

win32 {
TEMPLATE = vcapp
}

SOURCES += main.cpp \
		   MainWindow.cpp \
		   Scene.cpp


HEADERS  += MainWindow.h \
			Scene.h

FORMS    += MainWindow.ui
