#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <cmath>

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include "choosechanneldialog.h"
#include "imageprocessorwrapper.h"
#include "ltdialog.h"
#include "recentimages.h"
#include "rotatedialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->getRecentImagesToMenu();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionExit_triggered() {
    QApplication::exit();
}

void MainWindow::on_actionOpenFile_triggered() {
    QString imagePath
        = QFileDialog::getOpenFileName(this,
                                       QString("Відкрити файл..."),
                                       this->config.getRecentImgDir(),
                                       QString("Зображення (*.jpg *.jpeg *.png *.bmp *.gif)"));

    if (imagePath != "") {
        this->openImage(imagePath);
    }
}

void MainWindow::getRecentImagesToMenu() {
    QStringList recentImages = getRecentImages();
    ui->menuRecentlyOpen->clear();
    // Create actions for each recent image (list is read in reverse)
    for (auto i = recentImages.rbegin(); i != recentImages.rend(); ++i) {
        QAction *imageAction = new QAction(*i, this);
        connect(imageAction, &QAction::triggered, this, [imageAction, this]() {
            onRecentImagePathTriggered(imageAction->text());
        });
        imageAction->setIconVisibleInMenu(false);
        ui->menuRecentlyOpen->addAction(imageAction);
        this->recentImagesActions.push_back(imageAction);
    }
}

void MainWindow::openImage(const QString &imagePath) {
    // Open image and save the filename to recents file
    this->openedImage.load(imagePath);
    this->openedImage.convertTo(QImage::Format_ARGB32);
    savePathToFile(imagePath);
    this->openedImagePath = std::move(imagePath);

    // Show the image on a QGraphicsView
    imageViewScene = new QGraphicsScene(this);
    imageViewScene->addPixmap(QPixmap::fromImage(this->openedImage));
    imageViewScene->setSceneRect(this->openedImage.rect());
    ui->imageView->setScene(imageViewScene);

    // Fit the image to the QGraphicsView
    int viewWidth = ui->imageView->width();
    int viewHeight = ui->imageView->height();
    int imageWidth = this->openedImage.rect().width();
    int imageHeight = this->openedImage.rect().height();
    qreal scaler = 1;
    qreal zoomRatio = 0.8;
    if (imageWidth > imageHeight) {
        scaler = zoomRatio * viewWidth / this->openedImage.rect().width();
    } else {
        scaler = zoomRatio * viewHeight / this->openedImage.rect().height();
    }
    ui->imageView->resetTransform();
    ui->imageView->scale(scaler, scaler);

    // Refresh recent images menu
    this->getRecentImagesToMenu();
    // Update recent image directory
    this->config.setRecentImgDir(
        this->openedImagePath.first(this->openedImagePath.lastIndexOf("/")));
}

inline void MainWindow::onRecentImagePathTriggered(const QString filename) {
    this->openImage(filename);
}

void MainWindow::on_actionZoomIn_triggered() {
    if (!this->openedImage.isNull()) {
        qreal step = 1.2;
        ui->imageView->scale(step, step);
    }
}

void MainWindow::on_actionZoomOut_triggered() {
    if (!this->openedImage.isNull()) {
        qreal step = 0.8;
        ui->imageView->scale(step, step);
    }
}

void MainWindow::on_actionSave_triggered() {
    this->openedImage.save(this->openedImagePath);
}

void MainWindow::on_actionSaveAs_triggered() {
    QString imagePath
        = QFileDialog::getSaveFileName(this,
                                       QString("Зберегти як..."),
                                       this->config.getRecentImgDir(),
                                       QString("Зображення (*.jpg *.jpeg *.png *.bmp *.gif)"));
    if (imagePath != "") {
        this->openedImage.save(imagePath);
        this->openedImagePath = imagePath;
    }
}

void MainWindow::on_actionInvertColor_triggered() {
    ImageProcessorWrapper::invertColor(openedImage);
    imageViewScene->addPixmap(QPixmap::fromImage(this->openedImage));
    imageViewScene->setSceneRect(this->openedImage.rect());
    ui->imageView->setScene(imageViewScene);
}

void MainWindow::on_actionRotate_triggered() {
    RotateDialog dialog(this);
    double value = 0;
    if (dialog.exec() == QDialog::Accepted) {
        value = dialog.getValue();
        value = std::fmod(value, 360);
        if (value < 0)
            value += 360;
        QImage neww = ImageProcessorWrapper::rotateImage(openedImage, value);
        this->openedImage = neww;
        imageViewScene->addPixmap(QPixmap::fromImage(this->openedImage));
        imageViewScene->setSceneRect(this->openedImage.rect());
        ui->imageView->setScene(imageViewScene);
    }
    qDebug() << value;
}

void MainWindow::on_actionMirHor_triggered() {
    ImageProcessorWrapper::mirror(openedImage, true);
    imageViewScene->addPixmap(QPixmap::fromImage(this->openedImage));
    imageViewScene->setSceneRect(this->openedImage.rect());
    ui->imageView->setScene(imageViewScene);
}

void MainWindow::on_actionMirVer_triggered() {
    ImageProcessorWrapper::mirror(openedImage, false);
    imageViewScene->addPixmap(QPixmap::fromImage(this->openedImage));
    imageViewScene->setSceneRect(this->openedImage.rect());
    ui->imageView->setScene(imageViewScene);
}

void MainWindow::on_actionToGray_triggered() {
    ImageProcessorWrapper::toGray(openedImage);
    imageViewScene->addPixmap(QPixmap::fromImage(this->openedImage));
    imageViewScene->setSceneRect(this->openedImage.rect());
    ui->imageView->setScene(imageViewScene);
}

void MainWindow::on_actionHistEq_triggered() {
    ChooseChannelDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        const bool &red = dialog.getRed();
        const bool &green = dialog.getGreen();
        const bool &blue = dialog.getBlue();
        ImageProcessorWrapper::histEq(openedImage, red, green, blue);
        imageViewScene->addPixmap(QPixmap::fromImage(this->openedImage));
        imageViewScene->setSceneRect(this->openedImage.rect());
        ui->imageView->setScene(imageViewScene);
    }
}

void MainWindow::on_actionLogTransform_triggered() {
    LTDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        const bool &red = dialog.getRed();
        const bool &green = dialog.getGreen();
        const bool &blue = dialog.getBlue();
        const double &c = dialog.getC();
        ImageProcessorWrapper::logTransform(openedImage, c, red, green, blue);
        imageViewScene->addPixmap(QPixmap::fromImage(this->openedImage));
        imageViewScene->setSceneRect(this->openedImage.rect());
        ui->imageView->setScene(imageViewScene);
    }
}

void MainWindow::on_actionWeightAver_triggered() {
    ImageProcessorWrapper::weightedAverage(openedImage);
    imageViewScene->addPixmap(QPixmap::fromImage(this->openedImage));
    imageViewScene->setSceneRect(this->openedImage.rect());
    ui->imageView->setScene(imageViewScene);
}
