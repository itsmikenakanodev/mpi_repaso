cmake_minimum_required(VERSION 3.26)
project(mpi_repaso)

set(CMAKE_CXX_STANDARD 17)

find_package(MPI REQUIRED)
find_package(fmt)
find_package(SFML)

add_executable(mpi_repaso correccion_prueba.cpp)

target_link_libraries(mpi_repaso fmt::fmt sfml::sfml MPI::MPI_CXX)

add_executable(mpi_sfml mpi_sfml.cpp)

target_link_libraries(mpi_sfml fmt::fmt sfml::sfml MPI::MPI_CXX)

add_executable(mpi_sfml2 mpi_sfml2.cpp)

target_link_libraries(mpi_sfml2 fmt::fmt sfml::sfml MPI::MPI_CXX)

add_executable(grises mpi_sfml3.cpp)

target_link_libraries(grises fmt::fmt sfml::sfml MPI::MPI_CXX)

add_executable(suma mpi_sfml4.cpp)

target_link_libraries(suma fmt::fmt sfml::sfml MPI::MPI_CXX)

add_executable(multiplicar mpi_sfml5.cpp)

target_link_libraries(multiplicar fmt::fmt sfml::sfml MPI::MPI_CXX)

add_executable(resta mpi_sfml6.cpp)

target_link_libraries(resta fmt::fmt sfml::sfml MPI::MPI_CXX)

add_executable(espejo mpi_sfml7.cpp)

target_link_libraries(espejo fmt::fmt sfml::sfml MPI::MPI_CXX)

add_executable(medianaminmax mpi_sfml8.cpp)

target_link_libraries(medianaminmax fmt::fmt sfml::sfml MPI::MPI_CXX)

add_executable(bordes mpi_sfml9.cpp)

target_link_libraries(bordes fmt::fmt sfml::sfml MPI::MPI_CXX)