#include <iostream>
#include <cuda_runtime.h>

using namespace std;

__global__ 
void vectorAddition(float* a, float* b, float* c, int size) {
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if (index < size) {
        c[index] = b[index] + a[index];
    }
}


int main() {
    int size = 1 << 20;
    float *a, *b, *c;

    // Allocate Unified Memory -- accessible from CPU or GPU
    // Look more details in https://devblogs.nvidia.com/unified-memory-cuda-beginners/
    cudaMallocManaged(&a, size * sizeof(float));
    cudaMallocManaged(&b, size * sizeof(float));
    cudaMallocManaged(&c, size * sizeof(float));

    // Initialize input vectors
    for (int i = 0; i < size; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    // Set up grid and block dimensions
    int threadsPerBlock = 256;
    int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;

    // Launch the kernel on the GPU
    vectorAddition<<<blocksPerGrid, threadsPerBlock>>>(a, b, c, size);

    // Wait for GPU to finish before accessing on host
    cudaDeviceSynchronize();

    // Print the result vector
    //for (int i = 0; i < size; i++) {
        ///std::cout << a[i] << " + " << b[i] << " = " << c[i] << std::endl;
    //}

    // No need to call cudaFree for a, b, c

    return 0;
}
