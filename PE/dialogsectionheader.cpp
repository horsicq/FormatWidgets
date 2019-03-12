#include "dialogsectionheader.h"
#include "ui_dialogsectionheader.h"

DialogSectionHeader::DialogSectionHeader(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSectionHeader)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
}

DialogSectionHeader::~DialogSectionHeader()
{
    delete ui;
}

void DialogSectionHeader::setData(QIODevice *pDevice, FormatWidget::OPTIONS *pOptions, quint32 nNumber)
{
    ui->widget->setData(pDevice,pOptions,nNumber);
}
