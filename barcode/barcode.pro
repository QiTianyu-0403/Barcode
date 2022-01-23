#-------------------------------------------------
#
# Project created by QtCreator 2013-05-14T22:37:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = barcode_recog
TEMPLATE = app
CONFIG += c++11  # OpenCV 4 requires this

SOURCES += main.cpp \
        mainwindow.cpp \
        barcode_recog.cpp

HEADERS  += mainwindow.h \
    barcode_recog.h

FORMS    += mainwindow.ui

# opencv including directory

INCLUDEPATH += /usr/local/Cellar/opencv/4.5.3_2/include/opencv4

LIBS += -L/usr/local/Cellar/opencv/4.5.3_2/lib\
-lopencv_core \
 -lopencv_imgproc \
 -lopencv_imgcodecs \
  -lopencv_highgui \
