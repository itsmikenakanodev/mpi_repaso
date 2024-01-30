#include <iostream>
#include <mpi.h>
#include <vector>
#include <cmath>
#include <cstdlib>
 
int main(int argc, char** argv){
    MPI_Init(&argc,&argv);
 
    int rank, nprocs;
 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    std::vector<int> conteo(100);
    std::vector<int> datos;
 
   
 
    if(rank == 0){
        datos.resize(100);
         for(int i=0;i<100;i++){
            datos[i]=rand()%100;
         }
       
    }
    std::vector<int> datos_local(25);
    std::vector<int> conteo_local(100);
    MPI_Scatter(datos.data(),25,MPI_INT,datos_local.data(),25,MPI_INT,0,MPI_COMM_WORLD);
 
    for(int i=0; i<25;i++){
        //std::printf("Conteo[%d]=%d",i,datos_local[i]);
        conteo_local[datos_local[i]]+=1;
    }
 
    MPI_Reduce(conteo_local.data(),conteo.data(),100,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
   
    if(rank==0){
        int suma=0;
        for(int i=0; i<100;i++){
            std::printf("Conteo[%d]=%d\n",i,conteo[i]);
            suma +=conteo[i];
        }
        std::printf("Suma=%d",suma);
    }
   
 
    MPI_Finalize();
 
    return 0;
}