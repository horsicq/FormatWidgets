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
                        case N_mach_commands::cmd:          mach._setCommand_cmd(nOffset,nValue);                        break;
                        case N_mach_commands::cmdsize:      mach._setCommand_cmdsize(nOffset,nValue);                    break;
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
