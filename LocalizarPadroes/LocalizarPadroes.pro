#-------------------------------------------------
#
# Project created by QtCreator 2012-05-01T19:49:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LocalizarPadroes
TEMPLATE = app
CONFIG += precompile_header

SOURCES += main.cpp\
    controlelocalizarpadroes.cpp \
    processobuscaarquivos.cpp \
    telalocalizarpadroes.cpp \
    util.cpp \
    telatestarexpressaoregular.cpp \
    informacoesarquivo.cpp

HEADERS  += \
    controlelocalizarpadroes.h \
    stdcpp.h \
    processobuscaarquivos.h \
    telalocalizarpadroes.h \
    util.h \
    telatestarexpressaoregular.h \
    informacoesarquivo.h

FORMS    += \
    telalocalizarpadroes.ui \
    telatestarexpressaoregular.ui

PRECOMPILED_HEADER += stdcpp.h

MOC_DIR = moc
OBJECTS_DIR = objs
UI_DIR = ui

#Suporte multi-plataforma
#Teste de alteração

#Versões de boost sob a qual compilar
BOOST_VERSION_UNIX=1.46
BOOST_VERSION_WIN=1_54_0
BOOST_LIB_VERSION_WIN=1_54

unix {
    BOOST_INSTALL_DIR=../../../instlocal
    INCLUDEPATH += $$BOOST_INSTALL_DIR/boost-$${BOOST_VERSION_UNIX}/include
    LIBS += -L$$BOOST_INSTALL_DIR/boost-$${BOOST_VERSION_UNIX}/lib -lboost_filesystem -lboost_regex -lboost_system
    LIBS += -lboost_thread
}

# Força o MSBuild a tratar wchar_t como tipo interno
# para não dar erros de linkedição em boost::filesystem3
win32 {
    BOOST_INSTALL_DIR=C:\Boost
    win32-msvc2010:QMAKE_CXXFLAGS += /Zc:wchar_t -D_SCL_SECURE_NO_WARNINGS
    INCLUDEPATH += $$BOOST_INSTALL_DIR/boost_$${BOOST_VERSION_WIN}
    LIBS += -L$$BOOST_INSTALL_DIR/boost_$${BOOST_VERSION_WIN}/lib
    win32-msvc2010:LIBS += -lboost_system-vc100-mt-$$BOOST_LIB_VERSION_WIN -lboost_filesystem-vc100-mt-$$BOOST_LIB_VERSION_WIN -lboost_regex-vc100-mt-$$BOOST_LIB_VERSION_WIN -lboost_thread-vc100-mt-$$BOOST_LIB_VERSION_WIN
    win32-msvc2012:LIBS += -llibboost_system-vc110-mt-$$BOOST_LIB_VERSION_WIN -llibboost_filesystem-vc110-mt-$$BOOST_LIB_VERSION_WIN -llibboost_regex-vc110-mt-$$BOOST_LIB_VERSION_WIN -llibboost_thread-vc110-mt-$$BOOST_LIB_VERSION_WIN
    win32-g++: LIBS += -llibboost_system-mgw47-mt-$$BOOST_LIB_VERSION_WIN -llibboost_filesystem-mgw47-mt-$$BOOST_LIB_VERSION_WIN -llibboost_regex-mgw47-mt-$$BOOST_LIB_VERSION_WIN -llibboost_thread-mgw47-mt-$$BOOST_LIB_VERSION_WIN
}

OTHER_FILES += \
    ResponsividadeGUI.txt
