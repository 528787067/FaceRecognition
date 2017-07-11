#ifndef CAMERAUTILS_H
#define CAMERAUTILS_H

#include <QList>
#include <QStringList>
#include <QCameraInfo>

class CameraUtils
{
private:
    CameraUtils();
    static CameraUtils *cameraUtils;

public:
    static CameraUtils *getInstance();

    QStringList getCameraNames();           // 获取摄像头名字
    QList<QCameraInfo> getCameraInfo();     // 获取摄像头信息

private:
    class CGarbo
    {
    public:
        ~CGarbo()
        {
            if (CameraUtils::cameraUtils)
                delete CameraUtils::cameraUtils;
        }
    };
    static CGarbo carbo;
};

#endif // CAMERAUTILS_H
