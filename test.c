#include <stdio.h>
#include <omp.h>

int main() {
    #ifdef _OPENMP
        printf("OpenMP is enabled!\n");
    #else
        printf("OpenMP is NOT enabled!\n");
    #endif
    return 0;
}
