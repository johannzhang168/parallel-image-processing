
#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

void parallel_image_compression(const char* input, const char* output, int rank, int size);
void parallel_image_rotation(const char* input, const char* output, int rank, int size);

#endif
