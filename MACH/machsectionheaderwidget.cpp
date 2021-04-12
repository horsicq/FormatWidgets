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
#include "machsectionheaderwidget.h"
#include "ui_machsectionheaderwidget.h"

MACHSectionHeaderWidget::MACHSectionHeaderWidget(QWidget *pParent):
    FormatWidget(pParent),
    ui(new Ui::MACHSectionHeaderWidget)
{
    ui->setupUi(this);
}

MACHSectionHeaderWidget::MACHSectionHeaderWidget(QIODevice *pDevice,FW_DEF::OPTIONS options,quint32 nNumber,qint64 nOffset,qint32 nType,QWidget *pParent):
    MACHSectionHeaderWidget(pParent)
{
     MACHSectionHeaderWidget::setData(pDevice,options,nNumber,nOffset,nType);

     XMACH mach(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

     bool bIs64=mach.is64();

     g_ppLinedEdit=0;
     g_nLineEditSize=0;
     g_ppComboBox=0;
     g_nComboBoxSize=0;
     g_ppInvWidget=0;
     g_nInvWidgetSize=0;

     if(nType==SMACH::TYPE_mach_commands)
     {
         g_nLineEditSize=N_mach_commands::__data_size;
         g_nComboBoxSize=N_mach_commands::__CB_size;
         g_nInvWidgetSize=N_mach_commands::__INV_size;
     }
     if((nType==SMACH::TYPE_mach_libraries)||(nType==SMACH::TYPE_mach_id_library))
     {
         g_nLineEditSize=N_mach_library::__data_size;
         g_nComboBoxSize=N_mach_library::__CB_size;
         g_nInvWidgetSize=N_mach_library::__INV_size;
     }
     else if(nType==SMACH::TYPE_mach_segments)
     {
         g_nLineEditSize=N_mach_segments::__data_size;
         g_nComboBoxSize=N_mach_segments::__CB_size;
         g_nInvWidgetSize=N_mach_segments::__INV_size;
     }
     else if(nType==SMACH::TYPE_mach_sections)
     {
         if(bIs64)
         {
             g_nLineEditSize=N_mach_sections64::__data_size;
             g_nComboBoxSize=N_mach_sections64::__CB_size;
             g_nInvWidgetSize=N_mach_sections64::__INV_size;
         }
         else
         {
             g_nLineEditSize=N_mach_sections32::__data_size;
             g_nComboBoxSize=N_mach_sections32::__CB_size;
             g_nInvWidgetSize=N_mach_sections32::__INV_size;
         }
     }

     if(g_nLineEditSize)
     {
         g_ppLinedEdit=new PXLineEditHEX[g_nLineEditSize];
     }

     if(g_nComboBoxSize)
     {
         g_ppComboBox=new PXComboBoxEx[g_nComboBoxSize];
     }

     if(g_nInvWidgetSize)
     {
         g_ppInvWidget=new PInvWidget[g_nInvWidgetSize];
     }
}

MACHSectionHeaderWidget::~MACHSectionHeaderWidget()
{
    if(g_ppLinedEdit)
    {
        delete[] g_ppLinedEdit;
    }

    if(g_ppComboBox)
    {
        delete[] g_ppComboBox;
    }

    if(g_ppInvWidget)
    {
        delete[] g_ppInvWidget;
    }

    delete ui;
}

void MACHSectionHeaderWidget::clear()
{
    reset();

    memset(g_ppLinedEdit,0,g_nLineEditSize*sizeof(XLineEditHEX *));
    memset(g_ppComboBox,0,g_nComboBoxSize*sizeof(XComboBoxEx *));
    memset(g_ppInvWidget,0,g_nInvWidgetSize*sizeof(InvWidget *));

    g_pSubDevice=nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void MACHSectionHeaderWidget::cleanup()
{

}

void MACHSectionHeaderWidget::reset()
{
    g_bInit=false;
}

void MACHSectionHeaderWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

void MACHSectionHeaderWidget::setShortcuts(XShortcuts *pShortcuts)
{
    ui->widgetHex->setShortcuts(pShortcuts);
    XShortcutsWidget::setShortcuts(pShortcuts);
}

FormatWidget::SV MACHSectionHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)

    SV result=SV_NONE;

    quint64 nValue=vValue.toULongLong();
    QString sValue=vValue.toString();

    if(getDevice()->isWritable())
    {
        XMACH mach(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

        if(mach.isValid())
        {
            switch(nStype)
            {
                case SMACH::TYPE_mach_commands:
                    switch(nNdata)
                    {
                        case N_mach_commands::cmd:          g_ppComboBox[N_mach_commands::cmd]->setValue(nValue);               break;
                    }

                    break;
            }

            switch(nStype)
            {
                case SMACH::TYPE_mach_commands:
                    switch(nNdata)
                    {
                        case N_mach_commands::cmd:          mach._setCommand_cmd(nOffset,nValue);                       break;
                        case N_mach_commands::cmdsize:      mach._setCommand_cmdsize(nOffset,nValue);                   break;
                    }

                    break;

                case SMACH::TYPE_mach_libraries:
                    switch(nNdata)
                    {
                        case N_mach_library::timestamp:               mach._setLibraryRecord_timestamp(nOffset,nValue);               break;
                        case N_mach_library::current_version:         mach._setLibraryRecord_current_version(nOffset,nValue);         break;
                        case N_mach_library::compatibility_version:   mach._setLibraryRecord_compatibility_version(nOffset,nValue);   break;
                        case N_mach_library::name:                    mach._setLibraryRecord_name(nOffset,sValue);                    break;
                    }

                    break;

                case SMACH::TYPE_mach_segments:
                    if(mach.is64())
                    {
                        switch(nNdata)
                        {
                            case N_mach_segments::segname:      mach._setSegment64_segname(nOffset,sValue);             break;
                            case N_mach_segments::vmaddr:       mach._setSegment64_vmaddr(nOffset,nValue);              break;
                            case N_mach_segments::vmsize:       mach._setSegment64_vmsize(nOffset,nValue);              break;
                            case N_mach_segments::fileoff:      mach._setSegment64_fileoff(nOffset,nValue);             break;
                            case N_mach_segments::filesize:     mach._setSegment64_filesize(nOffset,nValue);            break;
                            case N_mach_segments::maxprot:      mach._setSegment64_maxprot(nOffset,nValue);             break;
                            case N_mach_segments::initprot:     mach._setSegment64_initprot(nOffset,nValue);            break;
                            case N_mach_segments::nsects:       mach._setSegment64_nsects(nOffset,nValue);              break;
                            case N_mach_segments::flags:        mach._setSegment64_flags(nOffset,nValue);               break;
                        }
                    }
                    else
                    {
                        switch(nNdata)
                        {
                            case N_mach_segments::segname:      mach._setSegment32_segname(nOffset,sValue);             break;
                            case N_mach_segments::vmaddr:       mach._setSegment32_vmaddr(nOffset,nValue);              break;
                            case N_mach_segments::vmsize:       mach._setSegment32_vmsize(nOffset,nValue);              break;
                            case N_mach_segments::fileoff:      mach._setSegment32_fileoff(nOffset,nValue);             break;
                            case N_mach_segments::filesize:     mach._setSegment32_filesize(nOffset,nValue);            break;
                            case N_mach_segments::maxprot:      mach._setSegment32_maxprot(nOffset,nValue);             break;
                            case N_mach_segments::initprot:     mach._setSegment32_initprot(nOffset,nValue);            break;
                            case N_mach_segments::nsects:       mach._setSegment32_nsects(nOffset,nValue);              break;
                            case N_mach_segments::flags:        mach._setSegment32_flags(nOffset,nValue);               break;
                        }
                    }

                    break;

                case SMACH::TYPE_mach_sections:
                    if(mach.is64())
                    {
                        switch(nNdata)
                        {
                            case N_mach_sections64::sectname:   mach._setSection64_sectname(nOffset,sValue);            break;
                            case N_mach_sections64::segname:    mach._setSection64_segname(nOffset,sValue);             break;
                            case N_mach_sections64::addr:       mach._setSection64_addr(nOffset,nValue);                break;
                            case N_mach_sections64::size:       mach._setSection64_size(nOffset,nValue);                break;
                            case N_mach_sections64::offset:     mach._setSection64_offset(nOffset,nValue);              break;
                            case N_mach_sections64::align:      mach._setSection64_align(nOffset,nValue);               break;
                            case N_mach_sections64::reloff:     mach._setSection64_reloff(nOffset,nValue);              break;
                            case N_mach_sections64::nreloc:     mach._setSection64_nreloc(nOffset,nValue);              break;
                            case N_mach_sections64::flags:      mach._setSection64_flags(nOffset,nValue);               break;
                            case N_mach_sections64::reserved1:  mach._setSection64_reserved1(nOffset,nValue);           break;
                            case N_mach_sections64::reserved2:  mach._setSection64_reserved2(nOffset,nValue);           break;
                            case N_mach_sections64::reserved3:  mach._setSection64_reserved3(nOffset,nValue);           break;
                        }
                    }
                    else
                    {
                        switch(nNdata)
                        {
                            case N_mach_sections32::sectname:   mach._setSection32_sectname(nOffset,sValue);            break;
                            case N_mach_sections32::segname:    mach._setSection32_segname(nOffset,sValue);             break;
                            case N_mach_sections32::addr:       mach._setSection32_addr(nOffset,nValue);                break;
                            case N_mach_sections32::size:       mach._setSection32_size(nOffset,nValue);                break;
                            case N_mach_sections32::offset:     mach._setSection32_offset(nOffset,nValue);              break;
                            case N_mach_sections32::align:      mach._setSection32_align(nOffset,nValue);               break;
                            case N_mach_sections32::reloff:     mach._setSection32_reloff(nOffset,nValue);              break;
                            case N_mach_sections32::nreloc:     mach._setSection32_nreloc(nOffset,nValue);              break;
                            case N_mach_sections32::flags:      mach._setSection32_flags(nOffset,nValue);               break;
                            case N_mach_sections32::reserved1:  mach._setSection32_reserved1(nOffset,nValue);           break;
                            case N_mach_sections32::reserved2:  mach._setSection32_reserved2(nOffset,nValue);           break;
                        }
                    }

                    break;
            }

            ui->widgetHex->reload();

            result=SV_EDITED;
        }
    }

    return result;
}

void MACHSectionHeaderWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(g_ppLinedEdit,g_nLineEditSize,bState);
    setComboBoxesReadOnly(g_ppComboBox,g_nComboBoxSize,bState);
}

void MACHSectionHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_ppLinedEdit,g_nLineEditSize,bState);
    _blockSignals((QObject **)g_ppComboBox,g_nComboBoxSize,bState);
}

void MACHSectionHeaderWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

    switch(nType)
    {
        case SMACH::TYPE_mach_commands:
            pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
            pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;

        case SMACH::TYPE_mach_id_library:
        case SMACH::TYPE_mach_libraries:
            pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
            pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;

        case SMACH::TYPE_mach_segments:
            pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
            pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;

        case SMACH::TYPE_mach_sections:
            pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
            pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;
    }
}

void MACHSectionHeaderWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void MACHSectionHeaderWidget::reloadData()
{
    int nType=getType();

    XMACH mach(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

    if(mach.isValid())
    {
        if(!g_bInit)
        {
            bool bIs64=mach.is64();
            bool bIsBigEndian=mach.isBigEndian();

            if(nType==SMACH::TYPE_mach_commands)
            {
                g_bInit=createHeaderTable(SMACH::TYPE_mach_commands,ui->tableWidget,N_mach_commands::records,g_ppLinedEdit,N_mach_commands::__data_size,getNumber(),getOffset());
                g_ppComboBox[N_mach_commands::CB_CMD]=createComboBox(ui->tableWidget,XMACH::getLoadCommandTypesS(),SMACH::TYPE_mach_commands,N_mach_commands::cmd,XComboBoxEx::CBTYPE_NORMAL);

                blockSignals(true);

                qint64 nHeaderOffset=getOffset();

                XMACH::COMMAND_RECORD cr=mach._readLoadCommand(nHeaderOffset,bIsBigEndian);

                g_ppLinedEdit[N_mach_commands::cmd]->setValue((quint32)cr.nType);
                g_ppLinedEdit[N_mach_commands::cmdsize]->setValue((quint32)cr.nSize);

                g_ppComboBox[N_mach_commands::CB_CMD]->setValue(cr.nType);

                qint64 nOffset=nHeaderOffset;
                qint64 nSize=mach.getCommandHeaderSize();
                qint64 nAddress=mach.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset,nSize,nAddress,&g_pSubDevice,ui->widgetHex);

                blockSignals(false);
            }
            else if((nType==SMACH::TYPE_mach_libraries)||(nType==SMACH::TYPE_mach_id_library))
            {
                g_bInit=createHeaderTable(nType,ui->tableWidget,N_mach_library::records,g_ppLinedEdit,N_mach_library::__data_size,getNumber(),getOffset());

                blockSignals(true);

                qint64 nHeaderOffset=getOffset();

                XMACH::LIBRARY_RECORD lr=mach._readLibraryRecord(nHeaderOffset,bIsBigEndian);
                XMACH::COMMAND_RECORD cr=mach._readLoadCommand(nHeaderOffset,bIsBigEndian);

                g_ppLinedEdit[N_mach_library::timestamp]->setValue((quint32)lr.timestamp);
                g_ppLinedEdit[N_mach_library::current_version]->setValue((quint32)lr.current_version);
                g_ppLinedEdit[N_mach_library::compatibility_version]->setValue((quint32)lr.compatibility_version);
                g_ppLinedEdit[N_mach_library::name]->setStringValue(lr.sFullName,lr.nMaxStringSize);

                if(lr.nMaxStringSize)
                {
                    QTableWidgetItem *pItem=ui->tableWidget->item(N_mach_library::name,0);

                    if(pItem)
                    {
                        pItem->setData(Qt::UserRole+HEADER_DATA_SIZE,lr.nMaxStringSize);
                    }
                }

                qint64 nOffset=nHeaderOffset;
                qint64 nSize=cr.nSize;
                qint64 nAddress=mach.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset,nSize,nAddress,&g_pSubDevice,ui->widgetHex);

                blockSignals(false);
            }
            else if(nType==SMACH::TYPE_mach_segments)
            {
                g_bInit=createHeaderTable(SMACH::TYPE_mach_segments,ui->tableWidget,bIs64?(N_mach_segments::records64):(N_mach_segments::records32),g_ppLinedEdit,N_mach_segments::__data_size,getNumber(),getOffset());
//                g_ppComboBox[N_mach_commands::CB_CMD]=createComboBox(ui->tableWidget,XMACH::getLoadCommandTypesS(),SMACH::TYPE_mach_commands,N_mach_commands::cmd,XComboBoxEx::CBTYPE_NORMAL);

                blockSignals(true);

                qint64 nHeaderOffset=getOffset();

                if(bIs64)
                {
                    XMACH_DEF::segment_command_64 segment=mach._readSegment64(nHeaderOffset,bIsBigEndian);

                    g_ppLinedEdit[N_mach_segments::segname]->setStringValue(segment.segname);
                    g_ppLinedEdit[N_mach_segments::vmaddr]->setValue(segment.vmaddr);
                    g_ppLinedEdit[N_mach_segments::vmsize]->setValue(segment.vmsize);
                    g_ppLinedEdit[N_mach_segments::fileoff]->setValue(segment.fileoff);
                    g_ppLinedEdit[N_mach_segments::filesize]->setValue(segment.filesize);
                    g_ppLinedEdit[N_mach_segments::maxprot]->setValue(segment.maxprot);
                    g_ppLinedEdit[N_mach_segments::initprot]->setValue(segment.initprot);
                    g_ppLinedEdit[N_mach_segments::nsects]->setValue(segment.nsects);
                    g_ppLinedEdit[N_mach_segments::flags]->setValue(segment.flags);
                }
                else
                {
                    XMACH_DEF::segment_command segment=mach._readSegment32(nHeaderOffset,bIsBigEndian);

                    g_ppLinedEdit[N_mach_segments::segname]->setStringValue(segment.segname);
                    g_ppLinedEdit[N_mach_segments::vmaddr]->setValue(segment.vmaddr);
                    g_ppLinedEdit[N_mach_segments::vmsize]->setValue(segment.vmsize);
                    g_ppLinedEdit[N_mach_segments::fileoff]->setValue(segment.fileoff);
                    g_ppLinedEdit[N_mach_segments::filesize]->setValue(segment.filesize);
                    g_ppLinedEdit[N_mach_segments::maxprot]->setValue(segment.maxprot);
                    g_ppLinedEdit[N_mach_segments::initprot]->setValue(segment.initprot);
                    g_ppLinedEdit[N_mach_segments::nsects]->setValue(segment.nsects);
                    g_ppLinedEdit[N_mach_segments::flags]->setValue(segment.flags);
                }

//                g_ppComboBox[N_mach_commands::CB_CMD]->setValue(cr.nType);

                qint64 nOffset=nHeaderOffset;
                qint64 nSize=mach.getSegmentHeaderSize();
                qint64 nAddress=mach.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset,nSize,nAddress,&g_pSubDevice,ui->widgetHex);

                blockSignals(false);
            }
            else if(nType==SMACH::TYPE_mach_sections)
            {
                g_bInit=createHeaderTable(SMACH::TYPE_mach_sections,ui->tableWidget,bIs64?(N_mach_sections64::records):(N_mach_sections32::records),g_ppLinedEdit,bIs64?(N_mach_sections64::__data_size):(N_mach_sections32::__data_size),getNumber(),getOffset());
//                g_ppComboBox[N_mach_commands::CB_CMD]=createComboBox(ui->tableWidget,XMACH::getLoadCommandTypesS(),SMACH::TYPE_mach_commands,N_mach_commands::cmd,XComboBoxEx::CBTYPE_NORMAL);

                blockSignals(true);

                qint64 nHeaderOffset=getOffset();

                if(bIs64)
                {
                    XMACH_DEF::section_64 section=mach._readSection64(nHeaderOffset,bIsBigEndian);

                    g_ppLinedEdit[N_mach_sections64::sectname]->setStringValue(section.sectname);
                    g_ppLinedEdit[N_mach_sections64::segname]->setStringValue(section.segname);
                    g_ppLinedEdit[N_mach_sections64::addr]->setValue(section.addr);
                    g_ppLinedEdit[N_mach_sections64::size]->setValue(section.size);
                    g_ppLinedEdit[N_mach_sections64::offset]->setValue(section.offset);
                    g_ppLinedEdit[N_mach_sections64::align]->setValue(section.align);
                    g_ppLinedEdit[N_mach_sections64::reloff]->setValue(section.reloff);
                    g_ppLinedEdit[N_mach_sections64::nreloc]->setValue(section.nreloc);
                    g_ppLinedEdit[N_mach_sections64::flags]->setValue(section.flags);
                    g_ppLinedEdit[N_mach_sections64::reserved1]->setValue(section.reserved1);
                    g_ppLinedEdit[N_mach_sections64::reserved2]->setValue(section.reserved2);
                    g_ppLinedEdit[N_mach_sections64::reserved3]->setValue(section.reserved3);
                }
                else
                {
                    XMACH_DEF::section section=mach._readSection32(nHeaderOffset,bIsBigEndian);

                    g_ppLinedEdit[N_mach_sections32::sectname]->setStringValue(section.sectname);
                    g_ppLinedEdit[N_mach_sections32::segname]->setStringValue(section.segname);
                    g_ppLinedEdit[N_mach_sections32::addr]->setValue(section.addr);
                    g_ppLinedEdit[N_mach_sections32::size]->setValue(section.size);
                    g_ppLinedEdit[N_mach_sections32::offset]->setValue(section.offset);
                    g_ppLinedEdit[N_mach_sections32::align]->setValue(section.align);
                    g_ppLinedEdit[N_mach_sections32::reloff]->setValue(section.reloff);
                    g_ppLinedEdit[N_mach_sections32::nreloc]->setValue(section.nreloc);
                    g_ppLinedEdit[N_mach_sections32::flags]->setValue(section.flags);
                    g_ppLinedEdit[N_mach_sections32::reserved1]->setValue(section.reserved1);
                    g_ppLinedEdit[N_mach_sections32::reserved2]->setValue(section.reserved2);
                }

                qint64 nOffset=nHeaderOffset;
                qint64 nSize=mach.getSectionHeaderSize();
                qint64 nAddress=mach.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset,nSize,nAddress,&g_pSubDevice,ui->widgetHex);

                blockSignals(false);
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void MACHSectionHeaderWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
        case SMACH::TYPE_mach_commands:
            switch(nNdata)
            {
                case N_mach_commands::cmd:      g_ppLinedEdit[N_mach_commands::cmd]->setValue((quint32)nValue);                     break;
            }
            break;
    }
}

void MACHSectionHeaderWidget::on_tableWidget_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex,ui->tableWidget);
}
