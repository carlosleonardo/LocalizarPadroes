#include "executorbusca.h"
#include "telalocalizarpadroes.h"
#include "ui_telalocalizarpadroes.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTime>
#include <QDebug>
#include <thread>
#include <boost/bind.hpp>

#include "telatestarexpressaoregular.h"

#ifdef WIN32
#ifdef WIN32_USE_SHELL
#include <ShellContextMenuWin.h>
#include <qevent.h>
#endif
#endif

TelaLocalizarPadroes::TelaLocalizarPadroes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TelaLocalizarPadroes)
{
    ui->setupUi(this);

    carregarConfiguracoes();

    // Usamos o modelo padrão que recebe strings, para usar na listview
    goModeloDados = new QStandardItemModel();
    ui->lvwLocalizados->setModel(goModeloDados);

    gbCancelar = false;

    // Conectamos o slot ao sinal boost da nossa classe controle de MVC
    // Como o sinal tem um parâmetro, precisamos indicá-lo através do argumento boost _1
    // Nos delegates, emitimos um signal Qt
    goLocalizarPadroes.notificadorLocalizado.connect(
                boost::bind(&TelaLocalizarPadroes::delegarPreencherLista, this, _1));
    goLocalizarPadroes.notificadorBusca.connect(
                boost::bind(&TelaLocalizarPadroes::delegarPesquisarLista, this,_1));
    goLocalizarPadroes.notificadorFinalizado.connect(
                boost::bind(&TelaLocalizarPadroes::delegarFinalizado, this));

    // Vinculamos os signals personalizados e slots de forma enfileirada.
    qRegisterMetaType<InformacoesArquivo>("InformacoesArquivo");
    QObject::connect(this, SIGNAL(preencheLista(InformacoesArquivo)), SLOT(on_preencherLista(InformacoesArquivo)),
                     Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(pesquisarLista(InformacoesArquivo)), SLOT(on_pesquisarLista(InformacoesArquivo)),
                     Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(finalizadaBusca()), SLOT(on_finalizarBusca()),Qt::QueuedConnection);

    // Instancia a fabrica de menu de contexto
#ifdef WIN32
#ifdef WIN32_USE_SHELL
    inicializaFabrica();
#endif
#endif

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
    salvarConfiguracoes();
    delete ui;
    delete goModeloDados;
    //delete fabrica;

    // Se o thread de pesquisa está ativo, aguarda seu término
    if (threadPesquisa != nullptr) {
        threadPesquisa.get()->juntar();
    }
}

void TelaLocalizarPadroes::carregarConfiguracoes()
{
    QSettings config;

    config.beginGroup("JanelaPrincipal");
    resize(config.value("Tamanho", QSize(350,500)).toSize());
    move(config.value("Posicao", QPoint(100,100)).toPoint());
    ui->txtPastaInicial->setText(config.value("Pasta Inicial").toString());
    ui->txtTextoBusca->setText(config.value("Padrão de busca").toString());
    config.endGroup();
}

void TelaLocalizarPadroes::salvarConfiguracoes()
{
    QSettings config;

    config.beginGroup("JanelaPrincipal");
    config.setValue("Tamanho", size());
    config.setValue("Posicao", pos());
    config.setValue("Pasta Inicial", ui->txtPastaInicial->text());
    config.setValue("Padrão de busca", ui->txtTextoBusca->text());
    config.endGroup();
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


    f = std::bind(&ControleLocalizarPadroes::buscarArquivos,
                                        &goLocalizarPadroes, pasta.toStdString());
    // Verificamos se o thread ainda está rodando. Se for o caso, espera sua conclusão
    if (threadPesquisa != nullptr)
        threadPesquisa.get()->juntar();
    threadPesquisa.reset(new ExecutorBusca(f));
    threadPesquisa.get()->iniciar();

}

#ifdef WIN32
#ifdef WIN32_USE_SHELL

#endif
#endif

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
    habilitarPesquisa(false);

    // Muda o cursor da aplicação enquanto faz a pesquisa

    QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));

    statusBar()->showMessage("Procurando...");

    QStringList loListaCabecalho;
    //loListaCabecalho << "Nome" << trUtf8("Ocorrências") << "Local";
    goModeloDados->setHorizontalHeaderLabels(loListaCabecalho);
    ui->lvwLocalizados->horizontalHeader()->setVisible(true);

    //m_tempo.start();

    definirPadraoBusca(lsBusca);
    goLocalizarPadroes.inicializarPesquisa();

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
    habilitarPesquisa(true);

    AdaptadorInterfaceThread *adaptadorInterface = threadPesquisa.get();
    adaptadorInterface->bloquear();
    goLocalizarPadroes.cancelarPesquisa();
    adaptadorInterface->desbloquear();

    adaptadorInterface->interromper();

}

void TelaLocalizarPadroes::habilitarPesquisa(bool habilitar)
{
    ui->grpProcura->setEnabled(habilitar);
    ui->grpOpcoes->setEnabled(habilitar);
    btnCancelar->setEnabled(!habilitar);
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
    //QCoreApplication::processEvents();
}

void TelaLocalizarPadroes::on_finalizarBusca()
{
    QApplication::restoreOverrideCursor();

    float tempoGasto = m_tempo.elapsed();
    QString lsTempo;

    //qDebug("%f", tempoGasto);
    if (tempoGasto < 1000) {
        lsTempo = " millisegundos";
    } else if (tempoGasto >= 1000 && tempoGasto <= 1000*60) {
        lsTempo = "segundo(s)";
        tempoGasto /= 1000;
    } else if (tempoGasto > 1000*60 && tempoGasto <= 1000*60*60) {
        lsTempo = "minuto(s)";
        tempoGasto /= 1000*60;
    }

    statusBar()->showMessage(QString(tr("Localizado(s) %1 arquivo(s) com o padrão em %2") + lsTempo).
                             arg(goModeloDados->rowCount()).arg(tempoGasto, 3, 'f', 2));
    habilitarPesquisa(true);
}

void TelaLocalizarPadroes::habilitadoExpressoesRegulares()
{
    bool lbExpressoesRegulares = goLocalizarPadroes.usarExpressoesRegulares();

    ui->chkUsarNomeArquivoComoPadrao->setEnabled(!lbExpressoesRegulares);
    ui->chkConsiderarCaixaLetra->setEnabled(!lbExpressoesRegulares);
    ui->chkPalavraInteira->setEnabled(!lbExpressoesRegulares);
    ui->btnTestarExpressaoRegular->setEnabled(lbExpressoesRegulares);
}

#ifdef WIN32
#ifdef WIN32_USE_SHELL
void TelaLocalizarPadroes::contextMenuEvent(QContextMenuEvent *event)
{
    IMenuContextoSistema *localCriarMenuContexto = fabrica->criarMenuContexto(menuContexto.name());
    Q_ASSERT(localCriarMenuContexto != nullptr);

    QString caminhoSelecionado = obterCaminhoItemSelecionado(event->pos());

    Window w;
    w.handle = (HWND)effectiveWinId();

    localCriarMenuContexto->showContextMenuFor(caminhoSelecionado.toStdWString(), event->globalX(), event->globalY(), w );
    delete localCriarMenuContexto;
}
#endif
#endif

/**
 * @brief Recupera o caminho de arquivo selecionado
 * @return
 */
QString TelaLocalizarPadroes::obterCaminhoItemSelecionado(const QPoint& pos)
{
    QModelIndex indItem = ui->lvwLocalizados->indexAt(pos);
    qDebug()<< indItem.data().toString() << "(" << indItem.row() << ", " << indItem.column() << ")";
    return indItem.data().toString();
}

void TelaLocalizarPadroes::on_chkPalavraInteira_toggled(bool checked)
{
    goLocalizarPadroes.setPalavraInteira(checked);
}

void TelaLocalizarPadroes::on_chkUsarNomeArquivoComoPadrao_toggled(bool checked)
{
    goLocalizarPadroes.setNomeArquivoParaBusca(checked);
}
