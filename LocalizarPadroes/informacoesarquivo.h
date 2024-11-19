#ifndef INFORMACOESARQUIVO_H
#define INFORMACOESARQUIVO_H

#include "stdcpp.h"

#include <vector>

class InformacoesArquivo
{
public:
    typedef std::vector<unsigned> PosicoesOcorrencias;

    InformacoesArquivo();

    inline const std::string &nomeArquivo() const
    {
        return gsNomeArquivo;
    }

    inline const PosicoesOcorrencias &posicoes() const
    {
        return goPosicoesOcorrencia;
    }

    inline unsigned numeroOcorrencias() const
    {
        return gnNumeroOcorrenciasPalavra;
    }

    friend class ControleLocalizarPadroes;

private:
    std::string gsNomeArquivo;
    unsigned gnNumeroOcorrenciasPalavra;
    PosicoesOcorrencias goPosicoesOcorrencia;
};

#endif // INFORMACOESARQUIVO_H
