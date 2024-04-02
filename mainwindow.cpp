#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionExit_triggered() { QApplication::exit(); }

void MainWindow::on_actionOpenFile_triggered() {
  QString imagePath = QFileDialog::getOpenFileName(
      this, QString("Відкрити файл..."), QDir::homePath(),
      QString("Зображення (*.jpg *.jpeg *.png *.bmp *.gif)"));

  this->openedImage = QImage(imagePath);

  QGraphicsScene* imageViewScene = new QGraphicsScene(this);
  imageViewScene->addPixmap(QPixmap::fromImage(this->openedImage));
  imageViewScene->setSceneRect(this->openedImage.rect());
  ui->imageView->setScene(imageViewScene);
  ui->imageView->scale(0.25, 0.25);
}
