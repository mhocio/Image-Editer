#include "photoapp.h"
#include "ui_photoapp.h"

PhotoApp::PhotoApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PhotoApp)
{
    ui->setupUi(this);
}

PhotoApp::~PhotoApp()
{
    delete ui;
}

