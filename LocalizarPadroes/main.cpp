#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "telalocalizarpadroes.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TelaLocalizarPadroes w;
    w.show();
    
    return a.exec();
}
