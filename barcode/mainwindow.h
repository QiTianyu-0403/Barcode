/********************************************************************************
* File Name:	mainwindow.h
* Description:	第3章 实例9：Group Box和ScrollArea的实例
* Reference book:《Linux环境下Qt4图形界面与MySQL编程》，机械工业出版社.2012.1
* E_mail: openlinux2011@gmail.com
*
********************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

using namespace cv;
typedef Mat_<uchar> MatU;


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString fileName;       /*-----------------文件名*----------------------*/
    MatU SrcImg,grayImg;


private:
    Ui::MainWindow *ui;
    QLabel *label ;/*添加一个label用于显示“识别结果”四个字*/

private slots:
//    void on_btnOpen_clicked();/*---------------------"打开"按钮*----------------*/

    void on_pushButton_2_clicked();/*--------------------"开始识别"按钮*-------------*/
    void on_pushButton_clicked();/*-----------------------"关闭"按钮-----------------*/
    void on_process_clicked();
    void on_start_clicked();
};

#endif // MAINWINDOW_H
