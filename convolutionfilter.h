#ifndef CONVOLUTIONFILTER_H
#define CONVOLUTIONFILTER_H

class ConvolutionFilter
{
public:
    int width;
    int height;

    int* filter;

    int anchor_x;
    int anchor_y;

    ConvolutionFilter(int width, int height, int* filter, int anchor_x, int anchor_y);
};

#endif // CONVOLUTIONFILTER_H
