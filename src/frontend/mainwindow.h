#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../config/iptconfigmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// TODO: config
// TODO: memento pattern for storing image versions
class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    /*!
     * \brief Exit program and prompt for saving
     */
    void on_actionExit_triggered();

    /*!
     * \brief Open file for processing, prompt for saving already opened one
     * \internal
     */
    void on_actionOpenFile_triggered();

    /*!
     * \brief opens an image wil filepath show in menu
     * \param filename filename chosen for opening
     */
    void onRecentImagePathTriggered(QString filename);

    void on_actionZoomIn_triggered();

    void on_actionZoomOut_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

  private:
    Ui::MainWindow *ui;

    IPTConfigManager *config;

    QString openedImagePath;
    /*!
     * \brief openedImage is a currently opened image
     */
    QImage openedImage;

    std::vector<QAction *> recentImagesActions;

    /*!
     * \brief shows up recent images filepaths in the menu
     */
    void getRecentImagesToMenu();

    void openImage(QString &filename);
};
#endif // MAINWINDOW_H
