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
#include "searchstringswidget.h"
#include "ui_searchstringswidget.h"

SearchStringsWidget::SearchStringsWidget(QWidget *pParent) :
    XShortcutsWidget(pParent),
    ui(new Ui::SearchStringsWidget)
{
    ui->setupUi(this);
    g_pDevice=nullptr;
    g_pBackupDevice=nullptr;
    g_pFilter=new QSortFilterProxyModel(this);

    g_options.nBaseAddress=0;
    g_pModel=nullptr;
    g_bInit=false;
    g_bIsReadonly=true;

    g_options={};

    ui->checkBoxCStrings->setEnabled(false);

    ui->checkBoxAnsi->setChecked(true);
    ui->checkBoxUTF8->setChecked(false);
    ui->checkBoxUnicode->setChecked(true);
    ui->checkBoxCStrings->setChecked(false);

    ui->spinBoxMinLength->setMinimum(2);
    ui->spinBoxMinLength->setValue(5);

    memset(shortCuts,0,sizeof shortCuts);

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

void SearchStringsWidget::setData(QIODevice *pDevice,OPTIONS options,bool bAuto)
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

void SearchStringsWidget::setBackupDevice(QIODevice *pDevice)
{
    g_pBackupDevice=pDevice;
}

QIODevice *SearchStringsWidget::getDevice()
{
    return g_pDevice;
}

QIODevice *SearchStringsWidget::getBackupDevice()
{
    QIODevice *pResult=nullptr;

    if(g_pBackupDevice)
    {
        pResult=g_pBackupDevice;
    }
    else
    {
        pResult=g_pDevice;
    }

    return pResult;
}

void SearchStringsWidget::reload()
{
    search();
}

bool SearchStringsWidget::getInitStatus()
{
    return g_bInit;
}

bool SearchStringsWidget::isEdited()
{
    bool bResult=XBinary::isBackupPresent(getBackupDevice());

    return bResult;
}

bool SearchStringsWidget::saveBackup()
{
    bool bResult=true;

    if((getGlobalOptions()->isSaveBackup())&&(!isEdited()))
    {
        // Save backup
        bResult=XBinary::saveBackup(getBackupDevice());
    }

    return bResult;
}

void SearchStringsWidget::setReadonly(bool bState)
{
    g_bIsReadonly=bState;
}

bool SearchStringsWidget::isReadonly()
{
    return g_bIsReadonly;
}

void SearchStringsWidget::on_pushButtonSave_clicked()
{
    if(g_pFilter)
    {
        XShortcutsWidget::saveModel(g_pFilter,XBinary::getResultFileName(g_pDevice,QString("%1.txt").arg(tr("Strings"))));
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

    QMenu menuCopy(tr("Copy"),this);
    QMenu menuFollowIn(tr("Follow in"),this);
    QMenu menuEdit(tr("Edit"),this);

    QAction actionCopyString(tr("String"),this);
    actionCopyString.setShortcut(getShortcuts()->getShortcut(X_ID_STRING_COPY_STRING));
    connect(&actionCopyString,SIGNAL(triggered()),this,SLOT(_copyString()));
    menuCopy.addAction(&actionCopyString);

    QString sCopyString;

    if(g_options.nBaseAddress)
    {
        sCopyString=tr("Address");
    }
    else
    {
        sCopyString=tr("Offset");
    }

    QAction actionCopyOffset(sCopyString,this);
    actionCopyOffset.setShortcut(getShortcuts()->getShortcut(X_ID_STRING_COPY_OFFSET));
    connect(&actionCopyOffset,SIGNAL(triggered()),this,SLOT(_copyOffset()));
    menuCopy.addAction(&actionCopyOffset);

    QAction actionCopySize(tr("Size"),this);
    actionCopySize.setShortcut(getShortcuts()->getShortcut(X_ID_STRING_COPY_SIZE));
    connect(&actionCopySize,SIGNAL(triggered()),this,SLOT(_copySize()));
    menuCopy.addAction(&actionCopySize);

    contextMenu.addMenu(&menuCopy);

    QAction actionHex(tr("Hex"),this);
    QAction actionDemangle(tr("Demangle"),this);

    if(g_options.bMenu_Hex)
    {
        actionHex.setShortcut(getShortcuts()->getShortcut(X_ID_STRING_FOLLOWIN_HEX));
        connect(&actionHex,SIGNAL(triggered()),this,SLOT(_hex()));

        menuFollowIn.addAction(&actionHex);
    }

    if(g_options.bMenu_Demangle)
    {
        actionDemangle.setShortcut(getShortcuts()->getShortcut(X_ID_STRING_DEMANGLE));
        connect(&actionDemangle,SIGNAL(triggered()),this,SLOT(_demangle()));
        contextMenu.addAction(&actionDemangle);
    }

    if(g_options.bMenu_Hex)
    {
        contextMenu.addMenu(&menuFollowIn);
    }

    QAction actionEditString(tr("String"),this);
    actionEditString.setShortcut(getShortcuts()->getShortcut(X_ID_STRING_EDIT_STRING));
    connect(&actionEditString,SIGNAL(triggered()),this,SLOT(_editString()));

    menuEdit.addAction(&actionEditString);

    menuEdit.setEnabled(!isReadonly());

    contextMenu.addMenu(&menuEdit);

    contextMenu.exec(ui->tableViewResult->viewport()->mapToGlobal(pos));
}

void SearchStringsWidget::_copyString()
{
    qint32 nRow=ui->tableViewResult->currentIndex().row();

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

        SubDevice *pSubDevice=static_cast<SubDevice *>(g_pDevice);

        if(pSubDevice)
        {
            nOffset+=pSubDevice->getInitOffset();
        }

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

void SearchStringsWidget::_editString()
{
    if(!isReadonly())
    {
        QModelIndex index0=ui->tableViewResult->selectionModel()->selectedIndexes().at(0);
        QModelIndex index1=ui->tableViewResult->selectionModel()->selectedIndexes().at(1);
        QModelIndex index2=ui->tableViewResult->selectionModel()->selectedIndexes().at(2);
        QModelIndex index3=ui->tableViewResult->selectionModel()->selectedIndexes().at(3);

        DialogEditString::DATA_STRUCT dataStruct={};

        dataStruct.nOffset=ui->tableViewResult->model()->data(index0,Qt::UserRole+MultiSearch::USERROLE_OFFSET).toLongLong();
        dataStruct.nSize=ui->tableViewResult->model()->data(index0,Qt::UserRole+MultiSearch::USERROLE_SIZE).toLongLong();
        dataStruct.recordType=(XBinary::MS_RECORD_TYPE)(ui->tableViewResult->model()->data(index0,Qt::UserRole+MultiSearch::USERROLE_TYPE).toLongLong());
        dataStruct.bIsCStrings=false;

        dataStruct.sString=ui->tableViewResult->model()->data(index3).toString();

        DialogEditString dialogEditString(this,g_pDevice,&dataStruct);

        if(dialogEditString.exec()==QDialog::Accepted)
        {
            bool bSuccess=false;

            if(saveBackup())
            {
                if(XBinary::write_array(g_pDevice,dataStruct.nOffset,XBinary::getStringData(dataStruct.recordType,dataStruct.sString,dataStruct.bIsCStrings)))
                {
                    ui->tableViewResult->model()->setData(index0,dataStruct.nSize,Qt::UserRole+MultiSearch::USERROLE_SIZE);
                    ui->tableViewResult->model()->setData(index0,dataStruct.recordType,Qt::UserRole+MultiSearch::USERROLE_TYPE);

                    ui->tableViewResult->model()->setData(index1,XBinary::valueToHexEx(dataStruct.sString.size()),Qt::DisplayRole);
                    ui->tableViewResult->model()->setData(index2,XBinary::msRecordTypeIdToString(dataStruct.recordType),Qt::DisplayRole);
                    ui->tableViewResult->model()->setData(index3,dataStruct.sString,Qt::DisplayRole);

                    bSuccess=true;
                }
            }

            if(bSuccess)
            {
                emit dataChanged();
            }
            else
            {
                QMessageBox::critical(XOptions::getMainWidget(this),tr("Error"),tr("Cannot save file")+QString(": %1").arg(XBinary::getBackupFileName(getBackupDevice())));
            }
        }
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

        #if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            QFuture<void> future=QtConcurrent::run(&SearchStringsWidget::deleteOldModel,this);
        #else
            QFuture<void> future=QtConcurrent::run(this,&SearchStringsWidget::deleteOldModel);
        #endif

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
        if(!shortCuts[SC_COPYSTRING])           shortCuts[SC_COPYSTRING]        =new QShortcut(getShortcuts()->getShortcut(X_ID_STRING_COPY_STRING),        this,SLOT(_copyString()));
        if(!shortCuts[SC_COPYOFFSET])           shortCuts[SC_COPYOFFSET]        =new QShortcut(getShortcuts()->getShortcut(X_ID_STRING_COPY_OFFSET),        this,SLOT(_copyOffset()));
        if(!shortCuts[SC_COPYSIZE])             shortCuts[SC_COPYSIZE]          =new QShortcut(getShortcuts()->getShortcut(X_ID_STRING_COPY_SIZE),          this,SLOT(_copySize()));
        if(!shortCuts[SC_HEX])                  shortCuts[SC_HEX]               =new QShortcut(getShortcuts()->getShortcut(X_ID_STRING_FOLLOWIN_HEX),       this,SLOT(_hex()));
        if(!shortCuts[SC_DEMANGLE])             shortCuts[SC_DEMANGLE]          =new QShortcut(getShortcuts()->getShortcut(X_ID_STRING_DEMANGLE),           this,SLOT(_demangle()));
        if(!shortCuts[SC_EDITSTRING])           shortCuts[SC_EDITSTRING]        =new QShortcut(getShortcuts()->getShortcut(X_ID_STRING_EDIT_STRING),        this,SLOT(_editString()));
    }
    else
    {
        for(qint32 i=0;i<__SC_SIZE;i++)
        {
            if(shortCuts[i])
            {
                delete shortCuts[i];
                shortCuts[i]=nullptr;
            }
        }
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
