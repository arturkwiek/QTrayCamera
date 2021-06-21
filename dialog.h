#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGraphicsPixmapItem>
#include <QSystemTrayIcon>

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

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

using namespace cv;
class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void captureFrame() {
        if(vcVideo.isOpened()) {
                vcVideo >> matFrame;
                if(matFrame.empty())
                {
                    vcVideo.release();
                    this->open();
                }
        }
    }

    void setVideoFrame();

    VideoCapture vcVideo;
    Mat matFrame;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void doSomething();
private:
    Ui::Dialog *ui;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QGraphicsPixmapItem pixmapItem;

};
#endif // DIALOG_H
