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

    this->openedImage = QImage(imagePath);
    std::string imgPath = imagePath.toStdString();
    savePathToFile(imgPath);

    QGraphicsScene *imageViewScene = new QGraphicsScene(this);
    imageViewScene->addPixmap(QPixmap::fromImage(this->openedImage));
    imageViewScene->setSceneRect(this->openedImage.rect());
    ui->imageView->setScene(imageViewScene);
    ui->imageView->scale(0.25, 0.25);
}

void MainWindow::getRecentImagesToMenu() {
    std::forward_list<std::string> recentImages = getRecentImages();
    for (std::string &image : recentImages) {
        QAction *imageAction = new QAction(QString::fromStdString(image), this);
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
