#include "util.h"

#include <iostream>
#include <fstream>
#include <exception>
#include <array>
#include <cassert>

using namespace std;

std::string toUpper(const std::string& psOrigem)
{
    std::string lsResultado;

    std::transform(psOrigem.begin(), psOrigem.end(), std::back_inserter(lsResultado), ::toupper);
    return lsResultado;
}

std::string toLower(const std::string &psOrigem)
{
    std::string lsResultado;

    std::transform(psOrigem.begin(), psOrigem.end(), std::back_inserter(lsResultado), ::tolower);
    return lsResultado;
}

/*
 Tenta descobrir se é um arquivo binário.
 Usa heurística para fazer isso.

 1 - Tenta pela extensão
 2 - Lê os primeiros bytes do arquivo

 */
bool decodifica(const char* buf, size_t tamanho)
{
    assert(buf != nullptr);
    int codigo = buf[0];

    // Extrai bytes mais significativos e menos significativos
    char loByte = char(codigo);
    char hiByte = char((codigo >> sizeof(int)) & 0xff);

    if (hiByte==1) {

    }
    return false;
}

bool arquivoTexto(const std::string& psArquivo)
{
    std::ifstream loArquivo;
    array<char, BUFSIZE> buffer;

    try {
        loArquivo.open(psArquivo.c_str(), std::ios_base::binary);

        /* Usamos mais de uma estratégia aqui para descobrir se o arquivo é binário
            1 - Mime types
            2 - Código mágico utilizando assinaturas conhecidas
        */
        loArquivo.read(buffer.data(), buffer.size());

        if (!decodifica(buffer.data(), buffer.size())) {
            return false;
        }
    }
    catch(std::exception& exc) {
        std::cerr << exc.what() << std::endl;
    }

    return true;
}

