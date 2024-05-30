#include "choosechanneldialog.h"
#include "ui_choosechanneldialog.h"

ChooseChannelDialog::ChooseChannelDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChooseChannelDialog) {
    ui->setupUi(this);
}

ChooseChannelDialog::~ChooseChannelDialog() {
    delete ui;
}

void ChooseChannelDialog::on_buttonBox_accepted() {
    this->red = this->ui->checkBoxRed->isChecked();
    this->green = this->ui->checkBoxGreen->isChecked();
    this->blue = this->ui->checkBoxBlue->isChecked();
}

const bool ChooseChannelDialog::getRed() const {
    return this->red;
}

const bool ChooseChannelDialog::getGreen() const {
    return this->green;
}

const bool ChooseChannelDialog::getBlue() const {
    return this->blue;
}
