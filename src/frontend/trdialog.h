#ifndef TRDIALOG_H
#define TRDIALOG_H

#include <QDialog>

namespace Ui {
class TRDialog;
}

class TRDialog : public QDialog {
    Q_OBJECT

  public:
    explicit TRDialog(QWidget *parent = nullptr);
    ~TRDialog();
    const double getSX() const;
    const double getSY() const;
  private slots:
    void on_buttonBox_accepted();

  private:
    Ui::TRDialog *ui;

    double sX = 1;
    double sY = 1;
};

#endif // TRDIALOG_H
