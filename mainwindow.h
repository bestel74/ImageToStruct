#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDir>
#include <QFileDialog>
#include <QString>
#include <QBitmap>
#include <QImage>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_load_clicked();

    void on_pb_save_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    QBitmap current_bitmap;
};
#endif // MAINWINDOW_H
