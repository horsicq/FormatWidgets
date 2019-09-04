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
#include "pewidget.h"
#include "ui_pewidget.h"

PEWidget::PEWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::PEWidget)
{
    ui->setupUi(this);
}

PEWidget::PEWidget(QIODevice *pDevice,OPTIONS *pOptions, QWidget *parent) :
    FormatWidget(pDevice,pOptions,parent),
    ui(new Ui::PEWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions);
    reload();
}

PEWidget::~PEWidget()
{
    delete ui;
}

void PEWidget::clear()
{
    memset(bInit,0,sizeof bInit);
    memset(lineEdit_IMAGE_DOS_HEADER,0,sizeof lineEdit_IMAGE_DOS_HEADER);
    memset(lineEdit_IMAGE_NT_HEADERS,0,sizeof lineEdit_IMAGE_NT_HEADERS);
    memset(lineEdit_IMAGE_FILE_HEADER,0,sizeof lineEdit_IMAGE_FILE_HEADER);
    memset(lineEdit_IMAGE_OPTIONAL_HEADER,0,sizeof lineEdit_IMAGE_OPTIONAL_HEADER);
    memset(lineEdit_TLS,0,sizeof lineEdit_TLS);
    memset(lineEdit_LoadConfig,0,sizeof lineEdit_LoadConfig);
    memset(comboBox,0,sizeof comboBox);
    memset(pushButton,0,sizeof pushButton);
    memset(dateTimeEdit,0,sizeof dateTimeEdit);

    pSubDeviceSection=nullptr;
    pSubDeviceOverlay=nullptr;
    pSubDeviceResource=nullptr;

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void PEWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(pe.isValid())
    {
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_TOOLS,tr("Tools")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_IMAGE_DOS_HEADER,"IMAGE_DOS_HEADER"));
        QTreeWidgetItem *pNtHeaders=createNewItem(SPE::TYPE_IMAGE_NT_HEADERS,"IMAGE_NT_HEADERS");
        ui->treeWidgetNavi->addTopLevelItem(pNtHeaders);
        pNtHeaders->addChild(createNewItem(SPE::TYPE_IMAGE_FILE_HEADER,"IMAGE_FILE_HEADER"));
        QTreeWidgetItem *pOptionalHeader=createNewItem(SPE::TYPE_IMAGE_OPTIONAL_HEADER,"IMAGE_OPTIONAL_HEADER");
        pNtHeaders->addChild(pOptionalHeader);
        pOptionalHeader->addChild(createNewItem(SPE::TYPE_IMAGE_DIRECTORY_ENTRIES,"IMAGE_DIRECTORY_ENTRIES"));

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_SECTIONS,tr("Sections")));

        if(pe.isExportPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_EXPORT,"Export"));
        }

        if(pe.isImportPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_IMPORT,"Import"));
        }

        if(pe.isResourcesPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_RESOURCE,"Resource"));
        }

        if(pe.isExceptionPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_EXCEPTION,"Exception"));
        }

        if(pe.isRelocsPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_RELOCS,"Relocs"));
        }

        if(pe.isTLSPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_TLS,"TLS"));
        }

        if(pe.isLoadConfigPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_LOADCONFIG,"Load Config"));
        }

        if(pe.isOverlayPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_OVERLAY,tr("Overlay")));
        }

        ui->treeWidgetNavi->expandAll();
        ui->treeWidgetNavi->setCurrentItem(ui->treeWidgetNavi->topLevelItem(1));
    }
}

bool PEWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype,int nPosition)
{
    bool bResult=false;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {
        XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

        if(pe.isValid())
        {
            switch(nStype)
            {
                case SPE::TYPE_IMAGE_DOS_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_DOS_HEADER::e_magic:
                            comboBox[CB_IMAGE_DOS_HEADER_e_magic]->setValue(nValue);
                            pe.set_e_magic((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_cblp:
                            pe.set_e_cblp((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_cp:
                            pe.set_e_cp((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_crlc:
                            pe.set_e_crlc((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_cparhdr:
                            pe.set_e_cparhdr((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_minalloc:
                            pe.set_e_minalloc((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_maxalloc:
                            pe.set_e_maxalloc((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_ss:
                            pe.set_e_ss((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_sp:
                            pe.set_e_sp((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_csum:
                            pe.set_e_csum((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_ip:
                            pe.set_e_ip((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_cs:
                            pe.set_e_cs((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_lfarlc:
                            pe.set_e_lfarlc((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_ovno:
                            pe.set_e_ovno((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res_0:
                            pe.set_e_res(0,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res_1:
                            pe.set_e_res(1,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res_2:
                            pe.set_e_res(2,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res_3:
                            pe.set_e_res(3,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_oemid:
                            pe.set_e_oemid((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_oeminfo:
                            pe.set_e_oeminfo((quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res2_0:
                            pe.set_e_res2(0,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res2_1:
                            pe.set_e_res2(1,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res2_2:
                            pe.set_e_res2(2,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res2_3:
                            pe.set_e_res2(3,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res2_4:
                            pe.set_e_res2(4,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res2_5:
                            pe.set_e_res2(5,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res2_6:
                            pe.set_e_res2(6,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res2_7:
                            pe.set_e_res2(7,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res2_8:
                            pe.set_e_res2(8,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_res2_9:
                            pe.set_e_res2(9,(quint16)nValue);
                            break;

                        case N_IMAGE_DOS_HEADER::e_lfanew:
                            pe.set_e_lfanew((quint32)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_IMAGE_NT_HEADERS:
                    switch(nNdata)
                    {
                        case N_IMAGE_NT_HEADERS::Signature:
                            pe.setNtHeaders_Signature((quint32)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_IMAGE_FILE_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_FILE_HEADER::Machine:
                            comboBox[CB_IMAGE_FILE_HEADER_Machine]->setValue(nValue);
                            pe.setFileHeader_Machine((quint16)nValue);
                            break;

                        case N_IMAGE_FILE_HEADER::NumberOfSections:
                            pe.setFileHeader_NumberOfSections((quint16)nValue);
                            break;

                        case N_IMAGE_FILE_HEADER::TimeDateStamp:
                            pe.setFileHeader_TimeDateStamp((quint32)nValue);
                            dateTimeEdit[DT_DateTimeStamp]->setValue(nValue);
                            break;

                        case N_IMAGE_FILE_HEADER::PointerToSymbolTable:
                            pe.setFileHeader_PointerToSymbolTable((quint32)nValue);
                            break;

                        case N_IMAGE_FILE_HEADER::NumberOfSymbols:
                            pe.setFileHeader_NumberOfSymbols((quint32)nValue);
                            break;

                        case N_IMAGE_FILE_HEADER::SizeOfOptionalHeader:
                            pe.setFileHeader_SizeOfOptionalHeader((quint16)nValue);
                            break;

                        case N_IMAGE_FILE_HEADER::Characteristics:
                            comboBox[CB_IMAGE_FILE_HEADER_Characteristics]->setValue(nValue);
                            pe.setFileHeader_Characteristics((quint16)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_OPTIONAL_HEADER::Magic:
                            comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic]->setValue(nValue);
                            pe.setOptionalHeader_Magic((quint16)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::MajorLinkerVersion:
                            pe.setOptionalHeader_MajorLinkerVersion((quint8)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::MinorLinkerVersion:
                            pe.setOptionalHeader_MinorLinkerVersion((quint8)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::SizeOfCode:
                            pe.setOptionalHeader_SizeOfCode((quint32)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::SizeOfInitializedData:
                            pe.setOptionalHeader_SizeOfInitializedData((quint32)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::SizeOfUninitializedData:
                            pe.setOptionalHeader_SizeOfUninitializedData((quint32)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint:
                            pe.setOptionalHeader_AddressOfEntryPoint((quint32)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::BaseOfCode:
                            pe.setOptionalHeader_BaseOfCode((quint32)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::BaseOfData:
                            pe.setOptionalHeader_BaseOfData((quint32)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::ImageBase:
                            pe.setOptionalHeader_ImageBase((quint64)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::SectionAlignment:
                            pe.setOptionalHeader_SectionAlignment((quint32)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::FileAlignment:
                            pe.setOptionalHeader_FileAlignment((quint32)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion:
                            pe.setOptionalHeader_MajorOperatingSystemVersion((quint16)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion:
                            pe.setOptionalHeader_MinorOperatingSystemVersion((quint16)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::MajorImageVersion:
                            pe.setOptionalHeader_MajorImageVersion((quint16)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::MinorImageVersion:
                            pe.setOptionalHeader_MinorImageVersion((quint16)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::MajorSubsystemVersion:
                            pe.setOptionalHeader_MajorSubsystemVersion((quint16)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::MinorSubsystemVersion:
                            pe.setOptionalHeader_MinorSubsystemVersion((quint16)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::Win32VersionValue:
                            pe.setOptionalHeader_Win32VersionValue((quint16)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::SizeOfImage:
                            pe.setOptionalHeader_SizeOfImage((quint32)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::SizeOfHeaders:
                            pe.setOptionalHeader_SizeOfHeaders((quint32)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::CheckSum:
                            pe.setOptionalHeader_CheckSum((quint32)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::Subsystem:
                            comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem]->setValue(nValue);
                            pe.setOptionalHeader_Subsystem((quint16)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::DllCharacteristics:
                            comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics]->setValue(nValue);
                            pe.setOptionalHeader_DllCharacteristics((quint16)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::SizeOfStackReserve:
                            pe.setOptionalHeader_SizeOfStackReserve((quint64)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::SizeOfStackCommit:
                            pe.setOptionalHeader_SizeOfStackCommit((quint64)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::SizeOfHeapReserve:
                            pe.setOptionalHeader_SizeOfHeapReserve((quint64)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::SizeOfHeapCommit:
                            pe.setOptionalHeader_SizeOfHeapCommit((quint64)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::LoaderFlags:
                            pe.setOptionalHeader_LoaderFlags((quint32)nValue);
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::NumberOfRvaAndSizes:
                            pe.setOptionalHeader_NumberOfRvaAndSizes((quint32)nValue);
                            break;
                    }
                    break;

//                case SPE::TYPE_IMAGE_DIRECTORY_ENTRIES:
//                    switch(nVtype)
//                    {
//                        case VAL_TYPE_RELADDRESS:
//                            pe.setOptionalHeader_DataDirectory_VirtualAddress((quint32)nNdata,(quint32)nValue);
//                            break;

//                        case VAL_TYPE_SIZE:
//                            pe.setOptionalHeader_DataDirectory_Size((quint32)nNdata,(quint32)nValue);
//                            break;
//                    }

//                    break;

                case SPE::TYPE_EXPORT:
                    switch(nNdata)
                    {
                        case N_IMAGE_EXPORT::Characteristics:
                            pe.setExportDirectory_Characteristics((quint32)nValue);
                            break;

                        case N_IMAGE_EXPORT::TimeDateStamp:
                            pe.setExportDirectory_TimeDateStamp((quint32)nValue);
                            break;

                        case N_IMAGE_EXPORT::MajorVersion:
                            pe.setExportDirectory_MajorVersion((quint16)nValue);
                            break;

                        case N_IMAGE_EXPORT::MinorVersion:
                            pe.setExportDirectory_MinorVersion((quint16)nValue);
                            break;

                        case N_IMAGE_EXPORT::Name:
                            pe.setExportDirectory_Name((quint32)nValue);
                            break;

                        case N_IMAGE_EXPORT::Base:
                            pe.setExportDirectory_Base((quint32)nValue);
                            break;

                        case N_IMAGE_EXPORT::NumberOfFunctions:
                            pe.setExportDirectory_NumberOfFunctions((quint32)nValue);
                            break;

                        case N_IMAGE_EXPORT::NumberOfNames:
                            pe.setExportDirectory_NumberOfNames((quint32)nValue);
                            break;

                        case N_IMAGE_EXPORT::AddressOfFunctions:
                            pe.setExportDirectory_AddressOfFunctions((quint32)nValue);
                            break;

                        case N_IMAGE_EXPORT::AddressOfNames:
                            pe.setExportDirectory_AddressOfNames((quint32)nValue);
                            break;

                        case N_IMAGE_EXPORT::AddressOfNameOrdinals:
                            pe.setExportDirectory_AddressOfNameOrdinals((quint32)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_TLS:
                    switch(nNdata)
                    {
                        case N_IMAGE_TLS::StartAddressOfRawData:
                            pe.setTLS_StartAddressOfRawData((quint64)nValue);
                            break;

                        case N_IMAGE_TLS::EndAddressOfRawData:
                            pe.setTLS_EndAddressOfRawData((quint64)nValue);
                            break;

                        case N_IMAGE_TLS::AddressOfIndex:
                            pe.setTLS_AddressOfIndex((quint64)nValue);
                            break;

                        case N_IMAGE_TLS::AddressOfCallBacks:
                            pe.setTLS_AddressOfCallBacks((quint64)nValue);
                            break;

                        case N_IMAGE_TLS::SizeOfZeroFill:
                            pe.setTLS_SizeOfZeroFill((quint32)nValue);
                            break;

                        case N_IMAGE_TLS::Characteristics:
                            pe.setTLS_Characteristics((quint32)nValue);
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

void PEWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_IMAGE_DOS_HEADER,N_IMAGE_DOS_HEADER::__data_size,bState);
    setLineEditsReadOnly(lineEdit_IMAGE_NT_HEADERS,N_IMAGE_NT_HEADERS::__data_size,bState);
    setLineEditsReadOnly(lineEdit_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::__data_size,bState);
    setLineEditsReadOnly(lineEdit_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::__data_size,bState);
    setLineEditsReadOnly(lineEdit_TLS,N_IMAGE_TLS::__data_size,bState);
    setLineEditsReadOnly(lineEdit_LoadConfig,N_IMAGE_LOADCONFIG::__data_size,bState);
//    setLineEditsReadOnly(lineEdit_IMAGE_DIRECTORY_ADDRESS,N_IMAGE_DIRECORIES::__data_size,bState);
//    setLineEditsReadOnly(lineEdit_IMAGE_DIRECTORY_SIZE,N_IMAGE_DIRECORIES::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);
    setPushButtonReadOnly(pushButton,__PB_size,bState);
    setDateTimeEditReadOnly(dateTimeEdit,__DT_size,bState);

    ui->widgetHex->setReadonly(bState);
    ui->widgetSectionHex->setReadonly(bState);
    ui->widgetOverlayHex->setReadonly(bState);
    ui->widgetResourceHex->setReadonly(bState);
}

void PEWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_IMAGE_DOS_HEADER,N_IMAGE_DOS_HEADER::__data_size,bState);
    _blockSignals((QObject **)lineEdit_IMAGE_NT_HEADERS,N_IMAGE_NT_HEADERS::__data_size,bState);
    _blockSignals((QObject **)lineEdit_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::__data_size,bState);
    _blockSignals((QObject **)lineEdit_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::__data_size,bState);
    _blockSignals((QObject **)lineEdit_TLS,N_IMAGE_TLS::__data_size,bState);
    _blockSignals((QObject **)lineEdit_LoadConfig,N_IMAGE_LOADCONFIG::__data_size,bState);
//    _blockSignals((QObject **)lineEdit_IMAGE_DIRECTORY_ADDRESS,N_IMAGE_DIRECORIES::__data_size,bState);
//    _blockSignals((QObject **)lineEdit_IMAGE_DIRECTORY_SIZE,N_IMAGE_DIRECORIES::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
    _blockSignals((QObject **)pushButton,__PB_size,bState);
    _blockSignals((QObject **)dateTimeEdit,__DT_size,bState);
}

void PEWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current)
    {
        reloadData();
    }
}

void PEWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
        case SPE::TYPE_IMAGE_DOS_HEADER:
            switch(nNdata)
            {
                case N_IMAGE_DOS_HEADER::e_magic:
                    lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_magic]->setValue((quint16)nValue);
                    break;
            }
            break;

        case SPE::TYPE_IMAGE_NT_HEADERS:
            switch(nNdata)
            {
                case N_IMAGE_NT_HEADERS::Signature:
                    lineEdit_IMAGE_NT_HEADERS[N_IMAGE_NT_HEADERS::Signature]->setValue((quint32)nValue);
                    break;
            }
            break;

        case SPE::TYPE_IMAGE_FILE_HEADER:
            switch(nNdata)
            {
                case N_IMAGE_FILE_HEADER::Machine:
                    lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Machine]->setValue((quint16)nValue);
                    break;

                case N_IMAGE_FILE_HEADER::TimeDateStamp:
                    lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::TimeDateStamp]->setValue((quint32)nValue);
                    break;

                case N_IMAGE_FILE_HEADER::Characteristics:
                    lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Characteristics]->setValue((quint16)nValue);
                    break;
            }
            break;

        case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
            switch(nNdata)
            {
                case N_IMAGE_OPTIONAL_HEADER::Magic:
                    lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Magic]->setValue((quint16)nValue);
                    break;

                case N_IMAGE_OPTIONAL_HEADER::Subsystem:
                    lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Subsystem]->setValue((quint16)nValue);
                    break;

                case N_IMAGE_OPTIONAL_HEADER::DllCharacteristics:
                    lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::DllCharacteristics]->setValue((quint16)nValue);
                    break;
            }
            break;
    }
}

void PEWidget::widgetAction()
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
        case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
            switch(nNdata)
            {
                case N_IMAGE_OPTIONAL_HEADER::CheckSum:
                    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

                    if(pe.isValid())
                    {
                        quint32 nCheckSum=pe.calculateCheckSum();
                        lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::CheckSum]->setValue(nCheckSum);
                    }
                    break;
            }
            break;
    }
}

void PEWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void PEWidget::editSectionHeader()
{
    int nRow=ui->tableWidget_Sections->currentRow();

    if(nRow!=-1)
    {
        DialogSectionHeader dsh(this);
        dsh.setData(getDevice(),getOptions(),(quint32)nRow);
        dsh.setEdited(isEdited());

        connect(&dsh,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

        dsh.exec();
        reloadData();
    }
}

void PEWidget::reloadData()
{
    int nData=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole).toInt();
    ui->stackedWidgetInfo->setCurrentIndex(nData);

    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(pe.isValid())
    {
        if(nData==SPE::TYPE_TOOLS)
        {
            if(!bInit[nData])
            {
                ui->widgetHex->setData(getDevice(),getOptions());
                ui->widgetHex->setEdited(isEdited());
                connect(ui->widgetHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

                bInit[nData]=true;
            }
        }
        else if(nData==SPE::TYPE_IMAGE_DOS_HEADER)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createHeaderTable(SPE::TYPE_IMAGE_DOS_HEADER,ui->tableWidget_IMAGE_DOS_HEADER,N_IMAGE_DOS_HEADER::records,lineEdit_IMAGE_DOS_HEADER,N_IMAGE_DOS_HEADER::__data_size,0);
                comboBox[CB_IMAGE_DOS_HEADER_e_magic]=createComboBox(ui->tableWidget_IMAGE_DOS_HEADER,XPE::getImageMagicsS(),SPE::TYPE_IMAGE_DOS_HEADER,N_IMAGE_DOS_HEADER::e_magic,XComboBoxEx::CBTYPE_NORMAL);
            }

            blockSignals(true);

            XMSDOS_DEF::IMAGE_DOS_HEADEREX msdosheaderex=pe.getDosHeaderEx();

            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_magic]->setValue(msdosheaderex.e_magic);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_cblp]->setValue(msdosheaderex.e_cblp);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_cp]->setValue(msdosheaderex.e_cp);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_crlc]->setValue(msdosheaderex.e_crlc);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_cparhdr]->setValue(msdosheaderex.e_cparhdr);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_minalloc]->setValue(msdosheaderex.e_minalloc);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_maxalloc]->setValue(msdosheaderex.e_maxalloc);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_ss]->setValue(msdosheaderex.e_ss);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_sp]->setValue(msdosheaderex.e_sp);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_csum]->setValue(msdosheaderex.e_csum);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_ip]->setValue(msdosheaderex.e_ip);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_cs]->setValue(msdosheaderex.e_cs);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_lfarlc]->setValue(msdosheaderex.e_lfarlc);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_ovno]->setValue(msdosheaderex.e_ovno);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res_0]->setValue(msdosheaderex.e_res[0]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res_1]->setValue(msdosheaderex.e_res[1]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res_2]->setValue(msdosheaderex.e_res[2]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res_3]->setValue(msdosheaderex.e_res[3]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_oemid]->setValue(msdosheaderex.e_oemid);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_oeminfo]->setValue(msdosheaderex.e_oeminfo);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_0]->setValue(msdosheaderex.e_res2[0]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_1]->setValue(msdosheaderex.e_res2[1]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_2]->setValue(msdosheaderex.e_res2[2]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_3]->setValue(msdosheaderex.e_res2[3]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_4]->setValue(msdosheaderex.e_res2[4]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_5]->setValue(msdosheaderex.e_res2[5]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_6]->setValue(msdosheaderex.e_res2[6]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_7]->setValue(msdosheaderex.e_res2[7]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_8]->setValue(msdosheaderex.e_res2[8]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_9]->setValue(msdosheaderex.e_res2[9]);
            lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_lfanew]->setValue(msdosheaderex.e_lfanew);

            comboBox[CB_IMAGE_DOS_HEADER_e_magic]->setValue(msdosheaderex.e_magic);

            blockSignals(false);
        }
        else if(nData==SPE::TYPE_IMAGE_NT_HEADERS)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createHeaderTable(SPE::TYPE_IMAGE_NT_HEADERS,ui->tableWidget_IMAGE_NT_HEADERS,N_IMAGE_NT_HEADERS::records,lineEdit_IMAGE_NT_HEADERS,N_IMAGE_NT_HEADERS::__data_size,0);
                comboBox[CB_IMAGE_NT_HEADERS_Signature]=createComboBox(ui->tableWidget_IMAGE_NT_HEADERS,XPE::getImageNtHeadersSignaturesS(),SPE::TYPE_IMAGE_NT_HEADERS,N_IMAGE_NT_HEADERS::Signature,XComboBoxEx::CBTYPE_NORMAL);
            }

            blockSignals(true);

            quint32 nSignature=pe.getNtHeaders_Signature();

            lineEdit_IMAGE_NT_HEADERS[N_IMAGE_NT_HEADERS::Signature]->setValue(nSignature);
            comboBox[CB_IMAGE_NT_HEADERS_Signature]->setValue(nSignature);

            blockSignals(false);
        }
        else if(nData==SPE::TYPE_IMAGE_FILE_HEADER)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createHeaderTable(SPE::TYPE_IMAGE_FILE_HEADER,ui->tableWidget_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::records,lineEdit_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::__data_size,0);
                comboBox[CB_IMAGE_FILE_HEADER_Machine]=createComboBox(ui->tableWidget_IMAGE_FILE_HEADER,XPE::getImageFileHeaderMachinesS(),SPE::TYPE_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::Machine,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_IMAGE_FILE_HEADER_Characteristics]=createComboBox(ui->tableWidget_IMAGE_FILE_HEADER,XPE::getImageFileHeaderCharacteristicsS(),SPE::TYPE_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::Characteristics,XComboBoxEx::CBTYPE_FLAGS);

                dateTimeEdit[DT_DateTimeStamp]=createDateTimeEdit(ui->tableWidget_IMAGE_FILE_HEADER,SPE::TYPE_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::TimeDateStamp,XDateTimeEditX::DT_TYPE_POSIX);
            }

            blockSignals(true);

            XPE_DEF::S_IMAGE_FILE_HEADER fileheader=pe.getFileHeader();

            lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Machine]->setValue(fileheader.Machine);
            lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::NumberOfSections]->setValue(fileheader.NumberOfSections);
            lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::TimeDateStamp]->setValue(fileheader.TimeDateStamp);
            lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::PointerToSymbolTable]->setValue(fileheader.PointerToSymbolTable);
            lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::NumberOfSymbols]->setValue(fileheader.NumberOfSymbols);
            lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::SizeOfOptionalHeader]->setValue(fileheader.SizeOfOptionalHeader);
            lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Characteristics]->setValue(fileheader.Characteristics);

            comboBox[CB_IMAGE_FILE_HEADER_Machine]->setValue(fileheader.Machine);
            comboBox[CB_IMAGE_FILE_HEADER_Characteristics]->setValue(fileheader.Characteristics);
            dateTimeEdit[DT_DateTimeStamp]->setValue(fileheader.TimeDateStamp);

            blockSignals(false);
        }
        else if(nData==SPE::TYPE_IMAGE_OPTIONAL_HEADER)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createHeaderTable(SPE::TYPE_IMAGE_OPTIONAL_HEADER,ui->tableWidget_IMAGE_OPTIONAL_HEADER,pe.is64()?(N_IMAGE_OPTIONAL_HEADER::records64):(N_IMAGE_OPTIONAL_HEADER::records32),lineEdit_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::__data_size,0);
                comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic]=createComboBox(ui->tableWidget_IMAGE_OPTIONAL_HEADER,XPE::getImageOptionalHeaderMagicS(),SPE::TYPE_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::Magic,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem]=createComboBox(ui->tableWidget_IMAGE_OPTIONAL_HEADER,XPE::getImageOptionalHeaderSubsystemS(),SPE::TYPE_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::Subsystem,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics]=createComboBox(ui->tableWidget_IMAGE_OPTIONAL_HEADER,XPE::getImageOptionalHeaderDllCharacteristicsS(),SPE::TYPE_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::DllCharacteristics,XComboBoxEx::CBTYPE_FLAGS);

                pushButton[PB_CalculateChecksum]=createPushButton(ui->tableWidget_IMAGE_OPTIONAL_HEADER,SPE::TYPE_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::CheckSum,tr("Calculate"));
            }

            blockSignals(true);

            if(pe.is64())
            {
                XPE_DEF::IMAGE_OPTIONAL_HEADER64S oh64=pe.getOptionalHeader64S();
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Magic]->setValue(oh64.Magic);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorLinkerVersion]->setValue(oh64.MajorLinkerVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorLinkerVersion]->setValue(oh64.MinorLinkerVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfCode]->setValue(oh64.SizeOfCode);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfInitializedData]->setValue(oh64.SizeOfInitializedData);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfUninitializedData]->setValue(oh64.SizeOfUninitializedData);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint]->setValue(oh64.AddressOfEntryPoint);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::BaseOfCode]->setValue(oh64.BaseOfCode);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::ImageBase]->setValue(oh64.ImageBase);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SectionAlignment]->setValue(oh64.SectionAlignment);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::FileAlignment]->setValue(oh64.FileAlignment);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion]->setValue(oh64.MajorOperatingSystemVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion]->setValue(oh64.MinorOperatingSystemVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorImageVersion]->setValue(oh64.MajorImageVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorImageVersion]->setValue(oh64.MinorImageVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorSubsystemVersion]->setValue(oh64.MajorSubsystemVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorSubsystemVersion]->setValue(oh64.MinorSubsystemVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Win32VersionValue]->setValue(oh64.Win32VersionValue);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfImage]->setValue(oh64.SizeOfImage);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeaders]->setValue(oh64.SizeOfHeaders);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::CheckSum]->setValue(oh64.CheckSum);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Subsystem]->setValue(oh64.Subsystem);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::DllCharacteristics]->setValue(oh64.DllCharacteristics);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfStackReserve]->setValue(oh64.SizeOfStackReserve);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfStackCommit]->setValue(oh64.SizeOfStackCommit);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeapReserve]->setValue(oh64.SizeOfHeapReserve);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeapCommit]->setValue(oh64.SizeOfHeapCommit);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::LoaderFlags]->setValue(oh64.LoaderFlags);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::NumberOfRvaAndSizes]->setValue(oh64.NumberOfRvaAndSizes);

                comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic]->setValue(oh64.Magic);
                comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem]->setValue(oh64.Subsystem);
                comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics]->setValue(oh64.DllCharacteristics);
            }
            else
            {
                XPE_DEF::IMAGE_OPTIONAL_HEADER32S oh32=pe.getOptionalHeader32S();
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Magic]->setValue(oh32.Magic);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorLinkerVersion]->setValue(oh32.MajorLinkerVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorLinkerVersion]->setValue(oh32.MinorLinkerVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfCode]->setValue(oh32.SizeOfCode);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfInitializedData]->setValue(oh32.SizeOfInitializedData);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfUninitializedData]->setValue(oh32.SizeOfUninitializedData);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint]->setValue(oh32.AddressOfEntryPoint);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::BaseOfCode]->setValue(oh32.BaseOfCode);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::BaseOfData]->setValue(oh32.BaseOfData);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::ImageBase]->setValue(oh32.ImageBase);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SectionAlignment]->setValue(oh32.SectionAlignment);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::FileAlignment]->setValue(oh32.FileAlignment);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion]->setValue(oh32.MajorOperatingSystemVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion]->setValue(oh32.MinorOperatingSystemVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorImageVersion]->setValue(oh32.MajorImageVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorImageVersion]->setValue(oh32.MinorImageVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorSubsystemVersion]->setValue(oh32.MajorSubsystemVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorSubsystemVersion]->setValue(oh32.MinorSubsystemVersion);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Win32VersionValue]->setValue(oh32.Win32VersionValue);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfImage]->setValue(oh32.SizeOfImage);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeaders]->setValue(oh32.SizeOfHeaders);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::CheckSum]->setValue(oh32.CheckSum);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Subsystem]->setValue(oh32.Subsystem);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::DllCharacteristics]->setValue(oh32.DllCharacteristics);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfStackReserve]->setValue(oh32.SizeOfStackReserve);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfStackCommit]->setValue(oh32.SizeOfStackCommit);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeapReserve]->setValue(oh32.SizeOfHeapReserve);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeapCommit]->setValue(oh32.SizeOfHeapCommit);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::LoaderFlags]->setValue(oh32.LoaderFlags);
                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::NumberOfRvaAndSizes]->setValue(oh32.NumberOfRvaAndSizes);

                comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic]->setValue(oh32.Magic);
                comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem]->setValue(oh32.Subsystem);
                comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics]->setValue(oh32.DllCharacteristics);
            }

            blockSignals(false);
        }
        else if(nData==SPE::TYPE_IMAGE_DIRECTORY_ENTRIES)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createDirectoryTable(SPE::TYPE_IMAGE_DIRECTORY_ENTRIES,ui->tableWidget_IMAGE_DIRECTORY_ENTRIES,N_IMAGE_DIRECORIES::records,N_IMAGE_DIRECORIES::__data_size);
            }

            blockSignals(true);

//            quint32 nNumberOfRvaAndSizes=pe.getOptionalHeader_NumberOfRvaAndSizes();

            for(int i=0; i<16; i++)
            {
                XPE_DEF::IMAGE_DATA_DIRECTORY dd=pe.getOptionalHeader_DataDirectory((quint32)i);
                ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i,2,new QTableWidgetItem(XBinary::valueToHex(dd.VirtualAddress)));

                QTableWidgetItem *pItem=new QTableWidgetItem(XBinary::valueToHex(dd.Size));
                ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i,3,pItem);

                // TODO !!!
//                ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i,3,new QTableWidgetItem(XBinary::valueToHex(dd.Size)));
//
//                lineEdit_IMAGE_DIRECTORY_ADDRESS[i]->setValue(dd.VirtualAddress);
//                lineEdit_IMAGE_DIRECTORY_SIZE[i]->setValue(dd.Size);

//                lineEdit_IMAGE_DIRECTORY_ADDRESS[i]->setEnabled(i<(int)nNumberOfRvaAndSizes);
//                lineEdit_IMAGE_DIRECTORY_SIZE[i]->setEnabled(i<(int)nNumberOfRvaAndSizes);
            }

            blockSignals(false);
        }
        else if(nData==SPE::TYPE_SECTIONS)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createSectionTable(SPE::TYPE_SECTIONS,ui->tableWidget_Sections,N_IMAGE_SECTION_HEADER::records,N_IMAGE_SECTION_HEADER::__data_size);
            }

            blockSignals(true);

            QList<XPE_DEF::IMAGE_SECTION_HEADER> listSections=pe.getSectionHeaders();
            int nCount=listSections.count();

            ui->tableWidget_Sections->setRowCount(nCount);

            //            record.sName=QString((char *)pList->at(i).Name);
            //            record.sName.resize(qMin(record.sName.length(),XPE_DEF::IMAGE_SIZEOF_SHORT_NAME));

            for(int i=0; i<nCount; i++)
            {
                QTableWidgetItem *itemNumber=new QTableWidgetItem(QString::number(i));

                itemNumber->setData(Qt::UserRole+SECTION_DATA_ADDRESS,listSections.at(i).VirtualAddress);

                if(getOptions()->bIsImage)
                {
                    itemNumber->setData(Qt::UserRole+SECTION_DATA_SIZE,listSections.at(i).Misc.VirtualSize);
                    itemNumber->setData(Qt::UserRole+SECTION_DATA_OFFSET,listSections.at(i).VirtualAddress);
                }
                else
                {
                    itemNumber->setData(Qt::UserRole+SECTION_DATA_SIZE,listSections.at(i).SizeOfRawData);
                    itemNumber->setData(Qt::UserRole+SECTION_DATA_OFFSET,listSections.at(i).PointerToRawData);
                }

                ui->tableWidget_Sections->setItem(i,0,itemNumber);

                QTableWidgetItem *itemName=new QTableWidgetItem();
                QString sName=QString((char *)listSections.at(i).Name);
                sName.resize(qMin(sName.length(),XPE_DEF::S_IMAGE_SIZEOF_SHORT_NAME));
                itemName->setText(sName);
                ui->tableWidget_Sections->setItem(i,N_IMAGE_SECTION_HEADER::Name+1,itemName);

                ui->tableWidget_Sections->setItem(i,N_IMAGE_SECTION_HEADER::VirtualSize+1,          new QTableWidgetItem(XBinary::valueToHex(listSections.at(i).Misc.VirtualSize)));
                ui->tableWidget_Sections->setItem(i,N_IMAGE_SECTION_HEADER::VirtualAddress+1,       new QTableWidgetItem(XBinary::valueToHex(listSections.at(i).VirtualAddress)));
                ui->tableWidget_Sections->setItem(i,N_IMAGE_SECTION_HEADER::SizeOfRawData+1,        new QTableWidgetItem(XBinary::valueToHex(listSections.at(i).SizeOfRawData)));
                ui->tableWidget_Sections->setItem(i,N_IMAGE_SECTION_HEADER::PointerToRawData+1,     new QTableWidgetItem(XBinary::valueToHex(listSections.at(i).PointerToRawData)));
                ui->tableWidget_Sections->setItem(i,N_IMAGE_SECTION_HEADER::PointerToRelocations+1, new QTableWidgetItem(XBinary::valueToHex(listSections.at(i).PointerToRelocations)));
                ui->tableWidget_Sections->setItem(i,N_IMAGE_SECTION_HEADER::PointerToLinenumbers+1, new QTableWidgetItem(XBinary::valueToHex(listSections.at(i).PointerToLinenumbers)));
                ui->tableWidget_Sections->setItem(i,N_IMAGE_SECTION_HEADER::NumberOfRelocations+1,  new QTableWidgetItem(XBinary::valueToHex(listSections.at(i).NumberOfRelocations)));
                ui->tableWidget_Sections->setItem(i,N_IMAGE_SECTION_HEADER::NumberOfLinenumbers+1,  new QTableWidgetItem(XBinary::valueToHex(listSections.at(i).NumberOfLinenumbers)));
                ui->tableWidget_Sections->setItem(i,N_IMAGE_SECTION_HEADER::Characteristics+1,      new QTableWidgetItem(XBinary::valueToHex(listSections.at(i).Characteristics)));
            }

            if(nCount)
            {
                if(ui->tableWidget_Sections->currentRow()==0)
                {
                    loadSection(0);
                }
                else
                {
                    ui->tableWidget_Sections->setCurrentCell(0,0);
                }
            }

            blockSignals(false);
        }
        else if(nData==SPE::TYPE_EXPORT)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createHeaderTable(SPE::TYPE_EXPORT,ui->tableWidget_ExportHeader,N_IMAGE_EXPORT::records,lineEdit_EXPORT,N_IMAGE_EXPORT::__data_size,0);
                createSectionTable(SPE::TYPE_EXPORT_FUNCTION,ui->tableWidget_ExportFunctions,N_IMAGE_EXPORT_FUNCTION::records,N_IMAGE_EXPORT_FUNCTION::__data_size);
            }

            blockSignals(true);

            XPE::EXPORT_HEADER eh=pe.getExport();

            lineEdit_EXPORT[N_IMAGE_EXPORT::Characteristics]->setValue(eh.directory.Characteristics);
            lineEdit_EXPORT[N_IMAGE_EXPORT::TimeDateStamp]->setValue(eh.directory.TimeDateStamp);
            lineEdit_EXPORT[N_IMAGE_EXPORT::MajorVersion]->setValue(eh.directory.MajorVersion);
            lineEdit_EXPORT[N_IMAGE_EXPORT::MinorVersion]->setValue(eh.directory.MinorVersion);
            lineEdit_EXPORT[N_IMAGE_EXPORT::Name]->setValue(eh.directory.Name);
            lineEdit_EXPORT[N_IMAGE_EXPORT::Base]->setValue(eh.directory.Base);
            lineEdit_EXPORT[N_IMAGE_EXPORT::NumberOfFunctions]->setValue(eh.directory.NumberOfFunctions);
            lineEdit_EXPORT[N_IMAGE_EXPORT::NumberOfNames]->setValue(eh.directory.NumberOfNames);
            lineEdit_EXPORT[N_IMAGE_EXPORT::AddressOfFunctions]->setValue(eh.directory.AddressOfFunctions);
            lineEdit_EXPORT[N_IMAGE_EXPORT::AddressOfNames]->setValue(eh.directory.AddressOfNames);
            lineEdit_EXPORT[N_IMAGE_EXPORT::AddressOfNameOrdinals]->setValue(eh.directory.AddressOfNameOrdinals);

            int nCount=eh.listPositions.count();
            ui->tableWidget_ExportFunctions->setRowCount(nCount);

            for(int i=0; i<nCount; i++)
            {
                ui->tableWidget_ExportFunctions->setItem(i,N_IMAGE_EXPORT_FUNCTION::Ordinal,                new QTableWidgetItem(XBinary::valueToHex(eh.listPositions.at(i).nOrdinal)));
                ui->tableWidget_ExportFunctions->setItem(i,N_IMAGE_EXPORT_FUNCTION::RVA,                    new QTableWidgetItem(XBinary::valueToHex(eh.listPositions.at(i).nRVA)));
                ui->tableWidget_ExportFunctions->setItem(i,N_IMAGE_EXPORT_FUNCTION::Name,                   new QTableWidgetItem(XBinary::valueToHex(eh.listPositions.at(i).nNameRVA)));
                ui->tableWidget_ExportFunctions->setItem(i,N_IMAGE_EXPORT_FUNCTION::Name+1,                 new QTableWidgetItem(eh.listPositions.at(i).sFunctionName));
            }

            blockSignals(false);
        }
        else if(nData==SPE::TYPE_IMPORT)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createSectionTable(SPE::TYPE_IMPORT,ui->tableWidget_ImportLibraries,N_IMAGE_IMPORT::records,N_IMAGE_IMPORT::__data_size);
                createSectionTable(SPE::TYPE_IMPORT_FUNCTION,ui->tableWidget_ImportFunctions,N_IMAGE_IMPORT_FUNCTION::records32,N_IMAGE_IMPORT_FUNCTION::__data_size);
            }

            QList<XPE::IMAGE_IMPORT_DESCRIPTOR_EX> listID=pe.getImportDescriptorsEx();
            int nCount=listID.count();
            ui->tableWidget_ImportLibraries->setRowCount(nCount);

            for(int i=0; i<nCount; i++)
            {
                ui->tableWidget_ImportLibraries->setItem(i,N_IMAGE_IMPORT::OriginalFirstThunk,              new QTableWidgetItem(XBinary::valueToHex(listID.at(i).OriginalFirstThunk)));
                ui->tableWidget_ImportLibraries->setItem(i,N_IMAGE_IMPORT::TimeDateStamp,                   new QTableWidgetItem(XBinary::valueToHex(listID.at(i).TimeDateStamp)));
                ui->tableWidget_ImportLibraries->setItem(i,N_IMAGE_IMPORT::ForwarderChain,                  new QTableWidgetItem(XBinary::valueToHex(listID.at(i).ForwarderChain)));
                ui->tableWidget_ImportLibraries->setItem(i,N_IMAGE_IMPORT::Name,                            new QTableWidgetItem(XBinary::valueToHex(listID.at(i).Name)));
                ui->tableWidget_ImportLibraries->setItem(i,N_IMAGE_IMPORT::FirstThunk,                      new QTableWidgetItem(XBinary::valueToHex(listID.at(i).FirstThunk)));
                ui->tableWidget_ImportLibraries->setItem(i,N_IMAGE_IMPORT::FirstThunk+1,                    new QTableWidgetItem(listID.at(i).sLibrary));
            }

            ui->tableWidget_ImportFunctions->setRowCount(0);

            if(nCount)
            {
                if(ui->tableWidget_ImportLibraries->currentRow()==0)
                {
                    loadImportLibrary(0);
                }
                else
                {
                    ui->tableWidget_ImportLibraries->selectRow(0);
                }
            }
        }
        else if(nData==SPE::TYPE_RESOURCE)
        {
//            if(pSubDeviceResource)
//            {
//                pSubDeviceResource->close();
//                delete pSubDeviceResource;
//            }

            ui->treeWidgetResource->clear();

            XPE::RESOURCE_HEADER rh=pe.getResourceHeader();

            int nHeaderCount=rh.listPositions.count();

            if(nHeaderCount)
            {
                QTreeWidgetItem *pRoot=new QTreeWidgetItem(ui->treeWidgetResource);
                pRoot->setText(0,tr("Resource"));
                //                ui->treeWidgetResource->insertTopLevelItem(0,);

                for(int i=0; i<nHeaderCount; i++)
                {
                    XPE::RESOURCE_POSITION pos=rh.listPositions.at(i);
                    QTreeWidgetItem *pPos=new QTreeWidgetItem(pRoot);
                    QString sPosText;

                    if(pos.rin.bIsName)
                    {
                        sPosText=QString("\"%1\"").arg(pos.rin.sName);
                    }
                    else
                    {
                        QMap<quint64, QString> mapRT=XPE::getResourceTypes();
                        QString sType=mapRT.value(pos.rin.nID);

                        if(sType!="")
                        {
                            sPosText=QString("%1(%2)").arg(sType).arg(pos.rin.nID);
                        }
                        else
                        {
                            sPosText=QString("%1").arg(pos.rin.nID);
                        }
                    }

                    pPos->setText(0,sPosText);

                    int nPosCount=pos.listPositions.count();

                    for(int j=0; j<nPosCount; j++)
                    {
                        XPE::RESOURCE_POSITION subpos=rh.listPositions.at(i).listPositions.at(j);
                        QTreeWidgetItem *pSubPos=new QTreeWidgetItem(pPos);
                        QString sSubPosText;

                        if(subpos.rin.bIsName)
                        {
                            sSubPosText=QString("\"%1\"").arg(subpos.rin.sName);
                        }
                        else
                        {
                            sSubPosText=QString("%1").arg(subpos.rin.nID);
                        }

                        pSubPos->setText(0,sSubPosText);

                        int nSubPosCount=subpos.listPositions.count();

                        for(int k=0; k<nSubPosCount; k++)
                        {
                            XPE::RESOURCE_POSITION record=rh.listPositions.at(i).listPositions.at(j).listPositions.at(k);
                            QTreeWidgetItem *pRecord=new QTreeWidgetItem(pSubPos);
                            QString sRecordText;

                            if(record.rin.bIsName)
                            {
                                sRecordText=QString("\"%1\"").arg(record.rin.sName);
                            }
                            else
                            {
                                sRecordText=QString("%1").arg(record.rin.nID);
                            }

                            pRecord->setText(0,sRecordText);

                            pRecord->setData(0,Qt::UserRole+SECTION_DATA_SIZE,record.data_entry.Size);
                            pRecord->setData(0,Qt::UserRole+SECTION_DATA_ADDRESS,record.data_entry.OffsetToData);

                            if(getOptions()->bIsImage)
                            {
                                pRecord->setData(0,Qt::UserRole+SECTION_DATA_OFFSET,record.data_entry.OffsetToData);
                            }
                            else
                            {
                                pRecord->setData(0,Qt::UserRole+SECTION_DATA_OFFSET,record.nDataOffset);
                            }
                        }
                    }
                }

                ui->treeWidgetResource->expandItem(pRoot);
            }
        }
        else if(nData==SPE::TYPE_EXCEPTION)
        {
            // TODO !!!
            blockSignals(true);
            blockSignals(false);
        }
        else if(nData==SPE::TYPE_RELOCS)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createSectionTable(SPE::TYPE_RELOCS,ui->tableWidget_Relocs,N_IMAGE_RELOCS::records,N_IMAGE_RELOCS::__data_size);
                createSectionTable(SPE::TYPE_RELOCS_POSITION,ui->tableWidget_RelocsPositions,N_IMAGE_RELOCS_POSITION::records,N_IMAGE_RELOCS_POSITION::__data_size);
            }

            QList<XPE::RELOCS_HEADER> listRH=pe.getRelocsHeaders();
            int nCount=listRH.count();
            ui->tableWidget_Relocs->setRowCount(nCount);

            for(int i=0; i<nCount; i++)
            {
                QTableWidgetItem *pItem=new QTableWidgetItem(XBinary::valueToHex(listRH.at(i).ibr.VirtualAddress));
                pItem->setData(Qt::UserRole,listRH.at(i).nOffset);
                ui->tableWidget_Relocs->setItem(i,N_IMAGE_RELOCS::VirtualAddress,               pItem);
                ui->tableWidget_Relocs->setItem(i,N_IMAGE_RELOCS::SizeOfBlock,                  new QTableWidgetItem(XBinary::valueToHex(listRH.at(i).ibr.SizeOfBlock)));
                ui->tableWidget_Relocs->setItem(i,N_IMAGE_RELOCS::SizeOfBlock+1,                new QTableWidgetItem(QString::number(listRH.at(i).nCount)));
            }

            ui->tableWidget_RelocsPositions->setRowCount(0);

            if(nCount)
            {
                if(ui->tableWidget_Relocs->currentRow()==0)
                {
                    loadRelocs(0);
                }
                else
                {
                    ui->tableWidget_Relocs->selectRow(0);
                }
            }            
        }
        else if(nData==SPE::TYPE_TLS)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createHeaderTable(SPE::TYPE_TLS,ui->tableWidget_TLS,pe.is64()?(N_IMAGE_TLS::records64):(N_IMAGE_TLS::records32),lineEdit_TLS,N_IMAGE_TLS::__data_size,0);
            }

            blockSignals(true);

            if(pe.is64())
            {
                XPE_DEF::S_IMAGE_TLS_DIRECTORY64 tls64=pe.getTLSDirectory64();
                lineEdit_TLS[N_IMAGE_TLS::StartAddressOfRawData]->setValue(tls64.StartAddressOfRawData);
                lineEdit_TLS[N_IMAGE_TLS::EndAddressOfRawData]->setValue(tls64.EndAddressOfRawData);
                lineEdit_TLS[N_IMAGE_TLS::AddressOfIndex]->setValue(tls64.AddressOfIndex);
                lineEdit_TLS[N_IMAGE_TLS::AddressOfCallBacks]->setValue(tls64.AddressOfCallBacks);
                lineEdit_TLS[N_IMAGE_TLS::SizeOfZeroFill]->setValue(tls64.SizeOfZeroFill);
                lineEdit_TLS[N_IMAGE_TLS::Characteristics]->setValue(tls64.Characteristics);
            }
            else
            {
                XPE_DEF::S_IMAGE_TLS_DIRECTORY32 tls32=pe.getTLSDirectory32();
                lineEdit_TLS[N_IMAGE_TLS::StartAddressOfRawData]->setValue(tls32.StartAddressOfRawData);
                lineEdit_TLS[N_IMAGE_TLS::EndAddressOfRawData]->setValue(tls32.EndAddressOfRawData);
                lineEdit_TLS[N_IMAGE_TLS::AddressOfIndex]->setValue(tls32.AddressOfIndex);
                lineEdit_TLS[N_IMAGE_TLS::AddressOfCallBacks]->setValue(tls32.AddressOfCallBacks);
                lineEdit_TLS[N_IMAGE_TLS::SizeOfZeroFill]->setValue(tls32.SizeOfZeroFill);
                lineEdit_TLS[N_IMAGE_TLS::Characteristics]->setValue(tls32.Characteristics);
            }

            blockSignals(false);
        }
        else if(nData==SPE::TYPE_LOADCONFIG)
        {
            if(!bInit[nData])
            {
                bInit[nData]=createHeaderTable(SPE::TYPE_LOADCONFIG,ui->tableWidget_LoadConfig,pe.is64()?(N_IMAGE_LOADCONFIG::records64):(N_IMAGE_LOADCONFIG::records32),lineEdit_LoadConfig,N_IMAGE_LOADCONFIG::__data_size,0);
            }

            blockSignals(true);
            blockSignals(false);
        }
        else if(nData==SPE::TYPE_OVERLAY)
        {
            if(pSubDeviceOverlay)
            {
                pSubDeviceOverlay->close();
                delete pSubDeviceOverlay;
            }

            qint64 nOverLayOffset=pe.getOverlayOffset();
            qint64 nOverlaySize=pe.getOverlaySize();

            pSubDeviceOverlay=new SubDevice(getDevice(),nOverLayOffset,nOverlaySize,this);
            pSubDeviceOverlay->open(getDevice()->openMode());

            FormatWidget::OPTIONS hexOptions=*getOptions();
            hexOptions.nImageBase=nOverLayOffset;
            ui->widgetOverlayHex->setData(pSubDeviceOverlay,&hexOptions);
            ui->widgetOverlayHex->setEdited(isEdited());
            connect(ui->widgetOverlayHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void PEWidget::on_tableWidget_Sections_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableWidget_Sections->currentRow();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEditHeader(tr("Edit header"),this);
        connect(&actionEditHeader, SIGNAL(triggered()), this, SLOT(editSectionHeader()));
        contextMenu.addAction(&actionEditHeader);

        contextMenu.exec(ui->tableWidget_Sections->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::loadImportLibrary(int nNumber)
{
    ui->tableWidget_ImportFunctions->setRowCount(0);

    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(pe.isValid())
    {
        bool bIs64=pe.is64();
        QList<XPE::IMPORT_POSITION> listIP=pe.getImportPositions(nNumber);

        int nCount=listIP.count();
        ui->tableWidget_ImportFunctions->setRowCount(nCount);

        for(int i=0; i<nCount; i++)
        {
            if(listIP.at(i).nOrdinal)
            {
                QString sOrdinal;

                if(bIs64)
                {
                    sOrdinal=XBinary::valueToHex((quint64)listIP.at(i).nOrdinal);
                }
                else
                {
                    sOrdinal=XBinary::valueToHex((quint32)listIP.at(i).nOrdinal);
                }

                ui->tableWidget_ImportFunctions->setItem(i,N_IMAGE_IMPORT_FUNCTION::Ordinal,    new QTableWidgetItem(sOrdinal));
            }
            else
            {
                QString sThunk;

                if(bIs64)
                {
                    sThunk=XBinary::valueToHex((quint64)listIP.at(i).nThunkValue);
                }
                else
                {
                    sThunk=XBinary::valueToHex((quint32)listIP.at(i).nThunkValue);
                }

                ui->tableWidget_ImportFunctions->setItem(i,N_IMAGE_IMPORT_FUNCTION::Thunk,      new QTableWidgetItem(sThunk));
                ui->tableWidget_ImportFunctions->setItem(i,N_IMAGE_IMPORT_FUNCTION::Hint,       new QTableWidgetItem(XBinary::valueToHex(listIP.at(i).nHint)));
                ui->tableWidget_ImportFunctions->setItem(i,N_IMAGE_IMPORT_FUNCTION::Hint+1,     new QTableWidgetItem(listIP.at(i).sName));
            }
        }
    }
}

void PEWidget::loadRelocs(int nNumber)
{
    qint64 nOffset=ui->tableWidget_Relocs->item(nNumber,0)->data(Qt::UserRole).toLongLong();

    ui->tableWidget_RelocsPositions->setRowCount(0);

    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(pe.isValid())
    {
        QList<XPE::RELOCS_POSITION> listRelocsPositions=pe.getRelocsPositions(nOffset);

        int nCount=listRelocsPositions.count();

        ui->tableWidget_RelocsPositions->setRowCount(nCount);

        QMap<quint64,QString> mapTypes=pe.getImageRelBasedS();

        for(int i=0; i<nCount; i++)
        {
            ui->tableWidget_RelocsPositions->setItem(i,N_IMAGE_RELOCS_POSITION::TypeOffset,       new QTableWidgetItem(XBinary::valueToHex(listRelocsPositions.at(i).nTypeOffset)));
            ui->tableWidget_RelocsPositions->setItem(i,N_IMAGE_RELOCS_POSITION::TypeOffset+1,     new QTableWidgetItem(mapTypes.value(listRelocsPositions.at(i).nType)));
            ui->tableWidget_RelocsPositions->setItem(i,N_IMAGE_RELOCS_POSITION::TypeOffset+2,     new QTableWidgetItem(XBinary::valueToHex((quint32)listRelocsPositions.at(i).nAddress)));
        }
    }
}

void PEWidget::loadSection(int nNumber)
{
    qint64 nOffset=ui->tableWidget_Sections->item(nNumber,0)->data(Qt::UserRole+SECTION_DATA_OFFSET).toLongLong();
    qint64 nSize=ui->tableWidget_Sections->item(nNumber,0)->data(Qt::UserRole+SECTION_DATA_SIZE).toLongLong();
    qint64 nAddress=ui->tableWidget_Sections->item(nNumber,0)->data(Qt::UserRole+SECTION_DATA_ADDRESS).toLongLong();
    //        qint64 nVSize=ui->tableWidget_Sections->item(currentRow,0)->data(Qt::UserRole+SECTION_DATA_VSIZE).toLongLong();

    if(pSubDeviceSection)
    {
        pSubDeviceSection->close();
        delete pSubDeviceSection;
    }

    pSubDeviceSection=new SubDevice(getDevice(),nOffset,nSize,this);

    pSubDeviceSection->open(getDevice()->openMode());

    FormatWidget::OPTIONS hexOptions=*getOptions();
    hexOptions.nImageBase=nAddress;

    ui->widgetSectionHex->setData(pSubDeviceSection,&hexOptions);
    ui->widgetSectionHex->setEdited(isEdited());
    connect(ui->widgetSectionHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));
}

void PEWidget::on_tableWidget_ImportLibraries_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(currentRow!=-1)
    {
        loadImportLibrary(currentRow);
    }
}

void PEWidget::on_tableWidget_Relocs_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(currentRow!=-1)
    {
        loadRelocs(currentRow);
    }
}

void PEWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    switch(type)
    {
        case SPE::TYPE_IMAGE_DOS_HEADER:
            pTableWidget->setColumnWidth(0,nSymbolWidth*10);
            pTableWidget->setColumnWidth(1,nSymbolWidth*6);
            pTableWidget->setColumnWidth(2,nSymbolWidth*8);
            pTableWidget->setColumnWidth(3,nSymbolWidth*13);
            break;

        case SPE::TYPE_IMAGE_NT_HEADERS:
            pTableWidget->setColumnWidth(0,nSymbolWidth*10);
            pTableWidget->setColumnWidth(1,nSymbolWidth*6);
            pTableWidget->setColumnWidth(2,nSymbolWidth*8);
            pTableWidget->setColumnWidth(3,nSymbolWidth*13);
            break;

        case SPE::TYPE_IMAGE_FILE_HEADER:
            pTableWidget->setColumnWidth(0,nSymbolWidth*15);
            pTableWidget->setColumnWidth(1,nSymbolWidth*6);
            pTableWidget->setColumnWidth(2,nSymbolWidth*8);
            pTableWidget->setColumnWidth(3,nSymbolWidth*24);
            break;

        case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
            pTableWidget->setColumnWidth(0,nSymbolWidth*18);
            pTableWidget->setColumnWidth(1,nSymbolWidth*9);
            pTableWidget->setColumnWidth(2,nSymbolWidth*14);
            pTableWidget->setColumnWidth(3,nSymbolWidth*22);
            break;

        case SPE::TYPE_EXPORT:
            pTableWidget->setColumnWidth(0,nSymbolWidth*18);
            pTableWidget->setColumnWidth(1,nSymbolWidth*6);
            pTableWidget->setColumnWidth(2,nSymbolWidth*14);
            pTableWidget->setColumnWidth(3,nSymbolWidth*22);
            break;

        case SPE::TYPE_TLS:
            pTableWidget->setColumnWidth(0,nSymbolWidth*15);
            pTableWidget->setColumnWidth(1,nSymbolWidth*9);
            pTableWidget->setColumnWidth(2,nSymbolWidth*14);
            pTableWidget->setColumnWidth(3,nSymbolWidth*13);
            break;
    }
}

bool PEWidget::createSectionTable(int type, QTableWidget *pTableWidget, const FormatWidget::HEADER_RECORD *pRecords, int nRecordCount)
{
    int nSymbolWidth=getSymbolWidth();

    QStringList slHeader;

    switch(type)
    {
        case SPE::TYPE_SECTIONS:
            pTableWidget->setColumnCount(nRecordCount+1);
            pTableWidget->setColumnWidth(0,nSymbolWidth*4);
            slHeader.append("");
            break;

        case SPE::TYPE_EXPORT_FUNCTION:
            pTableWidget->setColumnCount(nRecordCount+1);
            pTableWidget->setColumnWidth(nRecordCount,400);
            break;

        case SPE::TYPE_IMPORT_FUNCTION:
            pTableWidget->setColumnCount(nRecordCount+1);
            pTableWidget->setColumnWidth(0,120);
            pTableWidget->setColumnWidth(1,120);
            pTableWidget->setColumnWidth(2,50);
            pTableWidget->setColumnWidth(nRecordCount,400);
            break;

        case SPE::TYPE_IMPORT:
            pTableWidget->setColumnCount(nRecordCount+1);
            pTableWidget->setColumnWidth(nRecordCount,400);
            break;

        case SPE::TYPE_RELOCS:
            pTableWidget->setColumnCount(nRecordCount+1);
            break;

        case SPE::TYPE_RELOCS_POSITION:
            pTableWidget->setColumnCount(nRecordCount+2);
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
        case SPE::TYPE_EXPORT_FUNCTION:
        case SPE::TYPE_IMPORT_FUNCTION:
        case SPE::TYPE_IMPORT:
            slHeader.append("");
            break;

        case SPE::TYPE_RELOCS:
            slHeader.append(tr("Count"));
            break;

        case SPE::TYPE_RELOCS_POSITION:
            slHeader.append(tr("Type"));
            slHeader.append(tr("Address"));
            break;
    }

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    return true;
}

void PEWidget::on_tableWidget_Sections_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(currentRow!=-1)
    {
        loadSection(currentRow);
    }
}

void PEWidget::on_treeWidgetResource_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current)
    {
        // TODO more info in side widget
        if(current->data(0,Qt::UserRole+SECTION_DATA_SIZE).toLongLong())
        {
            qint64 nOffset=current->data(0,Qt::UserRole+SECTION_DATA_OFFSET).toLongLong();
            qint64 nSize=current->data(0,Qt::UserRole+SECTION_DATA_SIZE).toLongLong();
            qint64 nAddress=current->data(0,Qt::UserRole+SECTION_DATA_ADDRESS).toLongLong();

            if(pSubDeviceResource)
            {
                pSubDeviceResource->close();
                delete pSubDeviceResource;
            }

            pSubDeviceResource=new SubDevice(getDevice(),nOffset,nSize,this);

            pSubDeviceResource->open(getDevice()->openMode());

            FormatWidget::OPTIONS hexOptions=*getOptions();
            hexOptions.nImageBase=nAddress;

            ui->widgetResourceHex->setData(pSubDeviceResource,&hexOptions);
            ui->widgetResourceHex->setEdited(isEdited());
            connect(ui->widgetResourceHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));
        }
    }
}

void PEWidget::on_pushButtonReload_clicked()
{
    reload();
}
