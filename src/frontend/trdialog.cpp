#include "trdialog.h"
#include "ui_trdialog.h"

#include <QDoubleValidator>

TRDialog::TRDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TRDialog) {
    ui->setupUi(this);
    QDoubleValidator *val = new QDoubleValidator(0, 10000.0, 5, this);
    val->setNotation(QDoubleValidator::StandardNotation);
    this->ui->lineEditSx->setValidator(val);
    this->ui->lineEditSy->setValidator(val);
}

TRDialog::~TRDialog() {
    delete ui;
}

const double TRDialog::getSX() const {
    return this->sX;
}

const double TRDialog::getSY() const {
    return this->sY;
}

void TRDialog::on_buttonBox_accepted() {
    this->sX = this->ui->lineEditSx->text().replace(",", ".").toDouble();
    this->sY = this->ui->lineEditSy->text().replace(",", ".").toDouble();
}
