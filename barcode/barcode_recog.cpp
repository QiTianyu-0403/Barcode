#include "barcode_recog.h"

/*

*------
*/
#include <iostream>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <opencv2/opencv.hpp>


#define SPACE 0         //定义“空”为0
#define BAR 255


enum position{LEFT,RIGHT};
int front;
int mode[6];
const double eps=1e-5;
int z;

//----zz-----

string RES="1234567890123";
//----zz-----


int get_front(){


    string tmp="";
    int i;
    for(i=0; i<6; i++){
        if(mode[i]==0) tmp=tmp+"0";
        else tmp=tmp+"1";
    }
    /*d0可能的取值为0-9*/
    if(tmp=="000000") return 0;
    else if(tmp=="001011") return 1;
    else if(tmp=="001101") return 2;
    else if(tmp=="001110") return 3;
    else if(tmp=="010011") return 4;
    else if(tmp=="011001") return 5;
    else if(tmp=="011100") return 6;
    else if(tmp=="010101") return 7;
    else if(tmp=="010110") return 8;
    else if(tmp=="011010") return 9;
    /*说明识别失败，返回-1*/
    else return -1;
}

/*int read_digit(...)
  *返回解析后得到的数字*/
int read_digit(const MatU& img, Point& cur, int position){
    int pattern[4] = {0,0,0,0}, i;
    for (i=0; i<4; i++){
        int cur_val=img(cur);
        while(img(cur)==cur_val){
            ++pattern[i];
            ++cur.x;
        }
    }
    /*使用归一化方法测量条码值*/
    double sum=pattern[0]+pattern[1]+pattern[2]+pattern[3];
    double tmp1=(pattern[0]+pattern[1])*1.0;
    double tmp2=(pattern[1]+pattern[2])*1.0;
    int at1, at2;
    if(tmp1/sum < 2.5/7)  at1=2;
    else if(tmp1/sum < 3.5/7) at1=3;
    else if(tmp1/sum < 4.5/7) at1=4;
    else at1=5;

    if(tmp2/sum < 2.5/7)  at2=2;
    else if(tmp2/sum < 3.5/7) at2=3;
    else if(tmp2/sum < 4.5/7) at2=4;
    else at2=5;

    int digit=-1;

    if(position==LEFT){

        if(at1==2){
            if(at2==2) {
                mode[z++]=0;
                digit = 6;
            }
            else if(at2==3) {
                mode[z++]=1;
                digit = 0;
            }
            else if(at2==4) {
                mode[z++]=0;
                digit = 4;
            }
            else if(at2==5) {
                mode[z++]=1;
                digit = 3;
            }
        }
        else if(at1==3){
            if(at2==2) {
                mode[z++]=1;
                digit = 9;
            }
            else if(at2==3) {
                mode[z++]=0;
                if(pattern[2]+1<pattern[3]) digit = 8;
                else digit = 2;
            }
            else if(at2==4) {
                mode[z++]=1;
                if(pattern[1]+1<pattern[2]) digit = 7;
                else digit = 1;
            }
            else if(at2==5) {
                mode[z++]=0;
                digit = 5;
            }
        }
        else if(at1==4){
            if(at2==2) {
                mode[z++]=0;
                digit = 9;
            }
            else if(at2==3) {
                mode[z++]=1;
                if(pattern[1]+1<pattern[0]) digit = 8;
                else digit = 2;
            }
            else if(at2==4) {
                mode[z++]=0;
                if(pattern[0]+1<pattern[1]) digit = 7;
                else digit = 1;
            }
            else if(at2==5) {
                mode[z++]=1;
                digit = 5;
            }
        }
        else if(at1==5){
            if(at2==2) {
                mode[z++]=1;
                digit = 6;
            }
            else if(at2==3) {
                mode[z++]=0;
                digit = 0;
            }
            else if(at2==4) {
                mode[z++]=1;
                digit = 4;
            }
            else if(at2==5) {
                mode[z++]=0;
                digit=3;
            }
        }

    }else{

        if(at1==2){
            if(at2==2) digit = 6;
            else if(at2==4) digit = 4;
        }
        else if(at1==3){
            if(at2==3) {
                if(pattern[2]+1<pattern[3]) digit = 8;
                else digit = 2;
            }
            else if(at2==5) digit = 5;
        }
        else if(at1==4){
            if(at2==2) digit = 9;
            else if(at2==4) {
                if(pattern[0]+1<pattern[1]) digit = 7;
                else digit = 1;
            }
        }
        else if(at1==5){
            if(at2==3) digit = 0;
            else if(at2==5) digit=3;
        }
    }
    return digit;
}

void skip_quiet_zone(const MatU& img, Point& cur){//略过空白区域
    while (img(cur) == SPACE)
        ++cur.x;
}

void read_lguard(const MatU& img, Point& cur){//读取左侧固定的“条空条”调整扫描点cur的位置
    int pattern[3] = { BAR, SPACE, BAR };
    for (int i=0; i<3; i++)
        while (img(cur)==pattern[i])
            ++cur.x;
}

void skip_mguard(const MatU& img, Point& cur){      //略过左侧数据和右侧数据之间的分界
    int pattern[5] = { SPACE, BAR, SPACE, BAR, SPACE };
    for (int i=0; i<5; i++)
        while (img(cur)==pattern[i])
            ++cur.x;
}

void read_barcode(const MatU& src){      //读取条码主程序
    z=0;
    //载入灰度图像
    MatU img = src.clone();
    Size size = img.size();
    Point cur(0, size.height / 2);              //h表示图像的高度
    bitwise_not(img, img);      //黑白对调
    threshold(img, img, 128, 255, THRESH_BINARY);       /*二值图像*/
    skip_quiet_zone(img, cur);      //略过左侧的白色区域“静区”
    read_lguard(img, cur);      //读取左侧的6个数字
    vector<int> digits;     //digit存储识别的结果
    for (int i=0; i<6; i++){               //左侧数据解码
        int d = read_digit(img, cur, LEFT);
        digits.push_back(d);
    }
    skip_mguard(img, cur);
    for (int i = 0; i < 6; i++){
        int d = read_digit(img, cur, RIGHT);
        digits.push_back(d);
    }
    //整理解码结果
    RES[0]=get_front()+'0';
    for (int i = 0; i < 12; i++){
        RES[i+1]=digits[i]+'0';
    }
}

QVector<Mat> Image_correct(const string& filename){
    Mat image,imageGray,imageGuussian;
    Mat imageSobelX,imageSobelY,imageSobelOut;
    imageGray=imread(filename,0);
    imageGray.copyTo(image);
//    imshow("Source Image",image);
    GaussianBlur(imageGray,imageGuussian,Size(3,3),0);
    //水平和垂直方向灰度图像的梯度和,使用Sobel算子
    Mat imageX16S,imageY16S;
    Sobel(imageGuussian,imageX16S,CV_16S,1,0,3,1,0,4);
    Sobel(imageGuussian,imageY16S,CV_16S,0,1,3,1,0,4);
    convertScaleAbs(imageX16S,imageSobelX,1,0);
    convertScaleAbs(imageY16S,imageSobelY,1,0);
    imageSobelOut=imageSobelX+imageSobelY;
    //imshow("imageSobelOut",imageSobelOut);
    Mat srcImg =imageSobelOut;

    Mat padded;
    int opWidth = getOptimalDFTSize(srcImg.rows);
    int opHeight = getOptimalDFTSize(srcImg.cols);
    copyMakeBorder(srcImg, padded, 0, opWidth-srcImg.rows, 0, opHeight-srcImg.cols, BORDER_CONSTANT, Scalar::all(0));
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat comImg;

    merge(planes,2,comImg);
    dft(comImg, comImg);
    split(comImg, planes);
    magnitude(planes[0], planes[1], planes[0]);
    Mat magMat = planes[0];
    magMat += Scalar::all(1);
    log(magMat, magMat);
    magMat = magMat(Rect(0, 0, magMat.cols & -2, magMat.rows & -2));

    int cx = magMat.cols/2;
    int cy = magMat.rows/2;
    Mat q0(magMat, Rect(0, 0, cx, cy));
    Mat q1(magMat, Rect(0, cy, cx, cy));
    Mat q2(magMat, Rect(cx, cy, cx, cy));
    Mat q3(magMat, Rect(cx, 0, cx, cy));
    Mat tmp;
    q0.copyTo(tmp);
    q2.copyTo(q0);
    tmp.copyTo(q2);
    q1.copyTo(tmp);
    q3.copyTo(q1);
    tmp.copyTo(q3);
    normalize(magMat, magMat, 0, 1, NORM_MINMAX);
    Mat magImg(magMat.size(), CV_8UC1);
    magMat.convertTo(magImg,CV_8UC1,255,0);
//    imshow("magImg", magImg);
    //HoughLines查找傅里叶频谱的直线，该直线跟原图的一维码方向相互垂直
    threshold(magImg,magImg,180,255,THRESH_BINARY);
//    imshow("magImg2", magImg);
    vector<Vec2f> lines;
    float pi180 = (float)CV_PI/180;
    Mat linImg(magImg.size(),CV_8UC3);
    for(int i = 0 ; i < linImg.rows ; i++)
        for(int j = 0 ; j < linImg.cols ; j++){
            linImg.at<Vec3b>(i,j)[0] = 0;
            linImg.at<Vec3b>(i,j)[1] = 0;
            linImg.at<Vec3b>(i,j)[2] = 0;
        }
    HoughLines(magImg,lines,1,pi180,100,0,0);
    int numLines = lines.size();
    float theta;
    for(int l=0; l<numLines; l++)
    {
        float rho = lines[l][0];
        theta = lines[l][1];
        float aa=(theta/CV_PI)*180;
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line(linImg,pt1,pt2,Scalar(255,0,0),3,8,0);
    }
//    imshow("Hough",linImg);
    //校正角度计算
    float angelD=180*theta/CV_PI-90;
    Point center(image.cols/2, image.rows/2);
    Mat rotMat = getRotationMatrix2D(center,angelD,1.0);
    Mat imageSource = Mat::ones(image.size(),CV_8UC3);
    warpAffine(image,imageSource,rotMat,image.size(),1,0,Scalar(255,255,255));//变换校正图像

    Mat RGBimage;
    cvtColor(imageSource, RGBimage, COLOR_GRAY2RGB);
    rectangle(imageSource,cv::Point(0,imageSource.rows/2),cv::Point(imageSource.cols,imageSource.rows/2+1),Scalar(255,255,255),8,1,0);
//    imshow("imageSource",imageSource);

    QVector<Mat> a(3);

    a[0] = imageSobelOut;
    a[1] = linImg;
    a[2] = imageSource;


    return a;

}


