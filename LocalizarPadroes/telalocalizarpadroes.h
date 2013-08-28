#ifndef TELALOCALIZARPADROES_H
#define TELALOCALIZARPADROES_H

#include <QMainWindow>

#include "stdcpp.h"
#include "controlelocalizarpadroes.h"
#include "adaptadorinterfacethread.h"


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

    void executarThread(QString pasta);
private slots:
     void on_btnAlterarPasta_clicked();

     void on_btnProcurar_clicked();

     void on_btnCancelar_clicked();

     void on_chkConsiderarCaixaLetra_toggled(bool checked);

     void on_chkExpressoesRegulares_toggled(bool checked);

     void on_btnTestarExpressaoRegular_clicked();

     void on_preencherLista(const InformacoesArquivo& infoArquivo);

signals:
     void preencheLista(const InformacoesArquivo& infoArquivo);

private:
    Ui::TelaLocalizarPadroes *ui;

    ControleLocalizarPadroes goLocalizarPadroes;
    QStandardItemModel* goModeloDados;
    bool gbCancelar;
    QPushButton* btnCancelar;
    boost::shared_ptr<AdaptadorInterfaceThread> threadPesquisa;

    void delegarPreencherLista(const InformacoesArquivo& infoArquivo);
    bool pesquisaLista(const InformacoesArquivo& infoArquivo);
    void inicializarComponentes();
    void habilitadoExpressoesRegulares();
};

#endif // TELALOCALIZARPADROES_H
