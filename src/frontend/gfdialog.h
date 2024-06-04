#ifndef GFDIALOG_H
#define GFDIALOG_H

#include <QDialog>

namespace Ui {
class GFDialog;
}

class GFDialog : public QDialog {
    Q_OBJECT

  public:
    explicit GFDialog(QWidget *parent = nullptr);
    ~GFDialog();

  private slots:
    void on_buttonBox_accepted();

  public:
    const int getRadius() const;

  private:
    Ui::GFDialog *ui;

    int radius;
};

#endif // GFDIALOG_H
