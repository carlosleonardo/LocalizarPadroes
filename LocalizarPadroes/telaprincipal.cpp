#include "telaprincipal.h"
#include "ui_telaprincipal.h"

#include <QFileDialog>
#include <QMessageBox>
#include <boost/signals2/connection.hpp>
#include <boost/bind.hpp>
#include <QTime>
#include <QtCore>

TelaPrincipal::TelaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TelaPrincipal)
{
    ui->setupUi(this);

    // Usamos o modelo padrão que recebe strings
    goModeloDados = new QStandardItemModel();
    ui->lvwLocalizados->setModel(goModeloDados);

    gbCancelar = false;

    // Conectamos o slot ao sinal da nossa classe controle de MVC
    // Como o sinal tem um parâmetro, precisamos indicá-lo através do argumento boost _1
    goLocalizarPadroes.notificadorLocalizado.connect(boost::bind(&TelaPrincipal::preencheLista, this, _1));
    goLocalizarPadroes.notificadorBusca.connect(boost::bind(&TelaPrincipal::pesquisaLista, this, _1, _2));
}

TelaPrincipal::~TelaPrincipal()
{
    delete ui;
    delete goModeloDados;
}

bool TelaPrincipal::eventFilter(QObject *obj, QEvent *ev)
{
    /*if (obj == ui->btnCancelar) {
        return false;
    }*/
    return QMainWindow::eventFilter(obj, ev);
}

void TelaPrincipal::on_btnAlterarPasta_clicked()
{
    QString nomePasta = QFileDialog::getExistingDirectory(this, "Alterar pasta");

    if (!nomePasta.isEmpty()) {
        ui->txtPastaInicial->setText(nomePasta);
    }
}

void TelaPrincipal::on_btnProcurar_clicked()
{
    QString lsPasta = ui->txtPastaInicial->text();
    QString lsBusca = ui->txtTextoBusca->text();

    if (lsPasta.isEmpty()) {
        QMessageBox::information(this, this->windowTitle(), tr("Nenhuma pasta inicial informada."));
        return;
    }

    if (lsBusca.isEmpty()) {
        QMessageBox::information(this, this->windowTitle(), tr("Informe um texto de busca."));
        return;
    }

    QTime tempo;
    tempo.start();


    goModeloDados->clear();
    ui->btnProcurar->setEnabled(false);
    //ui->btnCancelar->setEnabled(true);

#ifdef __unix__
    goLocalizarPadroes.setPadraoPesquisa(lsBusca.toUtf8().constData());
#elif defined(WIN32)
    goLocalizarPadroes.setPadraoPesquisa(lsBusca.toLocal8Bit().constData());
#endif


    // Muda o cursor da aplicação enquanto faz a pesquisa

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

//    ui->lblStatus->setText("Procurando...");

    if (!goLocalizarPadroes.buscarArquivos(lsPasta.toUtf8().constData())) {
        QMessageBox::information(this, this->windowTitle(), "Nada encontrado");
        return;
    }
    QApplication::restoreOverrideCursor();

    float tempoGasto = tempo.elapsed();
    QString lsTempo;

    qDebug("%f", tempoGasto);
    if (tempoGasto < 1000) {
        lsTempo = " millisegundos";
    } else if (tempoGasto >= 1000 && tempoGasto <= 1000*60) {
        lsTempo = "segundo(s)";
        tempoGasto /= 1000;
    } else if (tempoGasto > 1000*60 && tempoGasto <= 1000*60*60) {
        lsTempo = "minuto(s)";
        tempoGasto /= 1000*60;
    }

//    ui->lblStatus->setText(QString("Localizado(s) %1 arquivos em %2" + lsTempo).
//                           arg(goModeloDados->rowCount()).arg(tempoGasto, 3, 'f', 2));
    //ui->btnCancelar->setEnabled(false);
    ui->btnProcurar->setEnabled(true);
}

void TelaPrincipal::preencheLista(const std::string &nomeCaminho)
{
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    goModeloDados->appendRow(new QStandardItem(QString(nomeCaminho.c_str())));
    //qDebug("Caminho: %s",nomeCaminho.c_str());
}

bool TelaPrincipal::pesquisaLista(const std::string &nomeCaminho, unsigned)
{
     return (!gbCancelar);
}
