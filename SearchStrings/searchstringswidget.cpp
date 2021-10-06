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
#include "searchstringswidget.h"
#include "ui_searchstringswidget.h"

SearchStringsWidget::SearchStringsWidget(QWidget *pParent) :
    XShortcutsWidget(pParent),
    ui(new Ui::SearchStringsWidget)
{
    ui->setupUi(this);
    g_pDevice=nullptr;
    g_pFilter=new QSortFilterProxyModel(this);

    g_options.nBaseAddress=0;
    g_pModel=nullptr;
    g_bInit=false;

    g_options={};

    ui->checkBoxCStrings->setEnabled(false);

    ui->checkBoxAnsi->setChecked(true);
    ui->checkBoxUTF8->setChecked(false);
    ui->checkBoxUnicode->setChecked(true);
    ui->checkBoxCStrings->setChecked(false);

    ui->spinBoxMinLength->setMinimum(2);
    ui->spinBoxMinLength->setValue(5);

    g_scCopyString=nullptr;
    g_scCopyOffset=nullptr;
    g_scCopySize=nullptr;
    g_scHex=nullptr;
    g_scDemangle=nullptr;

    // TODO Check Qt6
    ui->comboBoxANSICodec->addItem("");
    ui->comboBoxANSICodec->addItem("IBM866");
    ui->comboBoxANSICodec->addItem("ISO-8859-1");
    ui->comboBoxANSICodec->addItem("ISO-8859-10");
    ui->comboBoxANSICodec->addItem("ISO-8859-13");
    ui->comboBoxANSICodec->addItem("ISO-8859-14");
    ui->comboBoxANSICodec->addItem("ISO-8859-15");
    ui->comboBoxANSICodec->addItem("ISO-8859-16");
    ui->comboBoxANSICodec->addItem("ISO-8859-2");
    ui->comboBoxANSICodec->addItem("ISO-8859-3");
    ui->comboBoxANSICodec->addItem("ISO-8859-4");
    ui->comboBoxANSICodec->addItem("ISO-8859-5");
    ui->comboBoxANSICodec->addItem("ISO-8859-6");
    ui->comboBoxANSICodec->addItem("ISO-8859-7");
    ui->comboBoxANSICodec->addItem("ISO-8859-8");
    ui->comboBoxANSICodec->addItem("ISO-8859-9");
    ui->comboBoxANSICodec->addItem("KOI8-R");
    ui->comboBoxANSICodec->addItem("KOI8-U");
    ui->comboBoxANSICodec->addItem("windows-1250");
    ui->comboBoxANSICodec->addItem("windows-1251");
    ui->comboBoxANSICodec->addItem("windows-1252");
    ui->comboBoxANSICodec->addItem("windows-1253");
    ui->comboBoxANSICodec->addItem("windows-1254");
    ui->comboBoxANSICodec->addItem("windows-1255");
    ui->comboBoxANSICodec->addItem("windows-1256");
    ui->comboBoxANSICodec->addItem("windows-1257");
    ui->comboBoxANSICodec->addItem("windows-1258");

//    QList<QByteArray> listCodecs=QTextCodec::availableCodecs();

//    int nNumberOfCodecs=listCodecs.count();

//    for(int i=0;i<nNumberOfCodecs;i++)
//    {
//        QString sRecord=listCodecs.at(i).data();

//        qDebug(sRecord.toLatin1().data());
//    }

    ui->tableViewResult->installEventFilter(this);
}

SearchStringsWidget::~SearchStringsWidget()
{
    g_watcher.waitForFinished();

    delete ui;
}

void SearchStringsWidget::setData(QIODevice *pDevice, OPTIONS options, bool bAuto)
{
    this->g_pDevice=pDevice;

    ui->checkBoxAnsi->setChecked(options.bAnsi);
    ui->checkBoxUTF8->setChecked(options.bUTF8);
    ui->checkBoxUnicode->setChecked(options.bUnicode);
    ui->checkBoxCStrings->setChecked(options.bCStrings);

    g_bInit=false;

    qint64 nSize=pDevice->size();

    ui->tableViewResult->setModel(nullptr);

    g_options=options;

    if(g_options.nBaseAddress==-1)
    {
        g_options.nBaseAddress=0;
    }

    qint32 nAddressWidth=8;

    if(nSize+g_options.nBaseAddress>0xFFFFFFFF)
    {
        nAddressWidth=16;
    }
    else
    {
        nAddressWidth=8;
    }

//    QString sInfo=QString("0x%1 - 0x%2 ( 0x%3 )").arg(g_options.nBaseAddress,nAddressWidth,16,QChar('0'))
//                  .arg(g_options.nBaseAddress+nSize-1,nAddressWidth,16,QChar('0'))
//                  .arg(nSize,8,16,QChar('0'));
//    ui->labelInfo->setText(sInfo);

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
    return g_bInit;
}

void SearchStringsWidget::on_pushButtonSave_clicked()
{
    if(g_pModel)
    {
        QString sFileName=XBinary::getResultFileName(g_pDevice,QString("%1.txt").arg(tr("Strings")));
        sFileName=QFileDialog::getSaveFileName(this, tr("Save file"),sFileName, QString("%1 (*.txt);;%2 (*)").arg(tr("Text files"),tr("All files")));

        if(!sFileName.isEmpty())
        {
            XOptions::saveModel(g_pModel,sFileName);
        }
    }
}

void SearchStringsWidget::on_pushButtonSearch_clicked()
{
    search();
}

void SearchStringsWidget::on_lineEditFilter_textChanged(const QString &sText)
{
    filter(sText);
}

void SearchStringsWidget::filter(QString sString)
{
    g_pFilter->setFilterFixedString(sString);
    g_pFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    g_pFilter->setFilterKeyColumn(3);
}

void SearchStringsWidget::on_tableViewResult_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextMenu(this);

    QAction actionCopyString(tr("Copy string"),this);
    actionCopyString.setShortcut(getShortcuts()->getShortcut(XShortcuts::ID_STRINGS_COPYSTRING));
    connect(&actionCopyString,SIGNAL(triggered()),this,SLOT(_copyString()));
    contextMenu.addAction(&actionCopyString);

    QString sCopyString;

    if(g_options.nBaseAddress)
    {
        sCopyString=tr("Copy address");
    }
    else
    {
        sCopyString=tr("Copy offset");
    }

    QAction actionCopyOffset(sCopyString,this);
    actionCopyOffset.setShortcut(getShortcuts()->getShortcut(XShortcuts::ID_STRINGS_COPYOFFSET));
    connect(&actionCopyOffset,SIGNAL(triggered()),this,SLOT(_copyOffset()));
    contextMenu.addAction(&actionCopyOffset);

    QAction actionCopySize(tr("Copy size"),this);
    actionCopySize.setShortcut(getShortcuts()->getShortcut(XShortcuts::ID_STRINGS_COPYSIZE));
    connect(&actionCopySize,SIGNAL(triggered()),this,SLOT(_copySize()));
    contextMenu.addAction(&actionCopySize);

    QAction actionHex(tr("Hex"),this);
    QAction actionDemangle(tr("Demangle"),this);

    if(g_options.bMenu_Hex)
    {
        actionHex.setShortcut(getShortcuts()->getShortcut(XShortcuts::ID_STRINGS_HEX));
        connect(&actionHex,SIGNAL(triggered()),this,SLOT(_hex()));
        contextMenu.addAction(&actionHex);
    }

    if(g_options.bMenu_Demangle)
    {
        actionDemangle.setShortcut(getShortcuts()->getShortcut(XShortcuts::ID_STRINGS_DEMANGLE));
        connect(&actionDemangle,SIGNAL(triggered()),this,SLOT(_demangle()));
        contextMenu.addAction(&actionDemangle);
    }

    contextMenu.exec(ui->tableViewResult->viewport()->mapToGlobal(pos));
}

void SearchStringsWidget::_copyString()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(g_pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(3);

        QString sString=ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchStringsWidget::_copyOffset()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(g_pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(0);

        QString sString=ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchStringsWidget::_copySize()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(g_pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(1);

        QString sString=ui->tableViewResult->model()->data(index).toString();

        QApplication::clipboard()->setText(sString);
    }
}

void SearchStringsWidget::_hex()
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

void SearchStringsWidget::_demangle()
{
    int nRow=ui->tableViewResult->currentIndex().row();

    if((nRow!=-1)&&(g_pModel))
    {
        QModelIndex index=ui->tableViewResult->selectionModel()->selectedIndexes().at(3);

        QString sString=ui->tableViewResult->model()->data(index).toString();

        emit showDemangle(sString);
    }
}

void SearchStringsWidget::search()
{
    if(g_pDevice)
    {
        ui->lineEditFilter->clear();

        g_options.bAnsi=ui->checkBoxAnsi->isChecked();
        g_options.bUTF8=ui->checkBoxUTF8->isChecked();
        g_options.bUnicode=ui->checkBoxUnicode->isChecked();
        g_options.bCStrings=ui->checkBoxCStrings->isChecked();
        g_options.sANSICodec=ui->comboBoxANSICodec->currentText();
        g_options.nMinLenght=ui->spinBoxMinLength->value();

        if(g_options.bAnsi||g_options.bUnicode)
        {
            MultiSearch::OPTIONS options={};

            options.bAnsi=g_options.bAnsi;
            options.bUTF8=g_options.bUTF8;
            options.bUnicode=g_options.bUnicode;
            options.bCStrings=g_options.bCStrings;
            options.sANSICodec=g_options.sANSICodec;
            options.bMenu_Hex=g_options.bMenu_Hex;
            options.nMinLenght=g_options.nMinLenght;
            options.memoryMap=XBinary(g_pDevice,true,g_options.nBaseAddress).getMemoryMap();

            g_pOldModel=g_pModel;

            g_pFilter->setSourceModel(nullptr);
            ui->tableViewResult->setModel(nullptr);

            QList<XBinary::MS_RECORD> listRecords;

            QWidget *pParent=XOptions::getMainWidget(this);

            DialogMultiSearchProcess dsp(pParent);
            dsp.processSearch(g_pDevice,&listRecords,options,MultiSearch::TYPE_STRINGS);
            dsp.exec();

            DialogMultiSearchProcess dmp(pParent);
            dmp.processModel(&listRecords,&g_pModel,options,MultiSearch::TYPE_STRINGS);
            dmp.exec();

            g_pFilter->setSourceModel(g_pModel);
            ui->tableViewResult->setModel(g_pFilter);

            ui->tableViewResult->setColumnWidth(0,120);  // TODO
            ui->tableViewResult->setColumnWidth(1,80); // TODO
            ui->tableViewResult->setColumnWidth(2,30); // TODO

            QFuture<void> future=QtConcurrent::run(this,&SearchStringsWidget::deleteOldModel);

            g_watcher.setFuture(future);

//            watcher.waitForFinished();
        }

        g_bInit=true;
    }
}

void SearchStringsWidget::deleteOldModel()
{
    delete g_pOldModel;
}

void SearchStringsWidget::registerShortcuts(bool bState)
{
    if(bState)
    {
        if(!g_scCopyString)         g_scCopyString      =new QShortcut(getShortcuts()->getShortcut(XShortcuts::ID_STRINGS_COPYSTRING),      this,SLOT(_copyString()));
        if(!g_scCopyOffset)         g_scCopyOffset      =new QShortcut(getShortcuts()->getShortcut(XShortcuts::ID_STRINGS_COPYOFFSET),      this,SLOT(_copyOffset()));
        if(!g_scCopySize)           g_scCopySize        =new QShortcut(getShortcuts()->getShortcut(XShortcuts::ID_STRINGS_COPYSIZE),        this,SLOT(_copySize()));
        if(!g_scHex)                g_scHex             =new QShortcut(getShortcuts()->getShortcut(XShortcuts::ID_STRINGS_HEX),             this,SLOT(_hex()));
        if(!g_scDemangle)           g_scDemangle        =new QShortcut(getShortcuts()->getShortcut(XShortcuts::ID_STRINGS_DEMANGLE),        this,SLOT(_demangle()));
    }
    else
    {
        if(g_scCopyString)          {delete g_scCopyString;     g_scCopyString=nullptr;}
        if(g_scCopyOffset)          {delete g_scCopyOffset;     g_scCopyOffset=nullptr;}
        if(g_scCopySize)            {delete g_scCopySize;       g_scCopySize=nullptr;}
        if(g_scHex)                 {delete g_scHex;            g_scHex=nullptr;}
        if(g_scDemangle)            {delete g_scDemangle;       g_scDemangle=nullptr;}
    }
}

void SearchStringsWidget::on_checkBoxAnsi_stateChanged(int nArg)
{
    Q_UNUSED(nArg)

    adjust();
}

void SearchStringsWidget::on_checkBoxUTF8_stateChanged(int nArg)
{
    Q_UNUSED(nArg)

    adjust();
}

void SearchStringsWidget::on_checkBoxUnicode_stateChanged(int nArg)
{
    Q_UNUSED(nArg)

    adjust();
}

void SearchStringsWidget::adjust()
{
    bool bIsANSI=ui->checkBoxAnsi->isChecked();
    bool bIsUTF8=ui->checkBoxUTF8->isChecked();
    bool bIsUnicode=ui->checkBoxUnicode->isChecked();

    ui->comboBoxANSICodec->setEnabled(bIsANSI);

    ui->checkBoxCStrings->setEnabled(bIsANSI|bIsUTF8|bIsUnicode);
}
