#-------------------------------------------------
#
# Project created by QtCreator 2015-01-01T18:54:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AngelscriptIDE
TEMPLATE = app

INCLUDEPATH += "Angelscript/include"

SOURCES +=

HEADERS  +=

FORMS    +=

include("main/main.pri")
include("launcher/launcher.pri")

include("util/util.pri")

include("Angelscript/Angelscript.pri")
include("Angelscript/include/Angelscript.include.pri")
include("Angelscript/source/Angelscript.source.pri")
include("Angelscript/add_on/Angelscript.add_on.pri")
include("ide/ide.pri")

include("ui/util/ui.util.pri")
include("ui/ui.pri")
include("ui/options/ui.options.pri")
