#include "convolutionfilter.h"

ConvolutionFilter::ConvolutionFilter(int width, int height, int* filter, int anchor_x, int anchor_y, int divisor, int offset)
{
    this->height = height;
    this->width = width;
    this->filter = filter;
    this->anchor_x = anchor_x;
    this->anchor_y = anchor_y;
    this->divisor = divisor;
    this->offset = offset;
}
