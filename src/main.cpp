
#include <iostream>
#include <mpi.h>
#include "image_processing.h"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 3) {
        if (rank == 0) {
            std::cerr << "Usage: " << argv[0] << " <input_image> <output_image>" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    const char* input_image = argv[1];
    const char* output_image = argv[2];

    if (rank == 0) {
        std::cout << "Parallel Image Processing with OpenMPI" << std::endl;
    }
    std::string input;
    
    if (rank == 0) {
        std::cout << "Enter whether you want to compress or rotate: ";
        std::cin >> input;
        
        while (input != "compress" && input != "rotate") {
            std::cout << "Invalid input. Enter either 'compress' or 'rotate': ";
            std::cin >> input;
        }
    }

    int input_size = input.size() + 1;
    MPI_Bcast(&input_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    char* operation = new char[input_size];
    if (rank == 0) {
        std::copy(input.begin(), input.end(), operation);
    }
    MPI_Bcast(operation, input_size, MPI_CHAR, 0, MPI_COMM_WORLD);

    double start_time = MPI_Wtime();
    
    if (std::string(operation) == "compress") {
        parallel_image_compression(input_image, output_image, rank, size);
    }
    else if (std::string(operation) == "rotate") {
        parallel_image_rotation(input_image, output_image, rank, size);
    }

    double end_time = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Time taken: " << (end_time - start_time) << " seconds" << std::endl;
    }

    delete[] operation;
    

    MPI_Finalize();
    return 0;
}
