// SCRATCH : TODO PROPPER TEST
//  clang++ -std=c++20 -Xclang -fopenmp -lomp -I"$(brew --prefix libomp)/include" -L"$(brew --prefix libomp)/lib"  -o test_maMatrix test_maMatrix.cpp
#include "../include/monoAtomic/maMatrix.h"
#include <omp.h>
#include <sys/time.h> 
#include <iostream>

int main()
{


    // unsync the I/O of C and C++. 
    std::ios_base::sync_with_stdio(false);

    monoAtomic::maMatrix<float> m_matrix;
    
    int NROWS = 500;
    int NCOLS = 32;
    int NITER=100000;
    float norm =1.0;

    std::vector<float> ins(NROWS);
    std::vector<float> outs(NCOLS);
    
    m_matrix.resize(NROWS, NCOLS);


     struct timeval start, end; 
  
    // start timer. 
    gettimeofday(&start, NULL); 
 
    for(int i=0; i< NITER; i++){
        m_matrix.mult(ins.data(), outs.data(), norm);
    }

     gettimeofday(&end, NULL); 
    
    double time_taken;

    time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
    time_taken = (time_taken + (end.tv_usec -  
                              start.tv_usec)) * 1e-6; 
  
    std::cout << "Time taken for"<< NITER << " iterations is : " << std::fixed 
         << time_taken << std::setprecision(10); 
    std::cout << " sec" << std::endl;

}

    // qDebug() << "Before" << m_test.nRows() << "x" << m_test.nColumns();
    // m_test.resize(3,4);
    // qDebug() << "After" << m_test.nRows() << "x" << m_test.nColumns();

    // qDebug() << "before" << m_test(1,1);
    // m_test.set(1,1, .1234);
    // qDebug() << "after" << m_test(1,1);
    // m_test.resize(6,3);
    // qDebug() << "after resize" << m_test(1,1);
    // m_test.set(0,0, 1.1);
    // m_test.set(1,1, 2.2);
    // m_test.set(4, 2, 3.3);
    // m_test.print();
    // qDebug() << m_test.values();
    // std::vector<float> ins = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
    // std::vector<float> outs(3);
    // m_test.mult(&ins, &outs);
    // qDebug() << outs;
