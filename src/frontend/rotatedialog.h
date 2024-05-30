#ifndef ROTATEDIALOG_H
#define ROTATEDIALOG_H

#include <QDialog>

namespace Ui {
class RotateDialog;
}

class RotateDialog : public QDialog {
    Q_OBJECT

  public:
    explicit RotateDialog(QWidget *parent = nullptr);
    ~RotateDialog();

  private slots:
    void on_buttonBox_accepted();

  public:
    double getValue() const;

  private:
    Ui::RotateDialog *ui;
    double value;
};

#endif // ROTATEDIALOG_H
