#ifndef PHOTOAPP_H
#define PHOTOAPP_H

#include <QMainWindow>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QtCore>
#include <QtGui>
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include "image.h"
#include "graphicsscene.h"

#include <algorithm>
#include <vector>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class PhotoApp; }
QT_END_NAMESPACE

class PhotoApp : public QMainWindow
{
    Q_OBJECT

public:
    PhotoApp(QWidget *parent = nullptr);
    ~PhotoApp();
    void updateChangedPhoto();

    std::vector<std::pair<int,int> >points;

private slots:
    void on_actionOpen_triggered();

    void on_inverseFilterButton_clicked();

    void on_brightnessCorrectionButton_clicked();

    void on_gammaCorrectionButton_clicked();

    void on_convolutionButton_clicked();

    void on_constrastFilterButton_clicked();

    void on_resetImageButton_clicked();

    void on_sharpenFilterButton_clicked();

    void on_edgeDetectionFilterButton_clicked();

    void on_gaussianBlurFilterButton_clicked();

    void on_embossFilterButton_clicked();

    void on_actionExport_triggered();

    void on_widget_customContextMenuRequested(const QPoint &pos);

    void on_resetUserFunction_clicked();

    void on_applyUserFilterButton_clicked();

private:
    Ui::PhotoApp *ui;
    QString originalPhotoName;
    QPixmap currentQPixmap;
    QPixmap originalQPixmap;

    Image currentImage;

    GraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
};
#endif // PHOTOAPP_H
