#include "dialogpe.h"
#include "ui_dialogpe.h"

DialogPE::DialogPE(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPE)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
}

DialogPE::~DialogPE()
{
    delete ui;
}

void DialogPE::setData(QIODevice *pDevice, FormatWidget::OPTIONS *pOptions)
{
    ui->widget->setData(pDevice,pOptions);
}
