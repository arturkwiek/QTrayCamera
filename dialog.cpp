#include "dialog.h"
#include "ui_dialog.h"

#include <QCloseEvent>
#include <QMenu>
#include <QStyle>

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

using namespace cv;
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    /* Initialize the tray icon, set the icon of a set of system icons,
     * As well as set a tooltip
     * */
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
    trayIcon->setToolTip("Tray Program" "\n"
                         "Работа со сворачиванием программы трей");

    /* After that create a context menu of items */
    QMenu * menu = new QMenu(this);
    QAction * viewWindow = new QAction(trUtf8("Развернуть окно"), this);
    QAction * quitAction = new QAction(trUtf8("Выход"), this);

    /* connect the signals clicks on menu items to by appropriate slots.
     * The first menu item expands the application from the tray,
     * And the second menu item terminates the application
     * */
    connect(viewWindow, SIGNAL(triggered()), this, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(doSomething()));
    // ADD
    menu->addAction(viewWindow);
    menu->addAction(quitAction);

    /* Set the context menu on the icon
     * And show the application icon in the system tray
     * */
    trayIcon->setContextMenu(menu);
    trayIcon->show();

    /* Also connect clicking on the icon to the signal processor of this press
     * */
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));


    ui->gvVideo->setStyleSheet("background-color: rgb(0, 0, 0);");
    ui->gvVideo->setScene(new QGraphicsScene(this));
    ui->gvVideo->scene()->addItem(&pixmapItem);

    vcVideo.open(0);

    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMaximizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    flags |= Qt::WindowContextHelpButtonHint;
    this->setWindowFlags( flags );

}

Dialog::~Dialog()
{
    delete ui;
}

/* The method that handles click on the application icon MaximizeButtonHintin the system tray
 * */
void Dialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{

    switch (reason){
    case QSystemTrayIcon::Trigger:
        /* The event is ignored if the checkbox is not checked
         * */
        //        if(ui->trayCheckBox->isChecked()){
        /* otherwise, if the window is visible, it is hidden,
             * Conversely, if hidden, it unfolds on the screen
             * */
        if(!this->isVisible()){
            Qt::WindowFlags flags = windowFlags();
            setWindowFlags(flags | Qt::WindowStaysOnTopHint);
            this->show();
        } else {
            this->hide();
        }
        //        }
        break;
    default:
        break;
    }
}

void Dialog::doSomething()
{
    if(vcVideo.isOpened())
        vcVideo.release();
    qDebug() << "spadam stąd";
    exit(0);
}

/* The method that handles the closing event of the application window
 * */
void Dialog::closeEvent(QCloseEvent * event)
{
    /* If the window is visible, and the checkbox is checked, then the completion of the application
     * Ignored, and the window simply hides that accompanied
     * The corresponding pop-up message
     */
    //    if(this->isVisible() && ui->trayCheckBox->isChecked()){
    event->ignore();
    this->hide();
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);

    trayIcon->showMessage("Tray Program",
                          trUtf8("Приложение свернуто в трей. Для того чтобы, "
                                 "развернуть окно приложения, щелкните по иконке приложения в трее"),
                          icon,
                          2000);
    //    }
}

void Dialog::setVideoFrame()
{

    if(!matFrame.empty())
    {
        QImage qimg(matFrame.data,
                    matFrame.cols,
                    matFrame.rows,
                    matFrame.step,
                    QImage::Format_RGB888);

        ui->gvVideo->resetMatrix();
        pixmapItem.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
        ui->gvVideo->fitInView(&pixmapItem,Qt::KeepAspectRatio);
    }
}
