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

    scene = new GraphicsScene(this);
    ui->graphicsViewFilter->setScene(scene);

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
    BrightnessCorrectionFilter filter;
    currentImage.ApplyFunctionalFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_gammaCorrectionButton_clicked()
{
    GammaCorrectionFilter filter;
    currentImage.ApplyFunctionalFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_constrastFilterButton_clicked()
{
    ContrastFilter filter;
    currentImage.ApplyFunctionalFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_convolutionButton_clicked()
{
    int f[12] = {1,1,1,
                1,1,1,
                1,1,1,};

    ConvolutionFilter filter(3, 3, f, 1, 1);
    currentImage.ApplyConvolutionFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_sharpenFilterButton_clicked()
{
    int f[9] = {-1,-1,-1,
                -1, 9,-1,
                -1,-1,-1};

    ConvolutionFilter filter(3, 3, f, 1, 1);
    currentImage.ApplyConvolutionFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_edgeDetectionFilterButton_clicked()
{
    int f[9] = {0, -1, 0,
                0,  1, 0,
                0,  0, 0};

    ConvolutionFilter filter(3, 3, f, 1, 1);
    currentImage.ApplyConvolutionFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_gaussianBlurFilterButton_clicked()
{
    int f[9] = {0, 1, 0,
                1, 4, 1,
                0, 1, 0};

    ConvolutionFilter filter(3, 3, f, 1, 1);
    currentImage.ApplyConvolutionFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_embossFilterButton_clicked()
{
    int f[9] = {-1,-1,-1,
                 0, 1, 0,
                 1, 1, 1};

    ConvolutionFilter filter(3, 3, f, 1, 1);
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
    scene->removeAllPoints();
}

//User's Function Filter
void PhotoApp::on_applyUserFilterButton_clicked()
{
    auto points = scene->points;

    QImage image = currentImage._QPixmap.toImage().convertToFormat(QImage::Format_BGR888);
    uchar* bits = image.bits();
    uchar* bitsEnd = bits + image.sizeInBytes();

    int prev = 0;

    while(bits < bitsEnd) {
        bool applied = false;
        for (u_int i = 0; i < points.size()-1; i++) {

            if (applied)
                break;

            if (points[i].first == (int)(*bits)) {
                (*bits) = qBound(0, points[i].second, 255);
                applied = true;
            } else if (points[i].first < (*bits)) {
                auto A = points[i];
                auto B = points[i+1];

                int a = B.first - A.first;
                int b = B.second - A.second;
                int c = (int)(*bits) - A.first;

                double d = b*c/a;

                double res = A.second + d;
                (*bits) = qBound(0, (int)(255 - res), 255);
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
