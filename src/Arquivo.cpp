#include "Arquivo.hpp"

#include <fstream>
#include <iomanip>
#include <list>
#include <map>
#include <sstream>
#include <stdexcept>

Arquivo::Arquivo(const std::string &nomeArquivo)
    : nomeArquivo(nomeArquivo) {
}

std::vector<std::string> Arquivo::dividirLinha(const std::string &linha) const {
    std::vector<std::string> campos;
    std::string campo;
    std::string::size_type i;

    for (i = 0; i < linha.size(); ++i) {
        if (linha[i] == ';') {
            campos.push_back(campo);
            campo.clear();
        } else {
            campo += linha[i];
        }
    }
    campos.push_back(campo);

    return campos;
}

bool Arquivo::textoParaDouble(const std::string &texto, double &valor) const {
    std::istringstream fluxo(texto);
    char resto;

    fluxo >> valor;
    if (fluxo.fail()) {
        return false;
    }

    if (fluxo >> resto) {
        return false;
    }

    return true;
}

void Arquivo::carregar(Carteira &carteira) const {
    std::ifstream arquivo(nomeArquivo.c_str());
    if (!arquivo.is_open()) {
        return;
    }

    std::vector<std::string> linhas;
    std::string linha;
    while (std::getline(arquivo, linha)) {
        linhas.push_back(linha);
    }

    while (!linhas.empty() && linhas.back().empty()) {
        linhas.pop_back();
    }

    Carteira temporaria;
    std::vector<std::string> linhasGasto;

    std::vector<std::string>::const_iterator it;
    for (it = linhas.begin(); it != linhas.end(); ++it) {
        if (it->empty()) {
            throw std::invalid_argument(
                "Arquivo de dados inválido, começando vazio");
        }

        std::vector<std::string> campos = dividirLinha(*it);

        if (campos[0] == "C") {
            if (campos.size() != 3) {
                throw std::invalid_argument(
                    "Arquivo de dados inválido, começando vazio");
            }

            if (campos[1].empty()) {
                throw std::invalid_argument(
                    "Arquivo de dados inválido, começando vazio");
            }

            double limite;
            if (!textoParaDouble(campos[2], limite)) {
                throw std::invalid_argument(
                    "Arquivo de dados inválido, começando vazio");
            }

            temporaria.cadastrarCategoria(campos[1]);
            temporaria.definirLimite(campos[1], limite);
        } else if (campos[0] == "G") {
            linhasGasto.push_back(*it);
        } else {
            throw std::invalid_argument(
                "Arquivo de dados inválido, começando vazio");
        }
    }

    for (it = linhasGasto.begin(); it != linhasGasto.end(); ++it) {
        std::vector<std::string> campos = dividirLinha(*it);

        if (campos.size() != 5) {
            throw std::invalid_argument(
                "Arquivo de dados inválido, começando vazio");
        }

        if (campos[1].empty() || campos[4].empty()) {
            throw std::invalid_argument(
                "Arquivo de dados inválido, começando vazio");
        }

        double valor;
        if (!textoParaDouble(campos[2], valor)) {
            throw std::invalid_argument(
                "Arquivo de dados inválido, começando vazio");
        }

        temporaria.registrarGasto(campos[1], valor, campos[3], campos[4]);
    }

    carteira = temporaria;
}

void Arquivo::salvar(const Carteira &carteira) const {
    std::ofstream arquivo(nomeArquivo.c_str());
    if (!arquivo.is_open()) {
        throw std::runtime_error("Não consegui salvar os dados");
    }

    arquivo << std::setprecision(15);

    const std::map<std::string, Categoria> &categorias =
        carteira.getCategorias();
    std::map<std::string, Categoria>::const_iterator itCat;
    for (itCat = categorias.begin(); itCat != categorias.end(); ++itCat) {
        arquivo << "C;" << itCat->second.getNome() << ";"
                << itCat->second.getLimite() << "\n";
    }

    const std::list<Gasto> &gastos = carteira.getGastos();
    std::list<Gasto>::const_iterator itGasto;
    for (itGasto = gastos.begin(); itGasto != gastos.end(); ++itGasto) {
        arquivo << "G;" << itGasto->getDescricao() << ";"
                << itGasto->getValor() << ";" << itGasto->getData() << ";"
                << itGasto->getCategoria() << "\n";
    }

    arquivo.close();
    if (arquivo.fail()) {
        throw std::runtime_error("Não consegui salvar os dados");
    }
}
