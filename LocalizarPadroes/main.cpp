#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "telalocalizarpadroes.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Informações para armazenar configurações
    a.setOrganizationName("SERPRO");
    a.setOrganizationDomain("serpro.gov.br");
    a.setApplicationName("Localizar Padroes");

    TelaLocalizarPadroes w;
    w.show();
    
    return a.exec();
}
