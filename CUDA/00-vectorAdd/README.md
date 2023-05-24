## CUDA on C++: Basic Concepts


### What Is CUDA?

* CUDA Architecture:
  * Expose GPU parallelism for general-purpose computing
  * Expose/Enable performance

* CUDA C++
  * Based on industry-standard C++
  * Set of extensions to enable heterogeneous programming
  * Straightforward APIs to manage devices, memory, etc.


### GPU Kernels: Device Code

```c++
__global__ void myKernel(void) {
    // running in paralel multiple times to do something
}
```

CUDA C++ keyword `__global__` indicates a function that:

* Runs on the device
* Is called from host code (can also be called from other device code)

`nvcc` separaters source code into host and device components:

* Device functions (e.g. `myKernel()`) processed by NVIDIA compiler
* Host functions (e.g. `main()`) processed by standard host compiler (e.g. gcc)

```c++
myKernel<<<1, 1>>>();
```

Triple angle brackets amrk a call to device code.

* Also called a "kernel launch"
* We'll return to the parameters (1,1) in a moment
* The parameters inside the triple angle brackets are the CUDA kernel **execution configuration**

### Running Code in Parallel

GPU computing is about massive parallelism. So how do we run code in parallel on the device, for example adding one vector to another?

Instead of executing `add()` once,
```c++
add<<< 1, 1 >>>();
```
execute `N` times in parallel
```c++
add<<< N, 1 >>>();
```

**Multiple execution** of the same function (i.e. `add()` or any kernel function) they **have to be working together in concert to solve a problem** instead of doing the same thing redundantly.

This is why under the Flynn's taxonomy the GPU is known as single instruction multiple data streams (SIMD) computer arquitectures.

### Vector Addition On The Device

How can we write a CUDA kernel that can add two vector arrays?

* with `add()` running in parallel we can do vector addition
* Terminology: each parallel invocation of `add()` is referred to as a _block_
* Each invocation can refer to its block index using `blockIdx.x`

```c++
__global__ void add(int *a, int *b) {
    b[blockIdx.x] = b[blockIdx.x] + a[blockIdx.x];
}
```
By using `blockIdx.x` to index into the array, each block handles a different index (avoid redundancy).  In some sense, the kernel function is an implicit for-loop in where the body of the function is running many times over a particular index block  (`blockIdx.x`).


Built-in variables like `blockIdx.x` are zero-indexed (C/C++ style), `0, ..., N-1`, where `N` is from the kernel execution configuration indicated at the kernel launch.

```c++
int main() {
    int *a, int *b;
    int size = N * sizeof(int);
    // Allocate memory
    cudaMallocManaged((void **) &a, size);
    cudaMallocManaged((void **) &b, size);
    // Set up input values (randomly)
    random_ints(a, N);
    random_ints(b, N);
    // Launch add() kernel on GPU with N blocks
    add<<<N, 1>>>(a, b);
    // Cleanup
    cudaFree(a); cudaFree(b); cudaGree(c);
    return 0;
}
```

### CUDA Threads

* Terminology: a block can be split into parallel `threads`
* Let's change `add()` to use parallel threads instead of parallel blocks

```c++
__global__ void add(int *a, int *b) {
    b[threadIdx.x] = b[threadIdx.x] + a[threadIdx.x];
}
```

* We use `threadIdx.x` instead of `blockIdx.x`
* Need to make one change in `main() ->  add<<< 1, N>>>();`

We've seen parallel vector addition using:

* Many blocks with one thread each
* One block with many threads

Let's adapt vetor addition to use both _blocks_ and _threads_. Combining these two we will get a hierarchical parallelism for doing operations such as vector addition.

### Indexing Arrays with Block and Threads

No longer as simple as using `blockIdx.x` and `threadsIdx.x`.

With M threads/block a **unique index for each thread** is given by:

![](../assets/cuda_unique_index.png)

```c++
int index = threadIdx.x + blockIdx.x * M;

          =      3      +     2      * 8
          =            19
```

### Vector Addition with Blocks and Threads

Use the built-in variable `blockDim.x` (i.e. M) for threads per block.

```c++
__global__ void add(int *a, int *b) {
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    b[index] = b[index] + a[index];
}
```

What changes need to be made in `main()`?

```c++
int main() {
    int *a, *b;
    int size = N * sizeof(int);
    // Allocate memory
    cudaMallocManaged((void **) &a, size);
    cudaMallocManaged((void **) &b, size);
    // Set up input values
    random_ints(a, N);
    random_ints(b, N);
    // Launch add() kernel on GPU with N blocks (DIFFERENT)
    add<<<N / THREADS_PER_BLOCK, THREARDS_PER_BLOCK>>>(a, b);
    // Cleanup
    cudaFree(a); cudaFree(b); cudaFree(c);
    return 0;
```

### Handling Arbitrary Vector Sizes 

Typical problems are not friendly multiples of `blockDim.x`. Avoid accessing beyond the end of the arrays:

```c++
__global__ void add(int *a, int *b, int n) {
    int index = theadIdx.x + blockIdx.x * blockDim.x;
    if (index < n) {
        b[index] = b[index] + a[index];
    }
}
```

* `n` is the number of elements in the array.
 
Update the kernel launch:

```c++
add<<<(N + M - 1) / M, M>>>(a, b, N);
```

### Why Do We Have Hierarchical Parallelism?

Threads seem unnecessary:

* They add a level of complexity
* What do we again?

Unlike parallel blocks, threads have mechanisms to:

* Communicate
* Synchronize

Two level hierarchy maps more appropriately to GPU hardware design.


### Compilation

CUDA C++ files use the `.cu` file extension, and they need to be compiled by `nvcc`, the NVIDIA CUDA compiler, which understand both regular C++ and CUDA extensions.

```bash
nvcc -o vectorAdd vectorAdd.cu
```

Once the code is compiled, it's possible to profile the code execution
using `nvprof ./vectorAdd`:

```bash
==65131== NVPROF is profiling process 65131, command: ./vectorAdd
==65131== Profiling application: ./vectorAdd
==65131== Profiling result:
            Type  Time(%)      Time     Calls       Avg       Min       Max  Name
 GPU activities:   43.09%  1.6960us         1  1.6960us  1.6960us  1.6960us  vectorAddition(float*, float*, float*, int)
                   35.77%  1.4080us         2     704ns     608ns     800ns  [CUDA memcpy HtoD]
                   21.14%     832ns         1     832ns     832ns     832ns  [CUDA memcpy DtoH]
      API calls:   99.91%  438.47ms         3  146.16ms  2.2710us  438.47ms  cudaMalloc
                    0.05%  203.59us        97  2.0980us     185ns  88.886us  cuDeviceGetAttribute
                    0.02%  99.646us         3  33.215us  2.9280us  90.694us  cudaFree
                    0.01%  32.600us         3  10.866us  3.7220us  17.859us  cudaMemcpy
                    0.01%  27.571us         1  27.571us  27.571us  27.571us  cuDeviceGetName
                    0.01%  25.786us         1  25.786us  25.786us  25.786us  cudaLaunchKernel
                    0.00%  8.6570us         1  8.6570us  8.6570us  8.6570us  cuDeviceGetPCIBusId
                    0.00%  2.7880us         3     929ns     186ns  2.2600us  cuDeviceGetCount
                    0.00%  1.1660us         2     583ns     210ns     956ns  cuDeviceGet
                    0.00%     481ns         1     481ns     481ns     481ns  cuDeviceTotalMem
```

## Allocate Unified Memory: `cudaMallocManaged`

Unified Memory creates a pool of managed memory that is shared between the CPU and GPU. Managed memory is accessible to both the CPU and GPU, therefore defined your data in this share pool allow us to simplify code.

For using unified memory it's neccesary to use the `cudaMallocManaged`
command.

Check the difference between `vectorAdd.cu` and `vectorAddUniMem.cu`
programs:


```c++
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
```

Instead, `vectorAddUniMem.cu` used `cudaMallocManaged` avoiding the boilerplat code to move data from host-to-device and viceversa. In addition, it's not necessary to call `cudaFree`.


```c++
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

    // No need to call cudaFree for a, b, c

    return 0;
```

## References

* NVIDIA slides: https://www.nvidia.com/docs/IO/116711/sc11-cuda-c-basics.pdf
* [Unified Memory in CUDA 6](https://developer.nvidia.com/blog/unified-memory-in-cuda-6/)
* [An Even Easier Introduction to CUDA](https://developer.nvidia.com/blog/unified-memory-in-cuda-6/)
* https://www.youtube.com/watch?v=ymsUskSjIAg