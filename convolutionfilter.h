#ifndef CONVOLUTIONFILTER_H
#define CONVOLUTIONFILTER_H

#include <cstdio>

class Matrix {
    int sizeX;
    int sizeY;

    int* M;

    Matrix(int x, int y);
    ~Matrix();
public:
    size_t index( int x, int y ) const { return x + sizeX * y; }
private:
};

Matrix::Matrix(int sizeX, int sizeY) {
    this->sizeX = sizeX;
    this->sizeY = sizeY;

    M = new int[sizeX * sizeY];
}

Matrix::~Matrix() {
    delete [] M;
}

class ConvolutionFilter
{
public:
    int width;
    int height;

    int** filter;

    int anchor_x;
    int anchor_y;

    ConvolutionFilter(int width, int height, int** filter, int anchor_x, int anchor_y);
};

#endif // CONVOLUTIONFILTER_H
