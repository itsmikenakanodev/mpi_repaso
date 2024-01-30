#include <iostream>
#include <vector>
#include <mpi.h>
#include <cmath>

#define MATRIX_DIMENSION 25

void matrix_mult(double *A, double *B, double *C, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        double tmp = 0;
        for (int j = 0; j < cols; j++) {
            tmp = tmp + A[i * cols + j] * B[j];
        }
        C[i] = tmp;
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int rows_per_rank;
    int rows_alloc = MATRIX_DIMENSION;
    int padding = 0;

    //RELLENAR LAS FILAS PARA MANDAR DEL MISMO TAMAÑO A TODOS LOS RANKS
    if (MATRIX_DIMENSION % nprocs != 0) {
        rows_alloc = std::ceil((double) MATRIX_DIMENSION / nprocs) * nprocs;
        padding = rows_alloc - MATRIX_DIMENSION;
    }

    rows_per_rank = rows_alloc / nprocs;

    std::vector<double> b(MATRIX_DIMENSION); //25 elementos

    for (int i = 0; i < MATRIX_DIMENSION; i++) b[i] = 1;
    std::vector<double> A;
    std::vector<double> c;

    if (rank == 0) {
        //imprimir informacion
        std::printf("Dimension: %d, rows_alloc: %d, rows_per_rank: %d, padding: %d\n",
                    MATRIX_DIMENSION, rows_alloc, rows_per_rank, padding);

        A.resize(MATRIX_DIMENSION * rows_alloc); //tamaño 25 cols *28 filas
         c.resize(rows_alloc); //resultado va a tener 28 elementos;

        for (int i = 0; i < MATRIX_DIMENSION; i++) {
            for (int j = 0; j < MATRIX_DIMENSION; j++) {
                int index = i * MATRIX_DIMENSION + j;
                A[index] = i;
            }
        }

    }

    //enviar la matriz A
    //el scatter va a dividir y enviar bloques a los ranks
    //EL MPI IN PLACE ES APRA QUE EL CERO NO SE ENVIE LOS DATOS A SI MISMO PORQUE YA TIENE LA MTRIZ COMPLETA
    std::vector<double> A_local(rows_per_rank * MATRIX_DIMENSION);
    // std::vector<double> b_local(MATRIX_DIMENSION);
    std::vector<double> c_local(rows_per_rank);
    MPI_Scatter(A.data(), rows_per_rank * MATRIX_DIMENSION, MPI_DOUBLE,
                A_local.data(), rows_per_rank * MATRIX_DIMENSION, MPI_DOUBLE,
                0, MPI_COMM_WORLD);




    //realizar el calculo c=A x b
    int rows_per_rank_tmp = rows_per_rank;

    if (rank == nprocs - 1) {
        rows_per_rank_tmp = MATRIX_DIMENSION - (rank * rows_per_rank);
    }

    std::printf("RANK_%d: [%.0f..%.0f]\n", rank, A_local[0], A_local.back());

    matrix_mult(A_local.data(), b.data(), c_local.data(), rows_per_rank_tmp, MATRIX_DIMENSION);

    //enviar el resultado parcial
    MPI_Gather(c_local.data(), rows_per_rank, MPI_DOUBLE,
               c.data(), rows_per_rank, MPI_DOUBLE,
               0, MPI_COMM_WORLD);


    if (rank == 0) {
        c.resize(MATRIX_DIMENSION);

        //imprimir el resultado
        std::printf("resultado: \n");
        for (int i = 0; i < MATRIX_DIMENSION; i++) {
            std::printf("%.0f, ", c[i]);
        }
    }
    MPI_Finalize();

    return 0;
}