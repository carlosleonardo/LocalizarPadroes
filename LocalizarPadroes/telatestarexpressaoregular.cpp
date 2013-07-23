#include "telatestarexpressaoregular.h"
#include "ui_telatestarexpressaoregular.h"

TelaTestarExpressaoRegular::TelaTestarExpressaoRegular(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TelaTestarExpressaoRegular)
{
    ui->setupUi(this);
}

TelaTestarExpressaoRegular::~TelaTestarExpressaoRegular()
{
    delete ui;
}
