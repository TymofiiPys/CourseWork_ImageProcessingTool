#ifndef LTDIALOG_H
#define LTDIALOG_H

#include <QDialog>

namespace Ui {
class LTDialog;
}

class LTDialog : public QDialog {
    Q_OBJECT

  public:
    explicit LTDialog(QWidget *parent = nullptr);
    ~LTDialog();

  public:
    const bool getRed() const;
    const bool getGreen() const;
    const bool getBlue() const;
    const double getC() const;
  private slots:
    void on_buttonBox_accepted();

  private:
    Ui::LTDialog *ui;

    bool red = false;
    bool green = false;
    bool blue = false;
    double c = 0;
};

#endif // LTDIALOG_H
