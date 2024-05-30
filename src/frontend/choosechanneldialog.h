#ifndef CHOOSECHANNELDIALOG_H
#define CHOOSECHANNELDIALOG_H

#include <QDialog>

namespace Ui {
class ChooseChannelDialog;
}

class ChooseChannelDialog : public QDialog {
    Q_OBJECT

  public:
    explicit ChooseChannelDialog(QWidget *parent = nullptr);
    ~ChooseChannelDialog();

  private slots:
    void on_buttonBox_accepted();

  public:
    const bool getRed() const;
    const bool getGreen() const;
    const bool getBlue() const;

  private:
    Ui::ChooseChannelDialog *ui;
    bool red = false;
    bool green = false;
    bool blue = false;
};

#endif // CHOOSECHANNELDIALOG_H
