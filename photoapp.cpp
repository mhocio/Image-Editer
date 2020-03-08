#include "photoapp.h"
#include "ui_photoapp.h"
#include "functionalfilter.h"

PhotoApp::PhotoApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PhotoApp)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->horizontalFrame);

    scene = new QGraphicsScene(this);
    ui->graphicsViewFilter->setScene(scene);

    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QPen blackPen(Qt::black);
    blackPen.setWidth(6);

    ellipse = scene->addEllipse(10, 10, 100, 100, blackPen, redBrush);
    rectangle = scene->addRect(-100, -100, 50, 50, blackPen, blueBrush);
    rectangle->setFlag(QGraphicsItem::ItemIsMovable);

    QPen whitePen(Qt::white);

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
    int f[9] = {1,1,1,
                1,1,1,
                1,1,1};

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
