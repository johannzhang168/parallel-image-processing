
#include <opencv2/opencv.hpp>
#include <mpi.h>
#include <iostream>
#include "image_processing.h"

using namespace cv;
using namespace std;

void parallel_image_compression(const char* input, const char* output, int rank, int size) {
    if (rank == 0) {
        cout << "Starting image compression..." << endl;
    }

    Mat image;
    if (rank == 0) {
        image = imread(input, IMREAD_GRAYSCALE);
        if (image.empty()) {
            cerr << "Error loading image!" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    int rows = 0, cols = 0;
    if (rank == 0) {
        rows = image.rows;
        cols = image.cols;
    }
    MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);

    vector<uchar> image_data;
    if (rank == 0) {
        image_data.assign(image.datastart, image.dataend);
    }
    image_data.resize(rows * cols);
    MPI_Bcast(image_data.data(), rows * cols, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    int block_size = rows / size;
    int start_row = rank * block_size;
    int end_row = (rank == size - 1) ? rows : start_row + block_size;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < cols; j++) {
            image_data[i * cols + j] = image_data[i * cols + j] / 2;
        }
    }

    vector<uchar> compressed_data(rows * cols);
    MPI_Gather(image_data.data() + start_row * cols, block_size * cols, MPI_UNSIGNED_CHAR,
               compressed_data.data(), block_size * cols, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        Mat compressed_image(rows, cols, CV_8UC1, compressed_data.data());
        imwrite(output, compressed_image);
        cout << "Image compression completed." << endl;
    }
}



void parallel_image_rotation(const char* input, const char* output, int rank, int size) {
    if (rank == 0) {
        cout << "Starting image rotation..." << endl;
    }

    Mat image;
    if (rank == 0) {
        image = imread(input, IMREAD_GRAYSCALE);
        if (image.empty()) {
            cerr << "Error loading image!" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }


    int rows = 0, cols = 0;
    if (rank == 0) {
        rows = image.rows;
        cols = image.cols;
    }
    MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);


    vector<uchar> image_data;
    if (rank == 0) {
        image_data.assign(image.datastart, image.dataend);
    }
    image_data.resize(rows * cols);
    MPI_Bcast(image_data.data(), rows * cols, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);


    int block_size = cols / size;
    int start_col = rank * block_size;
    int end_col = (rank == size - 1) ? cols : start_col + block_size;


    vector<uchar> rotated_data(rows * cols, 0);


    for (int i = 0; i < rows; i++) {
        for (int j = start_col; j < end_col; j++) {
            int rotated_row = j;
            int rotated_col = rows - i - 1;
            rotated_data[rotated_row * rows + rotated_col] = image_data[i * cols + j];
        }
    }

    vector<uchar> final_data(rows * cols, 0);
    MPI_Reduce(rotated_data.data(), final_data.data(), rows * cols, MPI_UNSIGNED_CHAR, MPI_SUM, 0, MPI_COMM_WORLD);


    if (rank == 0) {
        Mat rotated_image(cols, rows, CV_8UC1, final_data.data());
        imwrite(output, rotated_image);
        cout << "Image rotation completed." << endl;
    }
}

