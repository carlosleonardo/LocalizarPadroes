#ifndef TELAPRINCIPAL_H
#define TELAPRINCIPAL_H

#include "stdcpp.h"
#include "controlelocalizarpadroes.h"

namespace Ui {
class TelaPrincipal;
}

class TelaPrincipal : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TelaPrincipal(QWidget *parent = 0);
    ~TelaPrincipal();

protected:
     bool eventFilter(QObject *obj, QEvent *ev);
    
private slots:
    void on_btnAlterarPasta_clicked();

    void on_btnProcurar_clicked();

private:
    Ui::TelaPrincipal *ui;

    ControleLocalizarPadroes goLocalizarPadroes;
    QStandardItemModel* goModeloDados;
    bool gbCancelar;

    void preencheLista(const std::string &nomeCaminho);
    bool pesquisaLista(const std::string& nomeCaminho, unsigned);
};

#endif // TELAPRINCIPAL_H
