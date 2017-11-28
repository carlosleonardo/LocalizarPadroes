#include "controlelocalizarpadroes.h"
#include "util.h"

#include <boost/array.hpp>
#include <boost/filesystem.hpp>
#include <regex>
#include <thread>
#include <fstream>
#include <iterator>
#include <locale>
#include <stack>
#include <iostream>

using namespace boost;

using namespace std;

#if BOOST_VERSION < 105400
#if BOOST_FILESYSTEM_VERSION == 3
using namespace boost::filesystem3;
#elif BOOST_FILESYSTEM_VERSION == 2
using namespace boost::filesystem;
#endif
#else
using namespace boost::filesystem;
#endif

ControleLocalizarPadroes::ControleLocalizarPadroes()
{
    gbUsarExpressoesRegulares = false;

    gbDistinguirMaiusculas = false;
    gbPalavraInteira = false;
    gbUsarNomeArquivoParaBusca = false;

    gbPesquisando = true;
    gbTerminado = false;
}

// Realiza busca pelo padrão a partir da pasta informada.
// Este será executado como um thread, usando o algoritmo de busca em profundidade
bool ControleLocalizarPadroes::buscarArquivos(const std::string& nomePastaInicial)
{
    std::string nomeCaminho;
    system::error_code erro;

    if (nomePastaInicial=="" || gsPadraoPesquisa == "") {
        return false;
    }

    try {

        std::stack<std::string> pastasVisitadas;

        pastasVisitadas.push(nomePastaInicial);

        while (!pastasVisitadas.empty())
        {
            nomeCaminho.assign(pastasVisitadas.top());
            pastasVisitadas.pop();
            path loCaminho(nomeCaminho);

            // Recupera os subdiretórios e arquivos do diretório informado e faz uma busca em cada um deles
            directory_iterator loIterador(loCaminho);

            while (loIterador != directory_iterator()) {
                nomeCaminho = absolute(loIterador->path()).make_preferred().generic_string();

                if (!estaPesquisando()) {
                    break;
                }

                if (is_regular_file(loIterador->path(), erro )) {

                    if (erro.value()==EACCES) {
                        std::cerr << erro.message() << std::endl;
                    }

                    // Notifica que arquivo está sendo pesquisado, e se localizado, notifica isto também
                    InformacoesArquivo loInfoArquivo;

                    loInfoArquivo.gsNomeArquivo = nomeCaminho;
                    if (!notificadorBusca.empty()) {
                        notificadorBusca(loInfoArquivo);
                    }

                    if (existePadrao(nomeCaminho, loInfoArquivo)) {
                        if (!notificadorLocalizado.empty()) {
                            notificadorLocalizado(loInfoArquivo);
                        }
                    }
                }
                else if (is_directory(loIterador->path(), erro)) {
                    if (erro.value()==EACCES) {
                        std::cerr << erro.message() << std::endl;
                    }
                    pastasVisitadas.push(nomeCaminho);

                }
                loIterador++;
            }
        }
    }
    catch(filesystem_error& fe) {
        std::cerr << fe.what() << std::endl;
    }

    if (!notificadorFinalizado.empty()) {
        notificadorFinalizado();
    }
    return true;
}

std::string ControleLocalizarPadroes::padraoPesquisa() const
{
    return gsPadraoPesquisa;
}

void ControleLocalizarPadroes::setPadraoPesquisa(const std::string &psPadraoPesquisa)
{
    gsPadraoPesquisa = psPadraoPesquisa;
}

bool ControleLocalizarPadroes::estaPesquisando()
{
    return gbPesquisando;
}

void ControleLocalizarPadroes::cancelarPesquisa()
{
    gbPesquisando = false;
    if (!notificadorFinalizado.empty()) {
        notificadorFinalizado();
    }
}

void ControleLocalizarPadroes::inicializarPesquisa()
{
    gbPesquisando = true;
}

// Localiza o padrão dentro do arquivo.
bool ControleLocalizarPadroes::existePadrao(const std::string& psCaminho, InformacoesArquivo &poInfoArquivo)
{
    std::fstream loArquivo;
    boost::array<char, BUFSIZE> buffer;
    std::ostringstream loStreamString;
    int lnTotal=0;

    // O arquivo é lido como binário, usando um buffer
    // Esse buffer é adicionado numa stream de string para fazer a pesquisa pelo conteúdo do arquivo ou pelo nome, na codificação
    // UTF-8.
    std::locale loc("");
    loArquivo.imbue(loc);

    loArquivo.open(psCaminho.c_str());

    while (loArquivo) {
        loArquivo.read(buffer.c_array(), BUFSIZE);
        loStreamString << buffer.c_array();
    }
    std::string lsConteudo = loStreamString.str();
    std::string lsPadraoPesquisa;


    if (!gbDistinguirMaiusculas) {
        lsConteudo = toUpper(lsConteudo);
        lsPadraoPesquisa = toUpper(gsPadraoPesquisa);
    } else {
        lsPadraoPesquisa = gsPadraoPesquisa;
    }

    size_t lnPosicao;
    regex loExpr;

    //if (!gbUsarExpressoesRegulares) {
        lnPosicao = lsConteudo.find(lsPadraoPesquisa);
    //} else {

    //}

    while (lnPosicao != std::string::npos) {
        lnTotal ++;
        poInfoArquivo.goPosicoesOcorrencia.push_back(lnPosicao);
        lnPosicao = lsConteudo.find(lsPadraoPesquisa, lnPosicao+1);
    }

    if (lnTotal==0) {
        return false;
    }

    poInfoArquivo.gnNumeroOcorrenciasPalavra = lnTotal;
    return true;
}

long ControleLocalizarPadroes::tamanhoArquivo(const std::string& psCaminho)
{
    return file_size(path(psCaminho));
}

void ControleLocalizarPadroes::validarOpcoesBusca()
{
    // Se expressões regulares está marcada, desabilita as outras
    if (gbUsarExpressoesRegulares) {
        gbPalavraInteira = false;
        gbUsarNomeArquivoParaBusca = false;
        gbDistinguirMaiusculas = false;
    } else {
        gbPalavraInteira = true;
        gbUsarNomeArquivoParaBusca = true;
        gbDistinguirMaiusculas = true;
    }
}

