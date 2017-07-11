#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->initData();
    this->initView();

    updateCameraInfoToCBox();
    this->timer->start(TIMER_DELAY);
    this->faceUtils->startTrainFace();
}

MainWindow::~MainWindow()
{
    if(timer->isActive())
        timer->stop();
    if(cameraThread->isRunning())
        if(!cameraThread->closeCamera())
            cameraThread->quit();

    delete ui;
    delete timer;
    delete cameraThread;
}

void MainWindow::initData()
{
    this->timer = new QTimer();
    this->cameraUtils = CameraUtils::getInstance();
    this->imageUtils = ImageUtils::getInstance();
    this->faceUtils = FaceUtils::getInstance();
    this->cameraThread = new CameraThread();

    this->mediaPlayer = new QMediaPlayer();
    this->mediaPlayer->setVolume(100);
    this->mediaPlayer->setMedia(QUrl::fromLocalFile(LOCAL_WELCOME_SOUND));

    connect(this->cameraThread, SIGNAL(onImageCapture(Mat&)),
            this, SLOT(onImageCaptureSlot(Mat&)),
            Qt::DirectConnection);
    connect(this->cameraThread, SIGNAL(onCameraClose()),
            this, SLOT(onCameraCloseSlot()));
    connect(this->cameraThread, SIGNAL(onCameraError()),
            this, SLOT(onCameraErrorSlot()));
    connect(this->faceUtils, SIGNAL(startTrain()), this, SLOT(startTrainSlot()));
    connect(this->faceUtils, SIGNAL(finishTrain(bool)), this, SLOT(finishTrainSlot(bool)));
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
}

void MainWindow::initView()
{
    this->setWindowTitle("人脸识别门禁系统");
    ui->savePathEdit->setText(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)[0]);

    connect(ui->cameraBtn, SIGNAL(clicked(bool)), this, SLOT(cameraBtnSlot()));
    connect(ui->reTrainFaceBtn, SIGNAL(clicked(bool)), this, SLOT(faceTrainBtnSlot()));
    connect(ui->clearImgBtn, SIGNAL(clicked(bool)), this, SLOT(clearImgBtnSlot()));
    connect(ui->choosePathBtn, SIGNAL(clicked(bool)), this, SLOT(choosePathBtnSlot()));
    connect(ui->captureBtn, SIGNAL(clicked(bool)), this, SLOT(captureBtnSlot()));
    connect(ui->openImgBtn, SIGNAL(clicked(bool)), this, SLOT(openImgBtnSlot()));
}

void MainWindow::updateCameraInfoToCBox()
{
    QStringList cameraNames = cameraUtils->getCameraNames();
    cameraNames.insert(0, "");
    while(ui->cameraCBox->count() > cameraNames.size())
        ui->cameraCBox->removeItem(cameraNames.size());
    while(ui->cameraCBox->count() < cameraNames.size())
        ui->cameraCBox->addItem(NULL);
    for(int i = 0; i < cameraNames.size(); i++)
        ui->cameraCBox->setItemText(i, cameraNames[i]);
}

void MainWindow::timerSlot()
{
    updateCameraInfoToCBox();
}

void MainWindow::cameraBtnSlot()
{
    if(ui->cameraCBox->isEnabled())
    {
        if(ui->cameraCBox->currentIndex() == 0)
        {
            QMessageBox::information(this, "警告", "请选择一个摄像头");
            return;
        }
        ui->cameraCBox->setEnabled(false);
        ui->cameraBtn->setText("关闭");
        cameraThread->setCameraIndex(ui->cameraCBox->currentIndex()-1);
        if(!cameraThread->openCamera())
        {
            ui->cameraCBox->setEnabled(true);
            ui->cameraBtn->setText("启动");
            QMessageBox::information(this, "警告", "摄像头打开失败");
        }
    }
    else
    {
        cameraThread->closeCamera();
        ui->cameraCBox->setEnabled(true);
        ui->cameraBtn->setText("启动");
    }
}

void MainWindow::faceTrainBtnSlot()
{
    this->faceUtils->startTrainFace();
}

void MainWindow::onImageCaptureSlot(Mat &image)
{
    vector<Rect> faces = faceUtils->faceDetection(image);

    foreach (Rect face, faces)
        rectangle((Mat)image, face, Scalar(0, 0, 255), 2, 8);

    foreach (Rect face, faces)
    {
        Mat imageRIO = image(face);
        cv::resize(imageRIO, imageRIO, Size(92, 112));

        RecognizerModel recognizerModel = PCA_MODEL;
        if(ui->eigenfacesRb->isChecked())
            recognizerModel = PCA_MODEL;
        else if(ui->fisherfacesRb->isChecked())
            recognizerModel = FISHER_MODEL;
        else if(ui->lbphRb->isChecked())
            recognizerModel = LBPH_MODEL;
        int result = faceUtils->faceRecognition(imageRIO, recognizerModel);
        cout << result << endl;

        switch(result)
        {
        case 35:
            putText(image, "ChenXuRan", Point(face.x, face.y),
                    FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));

            if(this->mediaPlayer->state() != QMediaPlayer::PlayingState)
                this->mediaPlayer->play();
            break;
        case 36:
            putText(image, "WanWeiHong", Point(face.x, face.y),
                    FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));
            break;
        case 37:
            putText(image, "ChenYongChao", Point(face.x, face.y),
                    FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));
            break;
        case 38:
            putText(image, "SuWenQing", Point(face.x, face.y),
                    FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));
            break;
        case 39:
            putText(image, "LiMingShi", Point(face.x, face.y),
                    FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));
            break;
        default:
            putText(image, "???", Point(face.x, face.y),
                    FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));
        }
    }

    QImage qImage = imageUtils->cvMat2QImage(image);
    ui->cameraShowLabel->setPixmap(QPixmap::fromImage(qImage));
}

void MainWindow::onCameraCloseSlot()
{
    ui->cameraShowLabel->clear();
}

void MainWindow::onCameraErrorSlot()
{
    QMessageBox::information(this, "警告", "设备出错");
    ui->cameraCBox->setEnabled(true);
    ui->cameraBtn->setText("启动");
    ui->cameraShowLabel->clear();
}

void MainWindow::startTrainSlot()
{
    ui->reTrainFaceBtn->setEnabled(false);
}

void MainWindow::finishTrainSlot(bool isSuccess)
{
    ui->reTrainFaceBtn->setEnabled(true);
    if(!isSuccess)
        QMessageBox::information(this, "警告", "样本训练失败");
}

void MainWindow::clearImgBtnSlot()
{
    ui->imgShowLabel->clear();
}

void MainWindow::choosePathBtnSlot()
{
    QString path, choosePath;

    path = ui->savePathEdit->text();

    if(QFileInfo(path).exists())
        choosePath = path;
    else
        choosePath = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)[0];

    choosePath = QFileDialog::getExistingDirectory(this, "选择截图保存路径", choosePath);

    if(choosePath.isEmpty())
        ui->savePathEdit->setText(path);
    else
        ui->savePathEdit->setText(choosePath);
}

void MainWindow::captureBtnSlot()
{
    if(ui->cameraCBox->isEnabled())
    {
        QMessageBox::information(this, "警告", "请打开摄像头后重试");
    }
    else
    {
        ui->imgShowLabel->setPixmap(*ui->cameraShowLabel->pixmap());
        QString imgSavePath = ui->savePathEdit->text();
        if(QFileInfo(imgSavePath).exists())
        {
            uint currentTime = QDateTime::currentDateTime().toTime_t();
            if(!imgSavePath.endsWith("/"))
                imgSavePath += "/";
            if(!imgSavePath.endsWith(".jpg"))
                imgSavePath += QString("%1.jpg").arg(currentTime);
            ui->cameraShowLabel->pixmap()->save(imgSavePath);
        }
        else
        {
            QMessageBox::information(this, "警告", "路径不存在，截图保存失败");
        }
    }
}

void MainWindow::openImgBtnSlot()
{
    QString path;
    if(ui->savePathEdit->text().isEmpty())
        path = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)[0];
    else
        path = ui->savePathEdit->text();

    if(QFileInfo(path).exists())
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    else
        QMessageBox::information(this, "警告", "当前路径不存在，请检查路径后重试");
}

