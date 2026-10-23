#include "Carteira.hpp"

#include <utility>

ResultadoCategoria Carteira::cadastrarCategoria(const std::string &nome) {
    if (categorias.find(nome) != categorias.end()) {
        return CATEGORIA_JA_EXISTE;
    }

    categorias.insert(std::make_pair(nome, Categoria(nome)));
    return CATEGORIA_OK;
}

ResultadoLimite Carteira::definirLimite(const std::string &nome,
                                        double limite) {
    std::map<std::string, Categoria>::iterator it = categorias.find(nome);

    if (it == categorias.end()) {
        return LIMITE_CATEGORIA_NAO_ENCONTRADA;
    }

    if (limite < 0.0) {
        return LIMITE_INVALIDO;
    }

    it->second.setLimite(limite);
    return LIMITE_OK;
}
