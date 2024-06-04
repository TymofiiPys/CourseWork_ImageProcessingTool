#ifndef BFDIALOG_H
#define BFDIALOG_H

#include <QDialog>

namespace Ui {
class BFDialog;
}

class BFDialog : public QDialog {
    Q_OBJECT

  public:
    explicit BFDialog(QWidget *parent = nullptr);
    ~BFDialog();

  private slots:
    void on_buttonBox_accepted();

  public:
    const int getSize() const;
    const double getCoef() const;

  private:
    Ui::BFDialog *ui;

    int size = 1;
    double coef = 1;
};

#endif // BFDIALOG_H
