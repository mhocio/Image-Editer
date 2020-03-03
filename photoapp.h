#ifndef PHOTOAPP_H
#define PHOTOAPP_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PhotoApp; }
QT_END_NAMESPACE

class PhotoApp : public QMainWindow
{
    Q_OBJECT

public:
    PhotoApp(QWidget *parent = nullptr);
    ~PhotoApp();

private:
    Ui::PhotoApp *ui;
};
#endif // PHOTOAPP_H
