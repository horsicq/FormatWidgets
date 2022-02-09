/* Copyright (c) 2019-2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "searchsignatureswidget.h"
#include "ui_searchsignatureswidget.h"

SearchSignaturesWidget::SearchSignaturesWidget(QWidget *pParent) :
    XShortcutsWidget(pParent),
    ui(new Ui::SearchSignaturesWidget)
{
    ui->setupUi(this);
    g_pDevice=nullptr;
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

void SearchSignaturesWidget::setData(QIODevice *pDevice, XBinary::FT fileType,OPTIONS options, bool bAuto)
{
    this->g_pDevice=pDevice;
    g_bInit=false;

    XFormats::setFileTypeComboBox(fileType,g_pDevice,ui->comboBoxType);

    // TODO get endianess XFormats;
    XFormats::setEndianessComboBox(ui->comboBoxEndianness,false);

    ui->tableViewResult->setModel(nullptr);

    setOptions(options);

    if(bAuto)
    {
        search();
    }
}

void SearchSignaturesWidget::setOptions(SearchSignaturesWidget::OPTIONS options)
{
    g_options=options;
    adjust();
}

SearchSignaturesWidget::OPTIONS SearchSignaturesWidget::getOptions()
{
    return g_options;
}

void SearchSignaturesWidget::updateSignaturesPath()
{
#if QT_VERSION >= QT_VERSION_CHECK(5,3,0)
    const QSignalBlocker block(ui->comboBoxFile);
#else
    const bool bBlocked1=ui->comboBoxFile->blockSignals(true);
#endif

    ui->comboBoxFile->clear();

    if(g_options.sUserSignature!="")
    {
        ui->comboBoxFile->addItem("",g_options.sUserSignature);
    }

    QList<QString> listFiles=XBinary::getAllFilesFromDirectory(XBinary::convertPathName(getGlobalOptions()->getSearchSignaturesPath()),"*.db");

    qint32 nNumberOfFiles=listFiles.count();

    for(qint32 i=0;i<nNumberOfFiles;i++)
    {
        QString sFileName=listFiles.at(i);
        ui->comboBoxFile->addItem(XBinary::getBaseFileName(sFileName),XBinary::convertPathName(getGlobalOptions()->getSearchSignaturesPath())+QDir::separator()+sFileName);
    }

    if(g_options.sUserSignature!="")
    {
        loadSignatures("");
    }
    else if(nNumberOfFiles)
    {
        loadSignatures(ui->comboBoxFile->currentData().toString());
    }
#if QT_VERSION < 0x050300
    ui->comboBoxFile->blockSignals(bBlocked1);
#endif
}

void SearchSignaturesWidget::reload()
{
    search();
}

bool SearchSignaturesWidget::getInitStatus()
{
    return g_bInit;
}

void SearchSignaturesWidget::adjust()
{
    updateSignaturesPath();
}

void SearchSignaturesWidget::on_pushButtonSave_clicked()
{
    if(g_pModel)
    {
        XShortcutsWidget::saveModel(g_pModel,XBinary::getResultFileName(g_pDevice,QString("%1.txt").arg(tr("Signatures"))));
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
    qint32 nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(g_pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(2);

        QString sString=ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchSignaturesWidget::_copySignature()
{
    qint32 nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(g_pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(0);

        QString sString=ui->tableViewResult->model()->data(index,Qt::UserRole+MultiSearch::USERROLE_STRING).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchSignaturesWidget::_copyAddress()
{
    qint32 nRow=ui->tableViewResult->currentIndex().row();

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

        qint64 nOffset=ui->tableViewResult->model()->data(index,Qt::UserRole+MultiSearch::USERROLE_OFFSET).toLongLong();
        qint64 nSize=ui->tableViewResult->model()->data(index,Qt::UserRole+MultiSearch::USERROLE_SIZE).toLongLong();

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

        QWidget *pParent=XOptions::getMainWidget(this);

        DialogMultiSearchProcess dsp(pParent);
        dsp.processSearch(g_pDevice,&listRecords,options,MultiSearch::TYPE_SIGNATURES);
        dsp.exec();

        DialogMultiSearchProcess dmp(pParent);
        dmp.processModel(&listRecords,&g_pModel,options,MultiSearch::TYPE_SIGNATURES);
        dmp.exec();

        g_pFilter->setSourceModel(g_pModel);
        ui->tableViewResult->setModel(g_pFilter);

        ui->tableViewResult->setColumnWidth(0,120);  // TODO
        ui->tableViewResult->setColumnWidth(1,120); // TODO
        ui->tableViewResult->setColumnWidth(2,120); // TODO

    #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        QFuture<void> future=QtConcurrent::run(&SearchSignaturesWidget::deleteOldModel,this);
    #else
        QFuture<void> future=QtConcurrent::run(this,&SearchSignaturesWidget::deleteOldModel);
    #endif

        g_watcher.setFuture(future);

//            watcher.waitForFinished();

        g_bInit=true;
    }
}

void SearchSignaturesWidget::deleteOldModel()
{
    if(g_pOldModel)
    {
        delete g_pOldModel;

        g_pOldModel=0;
    }
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

    ui->labelInfo->setText(QString("%1: %2").arg(tr("Signatures"),QString::number(nNumberOfSignatures)));
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
