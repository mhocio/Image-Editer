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
#include <map>

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
    void addFunctionFilters();
    void changeToGreyscale();

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

    void on_saveButtonFilter_clicked();

    void on_loadFilterButton_clicked();

    void resizeEvent(QResizeEvent* event);

    void on_medianFilterButton_clicked();

    void on_averageDitheringButton_clicked();

    void on_convertToGreyScaleButton_clicked();

    void on_pushButtonDithering2_clicked();

    void on_medianCutButton_clicked();

    void on_pushButtonMedianCut2_clicked();

    void on_YCrCbpushButton_clicked();

private:
    Ui::PhotoApp *ui;
    QString originalPhotoName;
    QPixmap currentQPixmap;
    QPixmap originalQPixmap;

    Image currentImage;

    GraphicsScene *sceneUserFilter;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
    std::map<QString, std::vector<std::pair<int,int>> > filtersDict;
};
#endif // PHOTOAPP_H
