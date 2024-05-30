#include "rotatedialog.h"
#include "ui_rotatedialog.h"

#include <QDoubleValidator>

RotateDialog::RotateDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RotateDialog) {
    ui->setupUi(this);
    this->value = 0;
    QDoubleValidator *val = new QDoubleValidator(-180.0, 180.0, 2, this);
    val->setNotation(QDoubleValidator::StandardNotation);
    this->ui->degreeLE->setValidator(val);
}

RotateDialog::~RotateDialog() {
    delete ui;
}

void RotateDialog::on_buttonBox_accepted() {
    this->value = this->ui->degreeLE->text().replace(",", ".").toDouble();
}

double RotateDialog::getValue() const {
    return this->value;
}
