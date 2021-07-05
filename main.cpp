#include "dialog.h"

#include <QApplication>

#include <QDebug>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
//#include <opencv2/videostab.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mat refFrame;
    Dialog w;
    w.show();
    typedef struct ST_DETECTION_ {
        double sum_value;
        Mat frame;
    }stDetection_t;

    stDetection_t detection;

    QVector <stDetection_t> vDetection;
    int ref = 0;
    bool bshowMax = false;
    while(1) {
        a.processEvents();

        if(!w.matFrame.empty()) {
            w.matFrame.copyTo(w.prevFrame);
        }

        w.vcVideo >> w.matFrame;

        if((ref++)%100 == 0)
            w.matFrame.copyTo(refFrame);

        if(w.prevFrame.empty()) {
            qDebug() << "init prev frame";
            w.vcVideo >> w.prevFrame;
        }

        if(w.diffFrame.empty()) {
            qDebug() << "init diff frame";
            w.vcVideo >> w.diffFrame;
        }

        absdiff(w.prevFrame,w.matFrame,w.diffFrame);

        if((ref)%100 == 0) {
            qDebug() << sum(w.diffFrame)[0]/1000000;
        }

        if(sum(w.diffFrame)[0]/1000000 > 2) {
            bshowMax = true;

            w.show();

            detection.sum_value = sum(w.diffFrame)[0]/1000000;
            w.matFrame.copyTo(detection.frame);
            vDetection.push_back(detection);
        } else {
            double max = 1.0;
            for(int i=0; i<vDetection.size();i++) {
                absdiff(vDetection.at(i).frame,refFrame,w.diffFrame);
                if(sum(w.diffFrame)[0]/1000000>max) {
                    max = sum(w.diffFrame)[0]/1000000;
                    if(true == bshowMax) {
                        imshow("max", vDetection.at(vDetection.size()/2).frame);
                        bshowMax = false;
                    }
                }
            }

            while (vDetection.count())
                 vDetection.takeLast();

            qDebug() << "vDetection: " << vDetection.size();
        }

        if(w.isVisible())
            w.setVideoFrame();


    }
        return a.exec();
}
