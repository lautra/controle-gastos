#include <iostream>

#include "Categoria.hpp"
#include "Gasto.hpp"

int main() {
    Categoria categoria("Mercado", 500.0);
    Gasto primeiro("Compras da semana", 120.0, "22/10/2026", "Mercado", 1);
    Gasto segundo("Feira", 80.0, "23/10/2026", "Mercado", 2);

    std::cout << categoria.getNome() << " " << categoria.getLimite() << '\n';
    std::cout << primeiro.getDescricao() << " " << primeiro.getValor() << " "
              << primeiro.getData() << " " << primeiro.getCategoria() << " "
              << primeiro.getOrdem() << '\n';
    std::cout << segundo.getDescricao() << " " << segundo.getValor() << " "
              << segundo.getData() << " " << segundo.getCategoria() << " "
              << segundo.getOrdem() << '\n';

    if (segundo < primeiro) {
        std::cout << "Segundo gasto tem valor menor que o primeiro\n";
    } else {
        std::cout << "Segundo gasto nao tem valor menor que o primeiro\n";
    }

    return 0;
}
