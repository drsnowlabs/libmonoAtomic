// verifying that OMP environment actually works
// compile with: clang++ -Xclang -fopenmp -lomp -I"$(brew --prefix libomp)/include" -L"$(brew --prefix libomp)/lib"  -o test_openmp test_openmp.cpp



#include <stdio.h>
#include <omp.h>

int main()
{
    #pragma omp parallel 
    {
        int ID = 0;
        printf("Hello(%d)", ID);
        printf("World(%d)\n", ID);
}}