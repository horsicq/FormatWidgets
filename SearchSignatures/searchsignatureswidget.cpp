// copyright (c) 2019-2021 hors<horsicq@gmail.com>
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
#include "searchsignatureswidget.h"
#include "ui_searchsignatureswidget.h"

SearchSignaturesWidget::SearchSignaturesWidget(QWidget *pParent) :
    XShortcutsWidget(pParent),
    ui(new Ui::SearchSignaturesWidget)
{
    ui->setupUi(this);
    g_pDevice=nullptr;
    this->g_pParent=pParent;
    g_pFilter=new QSortFilterProxyModel(this);

    g_pModel=nullptr;
    g_bInit=false;

    g_scCopyName=nullptr;
    g_scCopySignature=nullptr;
    g_scCopyAddress=nullptr;
    g_scCopyOffset=nullptr;
    g_scHex=nullptr;

    ui->tableViewResult->installEventFilter(this);
}

SearchSignaturesWidget::~SearchSignaturesWidget()
{
    g_watcher.waitForFinished();

    delete ui;
}

void SearchSignaturesWidget::setData(QIODevice *pDevice, XBinary::FT fileType,OPTIONS options, bool bAuto, QWidget *pParent)
{
    this->g_pDevice=pDevice;
    this->g_pParent=pParent;
    g_bInit=false;

    QSet<XBinary::FT> stFileType=XBinary::getFileTypes(pDevice,true);
    stFileType.insert(XBinary::FT_COM);
    QList<XBinary::FT> listFileTypes=XBinary::_getFileTypeListFromSet(stFileType);

    XBinary::FT _fileType=XFormats::setFileTypeComboBox(ui->comboBoxType,&listFileTypes,fileType);
    XFormats::setEndianessComboBox(ui->comboBoxEndianness,_fileType);

    ui->tableViewResult->setModel(nullptr);

    g_fileType=fileType;

    setOptions(options);

    if(bAuto)
    {
        search();
    }
}

void SearchSignaturesWidget::setOptions(SearchSignaturesWidget::OPTIONS options)
{
    g_options=options;
    setSignaturesPath(options.sSignaturesPath);
}

SearchSignaturesWidget::OPTIONS SearchSignaturesWidget::getOptions()
{
    return g_options;
}

void SearchSignaturesWidget::setSignaturesPath(QString sPath)
{
    g_sPath=sPath;

    QSignalBlocker block(ui->comboBoxFile);

    ui->comboBoxFile->clear();

    if(g_options.sUserSignature!="")
    {
        ui->comboBoxFile->addItem("",g_options.sUserSignature);
    }

    QList<QString> listFiles=XBinary::getAllFilesFromDirectory(sPath,"*.db");

    int nNumberOfFiles=listFiles.count();

    for(int i=0;i<nNumberOfFiles;i++)
    {
        QString sFileName=listFiles.at(i);
        ui->comboBoxFile->addItem(XBinary::getBaseFileName(sFileName),sPath+QDir::separator()+sFileName);
    }

    if(g_options.sUserSignature!="")
    {
        loadSignatures("");
    }
    else if(nNumberOfFiles)
    {
        loadSignatures(ui->comboBoxFile->currentData().toString());
    }
}

void SearchSignaturesWidget::reload()
{
    search();
}

bool SearchSignaturesWidget::getInitStatus()
{
    return g_bInit;
}

void SearchSignaturesWidget::on_pushButtonSave_clicked()
{
    if(g_pModel)
    {
        QString sFileName=QFileDialog::getSaveFileName(this, tr("Save file"),QString("%1.txt").arg(tr("Signatures")), QString("%1 (*.txt);;%2 (*)").arg(tr("Text files")).arg(tr("All files")));

        if(!sFileName.isEmpty())
        {
            QFile file;
            file.setFileName(sFileName);

            if(file.open(QIODevice::ReadWrite))
            {
                int nNumberOfRows=g_pModel->rowCount();

                QString sResult;

                for(int i=0; i<nNumberOfRows; i++)
                {
                    sResult+=QString("%1\t%2\t%3\r\n")
                             .arg(g_pModel->item(i,0)->text())
                             .arg(g_pModel->item(i,1)->text())
                             .arg(g_pModel->item(i,2)->text());
                }

                file.resize(0);
                file.write(sResult.toLatin1().data());

                file.close();
            }
        }
    }
}

void SearchSignaturesWidget::on_pushButtonSearch_clicked()
{
    search();
}

void SearchSignaturesWidget::on_tableViewResult_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextMenu(this);

    QAction actionCopyString(tr("Copy name"),this);
    actionCopyString.setShortcut(getShortcuts()->getShortcut(XShortcuts::ID_SIGNATURES_COPYNAME));
    connect(&actionCopyString,SIGNAL(triggered()),this,SLOT(_copyName()));
    contextMenu.addAction(&actionCopyString);

    QAction actionCopySignature(tr("Copy signature"),this);
    actionCopySignature.setShortcut(getShortcuts()->getShortcut(XShortcuts::ID_SIGNATURES_COPYSIGNATURE));
    connect(&actionCopySignature,SIGNAL(triggered()),this,SLOT(_copySignature()));
    contextMenu.addAction(&actionCopySignature);

    QAction actionCopyAddress(tr("Copy address"),this);
    actionCopyAddress.setShortcut(getShortcuts()->getShortcut(XShortcuts::ID_SIGNATURES_COPYADDRESS));
    connect(&actionCopyAddress,SIGNAL(triggered()),this,SLOT(_copyAddress()));
    contextMenu.addAction(&actionCopyAddress);

    QAction actionCopyOffset(tr("Copy offset"),this);
    actionCopyOffset.setShortcut(getShortcuts()->getShortcut(XShortcuts::ID_SIGNATURES_COPYOFFSET));
    connect(&actionCopyOffset,SIGNAL(triggered()),this,SLOT(_copyOffset()));
    contextMenu.addAction(&actionCopyOffset);

    QAction actionHex(tr("Hex"),this);

    if(g_options.bMenu_Hex)
    {
        actionHex.setShortcut(getShortcuts()->getShortcut(XShortcuts::ID_SIGNATURES_HEX));
        connect(&actionHex,SIGNAL(triggered()),this,SLOT(_hex()));
        contextMenu.addAction(&actionHex);
    }

    contextMenu.exec(ui->tableViewResult->viewport()->mapToGlobal(pos));
}

void SearchSignaturesWidget::_copyName()
{
    qDebug("void SearchSignaturesWidget::_copyName()");

    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(g_pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(2);

        QString sString=ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchSignaturesWidget::_copySignature()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(g_pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(0);

        QString sString=ui->tableViewResult->model()->data(index,Qt::UserRole+3).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchSignaturesWidget::_copyAddress()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(g_pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(0);

        QString sString=ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchSignaturesWidget::_copyOffset()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(g_pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(1);

        QString sString=ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchSignaturesWidget::_hex()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(g_pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset=ui->tableViewResult->model()->data(index,Qt::UserRole+1).toLongLong();
        qint64 nSize=ui->tableViewResult->model()->data(index,Qt::UserRole+2).toLongLong();

        emit showHex(nOffset,nSize);
    }
}

void SearchSignaturesWidget::search()
{
    if(g_pDevice)
    {
        g_pOldModel=g_pModel;

        g_pFilter->setSourceModel(nullptr);
        ui->tableViewResult->setModel(nullptr);

        XBinary::FT fileType=(XBinary::FT)(ui->comboBoxType->currentData().toInt());

        MultiSearch::OPTIONS options={};

        options.bMenu_Hex=g_options.bMenu_Hex;
        options.memoryMap=XFormats::getMemoryMap(fileType,g_pDevice);
        options.bSignature=true;
        options.bIsBigEndian=(ui->comboBoxEndianness->currentData().toBool());
        options.pListSignatureRecords=&g_listSignatureRecords;

        QList<XBinary::MS_RECORD> listRecords;

        DialogMultiSearchProcess dsp(g_pParent);
        dsp.processSearch(g_pDevice,&listRecords,options,MultiSearch::TYPE_SIGNATURES);
        dsp.exec();

        DialogMultiSearchProcess dmp(g_pParent);
        dmp.processModel(&listRecords,&g_pModel,options,MultiSearch::TYPE_SIGNATURES);
        dmp.exec();

        g_pFilter->setSourceModel(g_pModel);
        ui->tableViewResult->setModel(g_pFilter);

        ui->tableViewResult->setColumnWidth(0,120);  // TODO
        ui->tableViewResult->setColumnWidth(1,120); // TODO
        ui->tableViewResult->setColumnWidth(2,120); // TODO

        QFuture<void> future=QtConcurrent::run(this,&SearchSignaturesWidget::deleteOldModel);

        g_watcher.setFuture(future);

//            watcher.waitForFinished();

        g_bInit=true;
    }
}

void SearchSignaturesWidget::deleteOldModel()
{
    delete g_pOldModel;
}

void SearchSignaturesWidget::loadSignatures(QString sFileName)
{
    int nNumberOfSignatures=0;

    g_listSignatureRecords.clear();

    if(sFileName!="")
    {
        g_listSignatureRecords=MultiSearch::loadSignaturesFromFile(sFileName);
        nNumberOfSignatures=g_listSignatureRecords.count();
    }
    else
    {
        // User signature
        g_listSignatureRecords.append(MultiSearch::createSignature(g_options.sUserSignature,g_options.sUserSignature));
        nNumberOfSignatures=1;
    }

    ui->labelInfo->setText(QString("%1: %2").arg(tr("Signatures")).arg(nNumberOfSignatures));
}

void SearchSignaturesWidget::on_comboBoxFile_currentIndexChanged(int index)
{
    Q_UNUSED(index)

    QString sFileName=ui->comboBoxFile->currentData().toString();

    loadSignatures(sFileName);
}

void SearchSignaturesWidget::registerShortcuts(bool bState)
{
    if(bState)
    {
        if(!g_scCopyName)           g_scCopyName        =new QShortcut(getShortcuts()->getShortcut(XShortcuts::ID_SIGNATURES_COPYNAME),         this,SLOT(_copyName()));
        if(!g_scCopySignature)      g_scCopySignature   =new QShortcut(getShortcuts()->getShortcut(XShortcuts::ID_SIGNATURES_COPYSIGNATURE),    this,SLOT(_copySignature()));
        if(!g_scCopyAddress)        g_scCopyAddress     =new QShortcut(getShortcuts()->getShortcut(XShortcuts::ID_SIGNATURES_COPYADDRESS),      this,SLOT(_copyAddress()));
        if(!g_scCopyOffset)         g_scCopyOffset      =new QShortcut(getShortcuts()->getShortcut(XShortcuts::ID_SIGNATURES_COPYOFFSET),       this,SLOT(_copyOffset()));
        if(!g_scHex)                g_scHex             =new QShortcut(getShortcuts()->getShortcut(XShortcuts::ID_SIGNATURES_HEX),              this,SLOT(_hex()));
    }
    else
    {
        if(g_scCopyName)            {delete g_scCopyName;           g_scCopyName=nullptr;}
        if(g_scCopySignature)       {delete g_scCopySignature;      g_scCopySignature=nullptr;}
        if(g_scCopyAddress)         {delete g_scCopyAddress;        g_scCopyAddress=nullptr;}
        if(g_scCopyOffset)          {delete g_scCopyOffset;         g_scCopyOffset=nullptr;}
        if(g_scHex)                 {delete g_scHex;                g_scHex=nullptr;}
    }
}
