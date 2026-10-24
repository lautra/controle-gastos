#include "Carteira.hpp"

#include <algorithm>
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

bool Carteira::mesAnoValido(int mes, int ano) const {
    return mes >= 1 && mes <= 12 && ano >= 1;
}

bool Carteira::temGastosNoMes(int mes, int ano) const {
    if (!mesAnoValido(mes, ano)) {
        return false;
    }

    std::list<Gasto>::const_iterator it;
    for (it = gastos.begin(); it != gastos.end(); ++it) {
        int dia;
        int gmes;
        int gano;
        separarData(it->getData(), dia, gmes, gano);
        if (gmes == mes && gano == ano) {
            return true;
        }
    }

    return false;
}

std::map<std::string, double> Carteira::somarGastosDoMes(int mes, int ano) const {
    std::map<std::string, double> totais;
    std::list<Gasto>::const_iterator it;

    if (!mesAnoValido(mes, ano)) {
        return totais;
    }

    for (it = gastos.begin(); it != gastos.end(); ++it) {
        int dia;
        int gmes;
        int gano;
        separarData(it->getData(), dia, gmes, gano);
        if (gmes == mes && gano == ano) {
            totais[it->getCategoria()] += it->getValor();
        }
    }

    return totais;
}

std::vector<ResumoCategoria> Carteira::totaisPorCategoria(int mes, int ano) {
    std::vector<ResumoCategoria> resultado;
    if (!mesAnoValido(mes, ano)) {
        return resultado;
    }

    std::map<std::string, double> totais = somarGastosDoMes(mes, ano);
    std::map<std::string, Categoria>::const_iterator itCat;

    for (itCat = categorias.begin(); itCat != categorias.end(); ++itCat) {
        ResumoCategoria r;
        r.nome = itCat->first;
        r.limite = itCat->second.getLimite();
        std::map<std::string, double>::iterator itTotal = totais.find(itCat->first);
        if (itTotal == totais.end()) {
            r.total = 0.0;
        } else {
            r.total = itTotal->second;
        }
        resultado.push_back(r);
    }

    return resultado;
}

namespace {

bool compararGastosMaior(const Gasto &a, const Gasto &b) {
    if (a.getValor() != b.getValor()) {
        return a.getValor() > b.getValor();
    }
    return a.getOrdem() < b.getOrdem();
}

}

std::vector<Gasto> Carteira::maioresGastosDoMes(int mes, int ano) {
    std::vector<Gasto> selecionados;
    if (!mesAnoValido(mes, ano)) {
        return selecionados;
    }

    std::list<Gasto>::const_iterator it;
    for (it = gastos.begin(); it != gastos.end(); ++it) {
        int dia;
        int gmes;
        int gano;
        separarData(it->getData(), dia, gmes, gano);
        if (gmes == mes && gano == ano) {
            selecionados.push_back(*it);
        }
    }

    std::sort(selecionados.begin(), selecionados.end(), compararGastosMaior);

    if (selecionados.size() > 3) {
        selecionados.erase(selecionados.begin() + 3, selecionados.end());
    }

    return selecionados;
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
