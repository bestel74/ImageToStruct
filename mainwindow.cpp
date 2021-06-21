#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(nullptr)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(&scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_load_clicked()
{
    scene.clear();
    current_bitmap.clear();

    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                QDir::homePath());
    if(file.isEmpty())
    {
        return;
    }

    scene.addPixmap(file);
    current_bitmap = QBitmap(file);
    if(current_bitmap.isNull())
    {
        QMessageBox::warning(this, tr("Error"), tr("Cannot convert this format of file into Bitmap :'("));
        return;
    }

    QPixmap pixmap = QPixmap::fromImage(current_bitmap.toImage());


    QGraphicsPixmapItem *bit_ptr = scene.addPixmap(pixmap);

    bit_ptr->moveBy(pixmap.size().width(), 0);
}


void MainWindow::on_pb_save_clicked()
{
    if(current_bitmap.isNull())
    {
        QMessageBox::warning(this, tr("Error"), tr("Open and convert a file first :)"));
        return;
    }

    QString file = QFileDialog::getSaveFileName(this,
                                                tr("Save File"),
                                                QDir::homePath(),
                                                tr("C File (*.h *.c)"));
    if(file.isEmpty())
    {
        return;
    }

    QFile output(file);
    if(output.open(QIODevice::WriteOnly) == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't write file :'("));
        return;
    }

    QImage input = current_bitmap.toImage();

    output.write(QString("uint8_t image[" + QString::number(input.sizeInBytes())).toLocal8Bit() + "] = {\n");
    uchar *bits = input.scanLine(0);
    for(int i=0 ; i < input.sizeInBytes() ; i++)
    {
        QString bitstr;
        uchar bit = bits[i];

        bitstr.append("0x");
        bitstr.append(QString::number(bit, 16));
        if(i < input.sizeInBytes()-1) bitstr.append(", ");

        output.write(bitstr.toLocal8Bit());

        if(i%20 == 0)
        {
            output.write("\n");
        }
    }
    output.write("};\n");
}
