// copyright (c) 2019-2020 hors<horsicq@gmail.com>
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
    pFilter=new QSortFilterProxyModel(this);

    options.nAddressWidth=8;
    options.nBaseAddress=0;
    pModel=nullptr;
    bInit=false;

    ui->checkBoxAnsi->setChecked(true);
    ui->checkBoxUnicode->setChecked(true);
}

SearchStringsWidget::~SearchStringsWidget()
{
    delete ui;
}

void SearchStringsWidget::setData(QIODevice *pDevice, SearchStrings::OPTIONS *pOptions, bool bAuto)
{
    this->pDevice=pDevice;
    bInit=false;

    qint64 nSize=pDevice->size();

    ui->tableViewResult->setModel(nullptr);

    if(pOptions)
    {
        options=*pOptions;
    }
    else
    {
        options.nBaseAddress=0;
    }

    if(options.nBaseAddress==-1)
    {
        options.nBaseAddress=0;
    }

    if(nSize+options.nBaseAddress>0xFFFFFFFF)
    {
        options.nAddressWidth=16;
    }
    else
    {
        options.nAddressWidth=8;
    }

    QString sInfo=QString("0x%1 - 0x%2 ( 0x%3 )").arg(options.nBaseAddress,options.nAddressWidth,16,QChar('0'))
                  .arg(options.nBaseAddress+nSize-1,options.nAddressWidth,16,QChar('0'))
                  .arg(nSize,8,16,QChar('0'));
    ui->labelInfo->setText(sInfo);

    if(bAuto)
    {
        search();
    }
}

void SearchStringsWidget::reload()
{
    search();
}

bool SearchStringsWidget::getInitStatus()
{
    return bInit;
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

                for(int i=0; i<nCount; i++)
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
    pFilter->setFilterRegExp(sString);
    pFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    pFilter->setFilterKeyColumn(3);
}

void SearchStringsWidget::on_tableViewResult_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextMenu(this);

    QAction actionCopyString(QString("%1 %2").arg(tr("Copy")).arg(tr("String")),this);
    connect(&actionCopyString,SIGNAL(triggered()),this,SLOT(_copyString()));
    contextMenu.addAction(&actionCopyString);

    QAction actionCopyAddress(QString("%1 %2").arg(tr("Copy")).arg((options.nBaseAddress?(tr("Address")):(tr("Offset")))),this);
    connect(&actionCopyAddress,SIGNAL(triggered()),this,SLOT(_copyAddress()));
    contextMenu.addAction(&actionCopyAddress);

    QAction actionCopySize(QString("%1 %2").arg(tr("Copy")).arg(tr("Size")),this);
    connect(&actionCopySize,SIGNAL(triggered()),this,SLOT(_copySize()));
    contextMenu.addAction(&actionCopySize);

    contextMenu.exec(ui->tableViewResult->viewport()->mapToGlobal(pos));
}

void SearchStringsWidget::_copyString()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(3);

        QString sString=ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchStringsWidget::_copyAddress()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(0);

        QString sString=ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchStringsWidget::_copySize()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(1);

        QString sString=ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchStringsWidget::search()
{
    if(pDevice)
    {
        ui->lineEditFilter->clear();

        options.bSearchAnsi=ui->checkBoxAnsi->isChecked();
        options.bSearchUnicode=ui->checkBoxUnicode->isChecked();

        if(options.bSearchAnsi||options.bSearchUnicode)
        {
            QList<SearchStrings::RECORD> listRecords;

            DialogSearchStrings ds(this);
            ds.processSearch(pDevice,&listRecords,&options);
            ds.exec();

        //    QAbstractItemModel *pOldFilter=pModel;

            pFilter->setSourceModel(0);
            ui->tableViewResult->setModel(0);

            DialogSearchStrings dm(this);
            dm.processModel(&listRecords,&pModel,&options);
            dm.exec();

            pFilter->setSourceModel(pModel);
            ui->tableViewResult->setModel(pFilter);

//            delete pOldFilter; // TODO delete in Thread

            ui->tableViewResult->setColumnWidth(0,120);  // TODO
            ui->tableViewResult->setColumnWidth(1,60); // TODO
            ui->tableViewResult->setColumnWidth(2,30); // TODO
        }

        bInit=true;
    }
}
