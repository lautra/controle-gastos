#include "Carteira.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

void Carteira::cadastrarCategoria(const std::string &nome) {
    if (nome.find(';') != std::string::npos) {
        throw std::invalid_argument("Não use ; no texto");
    }

    if (categorias.find(nome) != categorias.end()) {
        throw std::invalid_argument("Categoria já cadastrada");
    }

    categorias.insert(std::make_pair(nome, Categoria(nome)));
}

void Carteira::definirLimite(const std::string &nome, double limite) {
    if (nome.find(';') != std::string::npos) {
        throw std::invalid_argument("Não use ; no texto");
    }

    std::map<std::string, Categoria>::iterator it = categorias.find(nome);

    if (it == categorias.end()) {
        throw std::invalid_argument("Categoria não encontrada");
    }

    if (limite < 0.0) {
        throw std::invalid_argument("Valor inválido");
    }

    it->second.setLimite(limite);
}

void Carteira::registrarGasto(const std::string &descricao, double valor,
                              const std::string &data,
                              const std::string &categoria) {
    if (descricao.find(';') != std::string::npos ||
        categoria.find(';') != std::string::npos) {
        throw std::invalid_argument("Não use ; no texto");
    }

    if (valor <= 0.0) {
        throw std::invalid_argument("Valor inválido");
    }

    if (!dataValida(data)) {
        throw std::invalid_argument("Data inválida");
    }

    if (categorias.find(categoria) == categorias.end()) {
        throw std::invalid_argument("Categoria não encontrada");
    }

    gastos.push_back(Gasto(descricao, valor, data, categoria, proximaOrdem));
    proximaOrdem = proximaOrdem + 1;
}

bool Carteira::mesAnoValido(int mes, int ano) const {
    return mes >= 1 && mes <= 12 && ano >= 1;
}

bool Carteira::temGastosNoMes(int mes, int ano) const {
    if (!mesAnoValido(mes, ano)) {
        throw std::invalid_argument("Data inválida");
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
        throw std::invalid_argument("Data inválida");
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
        throw std::invalid_argument("Data inválida");
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

std::vector<ResumoCategoria> Carteira::categoriasAcimaDoLimite(int mes, int ano) {
    std::vector<ResumoCategoria> resultado;
    if (!mesAnoValido(mes, ano)) {
        throw std::invalid_argument("Data inválida");
    }

    std::map<std::string, double> totais = somarGastosDoMes(mes, ano);
    std::map<std::string, Categoria>::const_iterator itCat;

    for (itCat = categorias.begin(); itCat != categorias.end(); ++itCat) {
        double total = 0.0;
        std::map<std::string, double>::iterator itTotal =
            totais.find(itCat->first);
        if (itTotal != totais.end()) {
            total = itTotal->second;
        }

        if (itCat->second.passouDoLimite(total)) {
            ResumoCategoria r;
            r.nome = itCat->first;
            r.total = total;
            r.limite = itCat->second.getLimite();
            resultado.push_back(r);
        }
    }

    return resultado;
}

const std::map<std::string, Categoria> &Carteira::getCategorias() const {
    return categorias;
}

const std::list<Gasto> &Carteira::getGastos() const {
    return gastos;
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
