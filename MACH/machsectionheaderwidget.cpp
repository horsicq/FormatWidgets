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
    if( (nType==SMACH::TYPE_mach_libraries)||
        (nType==SMACH::TYPE_mach_weak_libraries)||
        (nType==SMACH::TYPE_mach_id_library))
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
            g_nLineEditSize=N_mach_sections64_E::__data_size;
            g_nComboBoxSize=N_mach_sections64_E::__CB_size;
            g_nInvWidgetSize=N_mach_sections64_E::__INV_size;
        }
        else
        {
            g_nLineEditSize=N_mach_sections32_E::__data_size;
            g_nComboBoxSize=N_mach_sections32_E::__CB_size;
            g_nInvWidgetSize=N_mach_sections32_E::__INV_size;
        }
    }
    else if(nType==SMACH::TYPE_SYMBOLTABLE)
    {
        g_nLineEditSize=N_mach_nlist::__data_size;
        g_nComboBoxSize=N_mach_nlist::__CB_size;
        g_nInvWidgetSize=N_mach_nlist::__INV_size;
    }
    else if(nType==SMACH::TYPE_DICE)
    {
        g_nLineEditSize=N_mach_data_in_code_entry::__data_size;
        g_nComboBoxSize=N_mach_data_in_code_entry::__CB_size;
        g_nInvWidgetSize=N_mach_data_in_code_entry::__INV_size;
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
                        case N_mach_commands::cmd:          g_ppComboBox[N_mach_commands::CB_CMD]->setValue(nValue);            break;
                    }

                    break;

                case SMACH::TYPE_mach_segments:
                    switch(nNdata)
                    {
                        case N_mach_segments::initprot:     g_ppComboBox[N_mach_segments::CB_initprot]->setValue(nValue);       break;
                        case N_mach_segments::maxprot:      g_ppComboBox[N_mach_segments::CB_maxprot]->setValue(nValue);        break;
                    }

                    break;

                case SMACH::TYPE_SYMBOLTABLE:
                    switch(nNdata)
                    {
                        case N_mach_nlist::n_strx:
                            XBinary::OFFSETSIZE osStringTable=mach.getStringTableOS();
                            addComment(ui->tableWidget,N_mach_nlist::n_strx,HEADER_COLUMN_COMMENT,mach.getStringFromIndex(osStringTable.nOffset,osStringTable.nSize,nValue));
                            break;
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
                case SMACH::TYPE_mach_weak_libraries:
                case SMACH::TYPE_mach_id_library:
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
                            case N_mach_sections64_E::sectname:     mach._setSection64_sectname(nOffset,sValue);            break;
                            case N_mach_sections64_E::segname:      mach._setSection64_segname(nOffset,sValue);             break;
                            case N_mach_sections64_E::addr:         mach._setSection64_addr(nOffset,nValue);                break;
                            case N_mach_sections64_E::size:         mach._setSection64_size(nOffset,nValue);                break;
                            case N_mach_sections64_E::offset:       mach._setSection64_offset(nOffset,nValue);              break;
                            case N_mach_sections64_E::align:        mach._setSection64_align(nOffset,nValue);               break;
                            case N_mach_sections64_E::reloff:       mach._setSection64_reloff(nOffset,nValue);              break;
                            case N_mach_sections64_E::nreloc:       mach._setSection64_nreloc(nOffset,nValue);              break;
                            case N_mach_sections64_E::flags_0:      mach._setSection64_flags(nOffset,nValue);               break;
                            case N_mach_sections64_E::reserved1:    mach._setSection64_reserved1(nOffset,nValue);           break;
                            case N_mach_sections64_E::reserved2:    mach._setSection64_reserved2(nOffset,nValue);           break;
                            case N_mach_sections64_E::reserved3:    mach._setSection64_reserved3(nOffset,nValue);           break;
                        }
                    }
                    else
                    {
                        switch(nNdata)
                        {
                            case N_mach_sections32_E::sectname:     mach._setSection32_sectname(nOffset,sValue);            break;
                            case N_mach_sections32_E::segname:      mach._setSection32_segname(nOffset,sValue);             break;
                            case N_mach_sections32_E::addr:         mach._setSection32_addr(nOffset,nValue);                break;
                            case N_mach_sections32_E::size:         mach._setSection32_size(nOffset,nValue);                break;
                            case N_mach_sections32_E::offset:       mach._setSection32_offset(nOffset,nValue);              break;
                            case N_mach_sections32_E::align:        mach._setSection32_align(nOffset,nValue);               break;
                            case N_mach_sections32_E::reloff:       mach._setSection32_reloff(nOffset,nValue);              break;
                            case N_mach_sections32_E::nreloc:       mach._setSection32_nreloc(nOffset,nValue);              break;
                            case N_mach_sections32_E::flags_0:      mach._setSection32_flags(nOffset,nValue);               break;
                            case N_mach_sections32_E::reserved1:    mach._setSection32_reserved1(nOffset,nValue);           break;
                            case N_mach_sections32_E::reserved2:    mach._setSection32_reserved2(nOffset,nValue);           break;
                        }
                    }

                    break;

                case SMACH::TYPE_SYMBOLTABLE:
                    if(mach.is64())
                    {
                        switch(nNdata)
                        {
                            case N_mach_nlist::n_strx:          mach._set_nlist_64_n_strx(nOffset,nValue);              break;
                            case N_mach_nlist::n_type:          mach._set_nlist_64_n_type(nOffset,nValue);              break;
                            case N_mach_nlist::n_sect:          mach._set_nlist_64_n_sect(nOffset,nValue);              break;
                            case N_mach_nlist::n_desc:          mach._set_nlist_64_n_desc(nOffset,nValue);              break;
                            case N_mach_nlist::n_value:         mach._set_nlist_64_n_value(nOffset,nValue);             break;
                        }
                    }
                    else
                    {
                        switch(nNdata)
                        {
                            case N_mach_nlist::n_strx:          mach._set_nlist_n_strx(nOffset,nValue);                 break;
                            case N_mach_nlist::n_type:          mach._set_nlist_n_type(nOffset,nValue);                 break;
                            case N_mach_nlist::n_sect:          mach._set_nlist_n_sect(nOffset,nValue);                 break;
                            case N_mach_nlist::n_desc:          mach._set_nlist_n_desc(nOffset,nValue);                 break;
                            case N_mach_nlist::n_value:         mach._set_nlist_n_value(nOffset,nValue);                break;
                        }
                    }

                    break;

                case SMACH::TYPE_mach_data_in_code:
                    switch(nNdata)
                    {
                        case N_mach_data_in_code_entry::offset:         mach._set_data_in_code_entry_offset(nOffset,nValue);                break;
                        case N_mach_data_in_code_entry::length:         mach._set_data_in_code_entry_length(nOffset,nValue);                break;
                        case N_mach_data_in_code_entry::kind:           mach._set_data_in_code_entry_kind(nOffset,nValue);                  break;
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
    XBinary::MODE mode=XMACH::getMode(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,getColumnWidth(this,CW_UINT16,mode));
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,getColumnWidth(this,CW_TYPE,mode));

    switch(nType)
    {
        case SMACH::TYPE_mach_commands:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,getColumnWidth(this,CW_STRINGSHORT,mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,getColumnWidth(this,CW_UINT32,mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,getColumnWidth(this,CW_STRINGMID,mode));
            break;

        case SMACH::TYPE_mach_libraries:
        case SMACH::TYPE_mach_weak_libraries:
        case SMACH::TYPE_mach_id_library:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,getColumnWidth(this,CW_STRINGSHORT,mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,getColumnWidth(this,CW_UINT32,mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,getColumnWidth(this,CW_STRINGSHORT,mode));
            break;

        case SMACH::TYPE_mach_segments:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,getColumnWidth(this,CW_STRINGSHORT,mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,getColumnWidth(this,CW_UINTMODE,mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,getColumnWidth(this,CW_STRINGSHORT,mode));
            break;

        case SMACH::TYPE_mach_sections:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,getColumnWidth(this,CW_STRINGSHORT,mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,getColumnWidth(this,CW_UINTMODE,mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,getColumnWidth(this,CW_STRINGMID,mode));
            break;

        case SMACH::TYPE_SYMBOLTABLE:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,getColumnWidth(this,CW_STRINGSHORT,mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,getColumnWidth(this,CW_UINT32,mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,getColumnWidth(this,CW_STRINGSHORT,mode));
            break;

        case SMACH::TYPE_DICE:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,getColumnWidth(this,CW_STRINGSHORT,mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,getColumnWidth(this,CW_UINT32,mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,getColumnWidth(this,CW_STRINGSHORT,mode));
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
        bool bIs64=mach.is64();
        bool bIsBigEndian=mach.isBigEndian();

        if(nType==SMACH::TYPE_mach_commands)
        {
            createHeaderTable(SMACH::TYPE_mach_commands,ui->tableWidget,N_mach_commands::records,g_ppLinedEdit,N_mach_commands::__data_size,getNumber(),getOffset());
            g_ppComboBox[N_mach_commands::CB_CMD]=createComboBox(ui->tableWidget,XMACH::getLoadCommandTypesS(),SMACH::TYPE_mach_commands,N_mach_commands::cmd,XComboBoxEx::CBTYPE_LIST);

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
        else if((nType==SMACH::TYPE_mach_libraries)||
                (nType==SMACH::TYPE_mach_weak_libraries)||
                (nType==SMACH::TYPE_mach_id_library))
        {
            createHeaderTable(nType,ui->tableWidget,N_mach_library::records,g_ppLinedEdit,N_mach_library::__data_size,getNumber(),getOffset());

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
            createHeaderTable(SMACH::TYPE_mach_segments,ui->tableWidget,bIs64?(N_mach_segments::records64):(N_mach_segments::records32),g_ppLinedEdit,N_mach_segments::__data_size,getNumber(),getOffset());
            g_ppComboBox[N_mach_segments::CB_initprot]=createComboBox(ui->tableWidget,XMACH::getVMProtectionsS(),SMACH::TYPE_mach_segments,N_mach_segments::initprot,XComboBoxEx::CBTYPE_FLAGS);
            g_ppComboBox[N_mach_segments::CB_maxprot]=createComboBox(ui->tableWidget,XMACH::getVMProtectionsS(),SMACH::TYPE_mach_segments,N_mach_segments::maxprot,XComboBoxEx::CBTYPE_FLAGS);

            blockSignals(true);

            qint64 nHeaderOffset=getOffset();

            if(bIs64)
            {
                XMACH_DEF::segment_command_64 segment=mach._read_segment_command_64(nHeaderOffset,bIsBigEndian);

                g_ppLinedEdit[N_mach_segments::segname]->setStringValue(segment.segname);
                g_ppLinedEdit[N_mach_segments::vmaddr]->setValue(segment.vmaddr);
                g_ppLinedEdit[N_mach_segments::vmsize]->setValue(segment.vmsize);
                g_ppLinedEdit[N_mach_segments::fileoff]->setValue(segment.fileoff);
                g_ppLinedEdit[N_mach_segments::filesize]->setValue(segment.filesize);
                g_ppLinedEdit[N_mach_segments::maxprot]->setValue(segment.maxprot);
                g_ppLinedEdit[N_mach_segments::initprot]->setValue(segment.initprot);
                g_ppLinedEdit[N_mach_segments::nsects]->setValue(segment.nsects);
                g_ppLinedEdit[N_mach_segments::flags]->setValue(segment.flags);

                g_ppComboBox[N_mach_segments::CB_initprot]->setValue(segment.initprot);
                g_ppComboBox[N_mach_segments::CB_maxprot]->setValue(segment.maxprot);
            }
            else
            {
                XMACH_DEF::segment_command segment=mach._read_segment_command(nHeaderOffset,bIsBigEndian);

                g_ppLinedEdit[N_mach_segments::segname]->setStringValue(segment.segname);
                g_ppLinedEdit[N_mach_segments::vmaddr]->setValue(segment.vmaddr);
                g_ppLinedEdit[N_mach_segments::vmsize]->setValue(segment.vmsize);
                g_ppLinedEdit[N_mach_segments::fileoff]->setValue(segment.fileoff);
                g_ppLinedEdit[N_mach_segments::filesize]->setValue(segment.filesize);
                g_ppLinedEdit[N_mach_segments::maxprot]->setValue(segment.maxprot);
                g_ppLinedEdit[N_mach_segments::initprot]->setValue(segment.initprot);
                g_ppLinedEdit[N_mach_segments::nsects]->setValue(segment.nsects);
                g_ppLinedEdit[N_mach_segments::flags]->setValue(segment.flags);

                g_ppComboBox[N_mach_segments::CB_initprot]->setValue(segment.initprot);
                g_ppComboBox[N_mach_segments::CB_maxprot]->setValue(segment.maxprot);
            }

            qint64 nOffset=nHeaderOffset;
            qint64 nSize=mach.getSegmentHeaderSize();
            qint64 nAddress=mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset,nSize,nAddress,&g_pSubDevice,ui->widgetHex);

            blockSignals(false);
        }
        else if(nType==SMACH::TYPE_mach_sections)
        {
            createHeaderTable(SMACH::TYPE_mach_sections,ui->tableWidget,bIs64?(N_mach_sections64_E::records):(N_mach_sections32_E::records),g_ppLinedEdit,bIs64?(N_mach_sections64_E::__data_size):(N_mach_sections32_E::__data_size),getNumber(),getOffset());

            blockSignals(true);

            qint64 nHeaderOffset=getOffset();

            if(bIs64)
            {
                g_ppComboBox[N_mach_sections64_E::CB_flag0]=createComboBox(ui->tableWidget,XMACH::getSectionFlagsTypesS(),SMACH::TYPE_mach_sections,N_mach_sections64_E::flags_0,XComboBoxEx::CBTYPE_ELIST,XMACH_DEF::SECTION_TYPE);
                g_ppComboBox[N_mach_sections64_E::CB_flag1]=createComboBox(ui->tableWidget,XMACH::getSectionAttributesSysS(),SMACH::TYPE_mach_sections,N_mach_sections64_E::flags_1,XComboBoxEx::CBTYPE_ELIST,XMACH_DEF::SECTION_ATTRIBUTES_SYS);
                g_ppComboBox[N_mach_sections64_E::CB_flag2]=createComboBox(ui->tableWidget,XMACH::getSectionAttributesUsrS(),SMACH::TYPE_mach_sections,N_mach_sections64_E::flags_2,XComboBoxEx::CBTYPE_ELIST,XMACH_DEF::SECTION_ATTRIBUTES_USR);

                XMACH_DEF::section_64 section=mach._read_section_64(nHeaderOffset,bIsBigEndian);

                g_ppLinedEdit[N_mach_sections64_E::sectname]->setStringValue(section.sectname);
                g_ppLinedEdit[N_mach_sections64_E::segname]->setStringValue(section.segname);
                g_ppLinedEdit[N_mach_sections64_E::addr]->setValue(section.addr);
                g_ppLinedEdit[N_mach_sections64_E::size]->setValue(section.size);
                g_ppLinedEdit[N_mach_sections64_E::offset]->setValue(section.offset);
                g_ppLinedEdit[N_mach_sections64_E::align]->setValue(section.align);
                g_ppLinedEdit[N_mach_sections64_E::reloff]->setValue(section.reloff);
                g_ppLinedEdit[N_mach_sections64_E::nreloc]->setValue(section.nreloc);
                g_ppLinedEdit[N_mach_sections64_E::flags_0]->setValue(section.flags);
                g_ppLinedEdit[N_mach_sections64_E::reserved1]->setValue(section.reserved1);
                g_ppLinedEdit[N_mach_sections64_E::reserved2]->setValue(section.reserved2);
                g_ppLinedEdit[N_mach_sections64_E::reserved3]->setValue(section.reserved3);

                g_ppComboBox[N_mach_sections64_E::CB_flag0]->setValue(section.flags);
                g_ppComboBox[N_mach_sections64_E::CB_flag1]->setValue(section.flags);
                g_ppComboBox[N_mach_sections64_E::CB_flag2]->setValue(section.flags);
            }
            else
            {
                g_ppComboBox[N_mach_sections32_E::CB_flag0]=createComboBox(ui->tableWidget,XMACH::getSectionFlagsTypesS(),SMACH::TYPE_mach_sections,N_mach_sections32_E::flags_0,XComboBoxEx::CBTYPE_ELIST,XMACH_DEF::SECTION_TYPE);
                g_ppComboBox[N_mach_sections32_E::CB_flag1]=createComboBox(ui->tableWidget,XMACH::getSectionAttributesSysS(),SMACH::TYPE_mach_sections,N_mach_sections32_E::flags_1,XComboBoxEx::CBTYPE_ELIST,XMACH_DEF::SECTION_ATTRIBUTES_SYS);
                g_ppComboBox[N_mach_sections32_E::CB_flag2]=createComboBox(ui->tableWidget,XMACH::getSectionAttributesUsrS(),SMACH::TYPE_mach_sections,N_mach_sections32_E::flags_2,XComboBoxEx::CBTYPE_ELIST,XMACH_DEF::SECTION_ATTRIBUTES_USR);

                XMACH_DEF::section section=mach._read_section(nHeaderOffset,bIsBigEndian);

                g_ppLinedEdit[N_mach_sections32_E::sectname]->setStringValue(section.sectname);
                g_ppLinedEdit[N_mach_sections32_E::segname]->setStringValue(section.segname);
                g_ppLinedEdit[N_mach_sections32_E::addr]->setValue(section.addr);
                g_ppLinedEdit[N_mach_sections32_E::size]->setValue(section.size);
                g_ppLinedEdit[N_mach_sections32_E::offset]->setValue(section.offset);
                g_ppLinedEdit[N_mach_sections32_E::align]->setValue(section.align);
                g_ppLinedEdit[N_mach_sections32_E::reloff]->setValue(section.reloff);
                g_ppLinedEdit[N_mach_sections32_E::nreloc]->setValue(section.nreloc);
                g_ppLinedEdit[N_mach_sections32_E::flags_0]->setValue(section.flags);
                g_ppLinedEdit[N_mach_sections32_E::reserved1]->setValue(section.reserved1);
                g_ppLinedEdit[N_mach_sections32_E::reserved2]->setValue(section.reserved2);

                g_ppComboBox[N_mach_sections32_E::CB_flag0]->setValue(section.flags);
                g_ppComboBox[N_mach_sections32_E::CB_flag1]->setValue(section.flags);
                g_ppComboBox[N_mach_sections32_E::CB_flag2]->setValue(section.flags);
            }

            qint64 nOffset=nHeaderOffset;
            qint64 nSize=mach.getSectionHeaderSize();
            qint64 nAddress=mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset,nSize,nAddress,&g_pSubDevice,ui->widgetHex);

            blockSignals(false);
        }
        else if(nType==SMACH::TYPE_SYMBOLTABLE)
        {
            createHeaderTable(SMACH::TYPE_SYMBOLTABLE,ui->tableWidget,bIs64?(N_mach_nlist::records64):(N_mach_nlist::records32),g_ppLinedEdit,N_mach_nlist::__data_size,getNumber(),getOffset());

            blockSignals(true);

            XBinary::OFFSETSIZE osStringTable=mach.getStringTableOS();

            qint64 nHeaderOffset=getOffset();

            if(bIs64)
            {
                XMACH_DEF::nlist_64 nlist=mach._read_nlist_64(nHeaderOffset);

                g_ppLinedEdit[N_mach_nlist::n_strx]->setValue(nlist.n_strx);
                g_ppLinedEdit[N_mach_nlist::n_type]->setValue(nlist.n_type);
                g_ppLinedEdit[N_mach_nlist::n_sect]->setValue(nlist.n_sect);
                g_ppLinedEdit[N_mach_nlist::n_desc]->setValue(nlist.n_desc);
                g_ppLinedEdit[N_mach_nlist::n_value]->setValue(nlist.n_value);

                addComment(ui->tableWidget,N_mach_nlist::n_strx,HEADER_COLUMN_COMMENT,mach.getStringFromIndex(osStringTable.nOffset,osStringTable.nSize,nlist.n_strx));
            }
            else
            {
                XMACH_DEF::nlist nlist=mach._read_nlist(nHeaderOffset);

                g_ppLinedEdit[N_mach_nlist::n_strx]->setValue(nlist.n_strx);
                g_ppLinedEdit[N_mach_nlist::n_type]->setValue(nlist.n_type);
                g_ppLinedEdit[N_mach_nlist::n_sect]->setValue(nlist.n_sect);
                g_ppLinedEdit[N_mach_nlist::n_desc]->setValue(nlist.n_desc);
                g_ppLinedEdit[N_mach_nlist::n_value]->setValue(nlist.n_value);

                addComment(ui->tableWidget,N_mach_nlist::n_strx,HEADER_COLUMN_COMMENT,mach.getStringFromIndex(osStringTable.nOffset,osStringTable.nSize,nlist.n_strx));
            }

            qint64 nOffset=nHeaderOffset;
            qint64 nSize=0;

            if(bIs64)
            {
                nSize=mach.get_nlist_64_size();
            }
            else
            {
                nSize=mach.get_nlist_size();
            }

            qint64 nAddress=mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset,nSize,nAddress,&g_pSubDevice,ui->widgetHex);

            blockSignals(false);
        }
        else if(nType==SMACH::TYPE_DICE)
        {
            createHeaderTable(SMACH::TYPE_DICE,ui->tableWidget,N_mach_data_in_code_entry::records,g_ppLinedEdit,N_mach_data_in_code_entry::__data_size,getNumber(),getOffset());

            blockSignals(true);

            qint64 nHeaderOffset=getOffset();

            XMACH_DEF::data_in_code_entry dice=mach._read_data_in_code_entry(nHeaderOffset);

            g_ppLinedEdit[N_mach_data_in_code_entry::offset]->setValue(dice.offset);
            g_ppLinedEdit[N_mach_data_in_code_entry::length]->setValue(dice.length);
            g_ppLinedEdit[N_mach_data_in_code_entry::kind]->setValue(dice.kind);

            qint64 nOffset=nHeaderOffset;
            qint64 nSize=mach.get_data_in_code_entry_size();
            qint64 nAddress=mach.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset,nSize,nAddress,&g_pSubDevice,ui->widgetHex);

            blockSignals(false);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void MACHSectionHeaderWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    XMACH mach(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

    if(mach.isValid())
    {
        switch(nStype)
        {
            case SMACH::TYPE_mach_commands:
                switch(nNdata)
                {
                    case N_mach_commands::cmd:          g_ppLinedEdit[N_mach_commands::cmd]->setValue((quint32)nValue);                     break;
                }
                break;

            case SMACH::TYPE_mach_segments:
                switch(nNdata)
                {
                    case N_mach_segments::initprot:     g_ppLinedEdit[N_mach_segments::initprot]->setValue((quint32)nValue);                break;
                    case N_mach_segments::maxprot:      g_ppLinedEdit[N_mach_segments::maxprot]->setValue((quint32)nValue);                 break;
                }
                break;

            case SMACH::TYPE_mach_sections:

                if(mach.is64())
                {
                    switch(nNdata)
                    {
                        case N_mach_sections64_E::flags_0:
                        case N_mach_sections64_E::flags_1:
                        case N_mach_sections64_E::flags_2:
                            g_ppLinedEdit[N_mach_sections64_E::flags_0]->setValue((quint32)nValue);
                            g_ppComboBox[N_mach_sections64_E::CB_flag0]->setValue(nValue);
                            g_ppComboBox[N_mach_sections64_E::CB_flag1]->setValue(nValue);
                            g_ppComboBox[N_mach_sections64_E::CB_flag2]->setValue(nValue);
                            break;
                    }
                }
                else
                {
                    switch(nNdata)
                    {
                        case N_mach_sections32_E::flags_0:
                        case N_mach_sections32_E::flags_1:
                        case N_mach_sections32_E::flags_2:
                            g_ppLinedEdit[N_mach_sections32_E::flags_0]->setValue((quint32)nValue);
                            g_ppComboBox[N_mach_sections32_E::CB_flag0]->setValue(nValue);
                            g_ppComboBox[N_mach_sections32_E::CB_flag1]->setValue(nValue);
                            g_ppComboBox[N_mach_sections32_E::CB_flag2]->setValue(nValue);
                            break;
                    }
                }

                break;
        }
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
