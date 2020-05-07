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
#include "pewidget.h"
#include "ui_pewidget.h"

PEWidget::PEWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::PEWidget)
{
    ui->setupUi(this);
}

PEWidget::PEWidget(QIODevice *pDevice,FW_DEF::OPTIONS *pOptions, QWidget *parent) :
    FormatWidget(pDevice,pOptions,0,0,0,parent),
    ui(new Ui::PEWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions,0,0,0);
    reload();
}

PEWidget::~PEWidget()
{
    delete ui;
}

void PEWidget::clear()
{
    reset();

    memset(lineEdit_IMAGE_DOS_HEADER,0,sizeof lineEdit_IMAGE_DOS_HEADER);
    memset(lineEdit_IMAGE_NT_HEADERS,0,sizeof lineEdit_IMAGE_NT_HEADERS);
    memset(lineEdit_IMAGE_FILE_HEADER,0,sizeof lineEdit_IMAGE_FILE_HEADER);
    memset(lineEdit_IMAGE_OPTIONAL_HEADER,0,sizeof lineEdit_IMAGE_OPTIONAL_HEADER);
    memset(lineEdit_TLS,0,sizeof lineEdit_TLS);
    memset(lineEdit_LoadConfig,0,sizeof lineEdit_LoadConfig);
    memset(lineEdit_Version_FixedFileInfo,0,sizeof lineEdit_Version_FixedFileInfo);
    memset(lineEdit_NetHeader,0,sizeof lineEdit_NetHeader);
    memset(lineEdit_EXPORT,0,sizeof lineEdit_EXPORT);
    memset(comboBox,0,sizeof comboBox);
    memset(pushButton,0,sizeof pushButton);
    memset(dateTimeEdit,0,sizeof dateTimeEdit);
    memset(invWidget,0,sizeof invWidget);
    memset(subDevice,0,sizeof subDevice);

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void PEWidget::cleanup()
{

}

void PEWidget::reset()
{
    memset(bInit,0,sizeof bInit);
}

void PEWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(pe.isValid())
    {
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_HEX,tr("Hex")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_STRINGS,tr("Strings")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_MEMORYMAP,tr("Memory map")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_ENTROPY,tr("Entropy")));
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
            QTreeWidgetItem *pResources=createNewItem(SPE::TYPE_RESOURCE,"Resources");

            ui->treeWidgetNavi->addTopLevelItem(pResources);

            if(pe.isResourceVersionPresent())
            {
                pResources->addChild(createNewItem(SPE::TYPE_RESOURCE_VERSION,"Version"));
            }

            if(pe.isResourceManifestPresent())
            {
                pResources->addChild(createNewItem(SPE::TYPE_RESOURCE_MANIFEST,"Manifest"));
            }
        }

        if(pe.isExceptionPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_EXCEPTION,"Exception"));
        }

        if(pe.isRelocsPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_RELOCS,"Relocs"));
        }

        if(pe.isDebugPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_DEBUG,"Debug"));
        }

        if(pe.isTLSPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_TLS,"TLS"));
        }

        if(pe.isLoadConfigPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_LOADCONFIG,"Load Config"));
        }

        if(pe.isBoundImportPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_BOUNDIMPORT,"Bound import"));
        }

        if(pe.isDelayImportPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_DELAYIMPORT,"Delay import"));
        }

        if(pe.isNETPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_NETHEADER,".NET"));
        }

        if(pe.isOverlayPresent())
        {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_OVERLAY,tr("Overlay")));
        }

        ui->treeWidgetNavi->expandAll();
        ui->treeWidgetNavi->setCurrentItem(ui->treeWidgetNavi->topLevelItem(SPE::TYPE_IMAGE_DOS_HEADER));
    }
}

bool PEWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

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
                        case N_IMAGE_DOS_HEADER::e_magic:       comboBox[CB_IMAGE_DOS_HEADER_e_magic]->setValue(nValue);                            break;
                        case N_IMAGE_DOS_HEADER::e_lfanew:      invWidget[INV_IMAGE_DOS_HEADER_e_lfanew]->setOffsetAndSize(&pe,(quint32)nValue,0);  break;
                    }
                    break;

                case SPE::TYPE_IMAGE_NT_HEADERS:
                    switch(nNdata)
                    {
                        case N_IMAGE_NT_HEADERS::Signature:     comboBox[CB_IMAGE_NT_HEADERS_Signature]->setValue(nValue);                          break;
                    }
                    break;

                case SPE::TYPE_IMAGE_FILE_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_FILE_HEADER::Machine:              comboBox[CB_IMAGE_FILE_HEADER_Machine]->setValue(nValue);                                                               break;
                        case N_IMAGE_FILE_HEADER::TimeDateStamp:        dateTimeEdit[TD_IMAGE_FILE_HEADER_TimeDateStamp]->setValue(nValue);                                                     break;
                        case N_IMAGE_FILE_HEADER::PointerToSymbolTable: invWidget[INV_IMAGE_FILE_HEADER_PointerToSymbolTable]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);    break;
                        case N_IMAGE_FILE_HEADER::Characteristics:      comboBox[CB_IMAGE_FILE_HEADER_Characteristics]->setValue(nValue);                                                       break;
                    }
                    break;

                case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_OPTIONAL_HEADER::Magic:                comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic]->setValue(nValue);                                                             break;
                        case N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint:  invWidget[INV_IMAGE_OPTIONAL_HEADER_AddressOfEntryPoint]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0); break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfCode:           invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfCode]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);          break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfData:           invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfData]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);          break;
                        case N_IMAGE_OPTIONAL_HEADER::Subsystem:            comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem]->setValue(nValue);                                                         break;
                        case N_IMAGE_OPTIONAL_HEADER::DllCharacteristics:   comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics]->setValue(nValue);                                                break;
                    }
                    break;

                case SPE::TYPE_EXPORT:
                    switch(nNdata)
                    {
                        case N_IMAGE_EXPORT::TimeDateStamp:         dateTimeEdit[TD_IMAGE_EXPORT_TimeDateStamp]->setValue(nValue);                                                          break;
                        case N_IMAGE_EXPORT::Name:                  invWidget[INV_IMAGE_EXPORT_Name]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);                         break;
                        case N_IMAGE_EXPORT::AddressOfFunctions:    invWidget[INV_IMAGE_EXPORT_AddressOfFunctions]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);           break;
                        case N_IMAGE_EXPORT::AddressOfNames:        invWidget[INV_IMAGE_EXPORT_AddressOfNames]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);               break;
                        case N_IMAGE_EXPORT::AddressOfNameOrdinals: invWidget[INV_IMAGE_EXPORT_AddressOfNameOrdinals]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);        break;
                    }
                    break;

                case SPE::TYPE_TLS:
                    switch(nNdata)
                    {
                        case N_IMAGE_TLS::StartAddressOfRawData:    invWidget[INV_IMAGE_TLS_StartAddressOfRawData]->setAddressAndSize(&pe,(quint64)nValue,0);       break;
                        case N_IMAGE_TLS::EndAddressOfRawData:      invWidget[INV_IMAGE_TLS_EndAddressOfRawData]->setAddressAndSize(&pe,(quint64)nValue,0);         break;
                        case N_IMAGE_TLS::AddressOfIndex:           invWidget[INV_IMAGE_TLS_AddressOfIndex]->setAddressAndSize(&pe,(quint64)nValue,0);              break;
                        case N_IMAGE_TLS::AddressOfCallBacks:       invWidget[INV_IMAGE_TLS_AddressOfCallBacks]->setAddressAndSize(&pe,(quint64)nValue,0);          break;
                    }
                    break;

                case SPE::TYPE_NETHEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_NETHEADER::Flags:      comboBox[CB_IMAGE_NETHEADER_FLAGS]->setValue((quint32)nValue);      break;
                    }
                    break;

                case SPE::TYPE_LOADCONFIG:
                    switch(nNdata)
                    {
                        case N_IMAGE_LOADCONFIG::SecurityCookie:    invWidget[INV_IMAGE_LOADCONFIG_SecurityCookie]->setAddressAndSize(&pe,(quint64)nValue,0);   break;
                        case N_IMAGE_LOADCONFIG::SEHandlerTable:    invWidget[INV_IMAGE_LOADCONFIG_SEHandlerTable]->setAddressAndSize(&pe,(quint64)nValue,0);   break;
                    }
                    break;
            }

            switch(nStype)
            {
                case SPE::TYPE_IMAGE_DOS_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_DOS_HEADER::e_magic:               pe.set_e_magic((quint16)nValue);            break;
                        case N_IMAGE_DOS_HEADER::e_cblp:                pe.set_e_cblp((quint16)nValue);             break;
                        case N_IMAGE_DOS_HEADER::e_cp:                  pe.set_e_cp((quint16)nValue);               break;
                        case N_IMAGE_DOS_HEADER::e_crlc:                pe.set_e_crlc((quint16)nValue);             break;
                        case N_IMAGE_DOS_HEADER::e_cparhdr:             pe.set_e_cparhdr((quint16)nValue);          break;
                        case N_IMAGE_DOS_HEADER::e_minalloc:            pe.set_e_minalloc((quint16)nValue);         break;
                        case N_IMAGE_DOS_HEADER::e_maxalloc:            pe.set_e_maxalloc((quint16)nValue);         break;
                        case N_IMAGE_DOS_HEADER::e_ss:                  pe.set_e_ss((quint16)nValue);               break;
                        case N_IMAGE_DOS_HEADER::e_sp:                  pe.set_e_sp((quint16)nValue);               break;
                        case N_IMAGE_DOS_HEADER::e_csum:                pe.set_e_csum((quint16)nValue);             break;
                        case N_IMAGE_DOS_HEADER::e_ip:                  pe.set_e_ip((quint16)nValue);               break;
                        case N_IMAGE_DOS_HEADER::e_cs:                  pe.set_e_cs((quint16)nValue);               break;
                        case N_IMAGE_DOS_HEADER::e_lfarlc:              pe.set_e_lfarlc((quint16)nValue);           break;
                        case N_IMAGE_DOS_HEADER::e_ovno:                pe.set_e_ovno((quint16)nValue);             break;
                        case N_IMAGE_DOS_HEADER::e_res_0:               pe.set_e_res(0,(quint16)nValue);            break;
                        case N_IMAGE_DOS_HEADER::e_res_1:               pe.set_e_res(1,(quint16)nValue);            break;
                        case N_IMAGE_DOS_HEADER::e_res_2:               pe.set_e_res(2,(quint16)nValue);            break;
                        case N_IMAGE_DOS_HEADER::e_res_3:               pe.set_e_res(3,(quint16)nValue);            break;
                        case N_IMAGE_DOS_HEADER::e_oemid:               pe.set_e_oemid((quint16)nValue);            break;
                        case N_IMAGE_DOS_HEADER::e_oeminfo:             pe.set_e_oeminfo((quint16)nValue);          break;
                        case N_IMAGE_DOS_HEADER::e_res2_0:              pe.set_e_res2(0,(quint16)nValue);           break;
                        case N_IMAGE_DOS_HEADER::e_res2_1:              pe.set_e_res2(1,(quint16)nValue);           break;
                        case N_IMAGE_DOS_HEADER::e_res2_2:              pe.set_e_res2(2,(quint16)nValue);           break;
                        case N_IMAGE_DOS_HEADER::e_res2_3:              pe.set_e_res2(3,(quint16)nValue);           break;
                        case N_IMAGE_DOS_HEADER::e_res2_4:              pe.set_e_res2(4,(quint16)nValue);           break;
                        case N_IMAGE_DOS_HEADER::e_res2_5:              pe.set_e_res2(5,(quint16)nValue);           break;
                        case N_IMAGE_DOS_HEADER::e_res2_6:              pe.set_e_res2(6,(quint16)nValue);           break;
                        case N_IMAGE_DOS_HEADER::e_res2_7:              pe.set_e_res2(7,(quint16)nValue);           break;
                        case N_IMAGE_DOS_HEADER::e_res2_8:              pe.set_e_res2(8,(quint16)nValue);           break;
                        case N_IMAGE_DOS_HEADER::e_res2_9:              pe.set_e_res2(9,(quint16)nValue);           break;
                        case N_IMAGE_DOS_HEADER::e_lfanew:              pe.set_e_lfanew((quint32)nValue);           break;
                    }

                    ui->widgetHex_IMAGE_DOS_HEADER->reload();

                    break;

                case SPE::TYPE_IMAGE_NT_HEADERS:
                    switch(nNdata)
                    {
                        case N_IMAGE_NT_HEADERS::Signature:             pe.setNtHeaders_Signature((quint32)nValue);     break;
                    }

                    ui->widgetHex_IMAGE_NT_HEADERS->reload();

                    break;

                case SPE::TYPE_IMAGE_FILE_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_FILE_HEADER::Machine:              pe.setFileHeader_Machine((quint16)nValue);              break;
                        case N_IMAGE_FILE_HEADER::NumberOfSections:     pe.setFileHeader_NumberOfSections((quint16)nValue);     break;
                        case N_IMAGE_FILE_HEADER::TimeDateStamp:        pe.setFileHeader_TimeDateStamp((quint32)nValue);        break;
                        case N_IMAGE_FILE_HEADER::PointerToSymbolTable: pe.setFileHeader_PointerToSymbolTable((quint32)nValue); break;
                        case N_IMAGE_FILE_HEADER::NumberOfSymbols:      pe.setFileHeader_NumberOfSymbols((quint32)nValue);      break;
                        case N_IMAGE_FILE_HEADER::SizeOfOptionalHeader: pe.setFileHeader_SizeOfOptionalHeader((quint16)nValue); break;
                        case N_IMAGE_FILE_HEADER::Characteristics:      pe.setFileHeader_Characteristics((quint16)nValue);      break;
                    }

                    ui->widgetHex_IMAGE_FILE_HEADER->reload();

                    break;

                case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_OPTIONAL_HEADER::Magic:                        pe.setOptionalHeader_Magic((quint16)nValue);                        break;
                        case N_IMAGE_OPTIONAL_HEADER::MajorLinkerVersion:           pe.setOptionalHeader_MajorLinkerVersion((quint8)nValue);            break;
                        case N_IMAGE_OPTIONAL_HEADER::MinorLinkerVersion:           pe.setOptionalHeader_MinorLinkerVersion((quint8)nValue);            break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfCode:                   pe.setOptionalHeader_SizeOfCode((quint32)nValue);                   break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfInitializedData:        pe.setOptionalHeader_SizeOfInitializedData((quint32)nValue);        break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfUninitializedData:      pe.setOptionalHeader_SizeOfUninitializedData((quint32)nValue);      break;
                        case N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint:          pe.setOptionalHeader_AddressOfEntryPoint((quint32)nValue);          break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfCode:                   pe.setOptionalHeader_BaseOfCode((quint32)nValue);                   break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfData:                   pe.setOptionalHeader_BaseOfData((quint32)nValue);                   break;
                        case N_IMAGE_OPTIONAL_HEADER::ImageBase:                    pe.setOptionalHeader_ImageBase((quint64)nValue);                    break;
                        case N_IMAGE_OPTIONAL_HEADER::SectionAlignment:             pe.setOptionalHeader_SectionAlignment((quint32)nValue);             break;
                        case N_IMAGE_OPTIONAL_HEADER::FileAlignment:                pe.setOptionalHeader_FileAlignment((quint32)nValue);                break;
                        case N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion:  pe.setOptionalHeader_MajorOperatingSystemVersion((quint16)nValue);  break;
                        case N_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion:  pe.setOptionalHeader_MinorOperatingSystemVersion((quint16)nValue);  break;
                        case N_IMAGE_OPTIONAL_HEADER::MajorImageVersion:            pe.setOptionalHeader_MajorImageVersion((quint16)nValue);            break;
                        case N_IMAGE_OPTIONAL_HEADER::MinorImageVersion:            pe.setOptionalHeader_MinorImageVersion((quint16)nValue);            break;
                        case N_IMAGE_OPTIONAL_HEADER::MajorSubsystemVersion:        pe.setOptionalHeader_MajorSubsystemVersion((quint16)nValue);        break;
                        case N_IMAGE_OPTIONAL_HEADER::MinorSubsystemVersion:        pe.setOptionalHeader_MinorSubsystemVersion((quint16)nValue);        break;
                        case N_IMAGE_OPTIONAL_HEADER::Win32VersionValue:            pe.setOptionalHeader_Win32VersionValue((quint16)nValue);            break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfImage:                  pe.setOptionalHeader_SizeOfImage((quint32)nValue);                  break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfHeaders:                pe.setOptionalHeader_SizeOfHeaders((quint32)nValue);                break;
                        case N_IMAGE_OPTIONAL_HEADER::CheckSum:                     pe.setOptionalHeader_CheckSum((quint32)nValue);                     break;
                        case N_IMAGE_OPTIONAL_HEADER::Subsystem:                    pe.setOptionalHeader_Subsystem((quint16)nValue);                    break;
                        case N_IMAGE_OPTIONAL_HEADER::DllCharacteristics:           pe.setOptionalHeader_DllCharacteristics((quint16)nValue);           break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfStackReserve:           pe.setOptionalHeader_SizeOfStackReserve((quint64)nValue);           break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfStackCommit:            pe.setOptionalHeader_SizeOfStackCommit((quint64)nValue);            break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfHeapReserve:            pe.setOptionalHeader_SizeOfHeapReserve((quint64)nValue);            break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfHeapCommit:             pe.setOptionalHeader_SizeOfHeapCommit((quint64)nValue);             break;
                        case N_IMAGE_OPTIONAL_HEADER::LoaderFlags:                  pe.setOptionalHeader_LoaderFlags((quint32)nValue);                  break;
                        case N_IMAGE_OPTIONAL_HEADER::NumberOfRvaAndSizes:          pe.setOptionalHeader_NumberOfRvaAndSizes((quint32)nValue);          break;
                    }

                    ui->widgetHex_IMAGE_OPTIONAL_HEADER->reload();

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
                        case N_IMAGE_EXPORT::Characteristics:       pe.setExportDirectory_Characteristics((quint32)nValue);         break;
                        case N_IMAGE_EXPORT::TimeDateStamp:         pe.setExportDirectory_TimeDateStamp((quint32)nValue);           break;
                        case N_IMAGE_EXPORT::MajorVersion:          pe.setExportDirectory_MajorVersion((quint16)nValue);            break;
                        case N_IMAGE_EXPORT::MinorVersion:          pe.setExportDirectory_MinorVersion((quint16)nValue);            break;
                        case N_IMAGE_EXPORT::Name:                  pe.setExportDirectory_Name((quint32)nValue);                    break;
                        case N_IMAGE_EXPORT::Base:                  pe.setExportDirectory_Base((quint32)nValue);                    break;
                        case N_IMAGE_EXPORT::NumberOfFunctions:     pe.setExportDirectory_NumberOfFunctions((quint32)nValue);       break;
                        case N_IMAGE_EXPORT::NumberOfNames:         pe.setExportDirectory_NumberOfNames((quint32)nValue);           break;
                        case N_IMAGE_EXPORT::AddressOfFunctions:    pe.setExportDirectory_AddressOfFunctions((quint32)nValue);      break;
                        case N_IMAGE_EXPORT::AddressOfNames:        pe.setExportDirectory_AddressOfNames((quint32)nValue);          break;
                        case N_IMAGE_EXPORT::AddressOfNameOrdinals: pe.setExportDirectory_AddressOfNameOrdinals((quint32)nValue);   break;
                    }
                    break;

                case SPE::TYPE_TLS:
                    switch(nNdata)
                    {
                        case N_IMAGE_TLS::StartAddressOfRawData:    pe.setTLS_StartAddressOfRawData((quint64)nValue);           break;
                        case N_IMAGE_TLS::EndAddressOfRawData:      pe.setTLS_EndAddressOfRawData((quint64)nValue);             break;
                        case N_IMAGE_TLS::AddressOfIndex:           pe.setTLS_AddressOfIndex((quint64)nValue);                  break;
                        case N_IMAGE_TLS::AddressOfCallBacks:       pe.setTLS_AddressOfCallBacks((quint64)nValue);              break;
                        case N_IMAGE_TLS::SizeOfZeroFill:           pe.setTLS_SizeOfZeroFill((quint32)nValue);                  break;
                        case N_IMAGE_TLS::Characteristics:          pe.setTLS_Characteristics((quint32)nValue);                 break;
                    }

                    ui->widgetHex_TLS->reload();

                    break;

                case SPE::TYPE_RESOURCE_VERSION:
                    switch(nNdata)
                    {
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature:           pe.setFixedFileInfo_dwSignature((quint32)nValue);               break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwStrucVersion:        pe.setFixedFileInfo_dwStrucVersion((quint32)nValue);            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionMS:       pe.setFixedFileInfo_dwFileVersionMS((quint32)nValue);           break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionLS:       pe.setFixedFileInfo_dwFileVersionLS((quint32)nValue);           break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionMS:    pe.setFixedFileInfo_dwProductVersionMS((quint32)nValue);        break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionLS:    pe.setFixedFileInfo_dwProductVersionLS((quint32)nValue);        break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlagsMask:       pe.setFixedFileInfo_dwFileFlagsMask((quint32)nValue);           break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags:           pe.setFixedFileInfo_dwFileFlags((quint32)nValue);               break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS:              pe.setFixedFileInfo_dwFileOS((quint32)nValue);                  break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType:            pe.setFixedFileInfo_dwFileType((quint32)nValue);                break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileSubtype:         pe.setFixedFileInfo_dwFileSubtype((quint32)nValue);             break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileDateMS:          pe.setFixedFileInfo_dwFileDateMS((quint32)nValue);              break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileDateLS:          pe.setFixedFileInfo_dwFileDateLS((quint32)nValue);              break;
                    }
                    break;

                case SPE::TYPE_NETHEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_NETHEADER::cb:                                 pe.setNetHeader_cb((quint32)nValue);                            break;
                        case N_IMAGE_NETHEADER::MinorRuntimeVersion:                pe.setNetHeader_MinorRuntimeVersion((quint16)nValue);           break;
                        case N_IMAGE_NETHEADER::MajorRuntimeVersion:                pe.setNetHeader_MajorRuntimeVersion((quint16)nValue);           break;
                        case N_IMAGE_NETHEADER::MetaData_Address:                   pe.setNetHeader_MetaData_Address((quint32)nValue);              break;
                        case N_IMAGE_NETHEADER::MetaData_Size:                      pe.setNetHeader_MetaData_Size((quint32)nValue);                 break;
                        case N_IMAGE_NETHEADER::Flags:                              pe.setNetHeader_Flags((quint32)nValue);                         break;
                        case N_IMAGE_NETHEADER::EntryPoint:                         pe.setNetHeader_EntryPoint((quint32)nValue);                    break;
                    }

                    ui->widgetHex_NetHeader->reload();

                    break;

                case SPE::TYPE_LOADCONFIG:
                    switch(nNdata)
                    {
                        case N_IMAGE_LOADCONFIG::Size:                          pe.setLoadConfig_Size((quint32)nValue);                             break;
                        case N_IMAGE_LOADCONFIG::TimeDateStamp:                 pe.setLoadConfig_TimeDateStamp((quint32)nValue);                    break;
                        case N_IMAGE_LOADCONFIG::MinorVersion:                  pe.setLoadConfig_MinorVersion((quint16)nValue);                     break;
                        case N_IMAGE_LOADCONFIG::MajorVersion:                  pe.setLoadConfig_MajorVersion((quint16)nValue);                     break;
                        case N_IMAGE_LOADCONFIG::GlobalFlagsClear:              pe.setLoadConfig_GlobalFlagsClear((quint32)nValue);                 break;
                        case N_IMAGE_LOADCONFIG::GlobalFlagsSet:                pe.setLoadConfig_GlobalFlagsSet((quint32)nValue);                   break;
                        case N_IMAGE_LOADCONFIG::CriticalSectionDefaultTimeout: pe.setLoadConfig_CriticalSectionDefaultTimeout((quint32)nValue);    break;
                        case N_IMAGE_LOADCONFIG::DeCommitFreeBlockThreshold:    pe.setLoadConfig_DeCommitFreeBlockThreshold((quint64)nValue);       break;
                        case N_IMAGE_LOADCONFIG::DeCommitTotalFreeThreshold:    pe.setLoadConfig_DeCommitTotalFreeThreshold((quint64)nValue);       break;
                        case N_IMAGE_LOADCONFIG::LockPrefixTable:               pe.setLoadConfig_LockPrefixTable((quint64)nValue);                  break;
                        case N_IMAGE_LOADCONFIG::MaximumAllocationSize:         pe.setLoadConfig_MaximumAllocationSize((quint64)nValue);            break;
                        case N_IMAGE_LOADCONFIG::VirtualMemoryThreshold:        pe.setLoadConfig_VirtualMemoryThreshold((quint64)nValue);           break;
                        case N_IMAGE_LOADCONFIG::ProcessAffinityMask:           pe.setLoadConfig_ProcessAffinityMask((quint64)nValue);              break;
                        case N_IMAGE_LOADCONFIG::CSDVersion:                    pe.setLoadConfig_CSDVersion((quint16)nValue);                       break;
                        case N_IMAGE_LOADCONFIG::DependentLoadFlags:            pe.setLoadConfig_DependentLoadFlags((quint16)nValue);               break;
                        case N_IMAGE_LOADCONFIG::EditList:                      pe.setLoadConfig_EditList((quint64)nValue);                         break;
                        case N_IMAGE_LOADCONFIG::SecurityCookie:                pe.setLoadConfig_SecurityCookie((quint64)nValue);                   break;
                        case N_IMAGE_LOADCONFIG::SEHandlerTable:                pe.setLoadConfig_SEHandlerTable((quint64)nValue);                   break;
                        case N_IMAGE_LOADCONFIG::SEHandlerCount:                pe.setLoadConfig_SEHandlerCount((quint64)nValue);                   break;
                    }

                    ui->widgetHex_LoadConfig->reload();

                    break;
            }

            switch(nStype)
            {
                case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint:          addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress((quint32)nValue));   break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfCode:                   addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::BaseOfCode,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress((quint32)nValue));            break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfData:                   addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::BaseOfData,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress((quint32)nValue));            break;
                    }

                    break;

                case SPE::TYPE_EXPORT:
                    switch(nNdata)
                    {
                        case N_IMAGE_EXPORT::Name:                  addComment(ui->tableWidget_ExportHeader,N_IMAGE_EXPORT::Name,HEADER_COLUMN_COMMENT,pe.read_ansiString(pe.relAddressToOffset((quint32)nValue)));             break;
                        case N_IMAGE_EXPORT::AddressOfFunctions:    addComment(ui->tableWidget_ExportHeader,N_IMAGE_EXPORT::AddressOfFunctions,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress((quint32)nValue));      break;
                        case N_IMAGE_EXPORT::AddressOfNames:        addComment(ui->tableWidget_ExportHeader,N_IMAGE_EXPORT::AddressOfNames,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress((quint32)nValue));          break;
                        case N_IMAGE_EXPORT::AddressOfNameOrdinals: addComment(ui->tableWidget_ExportHeader,N_IMAGE_EXPORT::AddressOfNameOrdinals,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress((quint32)nValue));   break;
                    }

                    break;

                case SPE::TYPE_TLS:
                    switch(nNdata)
                    {
                        case N_IMAGE_TLS::StartAddressOfRawData:    addComment(ui->tableWidget_TLS,N_IMAGE_TLS::StartAddressOfRawData,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByAddress((quint64)nValue));                  break;
                        case N_IMAGE_TLS::EndAddressOfRawData:      addComment(ui->tableWidget_TLS,N_IMAGE_TLS::EndAddressOfRawData,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByAddress((quint64)nValue));                    break;
                        case N_IMAGE_TLS::AddressOfIndex:           addComment(ui->tableWidget_TLS,N_IMAGE_TLS::AddressOfIndex,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByAddress((quint64)nValue));                         break;
                        case N_IMAGE_TLS::AddressOfCallBacks:       addComment(ui->tableWidget_TLS,N_IMAGE_TLS::AddressOfCallBacks,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByAddress((quint64)nValue));                     break;
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
    setLineEditsReadOnly(lineEdit_Version_FixedFileInfo,N_IMAGE_RESOURCE_FIXEDFILEINFO::__data_size,bState);
    setLineEditsReadOnly(lineEdit_LoadConfig,N_IMAGE_LOADCONFIG::__data_size,bState);
    setLineEditsReadOnly(lineEdit_EXPORT,N_IMAGE_EXPORT::__data_size,bState);
    setLineEditsReadOnly(lineEdit_NetHeader,N_IMAGE_NETHEADER::__data_size,bState);
//    setLineEditsReadOnly(lineEdit_IMAGE_DIRECTORY_ADDRESS,N_IMAGE_DIRECORIES::__data_size,bState);
//    setLineEditsReadOnly(lineEdit_IMAGE_DIRECTORY_SIZE,N_IMAGE_DIRECORIES::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);
    setPushButtonReadOnly(pushButton,__PB_size,bState);
    setDateTimeEditReadOnly(dateTimeEdit,__TD_size,bState);

    ui->widgetHex->setReadonly(bState);
    ui->widgetSectionHex->setReadonly(bState);
    ui->widgetOverlayHex->setReadonly(bState);
    ui->widgetResourcesHex->setReadonly(bState);
}

void PEWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_IMAGE_DOS_HEADER,N_IMAGE_DOS_HEADER::__data_size,bState);
    _blockSignals((QObject **)lineEdit_IMAGE_NT_HEADERS,N_IMAGE_NT_HEADERS::__data_size,bState);
    _blockSignals((QObject **)lineEdit_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::__data_size,bState);
    _blockSignals((QObject **)lineEdit_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::__data_size,bState);
    _blockSignals((QObject **)lineEdit_TLS,N_IMAGE_TLS::__data_size,bState);
    _blockSignals((QObject **)lineEdit_Version_FixedFileInfo,N_IMAGE_RESOURCE_FIXEDFILEINFO::__data_size,bState);
    _blockSignals((QObject **)lineEdit_LoadConfig,N_IMAGE_LOADCONFIG::__data_size,bState);
    _blockSignals((QObject **)lineEdit_EXPORT,N_IMAGE_EXPORT::__data_size,bState);
    _blockSignals((QObject **)lineEdit_NetHeader,N_IMAGE_NETHEADER::__data_size,bState);
//    _blockSignals((QObject **)lineEdit_IMAGE_DIRECTORY_ADDRESS,N_IMAGE_DIRECORIES::__data_size,bState);
//    _blockSignals((QObject **)lineEdit_IMAGE_DIRECTORY_SIZE,N_IMAGE_DIRECORIES::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
    _blockSignals((QObject **)pushButton,__PB_size,bState);
    _blockSignals((QObject **)dateTimeEdit,__TD_size,bState);
}

void PEWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous)

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
                case N_IMAGE_DOS_HEADER::e_magic:   lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_magic]->setValue((quint16)nValue);      break;
            }
            break;

        case SPE::TYPE_IMAGE_NT_HEADERS:
            switch(nNdata)
            {
                case N_IMAGE_NT_HEADERS::Signature: lineEdit_IMAGE_NT_HEADERS[N_IMAGE_NT_HEADERS::Signature]->setValue((quint32)nValue);    break;
            }
            break;

        case SPE::TYPE_IMAGE_FILE_HEADER:
            switch(nNdata)
            {
                case N_IMAGE_FILE_HEADER::Machine:          lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Machine]->setValue((quint16)nValue);            break;
                case N_IMAGE_FILE_HEADER::TimeDateStamp:    lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::TimeDateStamp]->setValue((quint32)nValue);      break;
                case N_IMAGE_FILE_HEADER::Characteristics:  lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Characteristics]->setValue((quint16)nValue);    break;
            }
            break;

        case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
            switch(nNdata)
            {
                case N_IMAGE_OPTIONAL_HEADER::Magic:                lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Magic]->setValue((quint16)nValue);              break;
                case N_IMAGE_OPTIONAL_HEADER::Subsystem:            lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Subsystem]->setValue((quint16)nValue);          break;
                case N_IMAGE_OPTIONAL_HEADER::DllCharacteristics:   lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::DllCharacteristics]->setValue((quint16)nValue); break;
            }
            break;

        case SPE::TYPE_EXPORT:
            switch(nNdata)
            {
                case N_IMAGE_EXPORT::TimeDateStamp:         dateTimeEdit[TD_IMAGE_EXPORT_TimeDateStamp]->setValue(nValue);      break;
            }
            break;

        case SPE::TYPE_NETHEADER:
            switch(nNdata)
            {
                case N_IMAGE_NETHEADER::Flags:          lineEdit_NetHeader[N_IMAGE_NETHEADER::Flags]->setValue((quint32)nValue);    break;
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
    showSectionHeader(SPE::TYPE_IMAGE_SECTION_HEADER,ui->tableView_Sections);
}

void PEWidget::sectionHex()
{
    showSectionHex(ui->tableView_Sections);
}

void PEWidget::reloadData()
{
    int nType=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_TYPE).toInt();
    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(pe.isValid())
    {
        if(nType==SPE::TYPE_HEX)
        {
            if(!bInit[nType])
            {
                ui->widgetHex->setData(getDevice());
                ui->widgetHex->setBackupFileName(getOptions()->sBackupFileName);
                ui->widgetHex->enableReadOnly(false);
                connect(ui->widgetHex,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

                bInit[nType]=true;
            }
            ui->widgetHex->reload();
        }
        else if(nType==SPE::TYPE_STRINGS)
        {
            if(!bInit[nType])
            {
                ui->widgetStrings->setData(getDevice(),0,true);

                bInit[nType]=true;
            }
            ui->widgetHex->reload();
        }
        else if(nType==SPE::TYPE_MEMORYMAP)
        {
            if(!bInit[nType])
            {
                ui->widgetMemoryMap->setData(getDevice());

                bInit[nType]=true;
            }
            ui->widgetHex->reload();
        }
        else if(nType==SPE::TYPE_ENTROPY)
        {
            if(!bInit[nType])
            {
                ui->widgetEntropy->setData(getDevice(),0,getDevice()->size(),true);

                bInit[nType]=true;
            }
        }
        else if(nType==SPE::TYPE_IMAGE_DOS_HEADER)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createHeaderTable(SPE::TYPE_IMAGE_DOS_HEADER,ui->tableWidget_IMAGE_DOS_HEADER,N_IMAGE_DOS_HEADER::records,lineEdit_IMAGE_DOS_HEADER,N_IMAGE_DOS_HEADER::__data_size,0);
                comboBox[CB_IMAGE_DOS_HEADER_e_magic]=createComboBox(ui->tableWidget_IMAGE_DOS_HEADER,XPE::getImageMagicsS(),SPE::TYPE_IMAGE_DOS_HEADER,N_IMAGE_DOS_HEADER::e_magic,XComboBoxEx::CBTYPE_NORMAL);
                invWidget[INV_IMAGE_DOS_HEADER_e_lfanew]=createInvWidget(ui->tableWidget_IMAGE_DOS_HEADER,SPE::TYPE_IMAGE_DOS_HEADER,N_IMAGE_DOS_HEADER::e_lfanew,InvWidget::TYPE_HEX);
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

            invWidget[INV_IMAGE_DOS_HEADER_e_lfanew]->setOffsetAndSize(&pe,msdosheaderex.e_lfanew,0);

            qint64 nOffset=pe.getDosHeaderExOffset();
            qint64 nSize=pe.getDosHeaderExSize();
            qint64 nAddress=pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SPE::TYPE_IMAGE_DOS_HEADER],ui->widgetHex_IMAGE_DOS_HEADER);

            blockSignals(false);
        }
        else if(nType==SPE::TYPE_IMAGE_NT_HEADERS)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createHeaderTable(SPE::TYPE_IMAGE_NT_HEADERS,ui->tableWidget_IMAGE_NT_HEADERS,N_IMAGE_NT_HEADERS::records,lineEdit_IMAGE_NT_HEADERS,N_IMAGE_NT_HEADERS::__data_size,0);
                comboBox[CB_IMAGE_NT_HEADERS_Signature]=createComboBox(ui->tableWidget_IMAGE_NT_HEADERS,XPE::getImageNtHeadersSignaturesS(),SPE::TYPE_IMAGE_NT_HEADERS,N_IMAGE_NT_HEADERS::Signature,XComboBoxEx::CBTYPE_NORMAL);
            }

            blockSignals(true);

            quint32 nSignature=pe.getNtHeaders_Signature();

            lineEdit_IMAGE_NT_HEADERS[N_IMAGE_NT_HEADERS::Signature]->setValue(nSignature);
            comboBox[CB_IMAGE_NT_HEADERS_Signature]->setValue(nSignature);

            qint64 nOffset=pe.getNtHeadersOffset();
            qint64 nSize=4;
            qint64 nAddress=pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SPE::TYPE_IMAGE_NT_HEADERS],ui->widgetHex_IMAGE_NT_HEADERS);

            blockSignals(false);
        }
        else if(nType==SPE::TYPE_IMAGE_FILE_HEADER)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createHeaderTable(SPE::TYPE_IMAGE_FILE_HEADER,ui->tableWidget_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::records,lineEdit_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::__data_size,0);
                comboBox[CB_IMAGE_FILE_HEADER_Machine]=createComboBox(ui->tableWidget_IMAGE_FILE_HEADER,XPE::getImageFileHeaderMachinesS(),SPE::TYPE_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::Machine,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_IMAGE_FILE_HEADER_Characteristics]=createComboBox(ui->tableWidget_IMAGE_FILE_HEADER,XPE::getImageFileHeaderCharacteristicsS(),SPE::TYPE_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::Characteristics,XComboBoxEx::CBTYPE_FLAGS);

                dateTimeEdit[TD_IMAGE_FILE_HEADER_TimeDateStamp]=createTimeDateEdit(ui->tableWidget_IMAGE_FILE_HEADER,SPE::TYPE_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::TimeDateStamp,XDateTimeEditX::DT_TYPE_POSIX);
                invWidget[INV_IMAGE_FILE_HEADER_PointerToSymbolTable]=createInvWidget(ui->tableWidget_IMAGE_FILE_HEADER,SPE::TYPE_IMAGE_FILE_HEADER,N_IMAGE_FILE_HEADER::PointerToSymbolTable,InvWidget::TYPE_HEX);
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
            dateTimeEdit[TD_IMAGE_FILE_HEADER_TimeDateStamp]->setValue(fileheader.TimeDateStamp);

            invWidget[INV_IMAGE_FILE_HEADER_PointerToSymbolTable]->setAddressAndSize(&pe,pe.getBaseAddress()+fileheader.PointerToSymbolTable,0);

            qint64 nOffset=pe.getFileHeaderOffset();
            qint64 nSize=pe.getFileHeaderSize();
            qint64 nAddress=pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SPE::TYPE_IMAGE_FILE_HEADER],ui->widgetHex_IMAGE_FILE_HEADER);

            blockSignals(false);
        }
        else if(nType==SPE::TYPE_IMAGE_OPTIONAL_HEADER)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createHeaderTable(SPE::TYPE_IMAGE_OPTIONAL_HEADER,ui->tableWidget_IMAGE_OPTIONAL_HEADER,pe.is64()?(N_IMAGE_OPTIONAL_HEADER::records64):(N_IMAGE_OPTIONAL_HEADER::records32),lineEdit_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::__data_size,0);
                comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic]=createComboBox(ui->tableWidget_IMAGE_OPTIONAL_HEADER,XPE::getImageOptionalHeaderMagicS(),SPE::TYPE_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::Magic,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem]=createComboBox(ui->tableWidget_IMAGE_OPTIONAL_HEADER,XPE::getImageOptionalHeaderSubsystemS(),SPE::TYPE_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::Subsystem,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics]=createComboBox(ui->tableWidget_IMAGE_OPTIONAL_HEADER,XPE::getImageOptionalHeaderDllCharacteristicsS(),SPE::TYPE_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::DllCharacteristics,XComboBoxEx::CBTYPE_FLAGS);

                invWidget[INV_IMAGE_OPTIONAL_HEADER_AddressOfEntryPoint]=createInvWidget(ui->tableWidget_IMAGE_OPTIONAL_HEADER,SPE::TYPE_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint,InvWidget::TYPE_HEX);
                invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfCode]=createInvWidget(ui->tableWidget_IMAGE_OPTIONAL_HEADER,SPE::TYPE_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::BaseOfCode,InvWidget::TYPE_HEX);

                if(!pe.is64())
                {
                    invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfData]=createInvWidget(ui->tableWidget_IMAGE_OPTIONAL_HEADER,SPE::TYPE_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::BaseOfData,InvWidget::TYPE_HEX);
                }

                pushButton[PB_CalculateChecksum]=createPushButton(ui->tableWidget_IMAGE_OPTIONAL_HEADER,SPE::TYPE_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::CheckSum,tr("Calculate"));
            }

            blockSignals(true);

            XBinary::_MEMORY_MAP memoryMap=pe.getMemoryMap();

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

                addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress(&memoryMap,oh64.AddressOfEntryPoint));
                addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::BaseOfCode,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress(&memoryMap,oh64.BaseOfCode));

                invWidget[INV_IMAGE_OPTIONAL_HEADER_AddressOfEntryPoint]->setAddressAndSize(&pe,pe.getBaseAddress()+oh64.AddressOfEntryPoint,0);
                invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfCode]->setAddressAndSize(&pe,pe.getBaseAddress()+oh64.BaseOfCode,0);
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

                addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress(&memoryMap,oh32.AddressOfEntryPoint));
                addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::BaseOfCode,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress(&memoryMap,oh32.BaseOfCode));
                addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER,N_IMAGE_OPTIONAL_HEADER::BaseOfData,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress(&memoryMap,oh32.BaseOfData));

                invWidget[INV_IMAGE_OPTIONAL_HEADER_AddressOfEntryPoint]->setAddressAndSize(&pe,pe.getBaseAddress()+oh32.AddressOfEntryPoint,0);
                invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfCode]->setAddressAndSize(&pe,pe.getBaseAddress()+oh32.BaseOfCode,0);
                invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfData]->setAddressAndSize(&pe,pe.getBaseAddress()+oh32.BaseOfData,0);
            }

            qint64 nOffset=pe.getOptionalHeaderOffset();
            qint64 nSize=pe.getOptionalHeaderSize();
            qint64 nAddress=pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SPE::TYPE_IMAGE_OPTIONAL_HEADER],ui->widgetHex_IMAGE_OPTIONAL_HEADER);

            blockSignals(false);
        }
        else if(nType==SPE::TYPE_IMAGE_DIRECTORY_ENTRIES)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createSectionTable(SPE::TYPE_IMAGE_DIRECTORY_ENTRIES,ui->tableWidget_IMAGE_DIRECTORY_ENTRIES,N_IMAGE_DIRECORIES::records,N_IMAGE_DIRECORIES::__data_size);
            }

            blockSignals(true);

            qint32 nNumberOfRvaAndSizes=(qint32)pe.getOptionalHeader_NumberOfRvaAndSizes();

            ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setRowCount(16);

            XBinary::_MEMORY_MAP memoryMap=pe.getMemoryMap();
            QMap<quint64,QString> mapDD=XPE::getImageOptionalHeaderDataDirectoryS();

            for(int i=0; i<16; i++)
            {
                ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i,4,new QTableWidgetItem()); // Comment

                XPE_DEF::IMAGE_DATA_DIRECTORY dd=pe.getOptionalHeader_DataDirectory((quint32)i);

                QTableWidgetItem *itemNumber=new QTableWidgetItem(QString::number(i));

                itemNumber->setData(Qt::UserRole+FW_DEF::SECTION_DATA_SIZE,dd.Size);

                if(i!=XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_SECURITY)
                {
                    itemNumber->setData(Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS,dd.VirtualAddress);
                    itemNumber->setData(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,pe.addressToOffset(&memoryMap,memoryMap.nBaseAddress+dd.VirtualAddress));

                    if((dd.VirtualAddress)&&(pe.isRelAddressValid(&memoryMap,dd.VirtualAddress)))
                    {
                        addComment(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES,i,4,pe.getMemoryRecordInfoByRelAddress(&memoryMap,dd.VirtualAddress));
                    }
                }
                else
                {
                    itemNumber->setData(Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS,dd.VirtualAddress);
                    itemNumber->setData(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,dd.VirtualAddress);

                    if((dd.VirtualAddress)&&(pe.isOffsetValid(&memoryMap,dd.VirtualAddress)))
                    {
                        addComment(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES,i,4,pe.getMemoryRecordInfoByOffset(&memoryMap,dd.VirtualAddress));
                    }
                }

                ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i,0,itemNumber);

                ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i,1,new QTableWidgetItem(mapDD.value(i)));
                ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i,2,new QTableWidgetItem(XBinary::valueToHex(dd.VirtualAddress)));

                QTableWidgetItem *pItem=new QTableWidgetItem(XBinary::valueToHex(dd.Size));
                ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i,3,pItem);

                setItemEnable(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(i,0),i<nNumberOfRvaAndSizes);
                setItemEnable(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(i,1),i<nNumberOfRvaAndSizes);
                setItemEnable(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(i,2),i<nNumberOfRvaAndSizes);
                setItemEnable(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(i,3),i<nNumberOfRvaAndSizes);

                // TODO !!!
//                ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i,3,new QTableWidgetItem(XBinary::valueToHex(dd.Size)));
//
//                lineEdit_IMAGE_DIRECTORY_ADDRESS[i]->setValue(dd.VirtualAddress);
//                lineEdit_IMAGE_DIRECTORY_SIZE[i]->setValue(dd.Size);

//                lineEdit_IMAGE_DIRECTORY_ADDRESS[i]->setEnabled(i<(int)nNumberOfRvaAndSizes);
//                lineEdit_IMAGE_DIRECTORY_SIZE[i]->setEnabled(i<(int)nNumberOfRvaAndSizes);
            }

            if(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->currentRow()==0)
            {
                loadDirectory(0);
            }
            else
            {
                ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->selectRow(0);
            }

            blockSignals(false);
        }
        else if(nType==SPE::TYPE_SECTIONS)
        {
            PEProcessData peProcessData(SPE::TYPE_SECTIONS,&tvModel[SPE::TYPE_SECTIONS],&pe,0,0,0);

            ajustTableView(&peProcessData,&tvModel[SPE::TYPE_SECTIONS],ui->tableView_Sections);

            if(tvModel[SPE::TYPE_SECTIONS]->rowCount())
            {
                ui->tableView_Sections->setCurrentIndex(ui->tableView_Sections->model()->index(0,0));
            }
        }
        else if(nType==SPE::TYPE_EXPORT)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createHeaderTable(SPE::TYPE_EXPORT,ui->tableWidget_ExportHeader,N_IMAGE_EXPORT::records,lineEdit_EXPORT,N_IMAGE_EXPORT::__data_size,0);
                createSectionTable(SPE::TYPE_EXPORT_FUNCTION,ui->tableWidget_ExportFunctions,N_IMAGE_EXPORT_FUNCTION::records,N_IMAGE_EXPORT_FUNCTION::__data_size);

                invWidget[INV_IMAGE_EXPORT_Name]=createInvWidget(ui->tableWidget_ExportHeader,SPE::TYPE_EXPORT,N_IMAGE_EXPORT::Name,InvWidget::TYPE_HEX);
                invWidget[INV_IMAGE_EXPORT_AddressOfFunctions]=createInvWidget(ui->tableWidget_ExportHeader,SPE::TYPE_EXPORT,N_IMAGE_EXPORT::AddressOfFunctions,InvWidget::TYPE_HEX);
                invWidget[INV_IMAGE_EXPORT_AddressOfNameOrdinals]=createInvWidget(ui->tableWidget_ExportHeader,SPE::TYPE_EXPORT,N_IMAGE_EXPORT::AddressOfNameOrdinals,InvWidget::TYPE_HEX);
                invWidget[INV_IMAGE_EXPORT_AddressOfNames]=createInvWidget(ui->tableWidget_ExportHeader,SPE::TYPE_EXPORT,N_IMAGE_EXPORT::AddressOfNames,InvWidget::TYPE_HEX);

                dateTimeEdit[TD_IMAGE_EXPORT_TimeDateStamp]=createTimeDateEdit(ui->tableWidget_ExportHeader,SPE::TYPE_EXPORT,N_IMAGE_EXPORT::TimeDateStamp,XDateTimeEditX::DT_TYPE_POSIX);
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

            dateTimeEdit[TD_IMAGE_EXPORT_TimeDateStamp]->setValue(eh.directory.TimeDateStamp);

            invWidget[INV_IMAGE_EXPORT_Name]->setAddressAndSize(&pe,pe.getBaseAddress()+eh.directory.Name,0);
            invWidget[INV_IMAGE_EXPORT_AddressOfFunctions]->setAddressAndSize(&pe,pe.getBaseAddress()+eh.directory.AddressOfFunctions,0);
            invWidget[INV_IMAGE_EXPORT_AddressOfNameOrdinals]->setAddressAndSize(&pe,pe.getBaseAddress()+eh.directory.AddressOfNameOrdinals,0);
            invWidget[INV_IMAGE_EXPORT_AddressOfNames]->setAddressAndSize(&pe,pe.getBaseAddress()+eh.directory.AddressOfNames,0);

            XBinary::_MEMORY_MAP memoryMap=pe.getMemoryMap();

            addComment(ui->tableWidget_ExportHeader,N_IMAGE_EXPORT::Name,HEADER_COLUMN_COMMENT,pe.read_ansiString(pe.relAddressToOffset(&memoryMap,eh.directory.Name)));

            addComment(ui->tableWidget_ExportHeader,N_IMAGE_EXPORT::AddressOfFunctions,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress(&memoryMap,eh.directory.AddressOfFunctions));
            addComment(ui->tableWidget_ExportHeader,N_IMAGE_EXPORT::AddressOfNames,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress(&memoryMap,eh.directory.AddressOfNames));
            addComment(ui->tableWidget_ExportHeader,N_IMAGE_EXPORT::AddressOfNameOrdinals,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByRelAddress(&memoryMap,eh.directory.AddressOfNameOrdinals));

            int nCount=eh.listPositions.count();
            ui->tableWidget_ExportFunctions->setRowCount(nCount);

            // TODO Sort
            for(int i=0; i<nCount; i++)
            {
                ui->tableWidget_ExportFunctions->setItem(i,N_IMAGE_EXPORT_FUNCTION::Ordinal,                new QTableWidgetItem(XBinary::valueToHex(eh.listPositions.at(i).nOrdinal)));
                ui->tableWidget_ExportFunctions->setItem(i,N_IMAGE_EXPORT_FUNCTION::RVA,                    new QTableWidgetItem(XBinary::valueToHex(eh.listPositions.at(i).nRVA)));
                ui->tableWidget_ExportFunctions->setItem(i,N_IMAGE_EXPORT_FUNCTION::Name,                   new QTableWidgetItem(XBinary::valueToHex(eh.listPositions.at(i).nNameRVA)));
                ui->tableWidget_ExportFunctions->setItem(i,N_IMAGE_EXPORT_FUNCTION::Name+1,                 new QTableWidgetItem(eh.listPositions.at(i).sFunctionName));
            }

            blockSignals(false);
        }
        else if(nType==SPE::TYPE_IMPORT)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createSectionTable(SPE::TYPE_IMPORT,ui->tableWidget_ImportLibraries,N_IMAGE_IMPORT::records,N_IMAGE_IMPORT::__data_size);
//                createSectionTable(SPE::TYPE_IMPORT_FUNCTION,ui->tableWidget_ImportFunctions,pe.is64()?(N_IMAGE_IMPORT_FUNCTION::records64):(N_IMAGE_IMPORT_FUNCTION::records32),N_IMAGE_IMPORT_FUNCTION::__data_size);
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

//            ui->tableWidget_ImportFunctions->setRowCount(0);

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
        else if(nType==SPE::TYPE_RESOURCE)
        {
            ui->treeWidgetResources->clear();

            XPE::RESOURCE_HEADER rh=pe.getResourceHeader();

            int nHeaderCount=rh.listPositions.count();

            if(nHeaderCount)
            {
                QTreeWidgetItem *pRoot=new QTreeWidgetItem(ui->treeWidgetResources);
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

                            pRecord->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE,record.data_entry.Size);
                            pRecord->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS,record.data_entry.OffsetToData);

                            if(getOptions()->bIsImage)
                            {
                                pRecord->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,record.data_entry.OffsetToData);
                            }
                            else
                            {
                                pRecord->setData(0,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,record.nDataOffset);
                            }
                        }
                    }
                }

                ui->treeWidgetResources->expandItem(pRoot);
            }
        }
        else if(nType==SPE::TYPE_RESOURCE_VERSION)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createHeaderTable(SPE::TYPE_RESOURCE_VERSION,ui->tableWidget_Resources_Version,N_IMAGE_RESOURCE_FIXEDFILEINFO::records,lineEdit_Version_FixedFileInfo,N_IMAGE_RESOURCE_FIXEDFILEINFO::__data_size,0);
            }

            blockSignals(true);

            XPE::RESOURCE_VERSION resourceVersion=pe.getResourceVersion();

            lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature]->setValue(resourceVersion.fileInfo.dwSignature);
            lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwStrucVersion]->setValue(resourceVersion.fileInfo.dwStrucVersion);
            lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionMS]->setValue(resourceVersion.fileInfo.dwFileVersionMS);
            lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionLS]->setValue(resourceVersion.fileInfo.dwFileVersionLS);
            lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionMS]->setValue(resourceVersion.fileInfo.dwProductVersionMS);
            lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionLS]->setValue(resourceVersion.fileInfo.dwProductVersionLS);
            lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlagsMask]->setValue(resourceVersion.fileInfo.dwFileFlagsMask);
            lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags]->setValue(resourceVersion.fileInfo.dwFileFlags);
            lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS]->setValue(resourceVersion.fileInfo.dwFileOS);
            lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType]->setValue(resourceVersion.fileInfo.dwFileType);
            lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileSubtype]->setValue(resourceVersion.fileInfo.dwFileSubtype);
            lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileDateMS]->setValue(resourceVersion.fileInfo.dwFileDateMS);
            lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileDateLS]->setValue(resourceVersion.fileInfo.dwFileDateLS);

            ui->textEditResources_Version->clear();

            int nCount=resourceVersion.listRecords.count();

            QString sVersion;

            for(int i=0;i<nCount;i++)
            {
                sVersion+=resourceVersion.listRecords.at(i)+QString("\r\n");
            }

            ui->textEditResources_Version->setText(sVersion);

            blockSignals(false);
        }
        else if(nType==SPE::TYPE_RESOURCE_MANIFEST)
        {
            ui->textEditResources_Manifest->clear();

            QString sManifest=pe.getResourceManifest();

            ui->textEditResources_Manifest->setText(sManifest);
        }
        else if(nType==SPE::TYPE_EXCEPTION)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createSectionTable(SPE::TYPE_EXCEPTION,ui->tableWidget_Exceptions,N_IMAGE_EXCEPTIONS::records,N_IMAGE_EXCEPTIONS::__data_size);
            }

            QList<XPE_DEF::S_IMAGE_RUNTIME_FUNCTION_ENTRY> listRFE=pe.getExceptionsList();
            int nCount=listRFE.count();
            ui->tableWidget_Exceptions->setRowCount(nCount);

            XBinary::_MEMORY_MAP memoryMap=pe.getMemoryMap();

            for(int i=0; i<nCount; i++)
            {
                QTableWidgetItem *pItem=new QTableWidgetItem(XBinary::valueToHex(listRFE.at(i).BeginAddress));
//                pItem->setData(Qt::UserRole,listRH.at(i).nOffset);

                pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS,listRFE.at(i).BeginAddress);
                pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_SIZE,listRFE.at(i).EndAddress-listRFE.at(i).BeginAddress);
                pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,pe.addressToOffset(&memoryMap,memoryMap.nBaseAddress+listRFE.at(i).BeginAddress));

                ui->tableWidget_Exceptions->setItem(i,N_IMAGE_EXCEPTIONS::BeginAddress,         pItem);
                ui->tableWidget_Exceptions->setItem(i,N_IMAGE_EXCEPTIONS::EndAddress,           new QTableWidgetItem(XBinary::valueToHex(listRFE.at(i).EndAddress)));
                ui->tableWidget_Exceptions->setItem(i,N_IMAGE_EXCEPTIONS::UnwindInfoAddress,    new QTableWidgetItem(XBinary::valueToHex(listRFE.at(i).UnwindInfoAddress)));
            }

            if(nCount)
            {
                if(ui->tableWidget_Exceptions->currentRow()==0)
                {
                    loadException(0);
                }
                else
                {
                    ui->tableWidget_Exceptions->setCurrentCell(0,0);
                }
            }
        }
        else if(nType==SPE::TYPE_RELOCS)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createSectionTable(SPE::TYPE_RELOCS,ui->tableWidget_Relocs,N_IMAGE_RELOCS::records,N_IMAGE_RELOCS::__data_size);
//                createSectionTable(SPE::TYPE_RELOCS_POSITION,ui->tableWidget_RelocsPositions,N_IMAGE_RELOCS_POSITION::records,N_IMAGE_RELOCS_POSITION::__data_size);
            }

            QList<XPE::RELOCS_HEADER> listRH=pe.getRelocsHeaders();
            int nCount=listRH.count();
            ui->tableWidget_Relocs->setRowCount(nCount);

            for(int i=0; i<nCount; i++)
            {
                QTableWidgetItem *pItem=new QTableWidgetItem(XBinary::valueToHex(listRH.at(i).ibr.VirtualAddress));
                pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,listRH.at(i).nOffset);
                ui->tableWidget_Relocs->setItem(i,N_IMAGE_RELOCS::VirtualAddress,               pItem);
                ui->tableWidget_Relocs->setItem(i,N_IMAGE_RELOCS::SizeOfBlock,                  new QTableWidgetItem(XBinary::valueToHex(listRH.at(i).ibr.SizeOfBlock)));
                ui->tableWidget_Relocs->setItem(i,N_IMAGE_RELOCS::SizeOfBlock+1,                new QTableWidgetItem(QString::number(listRH.at(i).nCount)));
                ui->tableWidget_Relocs->setItem(i,3,new QTableWidgetItem()); // Comment
                addComment(ui->tableWidget_Relocs,i,3,pe.getMemoryRecordInfoByRelAddress(listRH.at(i).ibr.VirtualAddress));
            }

//            ui->tableWidget_RelocsPositions->setRowCount(0);

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
        else if(nType==SPE::TYPE_DEBUG)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createSectionTable(SPE::TYPE_DEBUG,ui->tableWidget_Debug,N_IMAGE_DEBUG::records,N_IMAGE_DEBUG::__data_size);
            }

            QList<XPE_DEF::S_IMAGE_DEBUG_DIRECTORY> listDebug=pe.getDebugList();

            int nCount=listDebug.count();
            ui->tableWidget_Debug->setRowCount(nCount);

            for(int i=0; i<nCount; i++)
            {
                QTableWidgetItem *pItem=new QTableWidgetItem(XBinary::valueToHex(listDebug.at(i).Characteristics));
                pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS,listDebug.at(i).AddressOfRawData);
                pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_SIZE,listDebug.at(i).SizeOfData);
                pItem->setData(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET,listDebug.at(i).PointerToRawData);
                ui->tableWidget_Debug->setItem(i,N_IMAGE_DEBUG::Characteristics,                pItem);
                ui->tableWidget_Debug->setItem(i,N_IMAGE_DEBUG::TimeDateStamp,                  new QTableWidgetItem(XBinary::valueToHex(listDebug.at(i).TimeDateStamp)));
                ui->tableWidget_Debug->setItem(i,N_IMAGE_DEBUG::MajorVersion,                   new QTableWidgetItem(XBinary::valueToHex(listDebug.at(i).MajorVersion)));
                ui->tableWidget_Debug->setItem(i,N_IMAGE_DEBUG::MinorVersion,                   new QTableWidgetItem(XBinary::valueToHex(listDebug.at(i).MinorVersion)));
                ui->tableWidget_Debug->setItem(i,N_IMAGE_DEBUG::Type,                           new QTableWidgetItem(XBinary::valueToHex(listDebug.at(i).Type)));
                ui->tableWidget_Debug->setItem(i,N_IMAGE_DEBUG::SizeOfData,                     new QTableWidgetItem(XBinary::valueToHex(listDebug.at(i).SizeOfData)));
                ui->tableWidget_Debug->setItem(i,N_IMAGE_DEBUG::AddressOfRawData,               new QTableWidgetItem(XBinary::valueToHex(listDebug.at(i).AddressOfRawData)));
                ui->tableWidget_Debug->setItem(i,N_IMAGE_DEBUG::PointerToRawData,               new QTableWidgetItem(XBinary::valueToHex(listDebug.at(i).PointerToRawData)));
            }

            if(nCount)
            {
                if(ui->tableWidget_Debug->currentRow()==0)
                {
                    loadDebug(0);
                }
                else
                {
                    ui->tableWidget_Debug->setCurrentCell(0,0);
                }
            }
        }
        else if(nType==SPE::TYPE_TLS)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createHeaderTable(SPE::TYPE_TLS,ui->tableWidget_TLS,pe.is64()?(N_IMAGE_TLS::records64):(N_IMAGE_TLS::records32),lineEdit_TLS,N_IMAGE_TLS::__data_size,0);

                invWidget[INV_IMAGE_TLS_AddressOfCallBacks]=createInvWidget(ui->tableWidget_TLS,SPE::TYPE_TLS,N_IMAGE_TLS::AddressOfCallBacks,InvWidget::TYPE_HEX);
                invWidget[INV_IMAGE_TLS_AddressOfIndex]=createInvWidget(ui->tableWidget_TLS,SPE::TYPE_TLS,N_IMAGE_TLS::AddressOfIndex,InvWidget::TYPE_HEX);
                invWidget[INV_IMAGE_TLS_EndAddressOfRawData]=createInvWidget(ui->tableWidget_TLS,SPE::TYPE_TLS,N_IMAGE_TLS::EndAddressOfRawData,InvWidget::TYPE_HEX);
                invWidget[INV_IMAGE_TLS_StartAddressOfRawData]=createInvWidget(ui->tableWidget_TLS,SPE::TYPE_TLS,N_IMAGE_TLS::StartAddressOfRawData,InvWidget::TYPE_HEX);
            }

            blockSignals(true);

            XBinary::_MEMORY_MAP memoryMap=pe.getMemoryMap();

            if(pe.is64())
            {
                XPE_DEF::S_IMAGE_TLS_DIRECTORY64 tls64=pe.getTLSDirectory64();
                lineEdit_TLS[N_IMAGE_TLS::StartAddressOfRawData]->setValue(tls64.StartAddressOfRawData);
                lineEdit_TLS[N_IMAGE_TLS::EndAddressOfRawData]->setValue(tls64.EndAddressOfRawData);
                lineEdit_TLS[N_IMAGE_TLS::AddressOfIndex]->setValue(tls64.AddressOfIndex);
                lineEdit_TLS[N_IMAGE_TLS::AddressOfCallBacks]->setValue(tls64.AddressOfCallBacks);
                lineEdit_TLS[N_IMAGE_TLS::SizeOfZeroFill]->setValue(tls64.SizeOfZeroFill);
                lineEdit_TLS[N_IMAGE_TLS::Characteristics]->setValue(tls64.Characteristics);

                invWidget[INV_IMAGE_TLS_AddressOfCallBacks]->setAddressAndSize(&pe,tls64.AddressOfCallBacks,0);
                invWidget[INV_IMAGE_TLS_AddressOfIndex]->setAddressAndSize(&pe,tls64.AddressOfIndex,0);
                invWidget[INV_IMAGE_TLS_EndAddressOfRawData]->setAddressAndSize(&pe,tls64.EndAddressOfRawData,0);
                invWidget[INV_IMAGE_TLS_StartAddressOfRawData]->setAddressAndSize(&pe,tls64.StartAddressOfRawData,0);

                addComment(ui->tableWidget_TLS,N_IMAGE_TLS::AddressOfCallBacks,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByAddress(&memoryMap,tls64.AddressOfCallBacks));
                addComment(ui->tableWidget_TLS,N_IMAGE_TLS::AddressOfIndex,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByAddress(&memoryMap,tls64.AddressOfIndex));
                addComment(ui->tableWidget_TLS,N_IMAGE_TLS::EndAddressOfRawData,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByAddress(&memoryMap,tls64.EndAddressOfRawData));
                addComment(ui->tableWidget_TLS,N_IMAGE_TLS::StartAddressOfRawData,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByAddress(&memoryMap,tls64.StartAddressOfRawData));
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

                invWidget[INV_IMAGE_TLS_AddressOfCallBacks]->setAddressAndSize(&pe,tls32.AddressOfCallBacks,0);
                invWidget[INV_IMAGE_TLS_AddressOfIndex]->setAddressAndSize(&pe,tls32.AddressOfIndex,0);
                invWidget[INV_IMAGE_TLS_EndAddressOfRawData]->setAddressAndSize(&pe,tls32.EndAddressOfRawData,0);
                invWidget[INV_IMAGE_TLS_StartAddressOfRawData]->setAddressAndSize(&pe,tls32.StartAddressOfRawData,0);

                addComment(ui->tableWidget_TLS,N_IMAGE_TLS::AddressOfCallBacks,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByAddress(&memoryMap,tls32.AddressOfCallBacks));
                addComment(ui->tableWidget_TLS,N_IMAGE_TLS::AddressOfIndex,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByAddress(&memoryMap,tls32.AddressOfIndex));
                addComment(ui->tableWidget_TLS,N_IMAGE_TLS::EndAddressOfRawData,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByAddress(&memoryMap,tls32.EndAddressOfRawData));
                addComment(ui->tableWidget_TLS,N_IMAGE_TLS::StartAddressOfRawData,HEADER_COLUMN_COMMENT,pe.getMemoryRecordInfoByAddress(&memoryMap,tls32.StartAddressOfRawData));
            }

            qint64 nOffset=pe.getTLSHeaderOffset();
            qint64 nSize=pe.getTLSHeaderSize();
            qint64 nAddress=pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SPE::TYPE_TLS],ui->widgetHex_TLS);

            blockSignals(false);
        }
        else if(nType==SPE::TYPE_LOADCONFIG)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createHeaderTable(SPE::TYPE_LOADCONFIG,ui->tableWidget_LoadConfig,pe.is64()?(N_IMAGE_LOADCONFIG::records64):(N_IMAGE_LOADCONFIG::records32),lineEdit_LoadConfig,N_IMAGE_LOADCONFIG::__data_size,0);
                invWidget[INV_IMAGE_LOADCONFIG_SecurityCookie]=createInvWidget(ui->tableWidget_LoadConfig,SPE::TYPE_LOADCONFIG,N_IMAGE_LOADCONFIG::SecurityCookie,InvWidget::TYPE_HEX);
                invWidget[INV_IMAGE_LOADCONFIG_SEHandlerTable]=createInvWidget(ui->tableWidget_LoadConfig,SPE::TYPE_LOADCONFIG,N_IMAGE_LOADCONFIG::SEHandlerTable,InvWidget::TYPE_HEX);
            }

            blockSignals(true);

            if(pe.is64())
            {
                XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64 lc64=pe.getLoadConfigDirectory64();
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Size]->setValue(lc64.Size);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::TimeDateStamp]->setValue(lc64.TimeDateStamp);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MinorVersion]->setValue(lc64.MinorVersion);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MajorVersion]->setValue(lc64.MajorVersion);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GlobalFlagsClear]->setValue(lc64.GlobalFlagsClear);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GlobalFlagsSet]->setValue(lc64.GlobalFlagsSet);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CriticalSectionDefaultTimeout]->setValue(lc64.CriticalSectionDefaultTimeout);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DeCommitFreeBlockThreshold]->setValue(lc64.DeCommitFreeBlockThreshold);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DeCommitTotalFreeThreshold]->setValue(lc64.DeCommitTotalFreeThreshold);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::LockPrefixTable]->setValue(lc64.LockPrefixTable);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MaximumAllocationSize]->setValue(lc64.MaximumAllocationSize);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::VirtualMemoryThreshold]->setValue(lc64.VirtualMemoryThreshold);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::ProcessAffinityMask]->setValue(lc64.ProcessAffinityMask);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CSDVersion]->setValue(lc64.CSDVersion);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DependentLoadFlags]->setValue(lc64.DependentLoadFlags);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::EditList]->setValue(lc64.EditList);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SecurityCookie]->setValue(lc64.SecurityCookie);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SEHandlerTable]->setValue(lc64.SEHandlerTable);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SEHandlerCount]->setValue(lc64.SEHandlerCount);

                invWidget[INV_IMAGE_LOADCONFIG_SecurityCookie]->setAddressAndSize(&pe,lc64.SecurityCookie,0);
                invWidget[INV_IMAGE_LOADCONFIG_SEHandlerTable]->setAddressAndSize(&pe,lc64.SEHandlerTable,0);
            }
            else
            {
                XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32 lc32=pe.getLoadConfigDirectory32();
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Size]->setValue(lc32.Size);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::TimeDateStamp]->setValue(lc32.TimeDateStamp);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MinorVersion]->setValue(lc32.MinorVersion);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MajorVersion]->setValue(lc32.MajorVersion);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GlobalFlagsClear]->setValue(lc32.GlobalFlagsClear);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GlobalFlagsSet]->setValue(lc32.GlobalFlagsSet);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CriticalSectionDefaultTimeout]->setValue(lc32.CriticalSectionDefaultTimeout);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DeCommitFreeBlockThreshold]->setValue(lc32.DeCommitFreeBlockThreshold);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DeCommitTotalFreeThreshold]->setValue(lc32.DeCommitTotalFreeThreshold);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::LockPrefixTable]->setValue(lc32.LockPrefixTable);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MaximumAllocationSize]->setValue(lc32.MaximumAllocationSize);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::VirtualMemoryThreshold]->setValue(lc32.VirtualMemoryThreshold);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::ProcessAffinityMask]->setValue(lc32.ProcessAffinityMask);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CSDVersion]->setValue(lc32.CSDVersion);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DependentLoadFlags]->setValue(lc32.DependentLoadFlags);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::EditList]->setValue(lc32.EditList);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SecurityCookie]->setValue(lc32.SecurityCookie);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SEHandlerTable]->setValue(lc32.SEHandlerTable);
                lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SEHandlerCount]->setValue(lc32.SEHandlerCount);

                invWidget[INV_IMAGE_LOADCONFIG_SecurityCookie]->setAddressAndSize(&pe,lc32.SecurityCookie,0);
                invWidget[INV_IMAGE_LOADCONFIG_SEHandlerTable]->setAddressAndSize(&pe,lc32.SEHandlerTable,0);
            }

            qint64 nOffset=pe.getLoadConfigDirectoryOffset();
            qint64 nSize=pe.getLoadConfigDirectorySize();
            qint64 nAddress=pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SPE::TYPE_LOADCONFIG],ui->widgetHex_LoadConfig);

            blockSignals(false);
        }
        else if(nType==SPE::TYPE_BOUNDIMPORT)
        {
            // TODO
        }
        else if(nType==SPE::TYPE_DELAYIMPORT)
        {
            PEProcessData peProcessData(SPE::TYPE_DELAYIMPORT,&tvModel[SPE::TYPE_DELAYIMPORT],&pe,0,0,0);

            ajustTableView(&peProcessData,&tvModel[SPE::TYPE_DELAYIMPORT],ui->tableView_DelayImport);

            if(tvModel[SPE::TYPE_DELAYIMPORT]->rowCount())
            {
                ui->tableView_DelayImport->setCurrentIndex(ui->tableView_DelayImport->model()->index(0,0));
            }
        }
        else if(nType==SPE::TYPE_NETHEADER)
        {
            if(!bInit[nType])
            {
                bInit[nType]=createHeaderTable(SPE::TYPE_NETHEADER,ui->tableWidget_NetHeader,N_IMAGE_NETHEADER::records,lineEdit_NetHeader,N_IMAGE_NETHEADER::__data_size,0);

                comboBox[CB_IMAGE_NETHEADER_FLAGS]=createComboBox(ui->tableWidget_NetHeader,XPE::getComImageFlagsS(),SPE::TYPE_NETHEADER,N_IMAGE_NETHEADER::Flags,XComboBoxEx::CBTYPE_FLAGS);
            }

            blockSignals(true);

            XPE::NET_HEADER netHeader=pe.getNetHeader();

            lineEdit_NetHeader[N_IMAGE_NETHEADER::cb]->setValue(netHeader.cb);
            lineEdit_NetHeader[N_IMAGE_NETHEADER::MajorRuntimeVersion]->setValue(netHeader.MajorRuntimeVersion);
            lineEdit_NetHeader[N_IMAGE_NETHEADER::MinorRuntimeVersion]->setValue(netHeader.MinorRuntimeVersion);
            lineEdit_NetHeader[N_IMAGE_NETHEADER::MetaData_Address]->setValue(netHeader.MetaData_Address);
            lineEdit_NetHeader[N_IMAGE_NETHEADER::MetaData_Size]->setValue(netHeader.MetaData_Size);
            lineEdit_NetHeader[N_IMAGE_NETHEADER::Flags]->setValue(netHeader.Flags);
            lineEdit_NetHeader[N_IMAGE_NETHEADER::EntryPoint]->setValue(netHeader.EntryPoint);

            // TODO more

            comboBox[CB_IMAGE_NETHEADER_FLAGS]->setValue(netHeader.Flags);

            qint64 nOffset=pe.getNetHeaderOffset();
            qint64 nSize=pe.getNetHeaderSize();
            qint64 nAddress=pe.offsetToRelAddress(nOffset);

            loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SPE::TYPE_NETHEADER],ui->widgetHex_NetHeader);

            blockSignals(false);
        }
        else if(nType==SPE::TYPE_OVERLAY)
        {
            qint64 nOverLayOffset=pe.getOverlayOffset();
            qint64 nOverlaySize=pe.getOverlaySize();

            loadHexSubdevice(nOverLayOffset,nOverlaySize,nOverLayOffset,&subDevice[SPE::TYPE_OVERLAY],ui->widgetOverlayHex);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void PEWidget::on_tableView_Sections_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableView_Sections->currentIndex().row();

    if(nRow!=-1)
    {
        bool bIsEnable=getTableViewItemSize(ui->tableView_Sections,nRow);

        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editSectionHeader()));
        contextMenu.addAction(&actionEdit);

        QAction actionHex(tr("Hex"),this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(sectionHex()));
        actionHex.setEnabled(bIsEnable);
        contextMenu.addAction(&actionHex);

        // TODO Entropy

        contextMenu.exec(ui->tableView_Sections->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::loadImportLibrary(int nNumber)
{
    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(pe.isValid())
    {
        PEProcessData peProcessData(SPE::TYPE_IMPORT_FUNCTION,&tvModel[SPE::TYPE_IMPORT_FUNCTION],&pe,nNumber,0,0);

        ajustTableView(&peProcessData,&tvModel[SPE::TYPE_IMPORT_FUNCTION],ui->tableView_ImportFunctions);

        if(tvModel[SPE::TYPE_IMPORT_FUNCTION]->rowCount())
        {
            ui->tableView_ImportFunctions->setCurrentIndex(ui->tableView_ImportFunctions->model()->index(0,0));
        }
    }
}

void PEWidget::loadRelocs(int nNumber)
{
    qint64 nOffset=ui->tableWidget_Relocs->item(nNumber,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();

    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(pe.isValid())
    {
        PEProcessData peProcessData(SPE::TYPE_RELOCS_POSITION,&tvModel[SPE::TYPE_RELOCS_POSITION],&pe,0,nOffset,0);

        ajustTableView(&peProcessData,&tvModel[SPE::TYPE_RELOCS_POSITION],ui->tableView_RelocsPositions);

        if(tvModel[SPE::TYPE_RELOCS_POSITION]->rowCount())
        {
            ui->tableView_RelocsPositions->setCurrentIndex(ui->tableView_RelocsPositions->model()->index(0,0));
        }
    }
}

void PEWidget::loadSection(int nRow)
{
    if(nRow!=-1)
    {
        QModelIndex index=ui->tableView_Sections->model()->index(nRow,0);

        qint64 nOffset=ui->tableView_Sections->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize=ui->tableView_Sections->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
        qint64 nAddress=ui->tableView_Sections->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SPE::TYPE_SECTIONS],ui->widgetSectionHex);
    }
}

void PEWidget::loadException(int nRow)
{
    qint64 nOffset=ui->tableWidget_Exceptions->item(nRow,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nSize=ui->tableWidget_Exceptions->item(nRow,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
    qint64 nAddress=ui->tableWidget_Exceptions->item(nRow,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

    loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SPE::TYPE_EXCEPTION],ui->widgetExceptionHex);
}

void PEWidget::loadDirectory(int nNumber)
{
    qint64 nOffset=ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(nNumber,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nSize=ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(nNumber,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
    qint64 nAddress=ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(nNumber,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

    loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SPE::TYPE_IMAGE_DIRECTORY_ENTRIES],ui->widgetDirectoryHex);
}

void PEWidget::loadDebug(int nRow)
{
    qint64 nOffset=ui->tableWidget_Debug->item(nRow,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nSize=ui->tableWidget_Debug->item(nRow,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
    qint64 nAddress=ui->tableWidget_Debug->item(nRow,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

    loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SPE::TYPE_DEBUG],ui->widgetDebugHex);
}

void PEWidget::on_tableWidget_ImportLibraries_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    if(currentRow!=-1)
    {
        loadImportLibrary(currentRow);
    }
}

void PEWidget::on_tableWidget_Relocs_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    if(currentRow!=-1)
    {
        loadRelocs(currentRow);
    }
}

void PEWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*6);

    switch(type)
    {
        case SPE::TYPE_IMAGE_DOS_HEADER:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;

        case SPE::TYPE_IMAGE_NT_HEADERS:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;

        case SPE::TYPE_IMAGE_FILE_HEADER:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*15);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*24);
            break;

        case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*18);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*14);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*24);
            break;

        case SPE::TYPE_EXPORT:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*18);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*14);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*22);
            break;

        case SPE::TYPE_TLS:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*15);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*14);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*13);
            break;

        case SPE::TYPE_LOADCONFIG:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*18);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*14);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*13);
            break;

        case SPE::TYPE_NETHEADER:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*18);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*18);
            break;

        case SPE::TYPE_RESOURCE_VERSION:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*16);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*18);
            break;
    }
}

QString PEWidget::typeIdToString(int type)
{
    QString sResult="Unknown";

    switch(type)
    {
        case SPE::TYPE_IMAGE_SECTION_HEADER:    sResult=QString("Section header");      break; // TODO tr
        case SPE::TYPE_IMPORT:                  sResult=QString("Import header");       break; // TODO tr
        case SPE::TYPE_DEBUG:                   sResult=QString("Debug header");        break; // TODO tr
    }

    return sResult;
}

bool PEWidget::createSectionTable(int type, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, int nRecordCount)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

    QStringList slHeader;

    switch(type)
    {
        case SPE::TYPE_SECTIONS:
            pTableWidget->setColumnCount(nRecordCount+1);
            pTableWidget->setColumnWidth(0,nSymbolWidth*4);
            slHeader.append("");
            break;

        case SPE::TYPE_EXPORT_FUNCTION:
            pTableWidget->setColumnCount(nRecordCount+1); // TODO
            pTableWidget->setColumnWidth(nRecordCount,nSymbolWidth*40);
            break;

        case SPE::TYPE_IMPORT_FUNCTION:
            pTableWidget->setColumnCount(nRecordCount+1);
            pTableWidget->setColumnWidth(0,nSymbolWidth*12);
            pTableWidget->setColumnWidth(1,nSymbolWidth*8);
            pTableWidget->setColumnWidth(2,nSymbolWidth*6);
            pTableWidget->setColumnWidth(nRecordCount,nSymbolWidth*22);
            break;

        case SPE::TYPE_IMPORT:
            pTableWidget->setColumnCount(nRecordCount+1);
            pTableWidget->setColumnWidth(nRecordCount,nSymbolWidth*20);
            break;

        case SPE::TYPE_RELOCS:
            pTableWidget->setColumnCount(nRecordCount+2);
            break;

        case SPE::TYPE_DEBUG:
            pTableWidget->setColumnCount(nRecordCount); // TODO
            break;

        case SPE::TYPE_RELOCS_POSITION:
            pTableWidget->setColumnCount(nRecordCount+2);
            break;

        case SPE::TYPE_IMAGE_DIRECTORY_ENTRIES:
            pTableWidget->setColumnCount(nRecordCount+2);
            pTableWidget->setColumnWidth(0,nSymbolWidth*3);
            pTableWidget->setColumnWidth(1,nSymbolWidth*12);
            pTableWidget->setColumnWidth(2,nSymbolWidth*8);
            pTableWidget->setColumnWidth(3,nSymbolWidth*8);
            slHeader.append("");
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
            pTableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
            slHeader.append(tr("Count"));
            slHeader.append("");
            break;

        case SPE::TYPE_RELOCS_POSITION:
            slHeader.append(tr("Type"));
            slHeader.append(tr("Address"));
            break;

        case SPE::TYPE_IMAGE_DIRECTORY_ENTRIES:
            pTableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
            slHeader.append("");
            break;
    }

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    return true;
}

void PEWidget::on_tableWidget_Sections_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    if(currentRow!=-1)
    {
        loadSection(currentRow);
    }
}

void PEWidget::on_treeWidgetResources_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous)

    if(current)
    {
        // TODO more info in side widget
        if(current->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong())
        {
            qint64 nOffset=current->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
            qint64 nSize=current->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();
            qint64 nAddress=current->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

            loadHexSubdevice(nOffset,nSize,nAddress,&subDevice[SPE::TYPE_RESOURCE],ui->widgetResourcesHex);
        }
    }
}

void PEWidget::on_pushButtonReload_clicked()
{
    ui->pushButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000,this,SLOT(enableButton()));
}

void PEWidget::enableButton()
{
    ui->pushButtonReload->setEnabled(true);
}

void PEWidget::on_tableWidget_Exceptions_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    if(currentRow!=-1)
    {
        loadException(currentRow);
    }
}

void PEWidget::on_tableWidget_IMAGE_DIRECTORY_ENTRIES_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    if(currentRow!=-1)
    {
        loadDirectory(currentRow);
    }
}

void PEWidget::on_tableWidget_Debug_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    if(currentRow!=-1)
    {
        loadDebug(currentRow);
    }
}

void PEWidget::on_tableWidget_IMAGE_DOS_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_IMAGE_DOS_HEADER,ui->tableWidget_IMAGE_DOS_HEADER);
}

void PEWidget::on_tableWidget_IMAGE_NT_HEADERS_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_IMAGE_NT_HEADERS,ui->tableWidget_IMAGE_NT_HEADERS);
}

void PEWidget::on_tableWidget_IMAGE_FILE_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_IMAGE_FILE_HEADER,ui->tableWidget_IMAGE_FILE_HEADER);
}

void PEWidget::on_tableWidget_IMAGE_OPTIONAL_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_IMAGE_OPTIONAL_HEADER,ui->tableWidget_IMAGE_OPTIONAL_HEADER);
}

void PEWidget::on_tableWidget_LoadConfig_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_LoadConfig,ui->tableWidget_LoadConfig);
}

void PEWidget::on_tableWidget_NetHeader_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_NetHeader,ui->tableWidget_NetHeader);
}

void PEWidget::on_tableWidget_TLS_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex_TLS,ui->tableWidget_TLS);
}

void PEWidget::on_tableWidget_ImportLibraries_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableWidget_ImportLibraries->currentRow();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editImportHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableWidget_ImportLibraries->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::on_tableWidget_ImportFunctions_customContextMenuRequested(const QPoint &pos)
{
    // TODO
}

void PEWidget::editImportHeader()
{
    showSectionHeader(SPE::TYPE_IMPORT,ui->tableWidget_ImportLibraries);
}

void PEWidget::on_tableWidget_ExportFunctions_customContextMenuRequested(const QPoint &pos)
{
    // TODO
}

void PEWidget::on_tableWidget_Exceptions_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableWidget_Exceptions->currentRow();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editExceptionHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableWidget_Exceptions->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::on_tableWidget_Debug_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableWidget_Debug->currentRow();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editDebugHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableWidget_Debug->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::editDebugHeader()
{
    showSectionHeader(SPE::TYPE_DEBUG,ui->tableWidget_Debug);
}

void PEWidget::on_tableWidget_Relocs_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableWidget_Relocs->currentRow();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editRelocsHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableWidget_Relocs->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::editRelocsHeader()
{
    int nRow=ui->tableWidget_Relocs->currentRow();

    if(nRow!=-1)
    {
        qint64 nOffset=ui->tableWidget_Relocs->item(nRow,0)->data(Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();

        RelocsHeaderWidget *pRelocsHeaderWidget=new RelocsHeaderWidget(this);
        DialogSectionHeader dsh(this);
        dsh.setWidget(pRelocsHeaderWidget);
        dsh.setData(getDevice(),getOptions(),(quint32)nRow,nOffset,tr("Relocs Header"),0);
        dsh.setEdited(isEdited());

        connect(&dsh,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

        dsh.exec();

        delete pRelocsHeaderWidget;

        reloadData();

        ui->tableWidget_Relocs->setCurrentCell(nRow,0);
    }
}

void PEWidget::editExceptionHeader()
{
    // TODO
    qDebug("void PEWidget::editExceptionHeader()");
}

void PEWidget::on_tableWidget_Sections_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editSectionHeader();
}

void PEWidget::on_tableWidget_ImportLibraries_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editImportHeader();
}

void PEWidget::on_tableWidget_Debug_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editDebugHeader();
}

void PEWidget::on_tableWidget_Relocs_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editRelocsHeader();
}

void PEWidget::on_tableWidget_Exceptions_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editExceptionHeader();
}

void PEWidget::showSectionHeader(int type, QTableView *pTableView)
{
    int nRow=pTableView->currentIndex().row();

    if(nRow!=-1)
    {
        QModelIndex index=pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();

        SectionHeaderWidget *pSectionHeaderWidget=new SectionHeaderWidget(getDevice(),getOptions(),(quint32)nRow,nOffset,type,this);

        DialogSectionHeader dsh(this);
        dsh.setWidget(pSectionHeaderWidget);
        dsh.setData(typeIdToString(type));
        dsh.setEdited(isEdited());

        connect(&dsh,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

        dsh.exec();

        reloadData();

        pTableView->setCurrentIndex(pTableView->model()->index(nRow,0));
    }
}

void PEWidget::on_tableView_Sections_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editSectionHeader();
}
