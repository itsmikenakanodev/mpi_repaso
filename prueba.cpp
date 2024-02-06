#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <mpi.h>
#include <cmath>

int max(int* lista, int n){
    int max=lista[0];
    for (int i = 1; i < n; ++i) {
        if(max<lista[i]){
            max=lista[i];
        }
    }
    return max;
}

int min(int* lista, int n){
    int min=lista[0];
    for (int i = 1; i < n; ++i) {
        if(min>lista[i]){
            min=lista[i];
        }
    }
    return min;
}

int sum(int* lista, int n){
    int sum=0;
    for (int i = 1; i < n; ++i) {     
            sum+=lista[i];
    }
    return sum;
}


std::vector<int> read_file() {
 std::fstream fs("/workspace/mpi_repaso/datos.txt", std::ios::in );
 std::string line;
 std::vector<int> ret;
 while( std::getline(fs, line) ){
 ret.push_back( std::stoi(line) );
 }
 fs.close();
 return ret;
} 

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    std::vector<int> datos=read_file();;

    int block_size;
    int residuo=0;
    
    if(datos.size()%nprocs!=0){
        residuo=datos.size()%nprocs;
        block_size= (datos.size()-residuo)/(nprocs);
    }else{
        block_size= (datos.size())/(nprocs);
    }
    int max_final;
    int min_final;

    //maximo y minimo 
    if (rank == 0) {
        
        std::printf("block_size=%d, residuo=%d\n",block_size,residuo);

        for (int i = 0; i < nprocs-1; i++) {
            int start=block_size*i+residuo;
            MPI_Send(&datos[start], block_size, MPI_INT, i+1, 0, MPI_COMM_WORLD);
        }

        int max_local = max(datos.data(),  residuo);
        int min_local = min(datos.data(),  residuo);

        MPI_Reduce(&max_local, &max_final, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&min_local, &min_final, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

        std::printf("Maximo=%d\n", max_final);
        std::printf("Minimo=%d\n", min_final);
    }else{
        MPI_Recv(datos.data(), block_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int maximo_local=max(datos.data(),datos.size());
        int minimo_local=min(datos.data(),datos.size());

        MPI_Reduce(&maximo_local, nullptr, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&minimo_local, nullptr, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    }

    //promedio
    int suma_total=0;
     if (rank == 0) {
        
        std::printf("block_size=%d, residuo=%d\n",block_size,residuo);

        for (int i = 0; i < nprocs-1; i++) {
            int start=block_size*i+residuo;
            MPI_Send(&datos[start], block_size, MPI_INT, i+1, 0, MPI_COMM_WORLD);
        }

        int sum_local = sum(datos.data(),  residuo);

        MPI_Reduce(&sum_local, &suma_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        std::printf("Suma=%d\n", suma_total);
        double promedio=(double) suma_total/datos.size();
        std::printf("Promedio=%f\n", promedio);
    }else{
        MPI_Recv(datos.data(), block_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int suma_local=sum(datos.data(),datos.size());

        MPI_Reduce(&suma_local, nullptr, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    }


    //tabla de frecuencias
    int real_size;
    int padding = 0;

    if (datos.size() % nprocs != 0) {
        real_size = std::ceil((double) datos.size() / nprocs) * nprocs;
        block_size = real_size/nprocs;
        padding = real_size-datos.size();
    }
    std::printf("block_size=%d, padding=%d\n",block_size,padding);

    std::vector<int> conteo(101);
    std::vector<int> datos_local(block_size);
    std::vector<int> conteo_local(101);

    MPI_Scatter(datos.data(),block_size,MPI_INT,datos_local.data(),block_size,MPI_INT,0,MPI_COMM_WORLD);

    if(rank==nprocs-1){
        block_size=block_size-padding;
        datos_local.resize(block_size);
    }

    for(int i=0; i<block_size;i++){
        conteo_local[datos_local[i]]+=1;
    }


    MPI_Reduce(conteo_local.data(),conteo.data(),101,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    if(rank==0){
        int suma=0;
        for(int i=0; i<=100;i++){
            std::printf("Conteo[%d]=%d\n",i,conteo[i]);
            suma +=conteo[i];
        }
        std::printf("Suma=%d",suma);
    }
   
    MPI_Finalize();
    return 0;
}