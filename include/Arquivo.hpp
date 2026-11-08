#ifndef ARQUIVO_HPP
#define ARQUIVO_HPP

#include <string>
#include <vector>

#include "Carteira.hpp"

class Arquivo {
public:
    Arquivo(const std::string &nomeArquivo = "dados.txt");

    void carregar(Carteira &carteira) const;
    void salvar(const Carteira &carteira) const;

private:
    std::string nomeArquivo;

    bool textoParaDouble(const std::string &texto, double &valor) const;
    std::vector<std::string> dividirLinha(const std::string &linha) const;
};

#endif
