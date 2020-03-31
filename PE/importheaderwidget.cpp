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
#include "importheaderwidget.h"
#include "ui_importheaderwidget.h"

ImportHeaderWidget::ImportHeaderWidget(QWidget *parent):
    FormatWidget(parent),
    ui(new Ui::ImportHeaderWidget)
{
    ui->setupUi(this);
}

ImportHeaderWidget::ImportHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, qint64 nOffset, QWidget *parent):
    FormatWidget(pDevice,pOptions,nNumber,nOffset,parent),
    ui(new Ui::ImportHeaderWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions,nNumber,nOffset);
}

ImportHeaderWidget::~ImportHeaderWidget()
{
    delete ui;
}

void ImportHeaderWidget::clear()
{
    bInit=false;

    memset(lineEdit_IMPORT_HEADER,0,sizeof lineEdit_IMPORT_HEADER);
    memset(invWidget,0,sizeof invWidget);

    pSubDevice=nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void ImportHeaderWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber,qint64 nOffset)
{
    FormatWidget::setData(pDevice,pOptions,nNumber,nOffset);
}

void ImportHeaderWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

bool ImportHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nOffset)

    bool bResult=false;

    quint64 nValue=vValue.toULongLong();

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
                        case N_IMAGE_IMPORT::OriginalFirstThunk:            invWidget[INV_OriginalFirstThunk]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);    break;
                        case N_IMAGE_IMPORT::Name:                          invWidget[INV_Name]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);                  break;
                        case N_IMAGE_IMPORT::FirstThunk:                    invWidget[INV_FirstThunk]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);            break;
                    }

                    break;
            }

            switch(nStype)
            {
                case SPE::TYPE_IMAGE_SECTION_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_IMPORT::OriginalFirstThunk:            pe.setImportDescriptor_OriginalFirstThunk(nPosition,(quint32)nValue);   break;
                        case N_IMAGE_IMPORT::TimeDateStamp:                 pe.setImportDescriptor_TimeDateStamp(nPosition,(quint32)nValue);        break;
                        case N_IMAGE_IMPORT::ForwarderChain:                pe.setImportDescriptor_ForwarderChain(nPosition,(quint32)nValue);       break;
                        case N_IMAGE_IMPORT::Name:                          pe.setImportDescriptor_Name(nPosition,(quint32)nValue);                 break;
                        case N_IMAGE_IMPORT::FirstThunk:                    pe.setImportDescriptor_FirstThunk(nPosition,(quint32)nValue);           break;
                    }

                    ui->widgetHex_IMPORT_HEADER->reload();

                    break;
            }

            switch(nStype)
            {
                case SPE::TYPE_IMAGE_SECTION_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_IMPORT::OriginalFirstThunk:            addComment(ui->tableWidget_IMPORT_HEADER,N_IMAGE_IMPORT::OriginalFirstThunk,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress((quint32)nValue));         break;
                        case N_IMAGE_IMPORT::Name:                          addComment(ui->tableWidget_IMPORT_HEADER,N_IMAGE_IMPORT::Name,HEADER_COLUMN_COMMENT,pe.read_ansiString(pe.relAddressToOffset((quint32)nValue)));                break;                                                                                              break;
                        case N_IMAGE_IMPORT::FirstThunk:                    addComment(ui->tableWidget_IMPORT_HEADER,N_IMAGE_IMPORT::FirstThunk,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress((quint32)nValue));                 break;
                    }

                    break;
            }

            bResult=true;
        }
    }

    return bResult;
}
void ImportHeaderWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_IMPORT_HEADER,N_IMAGE_SECTION_HEADER::__data_size,bState);
}

void ImportHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_IMPORT_HEADER,N_IMAGE_SECTION_HEADER::__data_size,bState);

}

void ImportHeaderWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*6);
    pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*20);
    pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
    pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
}

void ImportHeaderWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void ImportHeaderWidget::reloadData()
{
    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(pe.isValid())
    {
        if(!bInit)
        {
            bInit=createHeaderTable(SPE::TYPE_IMPORT,ui->tableWidget_IMPORT_HEADER,N_IMAGE_IMPORT::records,lineEdit_IMPORT_HEADER,N_IMAGE_IMPORT::__data_size,getNumber());

            invWidget[INV_OriginalFirstThunk]=createInvWidget(ui->tableWidget_IMPORT_HEADER,SPE::TYPE_IMPORT,N_IMAGE_IMPORT::OriginalFirstThunk,InvWidget::TYPE_HEX);
            invWidget[INV_Name]=createInvWidget(ui->tableWidget_IMPORT_HEADER,SPE::TYPE_IMPORT,N_IMAGE_IMPORT::Name,InvWidget::TYPE_HEX);
            invWidget[INV_FirstThunk]=createInvWidget(ui->tableWidget_IMPORT_HEADER,SPE::TYPE_IMPORT,N_IMAGE_IMPORT::FirstThunk,InvWidget::TYPE_HEX);
        }

        blockSignals(true);

        XBinary::_MEMORY_MAP memoryMap=pe.getMemoryMap();

        XPE_DEF::IMAGE_IMPORT_DESCRIPTOR idh=pe.getImportDescriptor(getNumber());

        lineEdit_IMPORT_HEADER[N_IMAGE_IMPORT::OriginalFirstThunk]->setValue(idh.OriginalFirstThunk);
        lineEdit_IMPORT_HEADER[N_IMAGE_IMPORT::TimeDateStamp]->setValue(idh.TimeDateStamp);
        lineEdit_IMPORT_HEADER[N_IMAGE_IMPORT::ForwarderChain]->setValue(idh.ForwarderChain);
        lineEdit_IMPORT_HEADER[N_IMAGE_IMPORT::Name]->setValue(idh.Name);
        lineEdit_IMPORT_HEADER[N_IMAGE_IMPORT::FirstThunk]->setValue(idh.FirstThunk);

        invWidget[INV_OriginalFirstThunk]->setAddressAndSize(&pe,pe.getBaseAddress()+idh.OriginalFirstThunk,0);
        invWidget[INV_Name]->setAddressAndSize(&pe,pe.getBaseAddress()+idh.Name,0);
        invWidget[INV_FirstThunk]->setAddressAndSize(&pe,pe.getBaseAddress()+idh.FirstThunk,0);

        addComment(ui->tableWidget_IMPORT_HEADER,N_IMAGE_IMPORT::OriginalFirstThunk,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress(&memoryMap,idh.OriginalFirstThunk));
        addComment(ui->tableWidget_IMPORT_HEADER,N_IMAGE_IMPORT::Name,HEADER_COLUMN_COMMENT,pe.read_ansiString(pe.relAddressToOffset(&memoryMap,idh.Name)));
        addComment(ui->tableWidget_IMPORT_HEADER,N_IMAGE_IMPORT::FirstThunk,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress(&memoryMap,idh.FirstThunk));

        qint64 nOffset=pe.getImportDescriptorOffset(getNumber());
        qint64 nSize=pe.getImportDescriptorSize();
        qint64 nAddress=pe.offsetToRelAddress(nOffset);

        loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex_IMPORT_HEADER);

        blockSignals(false);

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void ImportHeaderWidget::on_tableWidget_IMPORT_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_IMPORT_HEADER,ui->tableWidget_IMPORT_HEADER);
}
