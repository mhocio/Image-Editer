#include "photoapp.h"
#include "ui_photoapp.h"
#include "functionalfilter.h"
#include "graphicsscene.h"

PhotoApp::PhotoApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PhotoApp)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->horizontalFrame);

    sceneUserFilter = new GraphicsScene(this);
    ui->graphicsViewFilter->setScene(sceneUserFilter);

    addFunctionFilters();
}

void PhotoApp::addFunctionFilters()
{
    QString filterName = "Inverse";
    ui->FiltersComboBox->addItem(filterName);
    std::vector<std::pair<int,int> > points;
    points.push_back(std::make_pair(0, 0));
    points.push_back(std::make_pair(255, 255));
    std::pair<QString, std::vector<std::pair<int,int>>> elem = std::make_pair(filterName, points);
    filtersDict.insert(elem);

    filterName = "Brightness up";
    ui->FiltersComboBox->addItem(filterName);
    points.clear();
    points.push_back(std::make_pair(0, 200));
    points.push_back(std::make_pair(200, 0));
    points.push_back(std::make_pair(255, 0));
    elem = std::make_pair(filterName, points);
    filtersDict.insert(elem);

    filterName = "Brightness down";
    ui->FiltersComboBox->addItem(filterName);
    points.clear();
    points.push_back(std::make_pair(0, 255));
    points.push_back(std::make_pair(56, 255));
    points.push_back(std::make_pair(255, 56));
    elem = std::make_pair(filterName, points);
    filtersDict.insert(elem);

    filterName = "Contrast up";
    ui->FiltersComboBox->addItem(filterName);
    points.clear();
    points.push_back(std::make_pair(0, 255));
    points.push_back(std::make_pair(56, 255));
    points.push_back(std::make_pair(255-56, 0));
    points.push_back(std::make_pair(255, 0));
    elem = std::make_pair(filterName, points);
    filtersDict.insert(elem);
}

PhotoApp::~PhotoApp()
{
    delete ui;
}

//opens a new Image
void PhotoApp::on_actionOpen_triggered()
{
    originalPhotoName = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (QString::compare(originalPhotoName, QString()) != 0) {
        QImage image;

        if (image.load(originalPhotoName) == 0)
            QMessageBox::warning(this, "Warning", "Cannot open file");

        //image = image.scaled(ui->originalPhoto_label->width(), ui->originalPhoto_label->height(), Qt::KeepAspectRatio);

        currentQPixmap = originalQPixmap = QPixmap::fromImage(image);

        currentImage = Image(currentQPixmap);
        //ui->originalPhoto_label->setPixmap(currentQPixmap.scaled(ui->originalPhoto_label->width(), ui->originalPhoto_label->height(),  Qt::KeepAspectRatioByExpanding));
        //ui->changedPhoto_label->setPixmap(currentQPixmap.scaled(ui->originalPhoto_label->width(), ui->originalPhoto_label->height(),  Qt::KeepAspectRatioByExpanding));
        ui->originalPhoto_label->setPixmap(currentQPixmap);
        ui->changedPhoto_label->setPixmap(currentQPixmap);
    }
}

void PhotoApp::on_resetImageButton_clicked()
{
    currentImage._QPixmap = originalQPixmap;
    //ui->changedPhoto_label->setPixmap(originalQPixmap.scaled(ui->originalPhoto_label->width(), ui->originalPhoto_label->height(), Qt::KeepAspectRatio));
    this->updateChangedPhoto();
}

void PhotoApp::updateChangedPhoto() {
    ui->changedPhoto_label->setPixmap(currentImage._QPixmap.scaled(ui->originalPhoto_label->width(), ui->originalPhoto_label->height(), Qt::KeepAspectRatio));
}

void PhotoApp::on_inverseFilterButton_clicked()
{
    InverseFilter filter;
    currentImage.ApplyFunctionalFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_brightnessCorrectionButton_clicked()
{
    BrightnessCorrectionFilter filter = BrightnessCorrectionFilter(ui->horizontalSliderBrightness->value());
    currentImage.ApplyFunctionalFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_gammaCorrectionButton_clicked()
{
    GammaCorrectionFilter filter = GammaCorrectionFilter(ui->horizontalSliderGamma->value());
    currentImage.ApplyFunctionalFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_constrastFilterButton_clicked()
{
    ContrastFilter filter = ContrastFilter(ui->horizontalSliderContrast->value());
    currentImage.ApplyFunctionalFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_convolutionButton_clicked()
{
    int f[9] = {1,1,1,
                1,1,1,
                1,1,1,};

    ConvolutionFilter filter(3, 3, f, 1, 1, 9, 0);
    currentImage.ApplyConvolutionFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_sharpenFilterButton_clicked()
{
    int f[9] = {-1,-1,-1,
                -1, 9,-1,
                -1,-1,-1};

    ConvolutionFilter filter(3, 3, f, 1, 1, 1, 0);
    currentImage.ApplyConvolutionFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_edgeDetectionFilterButton_clicked()
{
    int f[9] = {0, -1, 0,
                0,  1, 0,
                0,  0, 0};

    ConvolutionFilter filter(3, 3, f, 1, 1, 1, 127);
    currentImage.ApplyConvolutionFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_gaussianBlurFilterButton_clicked()
{
    int f[9] = {0, 1, 0,
                1, 4, 1,
                0, 1, 0};

    ConvolutionFilter filter(3, 3, f, 1, 1, 8, 0);
    currentImage.ApplyConvolutionFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_embossFilterButton_clicked()
{
    int f[9] = {-1,-1,-1,
                 0, 1, 0,
                 1, 1, 1};

    ConvolutionFilter filter(3, 3, f, 1, 1, 1, 0);
    currentImage.ApplyConvolutionFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_actionExport_triggered()
{
    QString exportImageName = QFileDialog::getSaveFileName(this, tr("Export Image"), "", tr("Images (*.jpg"));
    if (exportImageName != "")
        currentImage._QPixmap.toImage().save(exportImageName);
}

void PhotoApp::on_widget_customContextMenuRequested(const QPoint &pos)
{

}

void PhotoApp::on_resetUserFunction_clicked()
{
    sceneUserFilter->removeAllPoints();
}

int GetY(std::pair<int,int> A, std::pair<int,int> B, int x) {
    auto a = (B.second - A.second) / (B.first - A.first);
    auto b = A.second - (a * A.first);

    return a*x + b;
}

//User's Function Filter
void PhotoApp::on_applyUserFilterButton_clicked()
{
    auto points = sceneUserFilter->points;

    QImage image = currentImage._QPixmap.toImage();
    uchar* bits = image.bits();
    uchar* bitsEnd = bits + image.sizeInBytes();

    int prev = 0;

    int i = 0;

    while(bits < bitsEnd) {
        if (i == 3) {
            i = 0;
            continue;
        }
        i++;

        bool applied = false;
        for (u_int i = 0; i < points.size()-1; i++) {

            if (applied)
                break;

            if (points[i].first == (int)(*bits)) {
                (*bits) = qBound(0, 255 - points[i].second, 255);
                applied = true;
            } else if (points[i].first < (*bits)) {
                /*
                auto A = points[i];
                auto B = points[i+1];

                double rise = (255 - B.second) - (255 - A.second);
                double run = B.first - A.first;

                double a = rise / run;
                double b;

                if (a < 0) {
                    b = (a * A.first) + (255 - A.second);
                } else {
                    b = (255 - B.second) - (a * B.first);
                }

                double res = a * (*bits) + b;
                (*bits) = qBound(0, (int)(res), 255);
                applied = true;
                */


                auto A = points[i];
                A.second = 255 - A.second;
                auto B = points[i+1];
                B.second = 255 - B.second;
                (*bits) = qBound(0, (int)(GetY(A, B, *bits)), 255);
                applied = true;

            }
        }

        if ((*bits) > 255)
            (*bits) = 255;

        if ((*bits) < 0)
            (*bits) = 0;

        /*if (!applied) {
            (*bits) = 255;
        }*/

        bits++;
    }

    currentImage._QPixmap = QPixmap::fromImage(image);
    updateChangedPhoto();
}

void PhotoApp::on_saveButtonFilter_clicked()
{
    QString filterName = ui->lineSaveFilterName->text();

    if (filterName == "") {
        QMessageBox Msgbox;
        Msgbox.setIcon(QMessageBox::Warning);
        Msgbox.setText("Fill some name of the Filter.");
        Msgbox.exec();
        return;
    } else if (filtersDict.find(filterName) != filtersDict.end()) {
        QMessageBox Msgbox;
        Msgbox.setIcon(QMessageBox::Warning);
        Msgbox.setText("Filter with the given name already exists.");
        Msgbox.exec();
        return;
    }

    ui->FiltersComboBox->addItem(filterName);

    std::pair<QString, std::vector<std::pair<int,int>>> elem = std::make_pair(filterName, this->sceneUserFilter->points);
    filtersDict.insert(elem);

    ui->lineSaveFilterName->setText("");
}

void PhotoApp::on_loadFilterButton_clicked()
{
    QString selectedFilterName = ui->FiltersComboBox->currentText();

    sceneUserFilter->points = filtersDict[selectedFilterName];
    sceneUserFilter->drawFunction();
}

void PhotoApp::resizeEvent(QResizeEvent* event)
{
   //QMainWindow::resizeEvent(event);
   updateChangedPhoto();
   ui->originalPhoto_label->setPixmap(currentQPixmap.scaled(ui->originalPhoto_label->width(), ui->originalPhoto_label->height(), Qt::KeepAspectRatio));
}

void PhotoApp::on_medianFilterButton_clicked()
{
    int f[9] = {1,1,1,
                1,1,1,
                1,1,1,};

    ConvolutionFilter filter(3, 3, f, 1, 1, 0, 0);
    currentImage.ApplyMedianFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::changeToGreyscale() {
    QImage image = currentImage._QPixmap.toImage();

    /*
    // https://stackoverflow.com/questions/27949569/convert-a-qimage-to-grayscale
    for (int ii = 0; ii < image.height(); ii++) {
        uchar* scan = image.scanLine(ii);
        int depth = 4;
        for (int jj = 0; jj < image.width(); jj++) {

            QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + jj*depth);
            int gray = qGray(*rgbpixel);
            *rgbpixel = QColor(gray, gray, gray).rgba();
        }
    }
    // https://stackoverflow.com/questions/27949569/convert-a-qimage-to-grayscale
    */
    image = image.convertToFormat(QImage::Format_Grayscale8);

    currentImage._QPixmap = QPixmap::fromImage(image);

    this->updateChangedPhoto();
}

void PhotoApp::on_averageDitheringButton_clicked()
{
    int K = ui->spinBoxAverageDitheringK->value();
    double split = 255 / (K - 1);

    std::vector<int> greyVals;
    greyVals.push_back(0);
    for (int i = 1; i <= K - 2; i++)
        greyVals.push_back((int)split * i);
    greyVals.push_back(255);

    //for (int el: greyVals)
      //  qDebug() << Q_FUNC_INFO << el;

    //this->changeToGreyscale();

    QImage image = currentImage._QPixmap.toImage();

    for (unsigned int channel = 0; channel < 3; channel++) {
        std::vector<int> averages;
        // calulate thersholds of each split
        for (int i = 0; i < K - 1; i++) {
            int counter = 0;
            long long sum = 0;

            uchar* bits = image.bits();
            uchar* bitsEnd = bits + image.sizeInBytes();

            bits += channel;

            while (bits < bitsEnd) {
                if (((*bits) >= greyVals[i]) && (((*bits) < greyVals[i + 1]) || (((*bits) <= greyVals[i + 1]) && (i == K - 2)) )) {
                    counter++;
                    sum += (*bits);
                }

                bits += 4;
            }

            int average;
            if (counter < 1)
                average = greyVals[i];
            else
                average = sum / counter;

            averages.push_back(average);
        }

        //for (int el: averages)
          //  qDebug() << Q_FUNC_INFO << el;

        uchar* bits = image.bits();
        uchar* bitsEnd = bits + image.sizeInBytes();

        bits += channel;

        while (bits < bitsEnd) {
            for (int i = 0; i < K - 1; i++) {
                if (((*bits) >= greyVals[i]) && (((*bits) < greyVals[i + 1]) || (((*bits) <= greyVals[i + 1]) && (i == K - 2)) )) {
                    if ((*bits) <= averages[i]) {
                        (*bits) = greyVals[i];
                        break;
                    } else {
                        (*bits) = greyVals[i + 1];
                        break;
                    }
                }
            }
            bits += 4;
        }

    }

    currentImage._QPixmap = QPixmap::fromImage(image);
    updateChangedPhoto();
}

void PhotoApp::on_convertToGreyScaleButton_clicked()
{
    this->changeToGreyscale();
}

std::vector<int> calculateThreshholds(std::vector<int> thresholds, uchar* bits, uchar* bitsEnd, int depth, int channel) {
    if (depth == 0)
        return thresholds;

    std::vector<int> newThresholdValues;

    for (unsigned int i = 0; i < thresholds.size() - 1; i++) {
        uchar* p = bits;
        uchar* pEnd = bitsEnd;

        int left = thresholds[i];
        int right = thresholds[i+1];

        long long sum = 0;
        int num = 0;

        std::vector<int> numbers;

        p += channel;

        while (p < pEnd) {
            if ((((*p) >= left) && ((*p) < right)) || ( (((*p) >= left) && ((*p) <= right)) && (i == thresholds.size()-2)) ) {
                sum += (*p);
                num++;
                numbers.push_back((*p));
            }
            p += 4;
        }

        sort(numbers.begin(), numbers.end());
        newThresholdValues.push_back((int)(sum / num));
        //newThresholdValues.push_back((int)(numbers[numbers.size() / 2 - 1]));
    }

    thresholds.insert(thresholds.end(), newThresholdValues.begin(), newThresholdValues.end());
    std::sort(thresholds.begin(), thresholds.end());

    return calculateThreshholds(thresholds, bits, bitsEnd, depth - 1, channel);
}

void PhotoApp::on_pushButtonDithering2_clicked()
{
    int K = this->ui->spinBoxAverageDitheringK2->value();

    if(K == 0 || (K & (K-1)) != 0) {
        QMessageBox Msgbox;
        Msgbox.setIcon(QMessageBox::Warning);
        Msgbox.setText("K must be the power of 2.");
        Msgbox.exec();
        return;
    }

    //this->changeToGreyscale();

    QImage image = currentImage._QPixmap.toImage();
    uchar* bits = image.bits();
    uchar* bitsEnd = bits + image.sizeInBytes();

    int depth = log2(K);
    //qDebug() << Q_FUNC_INFO << depth;

    //for (auto val:thresholds)
      //  qDebug() << Q_FUNC_INFO << val;

    double split = 255 / (K - 1);
    std::vector<int> greyVals;
    greyVals.push_back(0);
    for (int i = 1; i <= K - 2; i++)
        greyVals.push_back((int)(split * i));
    greyVals.push_back(255);

    //for (int el: greyVals)
      //  qDebug() << Q_FUNC_INFO << el;

    for (int channel = 0; channel < 3; channel++)
    {
        bits = image.bits();
        std::vector<int> thresholds;
        thresholds.push_back(0);
        thresholds.push_back(255);
        thresholds = calculateThreshholds(thresholds, bits, bitsEnd, depth, channel);

        bits += channel;
        while (bits < bitsEnd) {
            for (unsigned int i = 0; i < thresholds.size() - 1; i++) {
                int val = (*bits);
                if ((val >= thresholds[i]) && (val < thresholds[i+1])) {
                    (*bits) = static_cast<uchar>(greyVals[i]);
                    break;
                }
            }
            bits += 4;
        }
    }

    currentImage._QPixmap = QPixmap::fromImage(image);
    updateChangedPhoto();
}

void pixels_quantize(uchar* imageBits, std::vector<std::vector<int>> bucket) {
    long long R_sum = 0, G_sum = 0, B_sum = 0;
    for (auto pixel: bucket) {
        R_sum += pixel[0];
        G_sum += pixel[1];
        B_sum += pixel[2];
    }

    int R_average = R_sum / bucket.size();
    int G_average = G_sum / bucket.size();
    int B_average = B_sum / bucket.size();

    for (auto pixel: bucket) {
        *(imageBits + pixel[3] + 0) = R_average;
        *(imageBits + pixel[3] + 1) = G_average;
        *(imageBits + pixel[3] + 2) = B_average;
    }
}

void median_cut(uchar* imageBits, std::vector<std::vector<int>> bucket, int numColors) {
    int depth = (int)(log2(numColors));
    int colorIndex = 1;

    std::vector<std::vector<std::vector<int>>> buckets;
    buckets.push_back(bucket);

    for (int i = -1; i < depth; i++) {
        std::vector<std::vector<std::vector<int>>> new_buckets;

        for (auto bucket: buckets) {
            if (colorIndex >= numColors) {
                new_buckets.push_back(bucket);
                continue;
            }
            colorIndex++;

            int R_min = 255, G_min = 255, B_min = 255;
            int R_max = 0, G_max = 0, B_max = 0;

            for (auto pixel: bucket) {
                if (pixel[0] > R_max)
                    R_max = pixel[0];

                if (pixel[1] > G_max)
                    G_max = pixel[1];

                if (pixel[2] > B_max)
                    B_max = pixel[2];

                if (pixel[0] < R_min)
                    R_min = pixel[0];

                if (pixel[1] < G_min)
                    G_min = pixel[1];

                if (pixel[2] < B_min)
                    B_min = pixel[2];
            }

            int R_range = R_max - R_min;
            int G_range = G_max - G_min;
            int B_range = B_max - B_min;

            int colorBiggestRange;

            if (R_range >= G_range && R_range >= B_range)
                colorBiggestRange = 0;
            else if (G_range >= R_range && G_range >= B_range)
                colorBiggestRange = 1;
            else
                colorBiggestRange = 2;

            std::sort(bucket.begin(), bucket.end(),
                      [colorBiggestRange](const std::vector<int>& a, const std::vector<int>& b) {
              return a[colorBiggestRange] < b[colorBiggestRange];
            });

            std::size_t const medianIndex = (bucket.size() + 1) / 2;

            std::vector<std::vector<int>> split_lo(bucket.begin(), bucket.begin() + medianIndex);
            std::vector<std::vector<int>> split_hi(bucket.begin() + medianIndex, bucket.end());
            new_buckets.push_back(split_lo);
            new_buckets.push_back(split_hi);
        }

        buckets.clear();
        buckets.assign(new_buckets.begin(), new_buckets.end());
    }

    for (std::vector<std::vector<int>> elem: buckets)
        pixels_quantize(imageBits, elem);
}

void median_cut_rec(uchar* imageBits, std::vector<std::vector<int>> bucket, int depth) {
    if (depth == 0) {
        pixels_quantize(imageBits, bucket);
        return;
    }

    int R_min = 255, G_min = 255, B_min = 255;
    int R_max = 0, G_max = 0, B_max = 0;

    for (auto pixel: bucket) {
        if (pixel[0] > R_max)
            R_max = pixel[0];

        if (pixel[1] > G_max)
            G_max = pixel[1];

        if (pixel[2] > B_max)
            B_max = pixel[2];

        if (pixel[0] < R_min)
            R_min = pixel[0];

        if (pixel[1] < G_min)
            G_min = pixel[1];

        if (pixel[2] < B_min)
            B_min = pixel[2];
    }

    int R_range = R_max - R_min;
    int G_range = G_max - G_min;
    int B_range = B_max - B_min;

    int colorBiggestRange;

    if (R_range >= G_range && R_range >= B_range)
        colorBiggestRange = 0;
    else if (G_range >= R_range && G_range >= B_range)
        colorBiggestRange = 1;
    else
        colorBiggestRange = 2;

    std::sort(bucket.begin(), bucket.end(),
              [colorBiggestRange](const std::vector<int>& a, const std::vector<int>& b) {
      return a[colorBiggestRange] < b[colorBiggestRange];
    });

    std::size_t const medianIndex = (bucket.size() + 1) / 2;

    std::vector<std::vector<int>> split_lo(bucket.begin(), bucket.begin() + medianIndex);
    std::vector<std::vector<int>> split_hi(bucket.begin() + medianIndex, bucket.end());

    median_cut_rec(imageBits, split_lo, depth - 1);
    median_cut_rec(imageBits, split_hi, depth - 1);
}

void PhotoApp::on_medianCutButton_clicked()
{
    int K = this->ui->medianCutBoxK->value();

    QImage image = currentImage._QPixmap.toImage();
    uchar* bits = image.bits();
    uchar* bitsEnd = bits + image.sizeInBytes();

    std::vector<std::vector<int>> bucket;
    int i = 0;

    while (bits < bitsEnd) {
        std::vector<int> pixel;
        pixel.push_back(*(bits));      // R
        pixel.push_back(*(bits + 1));  // G
        pixel.push_back(*(bits + 2));  // B
        pixel.push_back(i);            // index

        bucket.push_back(pixel);

        bits += 4;
        i += 4;
    }

    bits = image.bits();
    median_cut_rec(bits, bucket, K);
    currentImage._QPixmap = QPixmap::fromImage(image);
    updateChangedPhoto();
}

void PhotoApp::on_pushButtonMedianCut2_clicked()
{
    int numColors = this->ui->spinBoxMedianCut2->value();

    QImage image = currentImage._QPixmap.toImage();
    uchar* bits = image.bits();
    uchar* bitsEnd = bits + image.sizeInBytes();

    std::vector<std::vector<int>> bucket;
    int i = 0;

    while (bits < bitsEnd) {
        std::vector<int> pixel;
        pixel.push_back(*(bits));      // R
        pixel.push_back(*(bits + 1));  // G
        pixel.push_back(*(bits + 2));  // B
        pixel.push_back(i);            // index

        bucket.push_back(pixel);

        bits += 4;
        i += 4;
    }

    bits = image.bits();
    median_cut(bits, bucket, numColors);
    currentImage._QPixmap = QPixmap::fromImage(image);
    updateChangedPhoto();
}
