#ifndef IMAGE_H
#define IMAGE_H

#include <QPixmap>
#include <functionalfilter.h>
#include <convolutionfilter.h>

class Image
{
public:
    Image(QPixmap map);
    Image();
    void ApplyFunctionalFilter(FunctionalFilter& filter);
    void ApplyConvolutionFilter(ConvolutionFilter& filter);
    void ApplyConvolutionFilter2(ConvolutionFilter& filter);
    QPixmap _QPixmap;
};

#endif // IMAGE_H
