#-------------------------------------------------
#
# Project created by QtCreator 2012-05-01T19:49:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LocalizarPadroes
TEMPLATE = app
CONFIG += precompile_header c++14

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

win32:g++:GCC_VERSION=48
BOOST_VERSION=1_65_1
BOOST_ROOT=C:\local
CONFIG += boost

unix {
    LIBS += -lboost_filesystem -lboost_regex -lboost_system -lboost_thread
}

# Força o MSBuild a tratar wchar_t como tipo interno
# para não dar erros de linkedição em boost::filesystem3
win32 {
    lessThan(QT_MAJOR_VERSION, 5) {
       lessThan(QT_MINOR_VERSION, 1) {
          MSVC_FLAG_QT = /Zc:wchar_t
       }
    }
    win32-msvc2010:QMAKE_CXXFLAGS += $$MSVC_FLAG_QT -D_SCL_SECURE_NO_WARNINGS
    win32-msvc2012:QMAKE_CXXFLAGS += -D_SCL_SECURE_NO_WARNINGS
    INCLUDEPATH += $$BOOST_ROOT/boost_$$BOOST_VERSION # Necessário para o QtCreator achar os includes

#Exceto o gcc, os demais compiladores windows usam auto link. Não é preciso especificar as bibliotecas
    g++:LIBS += \
    -llibboost_system-mgw$$GCC_VERSION-mt-$$BOOST_LIB_VERSION_WIN \
    -llibboost_filesystem-mgw$$GCC_VERSION-mt-$$BOOST_LIB_VERSION_WIN \
    -llibboost_regex-mgw$$GCC_VERSION-mt-$$BOOST_LIB_VERSION_WIN \
    -llibboost_thread-mgw$$GCC_VERSION-mt-$$BOOST_LIB_VERSION_WIN
}

OTHER_FILES += \
    ResponsividadeGUI.txt \
    localizarpadroes.rc \
    Localizar.ico

RESOURCES +=

#win32: LIBS += -L$$PWD"/../../../Visual Studio 2012/Projects/COMTest/COMTest/Debug/" -lMenuContextoSistema

#win32: INCLUDEPATH += $$PWD"/../../../Visual Studio 2012/Projects/COMTest/COMTest/MenuContextoSistema"
#win32: DEPENDPATH += $$PWD"/../../../Visual Studio 2012/Projects/COMTest/COMTest/MenuContextoSistema"
