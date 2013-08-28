#ifndef EXECUTORBUSCA_H
#define EXECUTORBUSCA_H

#include "threadboost.h"

#if _MSC_VER > 1600
#include <functional>
#else
#include <boost/functional.hpp>
#endif

/**
 * @brief The ExecutorBusca class
 *
 * Classe que executa a pesquisa.
 */
class ExecutorBusca : public ThreadBoost
{
public:
#if _MSC_VER > 1600
    typedef std::function<bool (const std::string&)> Funcao;
#else
    typedef boost::function<bool (const std::string&)> Funcao;
#endif

    ExecutorBusca(Funcao f);

    std::string getCaminhoArquivo() const;
    void setCaminhoArquivo(const std::string &value);

protected:
    Funcao funcao;
    std::string caminhoArquivo;
    virtual void executar();

};

#endif // EXECUTORBUSCA_H
