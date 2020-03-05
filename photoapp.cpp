#include "photoapp.h"
#include "ui_photoapp.h"
#include "functionalfilter.h"

PhotoApp::PhotoApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PhotoApp)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->horizontalFrame);
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

        image = image.scaled(ui->originalPhoto_label->width(), ui->originalPhoto_label->height(), Qt::KeepAspectRatio);

        currentQPixmap = originalQPixmap = QPixmap::fromImage(image);

        currentImage = Image(currentQPixmap);
        ui->originalPhoto_label->setPixmap(currentQPixmap);
        ui->changedPhoto_label->setPixmap(currentQPixmap);
    }
}

void PhotoApp::updateChangedPhoto() {
    ui->changedPhoto_label->setPixmap(currentImage._QPixmap);
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

void PhotoApp::on_convolutionButton_clicked()
{
    // Create a dynamic array of pointers
    int** f = new int* [3];

    // Create a row for every pointer
    for (int i=0; i<3; i++)
    {
       // Note : Rows may not be contiguous
       f[i] = new int[3];

       // Initialize all entries as false to indicate
       // that there are no edges initially
       memset(f[i], 1, 3*sizeof(int));
    }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            f[i][j] = 1;

    int f2[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    ConvolutionFilter filter(3, 3, f, 1, 1);
    currentImage.ApplyConvolutionFilter(filter);
    updateChangedPhoto();

    for (int i = 0; i < 3; i++)
        delete [] f[i];
    delete [] f;
}

void PhotoApp::on_constrastFilterButton_clicked()
{
    ContrastFilter filter;
    currentImage.ApplyFunctionalFilter(filter);
    updateChangedPhoto();
}

void PhotoApp::on_resetImageButton_clicked()
{
    currentImage._QPixmap = originalQPixmap;
    ui->changedPhoto_label->setPixmap(originalQPixmap);
}

void PhotoApp::on_sharpenFilterButton_clicked()
{
    // Create a dynamic array of pointers
    int** f = new int* [3];

    // Create a row for every pointer
    for (int i=0; i<3; i++)
    {
       // Note : Rows may not be contiguous
       f[i] = new int[3];

       // Initialize all entries as false to indicate
       // that there are no edges initially
       memset(f[i], 1, 3*sizeof(int));
    }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            f[i][j] = -1;

    f[1][1] =9;

    ConvolutionFilter filter(3, 3, f, 1, 1);
    currentImage.ApplyConvolutionFilter(filter);
    updateChangedPhoto();

    for (int i = 0; i < 3; i++)
        delete [] f[i];
    delete [] f;
}
