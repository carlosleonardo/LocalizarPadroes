#ifndef UTIL_H
#define UTIL_H

#include "stdcpp.h"

/**
  @brief Converte a string para maiúsculas

  @author Leonardo
  @param psOrigem string original
  */
std::string toUpper(const std::string &psOrigem);

/**
    @brief Converte a string para minúscula

    @author Leonardo
    @param psOrigem string original
  */
std::string toLower(const std::string &psOrigem);

/**
  @brief    Tenta descobrir se o arquivo é binário ou texto.

  @author   Leonardo

  @param    psArquivo   Caminho completo do arquivo
  */
bool arquivoTexto(const std::string& psArquivo);

const int BUFSIZE=512;

#endif // UTIL_H
