SOURCES	+= main.cpp control.cpp display.cpp operators.cpp renderer.cpp renderwindow.cpp
HEADERS	+= control.h display.h operators.h renderer.h renderwindow.h

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

FORMS	= gui.ui renderwindow_base.ui
TEMPLATE	=app
CONFIG	+= qt warn_on release
CONFIG += thread
CONFIG += staticlib
LANGUAGE	= C++
