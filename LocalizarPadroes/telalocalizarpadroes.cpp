#include "telalocalizarpadroes.h"
#include "ui_telalocalizarpadroes.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <boost/thread/thread.hpp>
#include "telatestarexpressaoregular.h"

TelaLocalizarPadroes::TelaLocalizarPadroes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TelaLocalizarPadroes)
{
    ui->setupUi(this);

    // Usamos o modelo padrão que recebe strings
    goModeloDados = new QStandardItemModel();
    ui->lvwLocalizados->setModel(goModeloDados);

    gbCancelar = false;

    // Conectamos o slot ao sinal da nossa classe controle de MVC
    // Como o sinal tem um parâmetro, precisamos indicá-lo através do argumento boost _1
    goLocalizarPadroes.notificadorLocalizado.connect(
                boost::bind(&TelaLocalizarPadroes::preencheLista, this, _1));
    goLocalizarPadroes.notificadorBusca.connect(
                boost::bind(&TelaLocalizarPadroes::pesquisaLista, this, _1));

    inicializarComponentes();
}

void TelaLocalizarPadroes::inicializarComponentes()
{
    // Cria o botão cancelar
    btnCancelar = new QPushButton("Cancelar", this);
    btnCancelar->setEnabled(false);

    QObject::connect(btnCancelar, SIGNAL(clicked()),SLOT(on_btnCancelar_clicked()), Qt::QueuedConnection);

    statusBar()->addPermanentWidget(btnCancelar);

    ui->btnTestarExpressaoRegular->setEnabled(goLocalizarPadroes.usarExpressoesRegulares());

    // Cria o cabeçalho
    QHeaderView* cabecalho = new QHeaderView(Qt::Horizontal, ui->lvwLocalizados);
    cabecalho->setModel(goModeloDados);
    cabecalho->setVisible(true);

    move(100,100);
}

TelaLocalizarPadroes::~TelaLocalizarPadroes()
{
    delete ui;
    delete goModeloDados;
}

void TelaLocalizarPadroes::on_btnAlterarPasta_clicked()
{
    QString nomePasta = QFileDialog::getExistingDirectory(this, "Alterar pasta");

    if (!nomePasta.isEmpty()) {
        ui->txtPastaInicial->setText(nomePasta);
    }
}

void TelaLocalizarPadroes::definirPadraoBusca(QString lsBusca)
{
#ifdef __unix__
    goLocalizarPadroes.setPadraoPesquisa(lsBusca.toUtf8().constData());
#elif defined(WIN32)
    goLocalizarPadroes.setPadraoPesquisa(lsBusca.toLocal8Bit().constData());
#endif
}

void TelaLocalizarPadroes::on_btnProcurar_clicked()
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

    QDir dir(lsPasta);

    if (!dir.exists())
    {
        QMessageBox::information(this, this->windowTitle(), tr("A pasta informada não existe"));
        return;
    }

    goModeloDados->clear();
    ui->btnProcurar->setEnabled(false);
    btnCancelar->setEnabled(true);

    // Muda o cursor da aplicação enquanto faz a pesquisa

    QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));

    statusBar()->showMessage("Procurando...");

    QStringList loListaCabecalho;
    loListaCabecalho << "Nome" << trUtf8("Ocorrências") << "Local";
    goModeloDados->setHorizontalHeaderLabels(loListaCabecalho);

    QTime tempo;
    tempo.start();

    definirPadraoBusca(lsBusca);

    // Executa o thread de pesquisa
    boost::thread meuThread(boost::bind(&ControleLocalizarPadroes::buscarArquivos,
                             &goLocalizarPadroes, _1), lsPasta.toUtf8().constData());
    //goLocalizarPadroes.buscarArquivos(lsPasta.toUtf8().constData());
    meuThread.join();

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

    statusBar()->showMessage(QString(trUtf8("Localizado(s) %1 arquivos com o padrão em %2") + lsTempo).
                             arg(goModeloDados->rowCount()).arg(tempoGasto, 3, 'f', 2));
    btnCancelar->setEnabled(false);
    ui->btnProcurar->setEnabled(true);

}

void TelaLocalizarPadroes::preencheLista(const InformacoesArquivo& infoArquivo)
{
    QFileInfo nomeArquivo(infoArquivo.nomeArquivo().c_str());

    // Criamos cada coluna de informação do arquivo e inserimos de uma vez só
    QStandardItem* itemArquivo = new QStandardItem;
    QStandardItem* itemOcorrencias = new QStandardItem;
    QStandardItem* itemLocalizacao = new QStandardItem;

    itemArquivo->setText(nomeArquivo.fileName());

    QString str = QString("%1").arg(infoArquivo.numeroOcorrencias());
    itemOcorrencias->setText(str);

    itemLocalizacao->setText(nomeArquivo.absolutePath());

    // Não deixamos editar
    itemArquivo->setEditable(false);
    itemOcorrencias->setEditable(false);
    itemLocalizacao->setEditable(false);

    // Agora inserimos
    QList<QStandardItem*> loListaInfoArquivo;

    loListaInfoArquivo << itemArquivo << itemOcorrencias << itemLocalizacao;
    goModeloDados->appendRow(loListaInfoArquivo);
}

bool TelaLocalizarPadroes::pesquisaLista(const InformacoesArquivo &infoArquivo)
{
    QCoreApplication::processEvents();
    return gbCancelar;
}

void TelaLocalizarPadroes::on_btnCancelar_clicked()
{
    ui->btnProcurar->setEnabled(true);
    btnCancelar->setEnabled(false);
    gbCancelar = true;

}

void TelaLocalizarPadroes::on_chkConsiderarCaixaLetra_toggled(bool checked)
{
    goLocalizarPadroes.setMaiusculas(checked);
}

void TelaLocalizarPadroes::on_chkExpressoesRegulares_toggled(bool checked)
{
    goLocalizarPadroes.setUsarExpressoesRegulares(checked);
    habilitadoExpressoesRegulares();
}

void TelaLocalizarPadroes::on_btnTestarExpressaoRegular_clicked()
{
    TelaTestarExpressaoRegular dlgTestarExpressao(this);

    dlgTestarExpressao.exec();
}

void TelaLocalizarPadroes::habilitadoExpressoesRegulares()
{
    bool lbExpressoesRegulares = goLocalizarPadroes.usarExpressoesRegulares();

    ui->chkUsarNomeArquivoComoPadrao->setEnabled(!lbExpressoesRegulares);
    ui->chkConsiderarCaixaLetra->setEnabled(!lbExpressoesRegulares);
    ui->chkPalavraInteira->setEnabled(!lbExpressoesRegulares);
    ui->btnTestarExpressaoRegular->setEnabled(lbExpressoesRegulares);
}
