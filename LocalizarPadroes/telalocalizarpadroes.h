#ifndef TELALOCALIZARPADROES_H
#define TELALOCALIZARPADROES_H

#include <QMainWindow>

#include "stdcpp.h"
#include "controlelocalizarpadroes.h"


namespace Ui {
class TelaLocalizarPadroes;
}

class TelaLocalizarPadroes : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TelaLocalizarPadroes(QWidget *parent = 0);
    ~TelaLocalizarPadroes();

    void definirPadraoBusca(QString lsBusca);

private slots:
     void on_btnAlterarPasta_clicked();

     void on_btnProcurar_clicked();

     void on_btnCancelar_clicked();

     void on_chkConsiderarCaixaLetra_toggled(bool checked);

     void on_chkExpressoesRegulares_toggled(bool checked);

     void on_btnTestarExpressaoRegular_clicked();

private:
    Ui::TelaLocalizarPadroes *ui;

    ControleLocalizarPadroes goLocalizarPadroes;
    QStandardItemModel* goModeloDados;
    bool gbCancelar;
    QPushButton* btnCancelar;

    void preencheLista(const InformacoesArquivo& infoArquivo);
    bool pesquisaLista(const InformacoesArquivo& infoArquivo);
    void inicializarComponentes();
    void habilitadoExpressoesRegulares();
};

#endif // TELALOCALIZARPADROES_H
