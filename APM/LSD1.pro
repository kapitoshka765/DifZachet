QT       += core gui
QT += network
QT += xml
QT += webenginewidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
    dragmetal.cpp \
    main.cpp \
    mainwindow.cpp \
    obmakc.cpp \
    sosakc.cpp

HEADERS += \
    dragmetal.h \
    mainwindow.h \
    obmakc.h \
    sosakc.h

FORMS += \
    dragmetal.ui \
    mainwindow.ui \
    obmakc.ui \
    sosakc.ui

TRANSLATIONS += \
    LSD1_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations
DEFINES += QT_WEBENGINE_DISABLE_GPU_RASTERIZATION
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
