#include "image.h"

Image::Image(QPixmap map)
{
    _QPixmap = map;
}

Image::Image()
{
}

void Image::ApplyFunctionalFilter(FunctionalFilter& filter)
{
    QImage image = _QPixmap.toImage().convertToFormat(QImage::Format_BGR888);
    uchar* bits = image.bits();
    uchar* bitsEnd = bits + image.sizeInBytes();

    while(bits < bitsEnd) {
        *bits = filter.function(*bits);
        bits++;
    }

    _QPixmap = QPixmap::fromImage(image);
}

bool fitsInBounds(int x, int y, int width, int height) {
    if (x < 0 || y < 0)
        return false;
    if (x >= width || y >= height)
        return false;
    return true;
}

void Image::ApplyConvolutionFilter(ConvolutionFilter& filter)
{
    QImage image = _QPixmap.toImage();
    uchar* bits = image.bits();

    QImage newImage = _QPixmap.toImage();
    uchar* newBits = newImage.bits();

    int noChannels = 3;
    int w = image.width();
    int imageSize = image.sizeInBytes();

    // iterate through all bits(RGBA)
    for (int iter = 0; iter < imageSize; iter += noChannels+1) {
        // iterate RGB channels skipping A
        for (int channel = 0; channel < noChannels; channel++) {
            int weight = 0;
            int newValue = 0;

            for (int i = 0; i < filter.width; i++)
                for (int j = 0; j < filter.height; j++) {

                    int x = i - filter.anchor_x;
                    int y = j - filter.anchor_y;
                    //int actual_x = i % w;
                    //int actual_y = i / w;
                    weight += filter.filter[i * filter.width + j];

                    if (0 < iter +channel + 4*x + 4*y*w < imageSize)
                        newValue += filter.filter[i * filter.width + j] * *(bits+iter +channel + 4*x + 4*y*w);
                    else
                        newValue += *(bits+iter+channel);
                }
            if (weight == 0)
                weight = 1;

            *(newBits+iter+channel) = qBound(0, (int)(newValue / weight), 255);
        }
    }

    _QPixmap = QPixmap::fromImage(newImage);
}
