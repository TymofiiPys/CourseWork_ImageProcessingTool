#include "ltdialog.h"
#include "ui_ltdialog.h"

#include <QDoubleValidator>

LTDialog::LTDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LTDialog) {
    ui->setupUi(this);
    QDoubleValidator *val = new QDoubleValidator(0, 10000.0, 5, this);
    val->setNotation(QDoubleValidator::StandardNotation);
    this->ui->lineEditC->setValidator(val);
}

LTDialog::~LTDialog() {
    delete ui;
}

void LTDialog::on_buttonBox_accepted() {
    this->red = this->ui->checkBoxRed->isChecked();
    this->green = this->ui->checkBoxGreen->isChecked();
    this->blue = this->ui->checkBoxBlue->isChecked();
    this->c = this->ui->lineEditC->text().replace(",", ".").toDouble();
}

const bool LTDialog::getRed() const {
    return this->red;
}

const bool LTDialog::getGreen() const {
    return this->green;
}

const bool LTDialog::getBlue() const {
    return this->blue;
}

const double LTDialog::getC() const {
    return this->c;
}
