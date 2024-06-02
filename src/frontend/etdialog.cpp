#include "etdialog.h"
#include "ui_etdialog.h"

#include <QDoubleValidator>

ETDialog::ETDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ETDialog) {
    ui->setupUi(this);
    QDoubleValidator *val = new QDoubleValidator(0, 10000.0, 5, this);
    val->setNotation(QDoubleValidator::StandardNotation);
    this->ui->lineEditC->setValidator(val);
    this->ui->lineEditA->setValidator(val);
}

ETDialog::~ETDialog() {
    delete ui;
}

void ETDialog::on_buttonBox_accepted() {
    this->red = this->ui->checkBoxRed->isChecked();
    this->green = this->ui->checkBoxGreen->isChecked();
    this->blue = this->ui->checkBoxBlue->isChecked();
    this->c = this->ui->lineEditC->text().replace(",", ".").toDouble();
    this->a = this->ui->lineEditA->text().replace(",", ".").toDouble();
}

const bool ETDialog::getRed() const {
    return this->red;
}

const bool ETDialog::getGreen() const {
    return this->green;
}

const bool ETDialog::getBlue() const {
    return this->blue;
}

const double ETDialog::getC() const {
    return this->c;
}

const double ETDialog::getA() const {
    return this->a;
}
