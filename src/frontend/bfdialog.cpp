#include "bfdialog.h"
#include "ui_bfdialog.h"

BFDialog::BFDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BFDialog) {
    ui->setupUi(this);
}

BFDialog::~BFDialog() {
    delete ui;
}

void BFDialog::on_buttonBox_accepted() {
    this->size = this->ui->lineEditSize->text().toInt();
    this->coef = this->ui->lineEditSize->text().toDouble();
}

const int BFDialog::getSize() const {
    return this->size;
}

const double BFDialog::getCoef() const {
    return this->coef;
}
