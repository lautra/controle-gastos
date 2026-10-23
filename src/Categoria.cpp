#include "Categoria.hpp"

Categoria::Categoria(const std::string &nome, double limite)
    : nome(nome), limite(limite) {
}

std::string Categoria::getNome() const {
    return nome;
}

double Categoria::getLimite() const {
    return limite;
}

void Categoria::setLimite(double limite) {
    this->limite = limite;
}

bool Categoria::passouDoLimite(double total) const {
    return limite > 0.0 && total > limite;
}
