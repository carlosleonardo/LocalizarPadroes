#-------------------------------------------------
#
# Project created by QtCreator 2012-05-01T19:49:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LocalizarPadroes
TEMPLATE = app
CONFIG += precompile_header c++17

SOURCES += main.cpp\
    controlelocalizarpadroes.cpp \
    telalocalizarpadroes.cpp \
    util.cpp \
    telatestarexpressaoregular.cpp \
    informacoesarquivo.cpp \
    adaptadorinterfacethread.cpp \
    threadboost.cpp \
    executorbusca.cpp

HEADERS  += \
    controlelocalizarpadroes.h \
    stdcpp.h \
    telalocalizarpadroes.h \
    util.h \
    telatestarexpressaoregular.h \
    informacoesarquivo.h \
    adaptadorinterfacethread.h \
    threadboost.h \
    executorbusca.h

FORMS    += \
    telalocalizarpadroes.ui \
    telatestarexpressaoregular.ui

PRECOMPILED_HEADER += stdcpp.h

MOC_DIR = moc
OBJECTS_DIR = objs
UI_DIR = ui
RC_FILE=localizarpadroes.rc

#Suporte multi-plataforma
win32 {
    SUFIXO=windows
}
linux {
    SUFIXO=linux
}
VCPKG_ROOT=$$getenv("VCPKG_ROOT")
INCLUDEPATH += $$VCPKG_ROOT/installed/x64-$$SUFIXO/include
LIBS += -L$$VCPKG_ROOT/installed/x64-$$SUFIXO/lib


OTHER_FILES += \
    ResponsividadeGUI.txt \
    localizarpadroes.rc \
    Localizar.ico
