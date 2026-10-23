#ifndef CARTEIRA_HPP
#define CARTEIRA_HPP

#include <map>
#include <string>

#include "Categoria.hpp"

enum ResultadoCategoria {
    CATEGORIA_OK,
    CATEGORIA_JA_EXISTE
};

enum ResultadoLimite {
    LIMITE_OK,
    LIMITE_CATEGORIA_NAO_ENCONTRADA,
    LIMITE_INVALIDO
};

class Carteira {
public:
    ResultadoCategoria cadastrarCategoria(const std::string &nome);
    ResultadoLimite definirLimite(const std::string &nome, double limite);

private:
    std::map<std::string, Categoria> categorias;
};

#endif
