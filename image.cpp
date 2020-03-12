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

    int weight = 0;
    for (int i = 0; i < filter.width; i++)
        for (int j = 0; j < filter.height; j++)
            weight += filter.filter[i * filter.width + j];

    if (weight <= 0)
        weight = 1;

    // iterate through all bits(RGBA)
    for (int iter = 0; iter < imageSize; iter += noChannels+1) {
        // iterate RGB channels skipping A
        for (int channel = 0; channel < noChannels; channel++) {
            int newValue = 0;

            for (int i = 0; i < filter.width; i++)
                for (int j = 0; j < filter.height; j++) {

                    int x = i - filter.anchor_x;
                    int y = j - filter.anchor_y;
                    //int actual_x = i % w;
                    //int actual_y = i / w;

                    int nextI = iter +channel + 4*x + 4*y*w;

                    if ((0 < nextI) && (nextI < imageSize))
                        newValue += filter.filter[i * filter.width + j] * *(bits + nextI);
                    else
                        newValue += *(bits+iter+channel);
                }

            *(newBits+iter+channel) = qBound(0, (int)((newValue / filter.divisor) + filter.offset), 255);
        }
    }

    _QPixmap = QPixmap::fromImage(newImage);
}

void Image::ApplyMedianFilter(ConvolutionFilter& filter)
{
    QImage image = _QPixmap.toImage().convertToFormat(QImage::Format_RGBA8888);
    uchar* bits = image.bits();

    QImage newImage = _QPixmap.toImage().convertToFormat(QImage::Format_RGBA8888);;
    uchar* newBits = newImage.bits();

    int noChannels = 3;
    int w = image.width();
    int imageSize = image.sizeInBytes();

    //int iter = 0 + 4 * 2300;
    //iterate through all bits(RGBA)
    for (int iter = 0; iter < imageSize; iter += noChannels+1) {

        std::vector<std::pair<double, std::pair<int,int>>> grayValues;

        for (int i = 0; i < filter.width; i++) {
            for (int j = 0; j < filter.height; j++) {

                int x = i - filter.anchor_x;
                int y = j - filter.anchor_y;
                int nextI = iter + 4*x + 4*y*w;

                double grayPixel;

                if ((0 <= nextI) && (nextI < imageSize)) {
                    grayPixel = 0.3* *(bits + nextI) + 0.6* *(bits + nextI + 1) + 0.1* *(bits + nextI + 2);
                    grayValues.push_back(std::make_pair(grayPixel, std::make_pair(i, j)));
                }
            }
        }

        if (!grayValues.size())
            continue;

        sort(grayValues.begin(), grayValues.end());

        int middle = (grayValues.size())/2;

        int x = grayValues[middle].second.first;
        int y = grayValues[middle].second.second;

        int nextI = 4*x + 4*y*w;

        *(newBits + iter) = *(bits + iter + nextI);
        *(newBits + iter + 1) = *(bits + iter + nextI + 1);
        *(newBits + iter + 2) = *(bits + iter + nextI + 2);
    }

    _QPixmap = QPixmap::fromImage(newImage);
}
