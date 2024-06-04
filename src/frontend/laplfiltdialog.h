#ifndef LAPLFILTDIALOG_H
#define LAPLFILTDIALOG_H

#include <QDialog>

namespace Ui {
class LaplFiltDialog;
}

class LaplFiltDialog : public QDialog {
    Q_OBJECT

  public:
    explicit LaplFiltDialog(QWidget *parent = nullptr);
    ~LaplFiltDialog();

  private slots:
    void on_buttonBox_accepted();

  public:
    const double getC() const;

  private:
    Ui::LaplFiltDialog *ui;

    double c = 1;
};

#endif // LAPLFILTDIALOG_H
