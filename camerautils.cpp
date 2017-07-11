#include "camerautils.h"

CameraUtils::CGarbo CameraUtils::carbo;
CameraUtils *CameraUtils::cameraUtils = NULL;

CameraUtils::CameraUtils()
{

}

CameraUtils *CameraUtils::getInstance()
{
    if(CameraUtils::cameraUtils == NULL)
        CameraUtils::cameraUtils = new CameraUtils();
    return CameraUtils::cameraUtils;
}

QStringList CameraUtils::getCameraNames()
{
    QStringList cameraNames;
    foreach (const QCameraInfo &cameraInfo, QCameraInfo::availableCameras())
    {
        cameraNames << cameraInfo.description();
    }
    return cameraNames;
}

QList<QCameraInfo> CameraUtils::getCameraInfo()
{
    QList<QCameraInfo> cameraInfos;
    foreach (const QCameraInfo &cameraInfo, QCameraInfo::availableCameras())
    {
        cameraInfos << cameraInfo;
    }
    return cameraInfos;
}
