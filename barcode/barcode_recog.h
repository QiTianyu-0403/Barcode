#ifndef BARCODE_RECOG_H
#define BARCODE_RECOG_H

/*version 0.02
*提高了识别精度
*第一位字符通过左侧6个数据反推得到，不仅仅限定于6和9
*------
*     ChrisZZ  2012.5.5
*/

#pragma once
#pragma execution_character_set("utf-8")//display chinese words

#include <iostream>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <QLabel>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

typedef Mat_<uchar> MatU;


//! 获取条码最前面的一位(d0).是通过条码中d1-d6反推得到的
int get_front();

//! 返回解析后得到的数字
int read_digit(const MatU& img, Point& cur, int position);

//! 略过空白区域
void skip_quiet_zone(const MatU& img, Point& cur);

//! 读取左侧固定的“条空条”,目的是调整扫描点cur的位置
void read_lguard(const MatU& img, Point& cur);

//! 略过左侧数据和右侧数据之间的分界
void skip_mguard(const MatU& img, Point& cur);

//! 读取条码主程序
void read_barcode(const MatU& src);

QVector<Mat> Image_correct(const string& filename);

#endif // BARCODE_RECOG_H
