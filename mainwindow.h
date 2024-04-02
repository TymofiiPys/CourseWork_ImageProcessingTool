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
  void onActionExitTriggered();

  /*!
   * \brief Open file for processing, prompt for saving already opened one
   * \internal
   */
  void onActionOpenFileTriggered();

  void onRecentImagePathTriggered(QString& filename);

private:
  Ui::MainWindow *ui;

  QImage openedImage;

  std::vector<QAction *> recentImagesActions;

  void getRecentImagesToMenu();
};
#endif // MAINWINDOW_H
