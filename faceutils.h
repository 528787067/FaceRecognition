#ifndef FACEUTILS_H
#define FACEUTILS_H

#include <QObject>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>

#define BASE_PATH                   "E:/WorkSpace/Windows/Qt/FaceRecognition/resources/"

#define FRONTALFACE_ALT             "haarcascade_frontalface_alt.xml"
#define FRONTALFACE_ALT_TREE        "haarcascade_frontalface_alt_tree.xml"
#define FRONTALFACE_ALT2            "haarcascade_frontalface_alt2.xml"
#define FRONTALFACE_DEFAULT         "haarcascade_frontalface_default.xml"

#define FACE_DETECTION_FILE         FRONTALFACE_ALT
#define FACE_DETECTION_XML_PATH     String(BASE_PATH) + FACE_DETECTION_FILE

#define FACE_RECOGNITION_CSV_PATH   String(BASE_PATH) + "at.txt"
#define FACE_PCA_MODEL_XML_PATH     String(BASE_PATH) + "FacePCAModel.xml"
#define FACE_FISHER_MODEL_XML_PATH  String(BASE_PATH) + "FaceFisherModel.xml"
#define FACE_LBPH_MODEL_XML_PATH    String(BASE_PATH) + "FaceLBPHModel.xml"

using namespace cv;
using namespace face;
using namespace std;

typedef enum{
    PCA_MODEL,
    FISHER_MODEL,
    LBPH_MODEL
} RecognizerModel;

class FaceUtils : public QObject
{
    Q_OBJECT

private:
    class FaceTrain : public QThread
    {
    private:
        FaceUtils *faceUtils;
    public:
        FaceTrain(FaceUtils *faceUtils)
        {
            this->faceUtils = faceUtils;
        }
    protected:
        void run()
        {
            faceUtils->isFaceTrained = faceUtils->faceTrain();
            emit(faceUtils->finishTrain(faceUtils->isFaceTrained));
        }
    };

public:
    FaceUtils();
    ~FaceUtils();
    static FaceUtils *getInstance();

    vector<Rect> faceDetection(const Mat &image);                                       // 人脸检测
    int faceRecognition(const Mat &image, RecognizerModel recognizerModel = PCA_MODEL); // 人脸识别

    void startTrainFace();                  // 开始对人脸进行训练
    bool faceTrain();                       // 人脸识别训练
    bool loadFaceTrainFile();               // 加载人脸识别训练文件
    bool faceHasTrain();                    // 是否已经训练过了

signals:
    void startTrain();
    void finishTrain(bool isSuccess);

private:
    CascadeClassifier cascadeClassifier;

    Ptr<FaceRecognizer> eigenFaceRecognizer;
    Ptr<FaceRecognizer> fisherFaceRecognizer;
    Ptr<FaceRecognizer> LBPHFaceRecognizer;

    FaceUtils::FaceTrain *faceTrainThread;
    bool isFaceTrained;

private:
    bool readCsvFile(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';');

private:
    static FaceUtils *faceUtils;

    class CGarbo
    {
    public:
        ~CGarbo()
        {
            if (FaceUtils::faceUtils)
                delete FaceUtils::faceUtils;
        }
    };
    static CGarbo carbo;
};

#endif // FACEUTILS_H
