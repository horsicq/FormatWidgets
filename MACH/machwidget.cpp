// copyright (c) 2017-2020 hors<horsicq@gmail.com>
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

MACHWidget::MACHWidget(QWidget *pParent) :
    FormatWidget(pParent),
    ui(new Ui::MACHWidget)
{
    ui->setupUi(this);
}

MACHWidget::MACHWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, QWidget *pParent) :
    FormatWidget(pDevice,pOptions,0,0,0,pParent),
    ui(new Ui::MACHWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions,0,0,0);
    MACHWidget::reload();
}

MACHWidget::~MACHWidget()
{
    delete ui;
}

void MACHWidget::clear()
{
    MACHWidget::reset();

    memset(g_lineEdit_mach_header,0,sizeof g_lineEdit_mach_header);
    memset(g_comboBox,0,sizeof g_comboBox);
    memset(g_subDevice,0,sizeof g_subDevice);

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void MACHWidget::cleanup()
{

}

void MACHWidget::reset()
{
    g_stInit.clear();
}

void MACHWidget::reload()
{
    MACHWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XMACH mach(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(mach.isValid())
    {
        setFileType(mach.getFileType());

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_HEX,tr("Hex")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_DISASM,tr("Disasm")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_STRINGS,tr("Strings")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_MEMORYMAP,tr("Memory map")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_ENTROPY,tr("Entropy")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_HEURISTICSCAN,tr("Heuristic scan")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_mach_header,mach.is64()?("mach_header_64"):("mach_header")));

        QList<XMACH::COMMAND_RECORD> listCommandRecords=mach.getCommandRecords();

        if(listCommandRecords.count())
        {
            QTreeWidgetItem *pItemCommands=createNewItem(SMACH::TYPE_mach_commands,tr("Commands"));

            ui->treeWidgetNavi->addTopLevelItem(pItemCommands);
        }

        QList<XMACH::SEGMENT_RECORD> listSegmentRecords=mach.getSegmentRecords(&listCommandRecords);

        if(listSegmentRecords.count())
        {
            QTreeWidgetItem *pItemSegments=createNewItem(SMACH::TYPE_mach_segments,tr("Segments"));

            ui->treeWidgetNavi->addTopLevelItem(pItemSegments);
        }

        QList<XMACH::SECTION_RECORD> listSectionRecords=mach.getSectionRecords(&listCommandRecords);

        if(listSectionRecords.count())
        {
            QTreeWidgetItem *pItemSections=createNewItem(SMACH::TYPE_mach_sections,tr("Sections"));

            ui->treeWidgetNavi->addTopLevelItem(pItemSections);
        }

        QList<XMACH::LIBRARY_RECORD> listLibraryRecords=mach.getLibraryRecords(&listCommandRecords);

        if(listLibraryRecords.count())
        {
            QTreeWidgetItem *pItemLibraries=createNewItem(SMACH::TYPE_mach_libraries,tr("Libraries"));

            ui->treeWidgetNavi->addTopLevelItem(pItemLibraries);
        }

        ui->treeWidgetNavi->expandAll();

        setTreeItem(ui->treeWidgetNavi,getOptions()->nStartType);
    }
}

bool MACHWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

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
                        case N_mach_header::magic:      g_comboBox[CB_mach_header_magic]->setValue(nValue);       break; // TODO reload all data
                        case N_mach_header::cputype:    g_comboBox[CB_mach_header_cputype]->setValue(nValue);     break;
                        case N_mach_header::filetype:   g_comboBox[CB_mach_header_filetype]->setValue(nValue);    break;
                        case N_mach_header::flags:      g_comboBox[CB_mach_header_flags]->setValue(nValue);       break;
                    }
                    break;
            }

            switch(nStype)
            {
                case SMACH::TYPE_mach_header:
                    switch(nNdata)
                    {
                        case N_mach_header::magic:      mach.setHeader_magic((quint32)nValue);          break;
                        case N_mach_header::cputype:    mach.setHeader_cputype((qint32)nValue);         break;
                        case N_mach_header::cpusubtype: mach.setHeader_cpusubtype((qint32)nValue);      break;
                        case N_mach_header::filetype:   mach.setHeader_filetype((quint32)nValue);       break;
                        case N_mach_header::ncmds:      mach.setHeader_ncmds((quint32)nValue);          break;
                        case N_mach_header::sizeofcmds: mach.setHeader_sizeofcmds((quint32)nValue);     break;
                        case N_mach_header::flags:      mach.setHeader_flags((quint32)nValue);          break;
                        case N_mach_header::reserved:   mach.setHeader_reserved((quint32)nValue);       break;
                    }

                    ui->widgetHex_mach_header->reload();

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
    setLineEditsReadOnly(g_lineEdit_mach_header,N_mach_header::__data_size,bState);

    setComboBoxesReadOnly(g_comboBox,__CB_size,bState);

    ui->widgetHex->setReadonly(bState);
}

void MACHWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_lineEdit_mach_header,N_mach_header::__data_size,bState);

    _blockSignals((QObject **)g_comboBox,__CB_size,bState);
}

void MACHWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_NAME,QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_OFFSET,QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_TYPE,QHeaderView::ResizeToContents);

    switch(nType)
    {
        case SMACH::TYPE_mach_header:
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*26);
            break;
    }
}

void MACHWidget::reloadData()
{
    int nType=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_TYPE).toInt();
    qint64 nDataOffset=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nDataSize=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();

    QString sInit=QString("%1-%2-%3").arg(nType).arg(nDataOffset).arg(nDataSize);

    if((g_nLastType==nType)&&(sInit!=g_sLastInit))
    {
        g_stInit.remove(sInit);
    }

    g_nLastType=nType;
    g_sLastInit=sInit;

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XMACH mach(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(mach.isValid())
    {
        if(nType==SMACH::TYPE_HEX)
        {
            if(!g_stInit.contains(sInit))
            {
                ui->widgetHex->setData(getDevice());
//                ui->widgetHex->setBackupFileName(getOptions()->sBackupFileName);
                ui->widgetHex->enableReadOnly(false);
                connect(ui->widgetHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));
            }
        }
        else if(nType==SMACH::TYPE_DISASM)
        {
            if(!g_stInit.contains(sInit))
            {
                ui->widgetDisasm->setData(getDevice(),mach.getFileType(),mach.getEntryPointAddress());
            }
        }
        else if(nType==SMACH::TYPE_STRINGS)
        {
            if(!g_stInit.contains(sInit))
            {
                ui->widgetStrings->setData(getDevice(),nullptr,true);
            }
        }
        else if(nType==SMACH::TYPE_MEMORYMAP)
        {
            if(!g_stInit.contains(sInit))
            {
                ui->widgetMemoryMap->setData(getDevice());
            }
        }
        else if(nType==SMACH::TYPE_ENTROPY)
        {
            if(!g_stInit.contains(sInit))
            {
                ui->widgetEntropy->setData(getDevice(),0,getDevice()->size(),true);
            }
        }
        else if(nType==SMACH::TYPE_HEURISTICSCAN)
        {
            if(!g_stInit.contains(sInit))
            {
                ui->widgetHeuristicScan->setData(getDevice(),true,XBinary::FT_MACHO);
            }
        }
        else if(nType==SMACH::TYPE_mach_header)
        {
            if(!g_stInit.contains(sInit))
            {
                if(!mach.is64())
                {
                    createHeaderTable(SMACH::TYPE_mach_header,ui->tableWidget_mach_header,N_mach_header::records32,g_lineEdit_mach_header,N_mach_header::__data_size-1,0);
                }
                else
                {
                    createHeaderTable(SMACH::TYPE_mach_header,ui->tableWidget_mach_header,N_mach_header::records64,g_lineEdit_mach_header,N_mach_header::__data_size,0);
                }

                g_comboBox[CB_mach_header_magic]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderMagicsS(),SMACH::TYPE_mach_header,N_mach_header::magic,XComboBoxEx::CBTYPE_NORMAL);
                g_comboBox[CB_mach_header_cputype]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderCpuTypesS(),SMACH::TYPE_mach_header,N_mach_header::cputype,XComboBoxEx::CBTYPE_NORMAL);
                g_comboBox[CB_mach_header_filetype]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderFileTypesS(),SMACH::TYPE_mach_header,N_mach_header::filetype,XComboBoxEx::CBTYPE_NORMAL);
                g_comboBox[CB_mach_header_flags]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderFlagsS(),SMACH::TYPE_mach_header,N_mach_header::flags,XComboBoxEx::CBTYPE_FLAGS);

                blockSignals(true);

                g_lineEdit_mach_header[N_mach_header::magic]->setValue(mach.getHeader_magic());
                g_lineEdit_mach_header[N_mach_header::cputype]->setValue(mach.getHeader_cputype());
                g_lineEdit_mach_header[N_mach_header::cpusubtype]->setValue(mach.getHeader_cpusubtype());
                g_lineEdit_mach_header[N_mach_header::filetype]->setValue(mach.getHeader_filetype());
                g_lineEdit_mach_header[N_mach_header::ncmds]->setValue(mach.getHeader_ncmds());
                g_lineEdit_mach_header[N_mach_header::sizeofcmds]->setValue(mach.getHeader_sizeofcmds());
                g_lineEdit_mach_header[N_mach_header::flags]->setValue(mach.getHeader_flags());

                if(mach.is64())
                {
                    g_lineEdit_mach_header[N_mach_header::reserved]->setValue(mach.getHeader_reserved());
                }

                g_comboBox[CB_mach_header_magic]->setValue(mach.getHeader_magic());
                g_comboBox[CB_mach_header_cputype]->setValue((quint32)mach.getHeader_cputype());
                g_comboBox[CB_mach_header_filetype]->setValue((quint32)mach.getHeader_filetype());
                g_comboBox[CB_mach_header_flags]->setValue((quint32)mach.getHeader_flags());

                qint64 nOffset=mach.getHeaderOffset();
                qint64 nSize=mach.getHeaderSize();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_header],ui->widgetHex_mach_header);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_commands)
        {
            if(!g_stInit.contains(sInit))
            {
                createSectionTable(SMACH::TYPE_mach_commands,ui->tableWidget_commands,N_mach_commands::records,N_mach_commands::__data_size);

                blockSignals(true);

                QList<XMACH::COMMAND_RECORD> listCommandRecords=mach.getCommandRecords();

                int nNumberOfCommands=listCommandRecords.count();

                ui->tableWidget_commands->setRowCount(nNumberOfCommands);

                QMap<quint64,QString> mapLC=mach.getLoadCommandTypesS();

                for(int i=0; i<nNumberOfCommands; i++)
                {
                    QTableWidgetItem *pItem=new QTableWidgetItem(QString::number(i));

                    pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,listCommandRecords.at(i).nOffset);
                    pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_SIZE,listCommandRecords.at(i).nSize);
                    pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS,listCommandRecords.at(i).nOffset);

                    ui->tableWidget_commands->setItem(i,0,                              pItem);
                    ui->tableWidget_commands->setItem(i,N_mach_commands::cmd+1,         new QTableWidgetItem(XBinary::valueToHex((quint32)listCommandRecords.at(i).nType)));
                    ui->tableWidget_commands->setItem(i,N_mach_commands::cmdsize+1,     new QTableWidgetItem(XBinary::valueToHex((quint32)listCommandRecords.at(i).nSize)));
                    ui->tableWidget_commands->setItem(i,N_mach_commands::cmdsize+2,     new QTableWidgetItem(mapLC.value(listCommandRecords.at(i).nType)));
                }

                if(nNumberOfCommands)
                {
                    if(ui->tableWidget_commands->currentRow()==0)
                    {
                        loadCommand(0);
                    }
                    else
                    {
                        ui->tableWidget_commands->setCurrentCell(0,0);
                    }
                }

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_segments)
        {
            if(!g_stInit.contains(sInit))
            {
                bool bIs64=mach.is64();

                createSectionTable(SMACH::TYPE_mach_segments,ui->tableWidget_segments,bIs64?(N_mach_segments::records64):(N_mach_segments::records32),N_mach_segments::__data_size);

                blockSignals(true);

                QList<XMACH::SEGMENT_RECORD> listSegmentRecords=mach.getSegmentRecords();

                int nNumberOfSegments=listSegmentRecords.count();

                ui->tableWidget_segments->setRowCount(nNumberOfSegments);

                for(int i=0; i<nNumberOfSegments; i++)
                {
                    QTableWidgetItem *pItem=new QTableWidgetItem(QString::number(i));

                    if(getOptions()->bIsImage)
                    {
                        pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,listSegmentRecords.at(i).vmaddr);
                        pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_SIZE,listSegmentRecords.at(i).vmsize);
                    }
                    else
                    {
                        pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,listSegmentRecords.at(i).fileoff);
                        pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_SIZE,listSegmentRecords.at(i).filesize);
                    }

                    pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS,listSegmentRecords.at(i).vmaddr);

                    ui->tableWidget_segments->setItem(i,0,                                  pItem);
                    ui->tableWidget_segments->setItem(i,N_mach_segments::segname+1,         new QTableWidgetItem(listSegmentRecords.at(i).segname));

                    if(bIs64)
                    {
                        ui->tableWidget_segments->setItem(i,N_mach_segments::vmaddr+1,      new QTableWidgetItem(XBinary::valueToHex((quint64)listSegmentRecords.at(i).vmaddr)));
                        ui->tableWidget_segments->setItem(i,N_mach_segments::vmsize+1,      new QTableWidgetItem(XBinary::valueToHex((quint64)listSegmentRecords.at(i).vmsize)));
                        ui->tableWidget_segments->setItem(i,N_mach_segments::fileoff+1,     new QTableWidgetItem(XBinary::valueToHex((quint64)listSegmentRecords.at(i).fileoff)));
                        ui->tableWidget_segments->setItem(i,N_mach_segments::filesize+1,    new QTableWidgetItem(XBinary::valueToHex((quint64)listSegmentRecords.at(i).filesize)));
                    }
                    else
                    {
                        ui->tableWidget_segments->setItem(i,N_mach_segments::vmaddr+1,      new QTableWidgetItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).vmaddr)));
                        ui->tableWidget_segments->setItem(i,N_mach_segments::vmsize+1,      new QTableWidgetItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).vmsize)));
                        ui->tableWidget_segments->setItem(i,N_mach_segments::fileoff+1,     new QTableWidgetItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).fileoff)));
                        ui->tableWidget_segments->setItem(i,N_mach_segments::filesize+1,    new QTableWidgetItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).filesize)));
                    }

                    ui->tableWidget_segments->setItem(i,N_mach_segments::maxprot+1,         new QTableWidgetItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).maxprot)));
                    ui->tableWidget_segments->setItem(i,N_mach_segments::initprot+1,        new QTableWidgetItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).initprot)));
                    ui->tableWidget_segments->setItem(i,N_mach_segments::nsects+1,          new QTableWidgetItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).nsects)));
                    ui->tableWidget_segments->setItem(i,N_mach_segments::flags+1,           new QTableWidgetItem(XBinary::valueToHex((quint32)listSegmentRecords.at(i).flags)));
                }

                if(nNumberOfSegments)
                {
                    if(ui->tableWidget_segments->currentRow()==0)
                    {
                        loadSegment(0);
                    }
                    else
                    {
                        ui->tableWidget_segments->setCurrentCell(0,0);
                    }
                }

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_sections)
        {
            if(!g_stInit.contains(sInit))
            {
                bool bIs64=mach.is64();

                createSectionTable(SMACH::TYPE_mach_sections,ui->tableWidget_sections,bIs64?(N_mach_sections::records64):(N_mach_sections::records32),N_mach_sections::__data_size);

                blockSignals(true);

                QList<XMACH::SECTION_RECORD> listSectionRecords=mach.getSectionRecords();

                int nNumberOfSections=listSectionRecords.count();

                ui->tableWidget_sections->setRowCount(nNumberOfSections);

                for(int i=0; i<nNumberOfSections; i++)
                {
                    QTableWidgetItem *pItem=new QTableWidgetItem(QString::number(i));

                    if(getOptions()->bIsImage)
                    {
                        pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,listSectionRecords.at(i).addr);
                    }
                    else
                    {
                        pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,listSectionRecords.at(i).offset);
                    }

                    pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_SIZE,listSectionRecords.at(i).size);
                    pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS,listSectionRecords.at(i).addr);

                    ui->tableWidget_sections->setItem(i,0,                              pItem);
                    ui->tableWidget_sections->setItem(i,N_mach_sections::segname+1,     new QTableWidgetItem(listSectionRecords.at(i).segname));
                    ui->tableWidget_sections->setItem(i,N_mach_sections::sectname+1,    new QTableWidgetItem(listSectionRecords.at(i).sectname));

                    if(bIs64)
                    {
                        ui->tableWidget_sections->setItem(i,N_mach_sections::addr+1,    new QTableWidgetItem(XBinary::valueToHex((quint64)listSectionRecords.at(i).addr)));
                        ui->tableWidget_sections->setItem(i,N_mach_sections::size+1,    new QTableWidgetItem(XBinary::valueToHex((quint64)listSectionRecords.at(i).size)));
                    }
                    else
                    {
                        ui->tableWidget_sections->setItem(i,N_mach_sections::addr+1,    new QTableWidgetItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).addr)));
                        ui->tableWidget_sections->setItem(i,N_mach_sections::size+1,    new QTableWidgetItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).size)));
                    }

                    ui->tableWidget_sections->setItem(i,N_mach_sections::offset+1,      new QTableWidgetItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).offset)));
                    ui->tableWidget_sections->setItem(i,N_mach_sections::align+1,       new QTableWidgetItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).align)));
                    ui->tableWidget_sections->setItem(i,N_mach_sections::reloff+1,      new QTableWidgetItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).reloff)));
                    ui->tableWidget_sections->setItem(i,N_mach_sections::nreloc+1,      new QTableWidgetItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).nreloc)));
                    ui->tableWidget_sections->setItem(i,N_mach_sections::flags+1,       new QTableWidgetItem(XBinary::valueToHex((quint32)listSectionRecords.at(i).flags)));
                }

                if(nNumberOfSections)
                {
                    if(ui->tableWidget_sections->currentRow()==0)
                    {
                        loadSection(0);
                    }
                    else
                    {
                        ui->tableWidget_sections->setCurrentCell(0,0);
                    }
                }

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_libraries)
        {
            if(!g_stInit.contains(sInit))
            {
                createSectionTable(SMACH::TYPE_mach_libraries,ui->tableWidget_libraries,N_mach_libraries::records,N_mach_libraries::__data_size);

                blockSignals(true);

                QList<XMACH::LIBRARY_RECORD> listLibraries=mach.getLibraryRecords();

                int nNumberOfLibraries=listLibraries.count();

                ui->tableWidget_libraries->setRowCount(nNumberOfLibraries);

                for(int i=0; i<nNumberOfLibraries; i++)
                {
                    ui->tableWidget_libraries->setItem(i,N_mach_libraries::timestamp,                   new QTableWidgetItem(XBinary::valueToHex(listLibraries.at(i).timestamp)));
                    ui->tableWidget_libraries->setItem(i,N_mach_libraries::current_version,             new QTableWidgetItem(XBinary::valueToHex(listLibraries.at(i).current_version)));
                    ui->tableWidget_libraries->setItem(i,N_mach_libraries::compatibility_version,       new QTableWidgetItem(XBinary::valueToHex(listLibraries.at(i).compatibility_version)));
                    ui->tableWidget_libraries->setItem(i,N_mach_libraries::compatibility_version+1,     new QTableWidgetItem(listLibraries.at(i).sFullName));
                }

                blockSignals(false);
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    g_stInit.insert(sInit);
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
                case N_mach_header::magic:      g_lineEdit_mach_header[N_mach_header::magic]->setValue((quint32)nValue);      break;
                case N_mach_header::cputype:    g_lineEdit_mach_header[N_mach_header::cputype]->setValue((quint32)nValue);    break;
                case N_mach_header::filetype:   g_lineEdit_mach_header[N_mach_header::filetype]->setValue((quint32)nValue);   break;
                case N_mach_header::flags:      g_lineEdit_mach_header[N_mach_header::flags]->setValue((quint32)nValue);      break;
            }

            break;
    }
}

void MACHWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious)
{
    Q_UNUSED(pPrevious)

    if(pCurrent)
    {
        reloadData();
    }
}

void MACHWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

bool MACHWidget::createSectionTable(int nType, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, int nRecordCount)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);
    QStringList slHeader;

    // TODO
    switch(nType)
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
            pTableWidget->setColumnWidth(0,nSymbolWidth*4);
            pTableWidget->setColumnWidth(1,nSymbolWidth*8);
            pTableWidget->setColumnWidth(2,nSymbolWidth*12);
            pTableWidget->setColumnWidth(3,nSymbolWidth*12);
            pTableWidget->setColumnWidth(4,nSymbolWidth*12);
            pTableWidget->setColumnWidth(5,nSymbolWidth*12);
            pTableWidget->setColumnWidth(6,nSymbolWidth*8);
            pTableWidget->setColumnWidth(7,nSymbolWidth*8);
            pTableWidget->setColumnWidth(8,nSymbolWidth*8);
            pTableWidget->setColumnWidth(9,nSymbolWidth*8);
            break;

        case SMACH::TYPE_mach_sections:
            slHeader.append(tr(""));
            pTableWidget->setColumnCount(nRecordCount+1);
            pTableWidget->setColumnWidth(0,nSymbolWidth*4);
            pTableWidget->setColumnWidth(1,nSymbolWidth*12);
            pTableWidget->setColumnWidth(2,nSymbolWidth*8);
            pTableWidget->setColumnWidth(3,nSymbolWidth*12);
            pTableWidget->setColumnWidth(4,nSymbolWidth*12);
            pTableWidget->setColumnWidth(5,nSymbolWidth*8);
            pTableWidget->setColumnWidth(6,nSymbolWidth*8);
            pTableWidget->setColumnWidth(7,nSymbolWidth*8);
            pTableWidget->setColumnWidth(8,nSymbolWidth*8);
            pTableWidget->setColumnWidth(9,nSymbolWidth*8);
            break;

        case SMACH::TYPE_mach_libraries:
            pTableWidget->setColumnCount(nRecordCount+1);
            pTableWidget->setColumnWidth(0,nSymbolWidth*10);
            pTableWidget->setColumnWidth(1,nSymbolWidth*10);
            pTableWidget->setColumnWidth(2,nSymbolWidth*10);
            pTableWidget->setColumnWidth(3,nSymbolWidth*45);
            break;

        default:
            pTableWidget->setColumnCount(nRecordCount);
    }

    pTableWidget->setRowCount(0);

    for(int i=0; i<nRecordCount; i++)
    {
        slHeader.append(pRecords[i].sName);
    }

    switch(nType)
    {
        case SMACH::TYPE_mach_commands:
            slHeader.append(tr("Type"));
            break;

        case SMACH::TYPE_mach_libraries:
            slHeader.append(tr("Library"));
            break;
    }

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    return true;
}

void MACHWidget::on_tableWidget_commands_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    if(nCurrentRow!=-1)
    {
        loadCommand(nCurrentRow);
    }
}

void MACHWidget::on_pushButtonReload_clicked()
{
    ui->pushButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000,this,SLOT(enableButton()));
}

void MACHWidget::enableButton()
{
    ui->pushButtonReload->setEnabled(true);
}

void MACHWidget::on_tableWidget_segments_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    if(nCurrentRow!=-1)
    {
        loadSegment(nCurrentRow);
    }
}

void MACHWidget::on_tableWidget_sections_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    if(nCurrentRow!=-1)
    {
        loadSection(nCurrentRow);
    }
}

void MACHWidget::loadCommand(int nNumber)
{
    qint64 nOffset=ui->tableWidget_commands->item(nNumber,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nSize=ui->tableWidget_commands->item(nNumber,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
    qint64 nAddress=ui->tableWidget_commands->item(nNumber,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

    loadHexSubdevice(nOffset,nSize,nAddress,&g_subDevice[SMACH::TYPE_mach_commands],ui->widgetHex_commands);
}

void MACHWidget::loadSegment(int nNumber)
{
    qint64 nOffset=ui->tableWidget_segments->item(nNumber,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nSize=ui->tableWidget_segments->item(nNumber,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
    qint64 nAddress=ui->tableWidget_segments->item(nNumber,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

    loadHexSubdevice(nOffset,nSize,nAddress,&g_subDevice[SMACH::TYPE_mach_segments],ui->widgetHex_segments);
}

void MACHWidget::loadSection(int nNumber)
{
    qint64 nOffset=ui->tableWidget_sections->item(nNumber,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nSize=ui->tableWidget_sections->item(nNumber,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
    qint64 nAddress=ui->tableWidget_sections->item(nNumber,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

    loadHexSubdevice(nOffset,nSize,nAddress,&g_subDevice[SMACH::TYPE_mach_sections],ui->widgetHex_sections);
}

void MACHWidget::on_tableWidget_mach_header_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_mach_header,ui->tableWidget_mach_header);
}

void MACHWidget::on_tableWidget_commands_customContextMenuRequested(const QPoint &pos)
{
    // TODO
}

void MACHWidget::on_tableWidget_segments_customContextMenuRequested(const QPoint &pos)
{
    // TODO
}

void MACHWidget::on_tableWidget_sections_customContextMenuRequested(const QPoint &pos)
{
    // TODO
}
