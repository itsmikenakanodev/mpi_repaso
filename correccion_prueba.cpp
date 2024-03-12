#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <mpi.h>
#include <cmath>

std::vector<int> read_file() {
 std::fstream fs("D:/Programacion Paralela/mpi_repaso/datos.txt", std::ios::in );
 std::string line;
 std::vector<int> ret;
 while( std::getline(fs, line) ){
 ret.push_back( std::stoi(line) );
 }
 fs.close();
 return ret;
} 

std::vector<int> calcular_histograma(int* datos, int size) {
    std::vector<int> histo(101);
    for(int i=0; i<size;i++){
        histo[datos[i]]++;
    }
    return histo;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int block_size;
    int real_size;
    int padding = 0;

    std::vector<int> A;
    std::vector<int> A_local;

    if(rank==0) {
        A = read_file();

        int max_items = A.size();

        real_size = max_items;

        if(max_items%nprocs!=0) {
            real_size = std::ceil((double)max_items/nprocs)*nprocs;
            block_size=real_size/nprocs;
            padding=real_size-max_items;
        }

        std::printf("RANK_%d: dimension: %d, real_size: %d, block_size: %d, padding: %d\n",rank,max_items,real_size,block_size,padding);
    }

    //enviar el tamaño del bloque
    MPI_Bcast( &block_size , 1 , MPI_INT , 0 , MPI_COMM_WORLD);

    if(rank==0){
        MPI_Send(&padding,1,MPI_INT,nprocs-1,0,MPI_COMM_WORLD);
    }
    else if(rank==nprocs-1) {
        MPI_Recv(&padding,1,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }

    A_local.resize(block_size);

    MPI_Scatter( A.data() , block_size , MPI_INT , A_local.data() , block_size, MPI_INT , 0 , MPI_COMM_WORLD);


    //realizar el calculo
    int new_size = block_size;

    if(rank==nprocs-1) {
        new_size = block_size-padding;
    }

    std::printf("RANK_%d calculando histograma, block_size=  %d", rank , new_size);

    auto H_local = calcular_histograma(A_local.data(), new_size);

    //recibir los datos
    std::vector<int> H;

    if(rank==0) {
        H.resize(101);
    }

    MPI_Reduce( H_local.data() , H.data() , 101 , 
    MPI_INT , MPI_SUM , 0 , 
    MPI_COMM_WORLD);

    if(rank==0) {\
    int suma=0;
        //imprimir el resultado
        for(int i=0; i<101;i++) {
            suma +=H[i];
            std::printf("%3d: %d", i, H[i]);        
            }
        std::printf("suma: %d", suma);
    }

    MPI_Finalize();
    return 0;
}