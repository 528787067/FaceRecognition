#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <opencv2/opencv.hpp>
#include <QImage>

using namespace cv;

class ImageUtils
{
public:
    ImageUtils();
    static ImageUtils *getInstance();

    Mat qImage2cvMat(const QImage &image);  // QImage 转 Mat
    QImage cvMat2QImage(const Mat& mat);    // Mat 转 QImage

private:
    static ImageUtils *imageUtils;
    class CGarbo
    {
    public:
        ~CGarbo()
        {
            if (ImageUtils::imageUtils)
                delete ImageUtils::imageUtils;
        }
    };
    static CGarbo carbo;
};

#endif // IMAGEUTILS_H
