TEMPLATE = app
CONFIG += app_bundle
LIBS += gdi32.lib dwmapi.lib

HEADERS += \
    QMainPanel.h \
    mainwindow1.h \
    BorderlessWindow.h \
    QFramelessWindow.h \
    testwindow.h

SOURCES += main.cpp \
    QMainPanel.cpp \
    mainwindow1.cpp \
    BorderlessWindow.cpp \
    QFramelessWindow.cpp \
    testwindow.cpp

include("QtWinMigrate.pri")
include("Common.pri")

RESOURCES += \
    Icons.qrc

OTHER_FILES += \
    BorderlessWindow.css

FORMS += \
    mainwindow1.ui
