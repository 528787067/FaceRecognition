#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QString>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QMediaPlayer>

#include <iostream>

#include "camerautils.h"
#include "camerathread.h"
#include "imageutils.h"
#include "faceutils.h"

#define TIMER_DELAY             5000
#define LOCAL_WELCOME_SOUND     QString(BASE_PATH) + "welcome.mp3"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initView();
    void initData();
    void updateCameraInfoToCBox();          // 更新摄像头信息到界面

private:
    Ui::MainWindow *ui;

    QTimer *timer;                          // 定时器，用于定时更新摄像头信息
    CameraUtils *cameraUtils;               // 摄像头工具类
    CameraThread *cameraThread;             // 摄像头线程类

    ImageUtils *imageUtils;                 // 图片工具类
    FaceUtils *faceUtils;                   // 人脸检测、识别工具类

    QMediaPlayer *mediaPlayer;              // 媒体播放器，用于播放音乐

private slots:
    void timerSlot();                       // 定时器回调
    void cameraBtnSlot();                   // 摄像头 启动/关闭 按钮回调
    void faceTrainBtnSlot();                // 人脸识别训练按钮回调
    void clearImgBtnSlot();                 // 清空显示按钮回调
    void choosePathBtnSlot();               // 截图保存路径选择按钮回调
    void captureBtnSlot();                  // 截图按钮回调
    void openImgBtnSlot();                  // 打开截图按钮回调
    void onImageCaptureSlot(Mat &image);    // 摄像头捕捉图像回调
    void onCameraCloseSlot();               // 摄像头关闭回调
    void onCameraErrorSlot();               // 摄像头出错回调
    void startTrainSlot();                  // 开始训练人脸识别
    void finishTrainSlot(bool isSuccess);   // 人脸识别训练完毕
};

#endif // MAINWINDOW_H
