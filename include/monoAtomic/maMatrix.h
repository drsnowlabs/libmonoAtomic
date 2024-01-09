#ifndef MAMATRIX_H
#define MAMATRIX_H


// Matrix includes
#include <iostream>
#include <vector>
namespace monoAtomic{

template <typename T> class maMatrix{
    public:
        maMatrix(){
            maMatrix(0,0);
        }

        maMatrix(uint32_t nRows, uint32_t nColumns){
            resize(nRows, nColumns);
        }

        void resize(uint32_t nRows, uint32_t nColumns){
            m_data.resize(nColumns);
            for( std::vector<T> & col : m_data){
                col.resize(nRows);
            }
        }

        uint32_t nRows(){
            if(m_data.empty())
                return 0;
            return m_data.at(0).size();
        }

        uint32_t nColumns(){
            return m_data.size();
        }

        size_t count(){
            return nRows() * nColumns();
        }

        void set(uint32_t row, uint32_t col, T val) {
            m_data.at(col).at(row) = val;
        }

        T operator()(uint32_t row, uint32_t col){
            return m_data.at(col).at(row);
        }

        void print(){
            uint32_t rows = nRows();
            uint32_t cols = nColumns();

            for(uint32_t r=0; r<rows; r++){
                for(uint32_t c=0; c<cols; c++) {
                    std::cout << m_data.at(c).at(r) << " ";
                }
                std::cout<< std::endl;
            }
        }

    private:
        std::vector< std::vector<T> > m_data;
    };

} // end namespace

#endif


