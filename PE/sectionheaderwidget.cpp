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
#include "sectionheaderwidget.h"
#include "ui_sectionheaderwidget.h"

SectionHeaderWidget::SectionHeaderWidget(QWidget *parent):
    FormatWidget(parent),
    ui(new Ui::SectionHeaderWidget)
{
    ui->setupUi(this);
}

SectionHeaderWidget::SectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *parent):
    FormatWidget(pDevice,pOptions,nNumber,nOffset,nType,parent),
    ui(new Ui::SectionHeaderWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions,nNumber,nOffset,nType);

    ppLinedEdit=0;
    nLineEditSize=0;
    ppComboBox=0;
    nComboBoxSize=0;
    ppInvWidget=0;
    nInvWidgetSize=0;

    if(nType==SPE::TYPE_IMAGE_SECTION_HEADER)
    {
        nLineEditSize=N_IMAGE_SECTION_HEADER::__data_size+1;
        nComboBoxSize=N_IMAGE_SECTION_HEADER::__CB_size;
        nInvWidgetSize=N_IMAGE_SECTION_HEADER::__INV_size;
    }
    else if(nType==SPE::TYPE_IMPORT)
    {
        nLineEditSize=N_IMAGE_IMPORT::__data_size;
        nComboBoxSize=N_IMAGE_IMPORT::__CB_size;
        nInvWidgetSize=N_IMAGE_IMPORT::__INV_size;
    }
    else if(nType==SPE::TYPE_DEBUG)
    {
        nLineEditSize=N_IMAGE_DEBUG::__data_size;
        nComboBoxSize=N_IMAGE_DEBUG::__CB_size;
        nInvWidgetSize=N_IMAGE_DEBUG::__INV_size;
    }
    else if(nType==SPE::TYPE_RELOCS)
    {
        nLineEditSize=N_IMAGE_RELOCS::__data_size;
        nComboBoxSize=N_IMAGE_RELOCS::__CB_size;
        nInvWidgetSize=N_IMAGE_RELOCS::__INV_size;
    }

    if(nLineEditSize)
    {
        ppLinedEdit=new PXLineEditHEX[nLineEditSize];
    }

    if(nComboBoxSize)
    {
        ppComboBox=new PXComboBoxEx[nComboBoxSize];
    }

    if(nInvWidgetSize)
    {
        ppInvWidget=new PInvWidget[nInvWidgetSize];
    }
}

SectionHeaderWidget::~SectionHeaderWidget()
{
    if(ppLinedEdit)
    {
        delete[] ppLinedEdit;
    }

    if(ppComboBox)
    {
        delete[] ppComboBox;
    }

    if(ppInvWidget)
    {
        delete[] ppInvWidget;
    }

    delete ui;
}

void SectionHeaderWidget::clear()
{
    reset();

    memset(ppLinedEdit,0,nLineEditSize*sizeof(XLineEditHEX *));
    memset(ppComboBox,0,nComboBoxSize*sizeof(XComboBoxEx *));
    memset(ppInvWidget,0,nInvWidgetSize*sizeof(InvWidget *));

    pSubDevice=nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void SectionHeaderWidget::cleanup()
{

}

void SectionHeaderWidget::reset()
{
    bInit=false;
}

void SectionHeaderWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

bool SectionHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype,int nPosition,qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nOffset)

    bool bResult=false;

    quint64 nValue=vValue.toULongLong();
    QString sValue=vValue.toString();

    if(getDevice()->isWritable())
    {
        XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

        if(pe.isValid())
        {
            switch(nStype)
            {
                case SPE::TYPE_IMAGE_SECTION_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_SECTION_HEADER::VirtualAddress:        ppInvWidget[N_IMAGE_SECTION_HEADER::INV_VirtualAddress]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);        break;
                        case N_IMAGE_SECTION_HEADER::PointerToRawData:      ppInvWidget[N_IMAGE_SECTION_HEADER::INV_PointerToRawData]->setOffsetAndSize(&pe,(quint32)nValue,0);                           break;
                    }
                    break;

                case SPE::TYPE_IMPORT:
                    switch(nNdata)
                    {
                        case N_IMAGE_IMPORT::OriginalFirstThunk:            ppInvWidget[N_IMAGE_IMPORT::INV_OriginalFirstThunk]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);    break;
                        case N_IMAGE_IMPORT::Name:                          ppInvWidget[N_IMAGE_IMPORT::INV_Name]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);                  break;
                        case N_IMAGE_IMPORT::FirstThunk:                    ppInvWidget[N_IMAGE_IMPORT::INV_FirstThunk]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);            break;
                    }
                    break;
            }

            switch(nStype)
            {
                case SPE::TYPE_IMAGE_SECTION_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_SECTION_HEADER::Name:                  pe.setSection_NameAsString((quint32)nPosition,sValue);                  break;
                        case N_IMAGE_SECTION_HEADER::VirtualSize:           pe.setSection_VirtualSize((quint32)nPosition,(quint32)nValue);          break;
                        case N_IMAGE_SECTION_HEADER::VirtualAddress:        pe.setSection_VirtualAddress((quint32)nPosition,(quint32)nValue);       break;
                        case N_IMAGE_SECTION_HEADER::SizeOfRawData:         pe.setSection_SizeOfRawData((quint32)nPosition,(quint32)nValue);        break;
                        case N_IMAGE_SECTION_HEADER::PointerToRawData:      pe.setSection_PointerToRawData((quint32)nPosition,(quint32)nValue);     break;
                        case N_IMAGE_SECTION_HEADER::PointerToRelocations:  pe.setSection_PointerToRelocations((quint32)nPosition,(quint32)nValue); break;
                        case N_IMAGE_SECTION_HEADER::PointerToLinenumbers:  pe.setSection_PointerToLinenumbers((quint32)nPosition,(quint32)nValue); break;
                        case N_IMAGE_SECTION_HEADER::NumberOfRelocations:   pe.setSection_NumberOfRelocations((quint32)nPosition,(quint16)nValue);  break;
                        case N_IMAGE_SECTION_HEADER::NumberOfLinenumbers:   pe.setSection_NumberOfLinenumbers((quint32)nPosition,(quint16)nValue);  break;
                        case N_IMAGE_SECTION_HEADER::Characteristics:       pe.setSection_Characteristics((quint32)nPosition,(quint32)nValue);      break;
                    }
                    break;

                case SPE::TYPE_IMPORT:
                    switch(nNdata)
                    {
                        case N_IMAGE_IMPORT::OriginalFirstThunk:            pe.setImportDescriptor_OriginalFirstThunk(nPosition,(quint32)nValue);   break;
                        case N_IMAGE_IMPORT::TimeDateStamp:                 pe.setImportDescriptor_TimeDateStamp(nPosition,(quint32)nValue);        break;
                        case N_IMAGE_IMPORT::ForwarderChain:                pe.setImportDescriptor_ForwarderChain(nPosition,(quint32)nValue);       break;
                        case N_IMAGE_IMPORT::Name:                          pe.setImportDescriptor_Name(nPosition,(quint32)nValue);                 break;
                        case N_IMAGE_IMPORT::FirstThunk:                    pe.setImportDescriptor_FirstThunk(nPosition,(quint32)nValue);           break;
                    }
                    break;

                case SPE::TYPE_DEBUG:
                    switch(nNdata)
                    {
                        case N_IMAGE_DEBUG::Characteristics:                pe.setDebugHeader_Characteristics(nPosition,(quint32)nValue);           break;
                        case N_IMAGE_DEBUG::TimeDateStamp:                  pe.setDebugHeader_TimeDateStamp(nPosition,(quint32)nValue);             break;
                        case N_IMAGE_DEBUG::MajorVersion:                   pe.setDebugHeader_MajorVersion(nPosition,(quint16)nValue);              break;
                        case N_IMAGE_DEBUG::MinorVersion:                   pe.setDebugHeader_MinorVersion(nPosition,(quint16)nValue);              break;
                        case N_IMAGE_DEBUG::Type:                           pe.setDebugHeader_Type(nPosition,(quint32)nValue);                      break;
                        case N_IMAGE_DEBUG::SizeOfData:                     pe.setDebugHeader_SizeOfData(nPosition,(quint32)nValue);                break;
                        case N_IMAGE_DEBUG::AddressOfRawData:               pe.setDebugHeader_AddressOfRawData(nPosition,(quint32)nValue);          break;
                        case N_IMAGE_DEBUG::PointerToRawData:               pe.setDebugHeader_PointerToRawData(nPosition,(quint32)nValue);          break;
                    }
                    break;

                case SPE::TYPE_RELOCS:
                    switch(nNdata)
                    {
                        case N_IMAGE_RELOCS::VirtualAddress:                pe.setRelocsVirtualAddress(nPosition,(quint32)nValue);                  break;
                        case N_IMAGE_RELOCS::SizeOfBlock:                   pe.setRelocsSizeOfBlock(nPosition,(quint32)nValue);                     break;
                    }
                    break;
            }

            switch(nStype)
            {
                case SPE::TYPE_IMPORT:
                    switch(nNdata)
                    {
                        case N_IMAGE_IMPORT::OriginalFirstThunk:            addComment(ui->tableWidget,N_IMAGE_IMPORT::OriginalFirstThunk,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress((quint32)nValue));         break;
                        case N_IMAGE_IMPORT::Name:                          addComment(ui->tableWidget,N_IMAGE_IMPORT::Name,HEADER_COLUMN_COMMENT,pe.read_ansiString(pe.relAddressToOffset((quint32)nValue)));                break;                                                                                              break;
                        case N_IMAGE_IMPORT::FirstThunk:                    addComment(ui->tableWidget,N_IMAGE_IMPORT::FirstThunk,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress((quint32)nValue));                 break;
                    }
                    break;
            }

            ui->widgetHex->reload();

            bResult=true;
        }
    }

    return bResult;
}
void SectionHeaderWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(ppLinedEdit,nLineEditSize,bState);

    setComboBoxesReadOnly(ppComboBox,nComboBoxSize,bState);
}

void SectionHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)ppLinedEdit,nLineEditSize,bState);

    _blockSignals((QObject **)ppComboBox,nComboBoxSize,bState);
}

void SectionHeaderWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

    switch(type)
    {
        case SPE::TYPE_IMAGE_SECTION_HEADER:
            pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
            pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*6);
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*16);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;

        case SPE::TYPE_IMPORT:
            pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
            pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*6);
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*20);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;

        case SPE::TYPE_DEBUG:
            pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
            pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*6);
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*20);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;

        case SPE::TYPE_RELOCS:
            pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
            pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*6);
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*16);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;
    }
}

void SectionHeaderWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void SectionHeaderWidget::reloadData()
{
    int nType=getType();

    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(pe.isValid())
    {
        if(!bInit)
        {
            if(nType==SPE::TYPE_IMAGE_SECTION_HEADER)
            {
                bInit=createHeaderTable(SPE::TYPE_IMAGE_SECTION_HEADER,ui->tableWidget,N_IMAGE_SECTION_HEADER::records,ppLinedEdit,N_IMAGE_SECTION_HEADER::__data_size+1,getNumber());
                ppComboBox[N_IMAGE_SECTION_HEADER::CB_CHARACTERISTICS]=createComboBox(ui->tableWidget,XPE::getImageSectionHeaderFlagsS(),SPE::TYPE_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::Characteristics,XComboBoxEx::CBTYPE_FLAGS);
                ppComboBox[N_IMAGE_SECTION_HEADER::CB_ALIGH]=createComboBox(ui->tableWidget,XPE::getImageSectionHeaderAlignsS(),SPE::TYPE_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::Characteristics+1,XComboBoxEx::CBTYPE_EFLAGS,XPE_DEF::S_IMAGE_SCN_ALIGN_MASK);

                ppInvWidget[N_IMAGE_SECTION_HEADER::INV_VirtualAddress]=createInvWidget(ui->tableWidget,SPE::TYPE_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::VirtualAddress,InvWidget::TYPE_HEX);
                ppInvWidget[N_IMAGE_SECTION_HEADER::INV_PointerToRawData]=createInvWidget(ui->tableWidget,SPE::TYPE_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::PointerToRawData,InvWidget::TYPE_HEX);

                blockSignals(true);

                XPE_DEF::IMAGE_SECTION_HEADER ish=pe.getSectionHeader(getNumber());

                //        lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::e_magic]->setValue(ish.);

                QString sName=QString((char *)ish.Name);
                sName.resize(qMin(sName.length(),XPE_DEF::S_IMAGE_SIZEOF_SHORT_NAME));

                ppLinedEdit[N_IMAGE_SECTION_HEADER::Name]->setStringValue(sName,XPE_DEF::S_IMAGE_SIZEOF_SHORT_NAME); // TODO Check
                ppLinedEdit[N_IMAGE_SECTION_HEADER::VirtualSize]->setValue(ish.Misc.VirtualSize);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::VirtualAddress]->setValue(ish.VirtualAddress);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::SizeOfRawData]->setValue(ish.SizeOfRawData);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::PointerToRawData]->setValue(ish.PointerToRawData);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::PointerToRelocations]->setValue(ish.PointerToRelocations);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::PointerToLinenumbers]->setValue(ish.PointerToLinenumbers);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::NumberOfRelocations]->setValue(ish.NumberOfRelocations);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::NumberOfLinenumbers]->setValue(ish.NumberOfLinenumbers);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::Characteristics]->setValue(ish.Characteristics);

                ppComboBox[N_IMAGE_SECTION_HEADER::CB_CHARACTERISTICS]->setValue(ish.Characteristics);
                ppComboBox[N_IMAGE_SECTION_HEADER::CB_ALIGH]->setValue(ish.Characteristics);

                ppInvWidget[N_IMAGE_SECTION_HEADER::INV_VirtualAddress]->setAddressAndSize(&pe,pe.getBaseAddress()+ish.VirtualAddress,0);
                ppInvWidget[N_IMAGE_SECTION_HEADER::INV_PointerToRawData]->setOffsetAndSize(&pe,ish.PointerToRawData,0);

                qint64 nOffset=pe.getSectionHeaderOffset(getNumber());
                qint64 nSize=pe.getSectionHeaderSize();
                qint64 nAddress=pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex);

                blockSignals(false);
            }
            else if(nType==SPE::TYPE_IMPORT)
            {
                if(!bInit)
                {
                    bInit=createHeaderTable(SPE::TYPE_IMPORT,ui->tableWidget,N_IMAGE_IMPORT::records,ppLinedEdit,N_IMAGE_IMPORT::__data_size,getNumber());

                    ppInvWidget[N_IMAGE_IMPORT::INV_OriginalFirstThunk]=createInvWidget(ui->tableWidget,SPE::TYPE_IMPORT,N_IMAGE_IMPORT::OriginalFirstThunk,InvWidget::TYPE_HEX);
                    ppInvWidget[N_IMAGE_IMPORT::INV_Name]=createInvWidget(ui->tableWidget,SPE::TYPE_IMPORT,N_IMAGE_IMPORT::Name,InvWidget::TYPE_HEX);
                    ppInvWidget[N_IMAGE_IMPORT::INV_FirstThunk]=createInvWidget(ui->tableWidget,SPE::TYPE_IMPORT,N_IMAGE_IMPORT::FirstThunk,InvWidget::TYPE_HEX);
                }

                blockSignals(true);

                XBinary::_MEMORY_MAP memoryMap=pe.getMemoryMap();

                XPE_DEF::IMAGE_IMPORT_DESCRIPTOR idh=pe.getImportDescriptor(getNumber());

                ppLinedEdit[N_IMAGE_IMPORT::OriginalFirstThunk]->setValue(idh.OriginalFirstThunk);
                ppLinedEdit[N_IMAGE_IMPORT::TimeDateStamp]->setValue(idh.TimeDateStamp);
                ppLinedEdit[N_IMAGE_IMPORT::ForwarderChain]->setValue(idh.ForwarderChain);
                ppLinedEdit[N_IMAGE_IMPORT::Name]->setValue(idh.Name);
                ppLinedEdit[N_IMAGE_IMPORT::FirstThunk]->setValue(idh.FirstThunk);

                ppInvWidget[N_IMAGE_IMPORT::INV_OriginalFirstThunk]->setAddressAndSize(&pe,pe.getBaseAddress()+idh.OriginalFirstThunk,0);
                ppInvWidget[N_IMAGE_IMPORT::INV_Name]->setAddressAndSize(&pe,pe.getBaseAddress()+idh.Name,0);
                ppInvWidget[N_IMAGE_IMPORT::INV_FirstThunk]->setAddressAndSize(&pe,pe.getBaseAddress()+idh.FirstThunk,0);

                addComment(ui->tableWidget,N_IMAGE_IMPORT::OriginalFirstThunk,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress(&memoryMap,idh.OriginalFirstThunk));
                addComment(ui->tableWidget,N_IMAGE_IMPORT::Name,HEADER_COLUMN_COMMENT,pe.read_ansiString(pe.relAddressToOffset(&memoryMap,idh.Name)));
                addComment(ui->tableWidget,N_IMAGE_IMPORT::FirstThunk,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress(&memoryMap,idh.FirstThunk));

                qint64 nOffset=pe.getImportDescriptorOffset(getNumber());
                qint64 nSize=pe.getImportDescriptorSize();
                qint64 nAddress=pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex);

                blockSignals(false);
            }
            else if(nType==SPE::TYPE_DEBUG)
            {
                if(!bInit)
                {
                    bInit=createHeaderTable(SPE::TYPE_DEBUG,ui->tableWidget,N_IMAGE_DEBUG::records,ppLinedEdit,N_IMAGE_DEBUG::__data_size,getNumber());
                }

                blockSignals(true);

                XPE_DEF::S_IMAGE_DEBUG_DIRECTORY idd=pe.getDebugHeader(getNumber());

                ppLinedEdit[N_IMAGE_DEBUG::Characteristics]->setValue(idd.Characteristics);
                ppLinedEdit[N_IMAGE_DEBUG::TimeDateStamp]->setValue(idd.TimeDateStamp);
                ppLinedEdit[N_IMAGE_DEBUG::MajorVersion]->setValue(idd.MajorVersion);
                ppLinedEdit[N_IMAGE_DEBUG::MinorVersion]->setValue(idd.MinorVersion);
                ppLinedEdit[N_IMAGE_DEBUG::Type]->setValue(idd.Type);
                ppLinedEdit[N_IMAGE_DEBUG::SizeOfData]->setValue(idd.SizeOfData);
                ppLinedEdit[N_IMAGE_DEBUG::AddressOfRawData]->setValue(idd.AddressOfRawData);
                ppLinedEdit[N_IMAGE_DEBUG::PointerToRawData]->setValue(idd.PointerToRawData);

                qint64 nOffset=pe.getDebugHeaderOffset(getNumber());
                qint64 nSize=pe.getDebugHeaderSize();
                qint64 nAddress=pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex);

                blockSignals(false);
            }
            else if(nType==SPE::TYPE_RELOCS)
            {
                if(!bInit)
                {
                    bInit=createHeaderTable(SPE::TYPE_RELOCS,ui->tableWidget,N_IMAGE_RELOCS::records,ppLinedEdit,N_IMAGE_RELOCS::__data_size,getNumber());
                }

                blockSignals(true);

                qint64 nOffset=getOffset();

                quint32 nVirtualAddress=pe.getRelocsVirtualAddress(nOffset);
                quint32 nSizeOfBlock=pe.getRelocsSizeOfBlock(nOffset);

                ppLinedEdit[N_IMAGE_RELOCS::VirtualAddress]->setValue(nVirtualAddress);
                ppLinedEdit[N_IMAGE_RELOCS::SizeOfBlock]->setValue(nSizeOfBlock);

                qint64 nSize=nSizeOfBlock;
                qint64 nAddress=pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex);

                blockSignals(false);
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void SectionHeaderWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
        case SPE::TYPE_IMAGE_SECTION_HEADER:
            switch(nNdata)
            {
                case N_IMAGE_SECTION_HEADER::Characteristics:
                case N_IMAGE_SECTION_HEADER::Characteristics+1:
                    ppLinedEdit[N_IMAGE_SECTION_HEADER::Characteristics]->setValue((quint32)nValue);
                    ppComboBox[N_IMAGE_SECTION_HEADER::CB_CHARACTERISTICS]->setValue(nValue);
                    ppComboBox[N_IMAGE_SECTION_HEADER::CB_ALIGH]->setValue(nValue);
                    break;
            }

            break;
    }
}

void SectionHeaderWidget::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex,ui->tableWidget);
}
