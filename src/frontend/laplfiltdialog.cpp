#include "laplfiltdialog.h"
#include "ui_laplfiltdialog.h"

LaplFiltDialog::LaplFiltDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LaplFiltDialog) {
    ui->setupUi(this);
}

LaplFiltDialog::~LaplFiltDialog() {
    delete ui;
}

void LaplFiltDialog::on_buttonBox_accepted() {
    this->c = this->ui->lineEditC->text().toDouble();
}

const double LaplFiltDialog::getC() const {
    return this->c;
}
