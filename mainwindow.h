#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  /*!
   * \brief Exit program and prompt for saving
   * \internal
   */
  void on_actionExit_triggered();

  /*!
   * \brief Open file for processing, prompt for saving already opened one
   * \internal
   */
  void on_actionOpenFile_triggered();

private:
  Ui::MainWindow *ui;

    QImage openedImage;
};
#endif // MAINWINDOW_H
