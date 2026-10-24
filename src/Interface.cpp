#include "Interface.hpp"

#include <iostream>
#include <limits>
#include <string>

namespace {

void ignorarLinha() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void mostrarMenu() {
    std::cout << "1. Cadastrar categoria\n";
    std::cout << "2. Definir limite\n";
    std::cout << "3. Registrar gastos\n";
    std::cout << "0. Sair\n";
}

void cadastrarCategoria(Carteira &carteira) {
    std::string nome;

    std::cout << "Nome da categoria: ";
    std::getline(std::cin, nome);

    if (carteira.cadastrarCategoria(nome) == CATEGORIA_OK) {
        std::cout << "Categoria cadastrada\n";
    } else {
        std::cout << "Categoria já cadastrada\n";
    }
}

void definirLimite(Carteira &carteira) {
    std::string nome;
    double limite;

    std::cout << "Categoria: ";
    std::getline(std::cin, nome);

    std::cout << "Limite: ";
    if (!(std::cin >> limite)) {
        std::cin.clear();
        ignorarLinha();
        std::cout << "Entrada inválida, tente de novo\n";
        return;
    }
    ignorarLinha();

    ResultadoLimite resultado = carteira.definirLimite(nome, limite);

    if (resultado == LIMITE_OK) {
        std::cout << "Limite definido\n";
    } else if (resultado == LIMITE_INVALIDO) {
        std::cout << "Valor inválido\n";
    } else {
        std::cout << "Categoria não encontrada\n";
    }
}

void registrarGasto(Carteira &carteira) {
    std::string descricao;
    double valor;
    std::string data;
    std::string categoria;

    std::cout << "Descrição: ";
    std::getline(std::cin, descricao);

    std::cout << "Valor: ";
    if (!(std::cin >> valor)) {
        std::cin.clear();
        ignorarLinha();
        std::cout << "Entrada inválida, tente de novo\n";
        return;
    }
    ignorarLinha();

    std::cout << "Data: ";
    std::getline(std::cin, data);

    std::cout << "Categoria: ";
    std::getline(std::cin, categoria);

    ResultadoGasto resultado =
        carteira.registrarGasto(descricao, valor, data, categoria);

    if (resultado == GASTO_OK) {
        std::cout << "Gasto registrado\n";
    } else if (resultado == GASTO_VALOR_INVALIDO) {
        std::cout << "Valor inválido\n";
    } else if (resultado == GASTO_DATA_INVALIDA) {
        std::cout << "Data inválida\n";
    } else {
        std::cout << "Categoria não encontrada\n";
    }
}

}

void executarInterface(Carteira &carteira) {
    int opcao;

    while (true) {
        mostrarMenu();
        std::cout << "Escolha: ";

        if (!(std::cin >> opcao)) {
            std::cin.clear();
            ignorarLinha();
            std::cout << "Entrada inválida, tente de novo\n";
            continue;
        }
        ignorarLinha();

        if (opcao == 0) {
            return;
        } else if (opcao == 1) {
            cadastrarCategoria(carteira);
        } else if (opcao == 2) {
            definirLimite(carteira);
        } else if (opcao == 3) {
            registrarGasto(carteira);
        } else {
            std::cout << "Opção inválida, tente de novo\n";
        }
    }
}
