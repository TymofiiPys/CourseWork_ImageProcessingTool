#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include "recentimages.h"

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
                                       QDir::homePath(),
                                       QString("Зображення (*.jpg *.jpeg *.png *.bmp *.gif)"));

    if (imagePath != "") {
        this->openImage(imagePath);
    }
}

void MainWindow::getRecentImagesToMenu() {
    QStringList recentImages = getRecentImages();
    ui->menuRecentlyOpen->clear();
    for (qsizetype i = recentImages.size() - 1; i >= 0; i--) {
        QAction *imageAction = new QAction(recentImages.at(i), this);
        connect(imageAction, &QAction::triggered, this, [imageAction, this]() {
            onRecentImagePathTriggered(imageAction->text());
        });
        imageAction->setIconVisibleInMenu(false);
        ui->menuRecentlyOpen->addAction(imageAction);
        this->recentImagesActions.push_back(imageAction);
    }
}

void MainWindow::openImage(QString &imagePath) {
    // Open image and save the filename to recents file
    this->openedImage = QImage(imagePath);
    savePathToFile(imagePath);
    this->openedImagePath = std::move(imagePath);

    // Show the image on a QGraphicsView
    QGraphicsScene *imageViewScene = new QGraphicsScene(this);
    imageViewScene->addPixmap(QPixmap::fromImage(this->openedImage));
    imageViewScene->setSceneRect(this->openedImage.rect());
    ui->imageView->setScene(imageViewScene);

    // Fit the image to the QGraphicsView
    int viewWidth = ui->imageView->width();
    int viewHeight = ui->imageView->height();
    int imageWidth = this->openedImage.rect().width();
    int imageHeight = this->openedImage.rect().height();
    qreal scaler = 1;
    qreal imageDimTakesViewDimAmt = 0.8;
    if (imageWidth > imageHeight) {
        scaler = imageDimTakesViewDimAmt * viewWidth / this->openedImage.rect().width();
    } else {
        scaler = imageDimTakesViewDimAmt * viewHeight / this->openedImage.rect().height();
    }
    ui->imageView->resetTransform();
    ui->imageView->scale(scaler, scaler);

    // Refresh recent images menu
    this->getRecentImagesToMenu();
}

void MainWindow::onRecentImagePathTriggered(QString filename) {
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
                                       this->openedImagePath.first(
                                           this->openedImagePath.lastIndexOf("/")),
                                       QString("Зображення (*.jpg *.jpeg *.png *.bmp *.gif)"));
    if (imagePath != "") {
        this->openedImage.save(imagePath);
        this->openedImagePath = imagePath;
    }
}
