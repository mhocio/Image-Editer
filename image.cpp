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
    uchar* bitsEnd = bits + image.byteCount();

    while(bits < bitsEnd) {
        *bits = filter.function(*bits);
        bits++;
    }

    _QPixmap = QPixmap::fromImage(image);
}

void minmax(uchar* value, int add){ // normalizacja wartości by nie przekraczała zakresu 0 - 255
    *value = (int)*value + add < 255 ? ((int)*value + add > 0 ? *value + add : 0) : 255;
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
    int imageSize = image.byteCount();

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
                    weight += filter.filter[i][j];

                    //if (fitsInBounds(actual_x + x, actual_y + y, w, h)) {
                    if (0 < iter +channel + 4*x + 4*y*w < imageSize)
                        newValue += filter.filter[i][j] * *(bits+iter +channel + 4*x + 4*y*w);
                    else
                        newValue += *(bits+iter+channel);
                }

            *(newBits+iter+channel) = (int)(newValue / weight);
        }
    }

    _QPixmap = QPixmap::fromImage(newImage);
}
