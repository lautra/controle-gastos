#ifndef CARTEIRA_HPP
#define CARTEIRA_HPP

#include <list>
#include <map>
#include <string>
#include <vector>

#include "Categoria.hpp"
#include "Gasto.hpp"

struct ResumoCategoria {
    std::string nome;
    double total;
    double limite;
};

class Carteira {
public:
    void cadastrarCategoria(const std::string &nome);
    void definirLimite(const std::string &nome, double limite);
    void registrarGasto(const std::string &descricao, double valor,
                        const std::string &data,
                        const std::string &categoria);

    bool mesAnoValido(int mes, int ano) const;
    bool temGastosNoMes(int mes, int ano) const;
    std::vector<ResumoCategoria> totaisPorCategoria(int mes, int ano);
    std::vector<Gasto> maioresGastosDoMes(int mes, int ano);
    std::vector<ResumoCategoria> categoriasAcimaDoLimite(int mes, int ano);

    const std::map<std::string, Categoria> &getCategorias() const;
    const std::list<Gasto> &getGastos() const;

private:
    bool dataValida(const std::string &data) const;
    void separarData(const std::string &data, int &dia, int &mes,
                     int &ano) const;
    bool bissexto(int ano) const;
    int diasNoMes(int mes, int ano) const;
    std::map<std::string, double> somarGastosDoMes(int mes, int ano) const;

    std::map<std::string, Categoria> categorias;
    std::list<Gasto> gastos;
    int proximaOrdem = 0;
};

#endif
