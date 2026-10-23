#ifndef CATEGORIA_HPP
#define CATEGORIA_HPP

#include <string>

class Categoria {
public:
    Categoria(const std::string &nome, double limite = 0.0);

    std::string getNome() const;
    double getLimite() const;
    void setLimite(double limite);
    bool passouDoLimite(double total) const;

private:
    std::string nome;
    double limite;
};

#endif
