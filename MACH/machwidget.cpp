// copyright (c) 2017-2019 hors<horsicq@gmail.com>
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
#include "machwidget.h"
#include "ui_machwidget.h"

MACHWidget::MACHWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::MACHWidget)
{
    ui->setupUi(this);
}

MACHWidget::MACHWidget(QIODevice *pDevice, OPTIONS *pOptions, QWidget *parent) :
    FormatWidget(pDevice,pOptions,parent),
    ui(new Ui::MACHWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions);
    reload();
}

MACHWidget::~MACHWidget()
{
    delete ui;
}

void MACHWidget::clear()
{
    memset(bInit,0,sizeof bInit);
    memset(lineEdit_mach_header,0,sizeof lineEdit_mach_header);
    memset(comboBox,0,sizeof comboBox);

    pSubDeviceCommand=nullptr;
    pSubDeviceSegment=nullptr;

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void MACHWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XMACH mach(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(mach.isValid())
    {
        // mb TODO 32/64
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_TOOLS,tr("Tools")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_mach_header,mach.is64()?("mach_header_64"):("mach_header")));

        // TODO Commands
        QList<XMACH::COMMAND_RECORD> listCommandRecords=mach.getCommandRecords();

        if(listCommandRecords.count())
        {
            QTreeWidgetItem *pItemCommands=createNewItem(SMACH::TYPE_mach_commands,tr("commands"));

            ui->treeWidgetNavi->addTopLevelItem(pItemCommands);
        }

        QList<XMACH::SEGMENT_RECORD> listSegmentRecords=mach.getSegmentRecords(&listCommandRecords);

        if(listSegmentRecords.count())
        {
            QTreeWidgetItem *pItemSegments=createNewItem(SMACH::TYPE_mach_segments,tr("segments"));

            ui->treeWidgetNavi->addTopLevelItem(pItemSegments);
        }

        ui->treeWidgetNavi->expandAll();

        ui->treeWidgetNavi->setCurrentItem(ui->treeWidgetNavi->topLevelItem(1));
    }
}

bool MACHWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition)
{
    bool bResult=false;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {
        XMACH mach(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

        if(mach.isValid())
        {
            switch(nStype)
            {
                case SMACH::TYPE_mach_header:
                    switch(nNdata)
                    {
                        case N_mach_header::magic:
                            // TODO reload all data
                            comboBox[CB_mach_header_magic]->setValue(nValue);
                            mach.setHeader_magic((quint32)nValue);
                            break;

                        case N_mach_header::cputype:
                            comboBox[CB_mach_header_cputype]->setValue(nValue);
                            mach.setHeader_cputype((qint32)nValue);
                            break;

                        case N_mach_header::cpusubtype:
                            mach.setHeader_cpusubtype((qint32)nValue);
                            break;

                        case N_mach_header::filetype:
                            comboBox[CB_mach_header_filetype]->setValue(nValue);
                            mach.setHeader_filetype((quint32)nValue);
                            break;

                        case N_mach_header::ncmds:
                            mach.setHeader_ncmds((quint32)nValue);
                            break;

                        case N_mach_header::sizeofcmds:
                            mach.setHeader_sizeofcmds((quint32)nValue);
                            break;

                        case N_mach_header::flags:
                            comboBox[CB_mach_header_flags]->setValue(nValue);
                            mach.setHeader_flags((quint32)nValue);
                            break;

                        case N_mach_header::reserved:
                            mach.setHeader_reserved((quint32)nValue);
                            break;
                    }

                    break;
            }

            bResult=true;
        }
    }

    blockSignals(false);

    return bResult;
}

void MACHWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_mach_header,N_mach_header::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);

    ui->widgetHex->setReadonly(bState);
}

void MACHWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_mach_header,N_mach_header::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void MACHWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    switch(type)
    {
        case SMACH::TYPE_mach_header:
            pTableWidget->setColumnWidth(0,nSymbolWidth*8);
            pTableWidget->setColumnWidth(1,nSymbolWidth*10);
            pTableWidget->setColumnWidth(2,nSymbolWidth*10);
            pTableWidget->setColumnWidth(3,nSymbolWidth*26);
            break;
    }
}

void MACHWidget::reloadData()
{
    int nData=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole).toInt();
    ui->stackedWidgetInfo->setCurrentIndex(nData);

    XMACH mach(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(mach.isValid())
    {
        if(nData==SMACH::TYPE_TOOLS)
        {
            if(!bInit[nData])
            {
                ui->widgetHex->setData(getDevice(),getOptions());
                ui->widgetHex->setEdited(isEdited());
                connect(ui->widgetHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

                bInit[nData]=true;
            }
        }
        else if(nData==SMACH::TYPE_mach_header)
        {
            if(!bInit[nData])
            {
                if(!mach.is64())
                {
                    bInit[nData]=createHeaderTable(SMACH::TYPE_mach_header,ui->tableWidget_mach_header,N_mach_header::records32,lineEdit_mach_header,N_mach_header::__data_size-1,0);
                }
                else
                {
                    bInit[nData]=createHeaderTable(SMACH::TYPE_mach_header,ui->tableWidget_mach_header,N_mach_header::records64,lineEdit_mach_header,N_mach_header::__data_size,0);
                }

                comboBox[CB_mach_header_magic]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderMagicsS(),SMACH::TYPE_mach_header,N_mach_header::magic,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_mach_header_cputype]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderCpuTypesS(),SMACH::TYPE_mach_header,N_mach_header::cputype,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_mach_header_filetype]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderFileTypesS(),SMACH::TYPE_mach_header,N_mach_header::filetype,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_mach_header_flags]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderFlagsS(),SMACH::TYPE_mach_header,N_mach_header::flags,XComboBoxEx::CBTYPE_FLAGS);
            }

            blockSignals(true);

            lineEdit_mach_header[N_mach_header::magic]->setValue(mach.getHeader_magic());
            lineEdit_mach_header[N_mach_header::cputype]->setValue(mach.getHeader_cputype());
            lineEdit_mach_header[N_mach_header::cpusubtype]->setValue(mach.getHeader_cpusubtype());
            lineEdit_mach_header[N_mach_header::filetype]->setValue(mach.getHeader_filetype());
            lineEdit_mach_header[N_mach_header::ncmds]->setValue(mach.getHeader_ncmds());
            lineEdit_mach_header[N_mach_header::sizeofcmds]->setValue(mach.getHeader_sizeofcmds());
            lineEdit_mach_header[N_mach_header::flags]->setValue(mach.getHeader_flags());

            if(mach.is64())
            {
                lineEdit_mach_header[N_mach_header::reserved]->setValue(mach.getHeader_reserved());
            }

            comboBox[CB_mach_header_magic]->setValue(mach.getHeader_magic());
            comboBox[CB_mach_header_cputype]->setValue((quint32)mach.getHeader_cputype());
            comboBox[CB_mach_header_filetype]->setValue((quint32)mach.getHeader_filetype());
            comboBox[CB_mach_header_flags]->setValue((quint32)mach.getHeader_flags());

            blockSignals(false);
        }
        else if(nData==SMACH::TYPE_mach_commands)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createSectionTable(SMACH::TYPE_mach_commands,ui->tableWidget_commands,N_mach_commands::records,N_mach_commands::__data_size);
            }

            blockSignals(true);

            QList<XMACH::COMMAND_RECORD> listCommandRecords=mach.getCommandRecords();

            int nCount=listCommandRecords.count();

            ui->tableWidget_commands->setRowCount(nCount);

            QMap<quint64,QString> mapLC=mach.getLoadCommandTypesS();

            for(int i=0; i<nCount; i++)
            {
                QTableWidgetItem *pItem=new QTableWidgetItem(QString::number(i));

                pItem->setData(Qt::UserRole+SECTION_DATA_OFFSET,listCommandRecords.at(i).nOffset);
                pItem->setData(Qt::UserRole+SECTION_DATA_SIZE,listCommandRecords.at(i).nSize);
                pItem->setData(Qt::UserRole+SECTION_DATA_ADDRESS,listCommandRecords.at(i).nOffset);

                ui->tableWidget_commands->setItem(i,0,                              pItem);
                ui->tableWidget_commands->setItem(i,N_mach_commands::cmd+1,         new QTableWidgetItem(XBinary::valueToHex((quint32)listCommandRecords.at(i).nType)));
                ui->tableWidget_commands->setItem(i,N_mach_commands::cmdsize+1,     new QTableWidgetItem(XBinary::valueToHex((quint32)listCommandRecords.at(i).nSize)));
                ui->tableWidget_commands->setItem(i,N_mach_commands::cmdsize+2,     new QTableWidgetItem(mapLC.value(listCommandRecords.at(i).nType)));
            }

            if(nCount)
            {
                ui->tableWidget_commands->setCurrentCell(0,0);
            }

            blockSignals(false);
        }
        else if(nData==SMACH::TYPE_mach_segments)
        {
            bool bIs64=mach.is64();

            if(!bInit[nData])
            {
                bInit[nData]=createSectionTable(SMACH::TYPE_mach_segments,ui->tableWidget_segments,bIs64?(N_mach_segments::records64):(N_mach_segments::records32),N_mach_segments::__data_size);
            }

            blockSignals(true);

            QList<XMACH::SEGMENT_RECORD> listSegmentRecords=mach.getSegmentRecords();

            int nCount=listSegmentRecords.count();

            ui->tableWidget_segments->setRowCount(nCount);


            for(int i=0; i<nCount; i++)
            {
                QTableWidgetItem *pItem=new QTableWidgetItem(QString::number(i));

                if(getOptions()->bIsImage)
                {
                    pItem->setData(Qt::UserRole+SECTION_DATA_OFFSET,listSegmentRecords.at(i).vmaddr);
                    pItem->setData(Qt::UserRole+SECTION_DATA_SIZE,listSegmentRecords.at(i).vmsize);
                }
                else
                {
                    pItem->setData(Qt::UserRole+SECTION_DATA_OFFSET,listSegmentRecords.at(i).fileoff);
                    pItem->setData(Qt::UserRole+SECTION_DATA_SIZE,listSegmentRecords.at(i).filesize);
                }

                pItem->setData(Qt::UserRole+SECTION_DATA_ADDRESS,listSegmentRecords.at(i).vmaddr);

                ui->tableWidget_segments->setItem(i,0,                              pItem);
                ui->tableWidget_segments->setItem(i,N_mach_segments::segname+1,     new QTableWidgetItem(listSegmentRecords.at(i).segname));

//                segname=0,
//                vmaddr,
//                vmsize,
//                fileoff,
//                filesize,
//                maxprot,
//                initprot,
//                nsects,
//                flags,
//                __data_size
            }

            if(nCount)
            {
                ui->tableWidget_segments->setCurrentCell(0,0);
            }

            blockSignals(false);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void MACHWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
        case SMACH::TYPE_mach_header:
            switch(nNdata)
            {
                case N_mach_header::magic:
                    lineEdit_mach_header[N_mach_header::magic]->setValue((quint32)nValue);
                    break;

                case N_mach_header::cputype:
                    lineEdit_mach_header[N_mach_header::cputype]->setValue((quint32)nValue);
                    break;

                case N_mach_header::filetype:
                    lineEdit_mach_header[N_mach_header::filetype]->setValue((quint32)nValue);
                    break;

                case N_mach_header::flags:
                    lineEdit_mach_header[N_mach_header::flags]->setValue((quint32)nValue);
                    break;
            }

            break;
    }
}

void MACHWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current)
    {
        reloadData();
    }
}

void MACHWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

bool MACHWidget::createSectionTable(int type, QTableWidget *pTableWidget, const FormatWidget::HEADER_RECORD *pRecords, int nRecordCount)
{
    int nSymbolWidth=getSymbolWidth();
    QStringList slHeader;

    switch(type)
    {
        case SMACH::TYPE_mach_commands:
            slHeader.append(tr(""));
            pTableWidget->setColumnCount(nRecordCount+2);
            pTableWidget->setColumnWidth(0,nSymbolWidth*4);
            pTableWidget->setColumnWidth(1,nSymbolWidth*10);
            pTableWidget->setColumnWidth(2,nSymbolWidth*10);
            pTableWidget->setColumnWidth(3,nSymbolWidth*20);
            break;

        case SMACH::TYPE_mach_segments:
            slHeader.append(tr(""));
            pTableWidget->setColumnCount(nRecordCount+1);
            break;

        default:
            pTableWidget->setColumnCount(nRecordCount);
    }

    pTableWidget->setRowCount(0);

    for(int i=0; i<nRecordCount; i++)
    {
        slHeader.append(pRecords[i].pszName);
    }

    switch(type)
    {
        case SMACH::TYPE_mach_commands:
            slHeader.append(tr("Type"));
            break;
    }

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    return true;
}

void MACHWidget::on_tableWidget_commands_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(currentRow!=-1)
    {
        qint64 nOffset=ui->tableWidget_commands->item(currentRow,0)->data(Qt::UserRole+SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize=ui->tableWidget_commands->item(currentRow,0)->data(Qt::UserRole+SECTION_DATA_SIZE).toLongLong();
        qint64 nAddress=ui->tableWidget_commands->item(currentRow,0)->data(Qt::UserRole+SECTION_DATA_ADDRESS).toLongLong();

        if(pSubDeviceCommand)
        {
            pSubDeviceCommand->close();
            delete pSubDeviceCommand;
        }

        pSubDeviceCommand=new SubDevice(getDevice(),nOffset,nSize,this);
        pSubDeviceCommand->open(getDevice()->openMode());

        FormatWidget::OPTIONS hexOptions=*getOptions();
        hexOptions.nImageBase=nAddress;

        ui->widgetCommandHex->setData(pSubDeviceCommand,&hexOptions);
        ui->widgetCommandHex->setEdited(isEdited());
        connect(ui->widgetCommandHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));
    }
}

void MACHWidget::on_pushButtonReload_clicked()
{
    reload();
}

void MACHWidget::on_tableWidget_segments_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(currentRow!=-1)
    {
        qint64 nOffset=ui->tableWidget_segments->item(currentRow,0)->data(Qt::UserRole+SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize=ui->tableWidget_segments->item(currentRow,0)->data(Qt::UserRole+SECTION_DATA_SIZE).toLongLong();
        qint64 nAddress=ui->tableWidget_segments->item(currentRow,0)->data(Qt::UserRole+SECTION_DATA_ADDRESS).toLongLong();

        if(pSubDeviceSegment)
        {
            pSubDeviceSegment->close();
            delete pSubDeviceSegment;
        }

        pSubDeviceSegment=new SubDevice(getDevice(),nOffset,nSize,this);
        pSubDeviceSegment->open(getDevice()->openMode());

        FormatWidget::OPTIONS hexOptions=*getOptions();
        hexOptions.nImageBase=nAddress;

        ui->widgetSegmentHex->setData(pSubDeviceSegment,&hexOptions);
        ui->widgetSegmentHex->setEdited(isEdited());
        connect(ui->widgetSegmentHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));
    }
}
