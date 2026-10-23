#include "Gasto.hpp"

Gasto::Gasto(const std::string &descricao, double valor,
             const std::string &data, const std::string &categoria, int ordem)
    : descricao(descricao), valor(valor), data(data), categoria(categoria),
      ordem(ordem) {
}

std::string Gasto::getDescricao() const {
    return descricao;
}

double Gasto::getValor() const {
    return valor;
}

std::string Gasto::getData() const {
    return data;
}

std::string Gasto::getCategoria() const {
    return categoria;
}

int Gasto::getOrdem() const {
    return ordem;
}

bool Gasto::operator<(const Gasto &outro) const {
    return valor < outro.valor;
}
