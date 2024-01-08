#ifndef MAMATRIX_H
#define MAMATRIX_H


// Matrix includes
#include <iostream>
#include <vector>
namespace monoAtomic{

template <typename T> class maMatrix{
    public:
        maMatrix();
        maMatrix(uint32_t nRows, uint32_t nColumns);
        void resize(uint32_t nRows, uint32_t nColumns);
        uint32_t nRows();
        uint32_t nColumns();

    private:
        std::vector< std::vector<T> > m_data;

    };


    template <typename T> maMatrix<T>::maMatrix(){
        maMatrix(0,0);
    }

    template <typename T> maMatrix<T>::maMatrix(uint32_t nRows, uint32_t nColumns){
        resize(nRows, nColumns);
    }

    template <typename T> void maMatrix<T>::resize(uint32_t nRows, uint32_t nColumns){
        m_data.resize(nColumns);
        for( std::vector<T> & col : m_data){
            col.resize(nRows);
        }
    }

    template <typename T> uint32_t maMatrix<T>::nRows(){
        if(m_data.empty())
            return 0;
        return m_data.at(0).size();
    }

    template <typename T> uint32_t maMatrix<T>::nColumns(){
        return m_data.size();
    }

} // end namespace

#endif


