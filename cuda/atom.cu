#if __CUDA_ARCH__ < 600
  #define atomicMin_block(X,Y) atomicMin(X,Y)
  #define atomicAdd_block(X,Y) atomicAdd(X,Y)
#endif


__global__
void glo(int * x, int * y) {
   atomicAdd(x+3,1);
}

__global__
void blo(int * x, int *  y) {
   atomicAdd_block(x+3,1);
}

__global__
void sha(int * x, int *  y) {
   __shared__ int c[1024];
   atomicAdd(c+3,1);
   x[3] = c[3];
}


__global__
void shablo(int * x, int *  y) {
   __shared__ int c[1024];
   atomicAdd_block(c+3,1);
   x[3] = c[3];
}

