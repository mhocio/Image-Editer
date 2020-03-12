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

    /*
    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QPen blackPen(Qt::black);
    blackPen.setWidth(3);

    ellipse = scene->addEllipse(10, 10, 100, 100, blackPen, redBrush);
    rectangle = scene->addRect(10, 10, 50, 50, blackPen, blueBrush);
    rectangle->setFlag(QGraphicsItem::ItemIsMovable);

    QPen whitePen(Qt::white);

    QLineF line = QLineF(0, 255, 255, 0);
    scene->addLine(line, blackPen);
    */

    /*
    static const QPointF points[3] = {
        QPointF(10.0, 80.0),
        QPointF(20.0, 10.0),
        QPointF(80.0, 30.0),
    };

    QPainter painter(this);
    painter.setPen(blackPen);
    painter.drawPolyline(points, 3);
    */

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
        ui->originalPhoto_label->setPixmap(currentQPixmap.scaled(ui->originalPhoto_label->width(), ui->originalPhoto_label->height(), Qt::KeepAspectRatio));
        ui->changedPhoto_label->setPixmap(currentQPixmap.scaled(ui->originalPhoto_label->width(), ui->originalPhoto_label->height(), Qt::KeepAspectRatio));
    }
}

void PhotoApp::on_resetImageButton_clicked()
{
    currentImage._QPixmap = originalQPixmap;
    ui->changedPhoto_label->setPixmap(originalQPixmap.scaled(ui->originalPhoto_label->width(), ui->originalPhoto_label->height(), Qt::KeepAspectRatio));
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

    QImage image = currentImage._QPixmap.toImage().convertToFormat(QImage::Format_RGB888);
    uchar* bits = image.bits();
    uchar* bitsEnd = bits + image.sizeInBytes();

    int prev = 0;

    int i = 0;

    while(bits < bitsEnd) {
        /*if (i == 3) {
            i = 0;
            continue;
        }
        i++;*/
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

        if (!applied) {
            (*bits) = 255;
        }

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
