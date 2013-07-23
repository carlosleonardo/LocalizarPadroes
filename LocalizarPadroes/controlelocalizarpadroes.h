#ifndef CONTROLELOCALIZARPADROES_H
#define CONTROLELOCALIZARPADROES_H

#include "stdcpp.h"
#include "util.h"
#include "informacoesarquivo.h"

#include <vector>
#ifndef Q_MOC_RUN
#include <boost/signals2/signal.hpp>
#include <boost/thread/mutex.hpp>
#endif

class ControleLocalizarPadroes
{
public:

    // --------------------------------- Eventos ------------------------------------------------------ //
    typedef boost::signals2::signal <bool (const InformacoesArquivo& infoArquivo)> NotificadorPesquisando;
    typedef boost::signals2::signal <void (const InformacoesArquivo& infoArquivo)> NotificadorLocalizado;
    typedef boost::signals2::signal <void ()> NotificadorFinalizado;

    // ---------------------------------- Construtores ------------------------------------------------
    ControleLocalizarPadroes();

    // ---------------------------------- Métodos públicos --------------------------------------------
    /**
      @brief Localiza os arquivos dado um padrão de pesquisa

      @author Leonardo
      */
    bool buscarArquivos(const std::string &nomePastaInicial);

    /**

      @brief Define o padrão de pesquisa a ser localizado

      @author Leonardo
      */
    inline std::string padraoPesquisa() const;
    void setPadraoPesquisa(const std::string& psPadraoPesquisa);

    /**
     * @brief notificadorBusca
     *
     *  @author Leonardo
     */
    NotificadorPesquisando notificadorBusca;

    /**
     * @brief notificadorLocalizado
     *
     * @author Leonardo
     */
    NotificadorLocalizado notificadorLocalizado;

    /**
     * @brief notificadorFinalizado
     *
     * @author Leonardo
     */
    NotificadorFinalizado notificadorFinalizado;

    inline void setNomeArquivoParaBusca(bool pbHabilitado) {
        gbUsarNomeArquivoParaBusca = pbHabilitado;
    }

    inline void setUsarExpressoesRegulares(bool pbHabilitado) {
        gbUsarExpressoesRegulares = pbHabilitado;
        validarOpcoesBusca();
    }

    inline void setPalavraInteira(bool pbHabilitado) {
        gbPalavraInteira = pbHabilitado;
    }

    inline void setMaiusculas(bool pbHabilitado) {
        gbDistinguirMaiusculas = pbHabilitado;
    }

    inline bool nomeArquivoParaBusca() const {
        return gbUsarNomeArquivoParaBusca;
    }
    inline bool usarExpressoesRegulares() const {
        return gbUsarExpressoesRegulares;
    }

    inline bool palavraInteira() const {
        return gbPalavraInteira;
    }

    inline bool maiusculas() {
        return gbDistinguirMaiusculas;
    }

    bool estaPesquisando();

    void ativar();

    void desativar();
private:
    std::string gsPadraoPesquisa;
    bool gbUsarExpressoesRegulares;
    bool gbPalavraInteira;
    bool gbUsarNomeArquivoParaBusca;
    bool gbDistinguirMaiusculas;
    bool gbPesquisando;
    boost::mutex m;


    bool existePadrao(const std::string &psCaminho, InformacoesArquivo &poInfoArquivo);
    inline long tamanhoArquivo(const std::string& psCaminho);

    void validarOpcoesBusca();
};

#endif // CONTROLELOCALIZARPADROES_H
