#include "dialogelf.h"
#include "ui_dialogelf.h"

DialogELF::DialogELF(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogELF)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
}

DialogELF::~DialogELF()
{
    delete ui;
}

void DialogELF::setData(QIODevice *pDevice, FormatWidget::OPTIONS *pOptions)
{
    ui->widget->setData(pDevice,pOptions);
}
