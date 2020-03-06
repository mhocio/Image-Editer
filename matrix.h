#ifndef MATRIX_H
#define MATRIX_H

#include <cstdio>
#include <initializer_list>
#include <stdexcept>
#include <iostream>

class Matrix {
public:
    size_t sizeX;
    size_t sizeY;

    Matrix(){}

    Matrix(int sizeX, int sizeY) {
        this->sizeX = sizeX;
        this->sizeY = sizeY;

        M = new int[sizeX * sizeY];
    }

    Matrix( const Matrix &obj) {
            sizeX = obj.sizeX;
            sizeY = obj.sizeY;
            M = new int[sizeX * sizeY];

            for (size_t i = 0; i< sizeX; i++)
                for (size_t j = 0; j < sizeY; j++)
                    M[ index( i, j )] = obj.M[ index( i, j )];
        }
        Matrix& operator=(const Matrix& other) {
            delete [] M;
            sizeX = other.sizeX;
            sizeY = other.sizeY;

            M = new int[sizeX * sizeY];

            for (size_t i = 0; i < sizeX; i++)
                for (size_t j = 0; j < sizeY; j++)
                    M[ index( i, j )] = other.M[ index( i, j )];

            return *this;
        }

        ~Matrix() {
            delete [] M;
        }

    void fill(const std::initializer_list<int> &il)
    {
        if (il.size() > sizeX*sizeY)
            throw new std::out_of_range("Too many arguments during initialization");
        int i = 0;
        for (auto elem: il) {
            M[i++] = elem;
        }
    }

    void print() {
        for (size_t i = 0; i< sizeX; i++) {
            for (size_t j = 0; j < sizeY; j++) {
                std::cout << M[ index( i, j )] << " ";
            }
            std::cout << "\n";
        }
    }

    int at(int x, int y) const
        {
            return M[y + sizeY * x];
        }

    size_t index( int x, int y ) const { return y + sizeY * x; }

private:
    int* M;
};


#endif // MATRIX_H
