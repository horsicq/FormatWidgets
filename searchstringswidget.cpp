// copyright (c) 2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "searchstringswidget.h"
#include "ui_searchstringswidget.h"

SearchStringsWidget::SearchStringsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchStringsWidget)
{
    ui->setupUi(this);
    pDevice=nullptr;
    pOptions=nullptr;
    pFilter=new QSortFilterProxyModel(this);
    nAddressWidth=8;
    nBaseAddress=0;
    pModel=nullptr;
}

SearchStringsWidget::~SearchStringsWidget()
{
    delete ui;
}

void SearchStringsWidget::setData(QIODevice *pDevice, SearchStrings::OPTIONS *pOptions, bool bAuto)
{
    this->pDevice=pDevice;
    this->pOptions=pOptions;

    qint64 nSize=pDevice->size();

    if(pOptions)
    {
        nBaseAddress=pOptions->nBaseAddress;
    }
    else
    {
        nBaseAddress=0;
    }

    if(nSize+nBaseAddress>0xFFFFFFFF)
    {
        nAddressWidth=16;
    }
    else
    {
        nAddressWidth=8;
    }

    QString sInfo=QString("0x%1 - 0x%2 ( 0x%3 )").arg(nBaseAddress,nAddressWidth,16,QChar('0'))
                                        .arg(nBaseAddress+nSize-1,nAddressWidth,16,QChar('0'))
                                        .arg(nSize,8,16,QChar('0'));

    ui->labelInfo->setText(sInfo);

    if(bAuto)
    {
        search();
    }
}

void SearchStringsWidget::on_pushButtonSave_clicked()
{
    if(pModel)
    {
        QString sFileName=QFileDialog::getSaveFileName(this, tr("Save file"),QString("%1.txt").arg(tr("Strings")), QString("%1 (*.txt);;%2 (*)").arg(tr("Text files")).arg(tr("All files")));

        if(!sFileName.isEmpty())
        {
            QFile file;
            file.setFileName(sFileName);

            if(file.open(QIODevice::ReadWrite))
            {
                int nCount=pModel->rowCount();

                QString sResult;
                for(int i=0;i<nCount;i++)
                {
                    sResult+=QString("%1\t%2\t%3\t%4\r\n")
                            .arg(pModel->item(i,0)->text())
                            .arg(pModel->item(i,1)->text())
                            .arg(pModel->item(i,2)->text())
                            .arg(pModel->item(i,3)->text());
                }

                file.resize(0);
                file.write(sResult.toLatin1().data());

                file.close();
            }
        }
    }
}

void SearchStringsWidget::on_pushButtonSearch_clicked()
{
    search();
}

void SearchStringsWidget::on_lineEditFilter_textChanged(const QString &arg1)
{
    filter(arg1);
}

void SearchStringsWidget::filter(QString sString)
{
    if(sString!="")
    {
        pFilter->setFilterRegExp(sString);
        pFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
        pFilter->setFilterKeyColumn(3);
    }
}

void SearchStringsWidget::on_tableViewResult_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextMenu(this);

    QAction actionCopyString(tr("Copy String"),this);
    connect(&actionCopyString,SIGNAL(triggered()),this,SLOT(_copyString()));
    contextMenu.addAction(&actionCopyString);

    QAction actionCopyAddress(QString("%1 %2").arg(tr("Copy")).arg((nBaseAddress?(tr("Address")):(tr("Offset")))),this);
    connect(&actionCopyAddress,SIGNAL(triggered()),this,SLOT(_copyAddress()));
    contextMenu.addAction(&actionCopyAddress);

    QAction actionCopySize(tr("Copy Size"),this);
    connect(&actionCopySize,SIGNAL(triggered()),this,SLOT(_copySize()));
    contextMenu.addAction(&actionCopySize);

    contextMenu.exec(ui->tableViewResult->viewport()->mapToGlobal(pos));
}

void SearchStringsWidget::_copyString()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(pModel))
    {
        QApplication::clipboard()->setText(pModel->item(nRow,3)->text());
    }
}

void SearchStringsWidget::_copyAddress()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(pModel))
    {
        QApplication::clipboard()->setText(pModel->item(nRow,0)->text());
    }
}

void SearchStringsWidget::_copySize()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(pModel))
    {
        QApplication::clipboard()->setText(pModel->item(nRow,1)->text());
    }
}

void SearchStringsWidget::search()
{
    if(pDevice)
    {
        ui->lineEditFilter->clear();

        QList<SearchStrings::RECORD> listRecords;
        DialogSearchStrings ds(this);
        ds.process(pDevice,&listRecords,pOptions);
        ds.exec();

        int nCount=listRecords.count();

        pModel=new QStandardItemModel(nCount,4);

        pModel->setHeaderData(0,Qt::Horizontal,nBaseAddress?(tr("Address")):(tr("Offset")));
        pModel->setHeaderData(1,Qt::Horizontal,tr("Size"));
        pModel->setHeaderData(2,Qt::Horizontal,tr("Type"));
        pModel->setHeaderData(3,Qt::Horizontal,tr("String"));

        for(int i=0;i<nCount;i++)
        {
            SearchStrings::RECORD record=listRecords.at(i);

            QStandardItem *pTypeAddress=new QStandardItem;
            pTypeAddress->setText(QString("%1").arg(record.nOffset,nAddressWidth,16,QChar('0')));
            pTypeAddress->setTextAlignment(Qt::AlignRight);
            pModel->setItem(i,0,pTypeAddress);

            QStandardItem *pTypeSize=new QStandardItem;
            pTypeSize->setText(QString("%1").arg(record.nSize,8,16,QChar('0')));
            pTypeSize->setTextAlignment(Qt::AlignRight);
            pModel->setItem(i,1,pTypeSize);

            QStandardItem *pTypeItem=new QStandardItem;
            if(record.recordType==SearchStrings::RECORD_TYPE_ANSI)
            {
                pTypeItem->setText("A");
            }
            else if(record.recordType==SearchStrings::RECORD_TYPE_UNICODE)
            {
                pTypeItem->setText("U");
            }
            pModel->setItem(i,2,pTypeItem);
            pModel->setItem(i,3,new QStandardItem(record.sString));
        }
        pFilter->setSourceModel(pModel);
        ui->tableViewResult->setModel(pFilter);
        ui->tableViewResult->setColumnWidth(0,120);
        ui->tableViewResult->setColumnWidth(1,60);
        ui->tableViewResult->setColumnWidth(2,30);
    }
}
