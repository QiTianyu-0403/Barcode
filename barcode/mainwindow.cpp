/********************************************************************************
* File Name:	mainwindow.cpp
* Description:	第3章 实例9：Group Box和ScrollArea的实例
* Reference book:《Linux环境下Qt4图形界面与MySQL编程》，机械工业出版社.2012.1
* E_mail: openlinux2011@gmail.com
*
********************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/opencv.hpp>

#include <QString>
#include <vector>
#include <iostream>
#include <string>
#include "barcode_recog.h"

using namespace std;
using namespace cv;

extern string RES;

QString s2q(const string &s);
string q2s(const QString &s);

QString s2q(const string &s)
{
    return QString(QString::fromLocal8Bit(s.c_str()));
}
string q2s(const QString &s)
{
    return string((const char *)s.toLocal8Bit());
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    label = new QLabel;
    //ui->scrollArea->setWidget(label);
    ui->label_4->setStyleSheet("QLabel{border:3px solid rgb(0, 0, 0);}");
    ui->label_5->setStyleSheet("QLabel{border:3px solid rgb(0, 0, 0);}");
    ui->label_6->setStyleSheet("QLabel{border:3px solid rgb(0, 0, 0);}");
    ui->label_7->setStyleSheet("QLabel{border:3px solid rgb(0, 0, 0);}");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    read_barcode(grayImg);


    QString res= s2q(RES);
    ui->label2->setText(res);
}


void MainWindow::on_pushButton_2_clicked()
{
   QApplication::quit();
}

void MainWindow::on_process_clicked()
{
    string imgname=q2s(fileName);
    //Mat gray;
    //cvtColor(Image_correct(imgname)[2], gray, COLOR_RGB2GRAY);
    grayImg = Image_correct(imgname)[2].clone();

    Mat src1,src2;
    src1 = Image_correct(imgname)[0].clone();
    src2 = Image_correct(imgname)[1].clone();

    QImage Qtemp0,Qtemp1,Qtemp2;

    Qtemp0 = QImage((const uchar*)(src1.data), src1.cols, src1.rows, src1.cols*src1.channels(), QImage::Format_Indexed8);
    ui->label_1->setPixmap(QPixmap::fromImage(Qtemp0));
    Qtemp0 = Qtemp0.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_1->setScaledContents(true);
    ui->label_1->resize(Qtemp0.size());
    ui->label_1->show();

    Qtemp1 = QImage((const uchar*)(src2.data), src2.cols, src2.rows, src2.cols*src2.channels(), QImage::Format_RGB888);
    ui->label_2->setPixmap(QPixmap::fromImage(Qtemp1));
    Qtemp1 = Qtemp1.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_2->setScaledContents(true);
    ui->label_2->resize(Qtemp1.size());
    ui->label_2->show();

    Qtemp2 = QImage((const uchar*)(grayImg.data), grayImg.cols, grayImg.rows, grayImg.cols*grayImg.channels(), QImage::Format_Indexed8);
    ui->label_3->setPixmap(QPixmap::fromImage(Qtemp2));
    Qtemp2 = Qtemp2.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_3->setScaledContents(true);
    ui->label_3->resize(Qtemp2.size());
    ui->label_3->show();

    //imshow("gray",grayImg);
}

void MainWindow::on_start_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open File"),"../../../../../../../Master/Semester1/Image_processing/ProjectFiles/ean13-barcode-recognition-master/Qt_project/image","files(*)");
    if (!fileName.isEmpty())/*若文件存在那么就打开*/
    {
        QImage image(fileName);
        QImage Qtemp;
        if (image.isNull())
        {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
        label->setPixmap(QPixmap::fromImage(image));
        string imgname=q2s(fileName);
        Mat src0 = imread(imgname);
        Mat src;
        cvtColor(src0, src, CV_BGR2RGB);

        SrcImg = src.clone();

        Qtemp = QImage((const uchar*)(src.data), src.cols, src.rows, src.cols*src.channels(), QImage::Format_RGB888);
        ui->label_0->setPixmap(QPixmap::fromImage(Qtemp));
        Qtemp = Qtemp.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->label_0->setScaledContents(true);
        ui->label_0->resize(Qtemp.size());
        ui->label_0->show();

        ui->label2->setText("XXXXXXXXXX");
    }
}
