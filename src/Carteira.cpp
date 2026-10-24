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

ResultadoGasto Carteira::registrarGasto(const std::string &descricao,
                                        double valor, const std::string &data,
                                        const std::string &categoria) {
    if (valor <= 0.0) {
        return GASTO_VALOR_INVALIDO;
    }

    if (!dataValida(data)) {
        return GASTO_DATA_INVALIDA;
    }

    if (categorias.find(categoria) == categorias.end()) {
        return GASTO_CATEGORIA_NAO_ENCONTRADA;
    }

    gastos.push_back(Gasto(descricao, valor, data, categoria, proximaOrdem));
    proximaOrdem = proximaOrdem + 1;
    return GASTO_OK;
}

bool Carteira::dataValida(const std::string &data) const {
    if (data.size() != 10 || data[2] != '/' || data[5] != '/') {
        return false;
    }

    std::string::size_type i;
    for (i = 0; i < data.size(); ++i) {
        if (i == 2 || i == 5) {
            continue;
        }
        if (data[i] < '0' || data[i] > '9') {
            return false;
        }
    }

    int dia;
    int mes;
    int ano;
    separarData(data, dia, mes, ano);

    if (mes < 1 || mes > 12 || ano < 1) {
        return false;
    }

    return dia >= 1 && dia <= diasNoMes(mes, ano);
}

void Carteira::separarData(const std::string &data, int &dia, int &mes,
                           int &ano) const {
    dia = (data[0] - '0') * 10 + (data[1] - '0');
    mes = (data[3] - '0') * 10 + (data[4] - '0');
    ano = (data[6] - '0') * 1000 + (data[7] - '0') * 100 +
          (data[8] - '0') * 10 + (data[9] - '0');
}

bool Carteira::bissexto(int ano) const {
    return (ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0;
}

int Carteira::diasNoMes(int mes, int ano) const {
    if (mes == 2) {
        return bissexto(ano) ? 29 : 28;
    }

    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        return 30;
    }

    return 31;
}
