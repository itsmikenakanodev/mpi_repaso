#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>

int maxNumero(int *lista, int tamanio)
{
    int max = lista[0];
    for (int i = 1; i < tamanio; ++i)
    {
        if (max < lista[i])
        {
            max = lista[i];
        }
    }
    return max;
}

int minNumero(int *lista, int tamanio)
{
    int min = lista[0];
    for (int i = 1; i < tamanio; ++i)
    {
        if (min > lista[i])
        {
            min = lista[i];
        }
    }
    return min;
}

//
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int tamanioLista = 12;
    std::vector<int> numeros(tamanioLista);
    int block_size = (tamanioLista / nprocs);
    int max;
    int min;

    if (rank == 0)
    {
        std::printf("Rank_%d =", rank);
        for (int i = 0; i < tamanioLista; ++i)
        {
            int numero = rand() % 1000;
            std::printf("[%d],", numero);
            numeros[i] = numero;
        }
        std::printf("\n");
    }

    std::vector<int> numeros_locales(block_size);
    MPI_Scatter(numeros.data(), block_size, MPI_INT, numeros_locales.data(), block_size, MPI_INT, 0, MPI_COMM_WORLD);
    std::printf("Rank_%d =", rank);

    for (int i = 0; i < block_size; ++i)
    {
        std::printf("[%d],", numeros_locales[i]);
    }
    std::printf("\n");

    int max_parcial = maxNumero(numeros_locales.data(), block_size);
    int min_parcial = minNumero(numeros_locales.data(), block_size);

    std::vector<int> maximos_locales(nprocs);
    std::vector<int> minimos_locales(nprocs);
    MPI_Gather(&max_parcial, 1, MPI_INT, maximos_locales.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&min_parcial, 1, MPI_INT, minimos_locales.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank == 0)
    {
        for (int i = 0; i < nprocs; ++i)
    {
        std::printf("max[%d],", maximos_locales[i]);
    }
    std::printf("\n");
    for (int i = 0; i < nprocs; ++i)
    {
        std::printf("min[%d],", minimos_locales[i]);
    }
    std::printf("\n");
        max = maxNumero(maximos_locales.data(), nprocs);
        min = minNumero(minimos_locales.data(), nprocs);
        std::printf("Maximo Total=%d\n", max);
        std::printf("Minimo Total=%d\n", min);
    }

    MPI_Finalize();
    return 0;
}