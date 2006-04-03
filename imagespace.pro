SOURCES += main.cpp	imagewindow.cpp	gui.cpp	operators.cpp	renderer.cpp
HEADERS += 		imagewindow.h	gui.h	operators.h	renderer.h
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

FORMS	 = gui.ui imagewindow.ui
TEMPLATE =app
CONFIG	+= qt warn_on release static
RESOURCES +=resources.qrc
DEFINES += 
LANGUAGE = C++
