#ifndef TELALOCALIZARPADROES_H
#define TELALOCALIZARPADROES_H

#include <QMainWindow>
#include <QTime>

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

    void executarThreadPesquisa(QString pasta);
private slots:
     void on_btnAlterarPasta_clicked();

     void on_btnProcurar_clicked();

     void on_btnCancelar_clicked();

     void on_chkConsiderarCaixaLetra_toggled(bool checked);

     void on_chkExpressoesRegulares_toggled(bool checked);

     void on_btnTestarExpressaoRegular_clicked();

     void on_preencherLista(const InformacoesArquivo& infoArquivo);

     void on_pesquisarLista(const InformacoesArquivo& infoArquivo);

     void on_finalizarBusca();

signals:
     void preencheLista(const InformacoesArquivo& infoArquivo);
     void pesquisarLista(const InformacoesArquivo& infoArquivo);
     void finalizadaBusca();

private:
    Ui::TelaLocalizarPadroes *ui;

    ControleLocalizarPadroes goLocalizarPadroes;
    QStandardItemModel* goModeloDados;
    bool gbCancelar;
    QPushButton* btnCancelar;
    boost::shared_ptr<AdaptadorInterfaceThread> threadPesquisa;
    QTime m_tempo;

    // Eventos usados pela pesquisa de arquivos
    void delegarPreencherLista(const InformacoesArquivo& infoArquivo);
    bool delegarPesquisarLista(const InformacoesArquivo& infoArquivo);
    void delegarFinalizado();

    void inicializarComponentes();
    void habilitadoExpressoesRegulares();
    void habilitarPesquisa(bool habilitar);
};

#endif // TELALOCALIZARPADROES_H
