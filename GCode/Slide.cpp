#include "Slide.h"
#include "ui_Slide.h"

Slide::Slide(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Slide)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Slide::~Slide()
{
    delete ui;
}

void Slide::progressAdd(int percent)
{
    ui->progressBar->setValue(ui->progressBar->value() + percent);
}

void Slide::progressDec(int percent)
{
    ui->progressBar->setValue(ui->progressBar->value() - percent);
}
