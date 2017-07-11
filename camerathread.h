#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QThread>
#include <opencv2/opencv.hpp>

#define CAPTURE_DELAY 50

using namespace cv;

class CameraThread : public QThread
{
    Q_OBJECT

public:
    CameraThread();
    ~CameraThread();

    bool openCamera();                  // 打开摄像头
    bool closeCamera();                 // 关闭摄像头
    bool isCameraOpen();                // 判断摄像头是否开启
    void setCameraIndex(int index);     // 设置摄像头

protected:
    void run();

private:
    bool runFlag;
    int cameraIndex;

    Mat frame;
    VideoCapture capture;

signals:
    void onImageCapture(Mat &img);
    void onCameraClose();
    void onCameraError();
};

#endif // CAMERATHREAD_H
