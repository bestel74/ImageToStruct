#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QDebug>
#include <QSize>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(nullptr)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(&scene);
    ui->graphicsView->setMainPtr(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::load(QString file)
{
    this->scene.clear();
    this->current_bitmap.clear();
    this->bit_ptr = nullptr;
    ui->slider_scale->setValue(100);

    this->org_ptr = scene.addPixmap(file);
    this->current_bitmap = QBitmap(file);

    if(current_bitmap.isNull())
    {
        QMessageBox::warning(this, tr("Error"), tr("Cannot convert this format of file into Bitmap :'("));
        return;
    }

    QPixmap pixmap = QPixmap::fromImage(current_bitmap.toImage());
    this->bit_ptr = scene.addPixmap(pixmap);

    bit_ptr->moveBy(pixmap.size().width(), 0);
    ui->l_image_size->setText(QString::number(this->bit_ptr->pixmap().width()) + " x " +
                              QString::number(this->bit_ptr->pixmap().height()));
}


void MainWindow::on_pb_load_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                QDir::homePath());
    if(file.isEmpty())
    {
        return;
    }

    load(file);
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


    // Struct
    QString str_struct = QString("typedef struct\n{\n    uint16_t width, height;\n    uint8_t image_data["
            + QString::number(input.sizeInBytes()) + "];\n} Image;\n\n");
    output.write(str_struct.toLocal8Bit());

    // Image data
    output.write(QString("Image img = {\n    .width=" + QString::number(this->org_ptr->pixmap().width() * this->org_ptr->scale()) + ",\n" +
                         "    .height=" + QString::number(this->org_ptr->pixmap().height() * this->org_ptr->scale()) + ",\n" +
                         "    .image_data = {").toLocal8Bit());

    uchar *bits = input.scanLine(0);
    for(int i=0 ; i < input.sizeInBytes() ; i++)
    {
        QString bitstr;
        uchar bit = bits[i];

        if(i%20 == 0)
        {
            output.write("\n        ");
        }

        bitstr.append("0x");
        bitstr.append(QString::number(bit, 16));
        if(i < input.sizeInBytes()-1) bitstr.append(", ");

        output.write(bitstr.toLocal8Bit());
    }
    output.write("\n    }\n};");
}


void MainWindow::on_slider_scale_valueChanged(int value)
{
    if(this->bit_ptr)
    {
        qreal scale = value / 100.0f;

        this->org_ptr->setScale(scale);
        ui->l_image_size->setText(QString::number(this->org_ptr->pixmap().width() * this->org_ptr->scale()) + " x " +
                                  QString::number(this->org_ptr->pixmap().height() * this->org_ptr->scale()));

        QPixmap zoomed(this->org_ptr->pixmap());
        zoomed = zoomed.scaledToWidth(this->org_ptr->pixmap().width() * this->org_ptr->scale());
        this->current_bitmap = QBitmap(zoomed);

        this->scene.removeItem(this->bit_ptr);
        this->bit_ptr = this->scene.addPixmap(QPixmap::fromImage(current_bitmap.toImage()));
        bit_ptr->moveBy(this->current_bitmap.width(), 0);
    }
}
