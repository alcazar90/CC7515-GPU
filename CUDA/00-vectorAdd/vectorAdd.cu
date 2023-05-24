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
    int size = 20;
    float* a = new float[size];
    float* b = new float[size];
    float* c = new float[size];

    // Initialize input vectors
    for (int i =0; i < size; i++) {
        a[i] = i;
        b[i] = i * 2;
    }
    // Allocate memory
    float* d_a, * d_b, * d_c;
    cudaMalloc((void **) &d_a, size * sizeof(float));
    cudaMalloc((void **) &d_b, size * sizeof(float));
    cudaMalloc((void **) &d_c, size * sizeof(float));

    // Copy input vectors from host memory to GPU buffers
    cudaMemcpy(d_a, a, size * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size * sizeof(float), cudaMemcpyHostToDevice);

    // Set up grid and block dimensions
    int threadsPerBlock = 256;
    int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;

    // Launch the kernel on the GPU
    vectorAddition<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, size);

    // Copy the result vector from device to host
    cudaMemcpy(c, d_c, size * sizeof(float), cudaMemcpyDeviceToHost);

    // Print the result vector
    for (int i = 0; i < size; i++) {
        std::cout << a[i] << " + " << b[i] << " = " << c[i] << std::endl;
    }

    // Free memory on the device
    cudaFree(d_a); 
    cudaFree(d_b); 
    cudaFree(d_c);

    // Free memoruy on the host
    delete[] a; 
    delete[] b; 
    delete[] c;

    return 0;
}
