#ifndef EXECUTORBUSCA_H
#define EXECUTORBUSCA_H

#include "threadboost.h"

#include <functional>

/**
 * @brief The ExecutorBusca class
 *
 * Classe que executa a pesquisa.
 */
class ExecutorBusca : public ThreadBoost
{
public:
    typedef std::function<bool (const std::string&)> Funcao;
    ExecutorBusca(Funcao f);

    std::string getCaminhoArquivo() const;
    void setCaminhoArquivo(const std::string &value);

protected:
    Funcao funcao;
    std::string caminhoArquivo;
    virtual void executar();

};

#endif // EXECUTORBUSCA_H
