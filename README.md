
# Parallel Image Processing with OpenMPI

## Features
- Parallel image compression using OpenMPI and OpenCV.

## Build Instructions
1. Install dependencies: OpenMPI, OpenCV, and CMake.
2. Compile the project:
   ```bash
   make
   ```

## Run Instructions
Run the compiled program with MPI:
```bash
mpirun -np <num_processes> ./build/main <input_image> <output_image>
```
