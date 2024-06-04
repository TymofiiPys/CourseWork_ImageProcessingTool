#include "gfdialog.h"
#include "ui_gfdialog.h"

GFDialog::GFDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GFDialog) {
    ui->setupUi(this);
}

GFDialog::~GFDialog() {
    delete ui;
}

void GFDialog::on_buttonBox_accepted() {
    this->radius = this->ui->lineEditRadius->text().toInt();
}

const int GFDialog::getRadius() const {
    return this->radius;
}
