#include "camerathread.h"

CameraThread::CameraThread()
{
    this->runFlag = false;
    this->cameraIndex = 0;
}

CameraThread::~CameraThread()
{
    if(this->isRunning())
    {
        runFlag = false;
        this->quit();
    }
}

bool CameraThread::openCamera()
{
    capture.open(this->cameraIndex);
    if(capture.isOpened())
    {
        this->runFlag = true;
        this->start();
        return true;
    }
    return false;
}

bool CameraThread::closeCamera()
{
    if(this->runFlag)
    {
        this->runFlag = false;
        return true;
    }
    return false;
}

bool CameraThread::isCameraOpen()
{
    return this->capture.isOpened();
}

void CameraThread::setCameraIndex(int index)
{
    this->cameraIndex = index;
}

void CameraThread::run()
{
    while(this->runFlag && this->capture.isOpened())
    {
        try
        {
            this->capture >> frame;
            emit onImageCapture(frame);
        }
        catch (cv::Exception& e)
        {
            std::cout << e.msg << std::endl;
            this->runFlag = false;
            if(capture.isOpened())
                this->capture.release();
            emit onCameraError();
            return;
        }
#if(CAPTURE_DELAY > 0)
        this->msleep(CAPTURE_DELAY);
#endif
    }
    if(capture.isOpened())
        capture.release();
    emit(onCameraClose());
}
