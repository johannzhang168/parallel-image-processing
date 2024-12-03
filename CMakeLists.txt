
cmake_minimum_required(VERSION 3.10)
project(ParallelImageProcessing)

find_package(OpenCV REQUIRED)
find_package(MPI REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${MPI_INCLUDE_PATH} include)

add_executable(main src/main.cpp src/image_processing.cpp)
target_link_libraries(main ${OpenCV_LIBS} ${MPI_LIBRARIES})
