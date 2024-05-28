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
        this->openedImage = QImage(imagePath);
        savePathToFile(imagePath);
        this->openedImagePath = std::move(imagePath);

        QGraphicsScene *imageViewScene = new QGraphicsScene(this);
        imageViewScene->addPixmap(QPixmap::fromImage(this->openedImage));
        imageViewScene->setSceneRect(this->openedImage.rect());
        ui->imageView->setScene(imageViewScene);
        int viewWidth = ui->imageView->width();
        int viewHeight = ui->imageView->height();
        int imageWidth = this->openedImage.rect().width();
        int imageHeight = this->openedImage.rect().height();
        qreal scaler = 1;
        if (imageWidth > imageHeight) {
            scaler = 0.8 * viewWidth / this->openedImage.rect().width();
        } else {
            scaler = 0.8 * viewHeight / this->openedImage.rect().height();
        }
        ui->imageView->resetTransform();
        ui->imageView->scale(scaler, scaler);
    }
}

void MainWindow::getRecentImagesToMenu() {
    QStringList recentImages = getRecentImages();
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

void MainWindow::onRecentImagePathTriggered(QString filename) {
    QMessageBox msgBox;
    msgBox.setText(filename);
    msgBox.exec();
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
