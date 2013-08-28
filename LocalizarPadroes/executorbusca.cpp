#include "executorbusca.h"

ExecutorBusca::ExecutorBusca(Funcao f):ThreadBoost()
{
    funcao = f;
}
std::string ExecutorBusca::getCaminhoArquivo() const
{
    return caminhoArquivo;
}

void ExecutorBusca::setCaminhoArquivo(const std::string &value)
{
    caminhoArquivo = value;
}


void ExecutorBusca::executar()
{
    if (funcao) {
        funcao(caminhoArquivo);
    }
}
