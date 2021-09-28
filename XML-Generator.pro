#-------------------------------------------------
#
# Project created by QtCreator 2019-07-07T15:09:04
#
#-------------------------------------------------

QT       += core gui widgets

TARGET   = XML-Generator
TEMPLATE = app

CONFIG   += c++19

SOURCES += \
	   main.cpp \
	   mainwindow.cpp \
	   generatorworker.cpp

HEADERS += \
	   mainwindow.hpp \
	   generatorworker.hpp

FORMS   += \
	   mainwindow.ui

TRANSLATIONS += xmlgenerator_pl.ts
