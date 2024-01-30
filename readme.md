# mpi2324 

## Intalación de MPI en Ubuntu

```
sudo apt-get update
sudo apt-get install libopenmpi-dev
```

## Compilación

```
mpicxx ejemplo02.cpp -o ejemplo02
```

## Ejecutar
```
mpiexec -n 4 ./ejemplo02
```
