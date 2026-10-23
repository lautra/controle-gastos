#ifndef GASTO_HPP
#define GASTO_HPP

#include <string>

class Gasto {
public:
    Gasto(const std::string &descricao, double valor, const std::string &data,
          const std::string &categoria, int ordem);

    std::string getDescricao() const;
    double getValor() const;
    std::string getData() const;
    std::string getCategoria() const;
    int getOrdem() const;

    bool operator<(const Gasto &outro) const;

private:
    std::string descricao;
    double valor;
    std::string data;
    std::string categoria;
    int ordem;
};

#endif
