#ifndef ETDIALOG_H
#define ETDIALOG_H

#include <QDialog>

namespace Ui {
class ETDialog;
}

class ETDialog : public QDialog {
    Q_OBJECT

  public:
    explicit ETDialog(QWidget *parent = nullptr);
    ~ETDialog();

  public:
    const bool getRed() const;
    const bool getGreen() const;
    const bool getBlue() const;
    const double getC() const;
    const double getA() const;

  private slots:
    void on_buttonBox_accepted();

  private:
    Ui::ETDialog *ui;

    bool red = false;
    bool green = false;
    bool blue = false;
    double c = 0;
    double a = 0;
};

#endif // ETDIALOG_H
