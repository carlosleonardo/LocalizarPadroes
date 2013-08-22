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

#Suporte multi-plataforma
#Teste de alteração

#Versões de boost sob a qual compilar
BOOST_VERSION_UNIX=1.46.1
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
    win32-msvc2012:QMAKE_CXXFLAGS += -D_SCL_SECURE_NO_WARNINGS

    INCLUDEPATH += $$BOOST_INSTALL_DIR/boost_$${BOOST_VERSION_WIN}
    LIBS += -L$$BOOST_INSTALL_DIR/boost_$${BOOST_VERSION_WIN}/lib

#Exceto o gcc, os demais compiladores windows usam auto link. Não é preciso especificar as bibliotecas
win32-g++:LIBS += \
    -llibboost_system-mgw47-mt-$$BOOST_LIB_VERSION_WIN \
    -llibboost_filesystem-mgw47-mt-$$BOOST_LIB_VERSION_WIN \
    -llibboost_regex-mgw47-mt-$$BOOST_LIB_VERSION_WIN \
    -llibboost_thread-mgw47-mt-$$BOOST_LIB_VERSION_WIN
}

OTHER_FILES += \
    ResponsividadeGUI.txt
