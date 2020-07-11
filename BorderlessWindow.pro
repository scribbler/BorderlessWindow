TEMPLATE = app
CONFIG += app_bundle
LIBS += gdi32.lib dwmapi.lib

HEADERS += \
    QMainPanel.h \
    BorderlessWindow.h \
    testwindow.h

SOURCES += main.cpp \
    QMainPanel.cpp \
    BorderlessWindow.cpp \
    testwindow.cpp

include("QtWinMigrate.pri")
include("Common.pri")

RESOURCES += \
    Icons.qrc

OTHER_FILES += \
    BorderlessWindow.css

FORMS +=
