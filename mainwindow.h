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
     */
    void onActionExitTriggered();

    /*!
     * \brief Open file for processing, prompt for saving already opened one
     * \internal
     */
    void onActionOpenFileTriggered();

    /*!
     * \brief opens an image wil filepath show in menu
     * \param filename filename chosen for opening
     */
    void onRecentImagePathTriggered(QString filename);

  private:
    Ui::MainWindow *ui;

    /*!
     * \brief openedImage is a currently opened image
     */
    QImage openedImage;

    std::vector<QAction *> recentImagesActions;

    /*!
     * \brief shows up recent images filepaths in the menu
     */
    void getRecentImagesToMenu();
};
#endif // MAINWINDOW_H
