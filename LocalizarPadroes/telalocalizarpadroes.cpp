#include "executorbusca.h"
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
    // Nos delegates, emitimos um signal Qt
    goLocalizarPadroes.notificadorLocalizado.connect(
                boost::bind(&TelaLocalizarPadroes::delegarPreencherLista, this, _1));
    goLocalizarPadroes.notificadorBusca.connect(
                boost::bind(&TelaLocalizarPadroes::delegarPesquisarLista, this, _1));
    goLocalizarPadroes.notificadorFinalizado.connect(
                boost::bind(&TelaLocalizarPadroes::delegarFinalizado, this));

    // Vinculamos os signals e slots de forma enfileirada.
    qRegisterMetaType<InformacoesArquivo>("InformacoesArquivo");
    QObject::connect(this, SIGNAL(preencheLista(InformacoesArquivo)), SLOT(on_preencherLista(InformacoesArquivo)),
                     Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(pesquisarLista(InformacoesArquivo)), SLOT(on_pesquisarLista(InformacoesArquivo)),
                     Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(finalizadaBusca()), SLOT(on_finalizarBusca()),Qt::QueuedConnection);

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
    cabecalho->setVisible(false);
    ui->lvwLocalizados->setHorizontalHeader(cabecalho);
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

void TelaLocalizarPadroes::executarThreadPesquisa(QString pasta)
{
    ExecutorBusca::Funcao f;

#if _MSC_VER > 1600
    f = std::bind(&ControleLocalizarPadroes::buscarArquivos,
                                        &goLocalizarPadroes, pasta.toStdString());
#else
    f = boost::bind(&ControleLocalizarPadroes::buscarArquivos,
                    &goLocalizarPadroes, pasta.toStdString());
#endif
    threadPesquisa.reset(new ExecutorBusca(f));
    threadPesquisa.get()->iniciar();
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
    ui->lvwLocalizados->horizontalHeader()->setVisible(true);

    m_tempo.start();

    definirPadraoBusca(lsBusca);

    // Executa o thread de pesquisa
    executarThreadPesquisa(lsPasta);
}

// Delegamos, pois a atualização da UI deve ser feita pela thread que a criou.
void TelaLocalizarPadroes::delegarPreencherLista(const InformacoesArquivo& infoArquivo)
{
    emit preencheLista(infoArquivo);
}

bool TelaLocalizarPadroes::delegarPesquisarLista(const InformacoesArquivo &infoArquivo)
{
    emit pesquisarLista(infoArquivo);
    return false;
}

void TelaLocalizarPadroes::delegarFinalizado()
{
    emit finalizadaBusca();
}

void TelaLocalizarPadroes::on_btnCancelar_clicked()
{
    ui->btnProcurar->setEnabled(true);
    btnCancelar->setEnabled(false);

    AdaptadorInterfaceThread *adaptadorInterface = threadPesquisa.get();
    adaptadorInterface->bloquear();
    goLocalizarPadroes.cancelarPesquisa();
    adaptadorInterface->desbloquear();

    adaptadorInterface->interromper();
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

// Preenche de fato a lista
void TelaLocalizarPadroes::on_preencherLista(const InformacoesArquivo &infoArquivo)
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

void TelaLocalizarPadroes::on_pesquisarLista(const InformacoesArquivo &infoArquivo)
{
    QCoreApplication::processEvents();
}

void TelaLocalizarPadroes::on_finalizarBusca()
{
    QApplication::restoreOverrideCursor();

    float tempoGasto = m_tempo.elapsed();
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

void TelaLocalizarPadroes::habilitadoExpressoesRegulares()
{
    bool lbExpressoesRegulares = goLocalizarPadroes.usarExpressoesRegulares();

    ui->chkUsarNomeArquivoComoPadrao->setEnabled(!lbExpressoesRegulares);
    ui->chkConsiderarCaixaLetra->setEnabled(!lbExpressoesRegulares);
    ui->chkPalavraInteira->setEnabled(!lbExpressoesRegulares);
    ui->btnTestarExpressaoRegular->setEnabled(lbExpressoesRegulares);
}
