/* Copyright (c) 2017-2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "pewidget.h"

#include "ui_pewidget.h"

PEWidget::PEWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::PEWidget)
{
    ui->setupUi(this);

    memset(g_subDevice, 0, sizeof g_subDevice);

    initWidget();
    initDisasmView(ui->widgetDisasm_DosStub);
    connect(ui->widgetTools, SIGNAL(dataChanged()), this, SLOT(allReload()));

    ui->groupBoxHash32->setTitle(QString("%1 32").arg(tr("Hash")));
    ui->groupBoxHash64->setTitle(QString("%1 64").arg(tr("Hash")));

    ui->checkBox_ManifestFormat->setChecked(false);

#if defined(_MSC_VER)
    ui->widgetCertificateCheck->show();
#else
    ui->widgetCertificateCheck->hide();
#endif
}

PEWidget::PEWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent) : PEWidget(pParent)
{
    PEWidget::setData(pDevice, options, 0, 0, 0);
    PEWidget::reload();
}

PEWidget::~PEWidget()
{
    delete ui;
}

void PEWidget::clear()
{
    PEWidget::reset();

    memset(g_lineEdit_IMAGE_DOS_HEADER, 0, sizeof g_lineEdit_IMAGE_DOS_HEADER);
    memset(g_lineEdit_IMAGE_NT_HEADERS, 0, sizeof g_lineEdit_IMAGE_NT_HEADERS);
    memset(g_lineEdit_IMAGE_FILE_HEADER, 0, sizeof g_lineEdit_IMAGE_FILE_HEADER);
    memset(g_lineEdit_IMAGE_OPTIONAL_HEADER, 0, sizeof g_lineEdit_IMAGE_OPTIONAL_HEADER);
    memset(g_lineEdit_TLS, 0, sizeof g_lineEdit_TLS);
    memset(g_lineEdit_LoadConfig, 0, sizeof g_lineEdit_LoadConfig);
    memset(g_lineEdit_Version_FixedFileInfo, 0, sizeof g_lineEdit_Version_FixedFileInfo);
    memset(g_lineEdit_NetHeader, 0, sizeof g_lineEdit_NetHeader);
    memset(g_lineEdit_Net_Metadata, 0, sizeof g_lineEdit_Net_Metadata);
    memset(g_lineEdit_EXPORT, 0, sizeof g_lineEdit_EXPORT);
    memset(g_lineEdit_Resources, 0, sizeof g_lineEdit_Resources);
    memset(g_comboBox, 0, sizeof g_comboBox);
    memset(g_pushButton, 0, sizeof g_pushButton);
    memset(g_dateTimeEdit, 0, sizeof g_dateTimeEdit);
    memset(g_invWidget, 0, sizeof g_invWidget);

    _deleteSubdevices(g_subDevice, (sizeof g_subDevice) / (sizeof(SubDevice *)));

    resetWidget();

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void PEWidget::cleanup()
{
    PEWidget::clear();
}

void PEWidget::reload()
{
    PEWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (pe.isValid()) {
        XBinary::_MEMORY_MAP memoryMap = pe.getMemoryMap();

        setFileType(pe.getFileType());

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_INFO, tr("Info")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_VIRUSTOTAL, "VirusTotal"));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_HEX, tr("Hex")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_DISASM, tr("Disasm")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_HASH, tr("Hash")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_STRINGS, tr("Strings")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_SIGNATURES, tr("Signatures")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_MEMORYMAP, tr("Memory map")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_ENTROPY, tr("Entropy")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_HEURISTICSCAN, tr("Heuristic scan")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_EXTRACTOR, tr("Extractor")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_TOOLS, tr("Tools")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_IMAGE_DOS_HEADER, "IMAGE_DOS_HEADER"));

        if (pe.isDosStubPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_DOS_STUB, "Dos stub"));
        }

        QTreeWidgetItem *pNtHeaders = createNewItem(SPE::TYPE_IMAGE_NT_HEADERS, "IMAGE_NT_HEADERS");
        ui->treeWidgetNavi->addTopLevelItem(pNtHeaders);
        pNtHeaders->addChild(createNewItem(SPE::TYPE_IMAGE_FILE_HEADER, "IMAGE_FILE_HEADER"));
        QTreeWidgetItem *pOptionalHeader = createNewItem(SPE::TYPE_IMAGE_OPTIONAL_HEADER, "IMAGE_OPTIONAL_HEADER");
        pNtHeaders->addChild(pOptionalHeader);
        pOptionalHeader->addChild(createNewItem(SPE::TYPE_IMAGE_DIRECTORY_ENTRIES, "IMAGE_DIRECTORY_ENTRIES"));

        if (pe.isRichSignaturePresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_RICH, QString("Rich %1").arg(tr("Signature"))));
        }

        if (pe.getFileHeader_NumberOfSections()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_SECTIONS, tr("Sections")));
        }

        if (pe.isExportPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_EXPORT, tr("Export")));
        }

        if (pe.isImportPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_IMPORT, tr("Import")));
        }

        if (pe.isResourcesPresent()) {
            QTreeWidgetItem *pResources = createNewItem(SPE::TYPE_RESOURCES, tr("Resources"));

            ui->treeWidgetNavi->addTopLevelItem(pResources);

            if (pe.isResourceStringTablePresent()) {
                pResources->addChild(createNewItem(SPE::TYPE_RESOURCES_STRINGTABLE, tr("String table")));
            }

            if (pe.isResourceVersionPresent()) {
                pResources->addChild(createNewItem(SPE::TYPE_RESOURCES_VERSION, tr("Version")));
            }

            if (pe.isResourceManifestPresent()) {
                pResources->addChild(createNewItem(SPE::TYPE_RESOURCES_MANIFEST, tr("Manifest")));
            }
        }

        if (pe.isExceptionPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_EXCEPTION, tr("Exceptions")));
        }

        if (pe.isSignPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_CERTIFICATE, tr("Certificate")));
        }

        if (pe.isRelocsPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_RELOCS, tr("Relocs")));
        }

        if (pe.isDebugPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_DEBUG, tr("Debug")));
        }

        if (pe.isTLSPresent()) {
            QTreeWidgetItem *pTLS = createNewItem(SPE::TYPE_TLS, QString("TLS"));

            ui->treeWidgetNavi->addTopLevelItem(pTLS);

            if (pe.isTLSCallbacksPresent(&memoryMap)) {
                pTLS->addChild(createNewItem(SPE::TYPE_TLSCALLBACKS, QString("TLS %1").arg(tr("Callbacks"))));
            }
        }

        if (pe.isLoadConfigPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_LOADCONFIG, tr("Load config")));
        }

        if (pe.isBoundImportPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_BOUNDIMPORT, tr("Bound import")));
        }

        if (pe.isDelayImportPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_DELAYIMPORT, tr("Delay import")));
        }

        if (pe.isNETPresent()) {
            QTreeWidgetItem *pNetHeader = createNewItem(SPE::TYPE_NETHEADER, QString(".NET"));
            ui->treeWidgetNavi->addTopLevelItem(pNetHeader);

            XPE::CLI_INFO cliInfo = pe.getCliInfo(true);

            if (pe.isNetMetadataPresent(&cliInfo, &memoryMap)) {
                QTreeWidgetItem *pNetMetadata = createNewItem(SPE::TYPE_NET_METADATA, tr("Metadata"));
                pNetHeader->addChild(pNetMetadata);

                qint32 nNumberOfStreams = cliInfo.metaData.listStreams.count();

                for (qint32 i = 0; i < nNumberOfStreams; i++) {
                    pNetMetadata->addChild(createNewItem(SPE::TYPE_NET_METADATA_STREAM, cliInfo.metaData.listStreams.at(i).sName, cliInfo.metaData.listStreams.at(i).nOffset,
                                                         cliInfo.metaData.listStreams.at(i).nSize));
                }
            }

            // TODO NET Resources
            // TODO more NET
        }

        if (pe.isOverlayPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_OVERLAY, tr("Overlay")));
        }

        ui->treeWidgetNavi->expandAll();

        setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);

        setDisasmInitAddress(pe.getEntryPointAddress());  // Optimize
    }
}

void PEWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->widgetTools->setGlobal(pShortcuts, pXOptions);
    FormatWidget::setGlobal(pShortcuts, pXOptions);
}

FormatWidget::SV PEWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)
    Q_UNUSED(nOffset)

    SV result = SV_NONE;

    blockSignals(true);

    quint64 nValue = vValue.toULongLong();
    QString sValue = vValue.toString();

    if (getDevice()->isWritable()) {
        XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

        if (pe.isValid()) {
            switch (nStype) {
                case SPE::TYPE_IMAGE_DOS_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_DOS_HEADER::e_magic:
                            g_comboBox[CB_IMAGE_DOS_HEADER_e_magic]->setValue(nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_lfanew:
                            g_invWidget[INV_IMAGE_DOS_HEADER_e_lfanew]->setOffsetAndSize(&pe, (quint32)nValue, 0);
                            break;
                    }
                    break;

                case SPE::TYPE_IMAGE_NT_HEADERS:
                    switch (nNdata) {
                        case N_IMAGE_NT_HEADERS::Signature:
                            g_comboBox[CB_IMAGE_NT_HEADERS_Signature]->setValue(nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_IMAGE_FILE_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_FILE_HEADER::Machine:
                            g_comboBox[CB_IMAGE_FILE_HEADER_Machine]->setValue(nValue);
                            break;
                        case N_IMAGE_FILE_HEADER::TimeDateStamp:
                            g_dateTimeEdit[TD_IMAGE_FILE_HEADER_TimeDateStamp]->setValue(nValue);
                            break;
                        case N_IMAGE_FILE_HEADER::PointerToSymbolTable:
                            g_invWidget[INV_IMAGE_FILE_HEADER_PointerToSymbolTable]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                        case N_IMAGE_FILE_HEADER::Characteristics:
                            g_comboBox[CB_IMAGE_FILE_HEADER_Characteristics]->setValue(nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_OPTIONAL_HEADER::Magic:
                            g_comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic]->setValue(nValue);
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint:
                            g_invWidget[INV_IMAGE_OPTIONAL_HEADER_AddressOfEntryPoint]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfCode:
                            g_invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfCode]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfData:
                            g_invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfData]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::Subsystem:
                            g_comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem]->setValue(nValue);
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::DllCharacteristics:
                            g_comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics]->setValue(nValue);
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion:
                        case N_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion:
                        case N_IMAGE_OPTIONAL_HEADER::OperatingSystemVersion:
                            quint32 nVersion = pe.getOperatingSystemVersion();
                            g_comboBox[CB_IMAGE_OPTIONAL_HEADER_OperationSystemVersion]->setValue(nVersion);
                            break;
                    }
                    break;

                case SPE::TYPE_EXPORT:
                    switch (nNdata) {
                        case N_IMAGE_EXPORT::TimeDateStamp:
                            g_dateTimeEdit[TD_IMAGE_EXPORT_TimeDateStamp]->setValue(nValue);
                            break;
                        case N_IMAGE_EXPORT::Name:
                            g_invWidget[INV_IMAGE_EXPORT_Name]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                        case N_IMAGE_EXPORT::AddressOfFunctions:
                            g_invWidget[INV_IMAGE_EXPORT_AddressOfFunctions]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                        case N_IMAGE_EXPORT::AddressOfNames:
                            g_invWidget[INV_IMAGE_EXPORT_AddressOfNames]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                        case N_IMAGE_EXPORT::AddressOfNameOrdinals:
                            g_invWidget[INV_IMAGE_EXPORT_AddressOfNameOrdinals]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                    }
                    break;

                case SPE::TYPE_TLS:
                    switch (nNdata) {
                        case N_IMAGE_TLS::StartAddressOfRawData:
                            g_invWidget[INV_IMAGE_TLS_StartAddressOfRawData]->setAddressAndSize(&pe, (quint64)nValue, 0);
                            break;
                        case N_IMAGE_TLS::EndAddressOfRawData:
                            g_invWidget[INV_IMAGE_TLS_EndAddressOfRawData]->setAddressAndSize(&pe, (quint64)nValue, 0);
                            break;
                        case N_IMAGE_TLS::AddressOfIndex:
                            g_invWidget[INV_IMAGE_TLS_AddressOfIndex]->setAddressAndSize(&pe, (quint64)nValue, 0);
                            break;
                        case N_IMAGE_TLS::AddressOfCallBacks:
                            g_invWidget[INV_IMAGE_TLS_AddressOfCallBacks]->setAddressAndSize(&pe, (quint64)nValue, 0);
                            break;
                    }
                    break;

                case SPE::TYPE_NETHEADER:
                    switch (nNdata) {
                        case N_IMAGE_NETHEADER::Flags:
                            g_comboBox[CB_IMAGE_NETHEADER_FLAGS]->setValue((quint32)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_RESOURCES_VERSION:
                    switch (nNdata) {
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature:
                            g_comboBox[CB_RESOURCES_VERSION_dwSignature]->setValue((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags:
                            g_comboBox[CB_RESOURCES_VERSION_dwFileFlags]->setValue((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS:
                            g_comboBox[CB_RESOURCES_VERSION_dwFileOS]->setValue((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType:
                            g_comboBox[CB_RESOURCES_VERSION_dwFileType]->setValue((quint32)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_LOADCONFIG:
                    switch (nNdata) {
                        case N_IMAGE_LOADCONFIG::SecurityCookie:
                            g_invWidget[INV_IMAGE_LOADCONFIG_SecurityCookie]->setAddressAndSize(&pe, (quint64)nValue, 0);
                            break;
                        case N_IMAGE_LOADCONFIG::SEHandlerTable:
                            g_invWidget[INV_IMAGE_LOADCONFIG_SEHandlerTable]->setAddressAndSize(&pe, (quint64)nValue, 0);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer:
                            g_invWidget[INV_IMAGE_LOADCONFIG_GuardCFCheckFunctionPointer]->setAddressAndSize(&pe, (quint64)nValue, 0);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer:
                            g_invWidget[INV_IMAGE_LOADCONFIG_GuardCFDispatchFunctionPointer]->setAddressAndSize(&pe, (quint64)nValue, 0);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer:
                            g_invWidget[INV_IMAGE_LOADCONFIG_GuardMemcpyFunctionPointer]->setAddressAndSize(&pe, (quint64)nValue, 0);
                            break;
                    }
                    break;
            }

            switch (nStype) {
                case SPE::TYPE_IMAGE_DOS_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_DOS_HEADER::e_magic:
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
                            pe.set_e_res(0, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_res_1:
                            pe.set_e_res(1, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_res_2:
                            pe.set_e_res(2, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_res_3:
                            pe.set_e_res(3, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_oemid:
                            pe.set_e_oemid((quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_oeminfo:
                            pe.set_e_oeminfo((quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_res2_0:
                            pe.set_e_res2(0, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_res2_1:
                            pe.set_e_res2(1, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_res2_2:
                            pe.set_e_res2(2, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_res2_3:
                            pe.set_e_res2(3, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_res2_4:
                            pe.set_e_res2(4, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_res2_5:
                            pe.set_e_res2(5, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_res2_6:
                            pe.set_e_res2(6, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_res2_7:
                            pe.set_e_res2(7, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_res2_8:
                            pe.set_e_res2(8, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_res2_9:
                            pe.set_e_res2(9, (quint16)nValue);
                            break;
                        case N_IMAGE_DOS_HEADER::e_lfanew:
                            pe.set_e_lfanew((quint32)nValue);
                            break;
                    }

                    ui->widgetHex_IMAGE_DOS_HEADER->reload();
                    break;

                case SPE::TYPE_IMAGE_NT_HEADERS:
                    switch (nNdata) {
                        case N_IMAGE_NT_HEADERS::Signature:
                            pe.setNtHeaders_Signature((quint32)nValue);
                            break;
                    }

                    ui->widgetHex_IMAGE_NT_HEADERS->reload();
                    break;

                case SPE::TYPE_IMAGE_FILE_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_FILE_HEADER::Machine:
                            pe.setFileHeader_Machine((quint16)nValue);
                            break;
                        case N_IMAGE_FILE_HEADER::NumberOfSections:
                            pe.setFileHeader_NumberOfSections((quint16)nValue);
                            break;
                        case N_IMAGE_FILE_HEADER::TimeDateStamp:
                            pe.setFileHeader_TimeDateStamp((quint32)nValue);
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
                            pe.setFileHeader_Characteristics((quint16)nValue);
                            break;
                    }

                    ui->widgetHex_IMAGE_FILE_HEADER->reload();
                    break;

                case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_OPTIONAL_HEADER::Magic:
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
                            pe.setOptionalHeader_Subsystem((quint16)nValue);
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::DllCharacteristics:
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
                        // Extra
                        case N_IMAGE_OPTIONAL_HEADER::OperatingSystemVersion:
                            pe.setOperatingSystemVersion((quint32)nValue);
                            break;
                    }

                    ui->widgetHex_IMAGE_OPTIONAL_HEADER->reload();
                    break;

                case SPE::TYPE_EXPORT:
                    switch (nNdata) {
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
                    switch (nNdata) {
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

                    ui->widgetHex_TLS->reload();
                    break;

                case SPE::TYPE_RESOURCES_VERSION:
                    switch (nNdata) {
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature:
                            pe.setFixedFileInfo_dwSignature((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwStrucVersion:
                            pe.setFixedFileInfo_dwStrucVersion((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionMS:
                            pe.setFixedFileInfo_dwFileVersionMS((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionLS:
                            pe.setFixedFileInfo_dwFileVersionLS((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionMS:
                            pe.setFixedFileInfo_dwProductVersionMS((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionLS:
                            pe.setFixedFileInfo_dwProductVersionLS((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlagsMask:
                            pe.setFixedFileInfo_dwFileFlagsMask((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags:
                            pe.setFixedFileInfo_dwFileFlags((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS:
                            pe.setFixedFileInfo_dwFileOS((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType:
                            pe.setFixedFileInfo_dwFileType((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileSubtype:
                            pe.setFixedFileInfo_dwFileSubtype((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileDateMS:
                            pe.setFixedFileInfo_dwFileDateMS((quint32)nValue);
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileDateLS:
                            pe.setFixedFileInfo_dwFileDateLS((quint32)nValue);
                            break;
                    }
                    break;

                case SPE::TYPE_NETHEADER:
                    switch (nNdata) {
                        case N_IMAGE_NETHEADER::cb:
                            pe.setNetHeader_cb((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::MinorRuntimeVersion:
                            pe.setNetHeader_MinorRuntimeVersion((quint16)nValue);
                            break;
                        case N_IMAGE_NETHEADER::MajorRuntimeVersion:
                            pe.setNetHeader_MajorRuntimeVersion((quint16)nValue);
                            break;
                        case N_IMAGE_NETHEADER::MetaData_Address:
                            pe.setNetHeader_MetaData_Address((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::MetaData_Size:
                            pe.setNetHeader_MetaData_Size((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::Flags:
                            pe.setNetHeader_Flags((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::EntryPoint:
                            pe.setNetHeader_EntryPoint((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::Resources_Address:
                            pe.setNetHeader_Resources_Address((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::Resources_Size:
                            pe.setNetHeader_Resources_Size((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::StrongNameSignature_Address:
                            pe.setNetHeader_StrongNameSignature_Address((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::StrongNameSignature_Size:
                            pe.setNetHeader_StrongNameSignature_Size((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::CodeManagerTable_Address:
                            pe.setNetHeader_CodeManagerTable_Address((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::CodeManagerTable_Size:
                            pe.setNetHeader_CodeManagerTable_Size((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::VTableFixups_Address:
                            pe.setNetHeader_VTableFixups_Address((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::VTableFixups_Size:
                            pe.setNetHeader_VTableFixups_Size((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::ExportAddressTableJumps_Address:
                            pe.setNetHeader_ExportAddressTableJumps_Address((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::ExportAddressTableJumps_Size:
                            pe.setNetHeader_ExportAddressTableJumps_Size((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::ManagedNativeHeader_Address:
                            pe.setNetHeader_ManagedNativeHeader_Address((quint32)nValue);
                            break;
                        case N_IMAGE_NETHEADER::ManagedNativeHeader_Size:
                            pe.setNetHeader_ManagedNativeHeader_Size((quint32)nValue);
                            break;
                    }

                    ui->widgetHex_NetHeader->reload();
                    break;

                case SPE::TYPE_NET_METADATA:
                    switch (nNdata) {
                        case N_IMAGE_NET_METADATA::Signature:
                            pe.setMetadataHeader_Signature((quint32)nValue);
                            break;
                        case N_IMAGE_NET_METADATA::MajorVersion:
                            pe.setMetadataHeader_MajorVersion((quint16)nValue);
                            break;
                        case N_IMAGE_NET_METADATA::MinorVersion:
                            pe.setMetadataHeader_MinorVersion((quint16)nValue);
                            break;
                        case N_IMAGE_NET_METADATA::Reserved:
                            pe.setMetadataHeader_Reserved((quint32)nValue);
                            break;
                        case N_IMAGE_NET_METADATA::VersionStringLength:
                            pe.setMetadataHeader_VersionStringLength((quint32)nValue);
                            break;  // TODO reload!!!
                        case N_IMAGE_NET_METADATA::Version:
                            pe.setMetadataHeader_Version(sValue);
                            break;
                        case N_IMAGE_NET_METADATA::Flags:
                            pe.setMetadataHeader_Flags((quint16)nValue);
                            break;
                        case N_IMAGE_NET_METADATA::Streams:
                            pe.setMetadataHeader_Streams((quint16)nValue);
                            break;
                    }

                    ui->widgetHex_Net_Metadata->reload();
                    break;

                case SPE::TYPE_LOADCONFIG:
                    switch (nNdata) {
                        case N_IMAGE_LOADCONFIG::Size:
                            pe.setLoadConfig_Size((quint32)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::TimeDateStamp:
                            pe.setLoadConfig_TimeDateStamp((quint32)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::MinorVersion:
                            pe.setLoadConfig_MinorVersion((quint16)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::MajorVersion:
                            pe.setLoadConfig_MajorVersion((quint16)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GlobalFlagsClear:
                            pe.setLoadConfig_GlobalFlagsClear((quint32)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GlobalFlagsSet:
                            pe.setLoadConfig_GlobalFlagsSet((quint32)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::CriticalSectionDefaultTimeout:
                            pe.setLoadConfig_CriticalSectionDefaultTimeout((quint32)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::DeCommitFreeBlockThreshold:
                            pe.setLoadConfig_DeCommitFreeBlockThreshold((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::DeCommitTotalFreeThreshold:
                            pe.setLoadConfig_DeCommitTotalFreeThreshold((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::LockPrefixTable:
                            pe.setLoadConfig_LockPrefixTable((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::MaximumAllocationSize:
                            pe.setLoadConfig_MaximumAllocationSize((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::VirtualMemoryThreshold:
                            pe.setLoadConfig_VirtualMemoryThreshold((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::ProcessAffinityMask:
                            pe.setLoadConfig_ProcessAffinityMask((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::CSDVersion:
                            pe.setLoadConfig_CSDVersion((quint16)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::DependentLoadFlags:
                            pe.setLoadConfig_DependentLoadFlags((quint16)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::EditList:
                            pe.setLoadConfig_EditList((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::SecurityCookie:
                            pe.setLoadConfig_SecurityCookie((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::SEHandlerTable:
                            pe.setLoadConfig_SEHandlerTable((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::SEHandlerCount:
                            pe.setLoadConfig_SEHandlerCount((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer:
                            pe.setLoadConfig_GuardCFCheckFunctionPointer((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer:
                            pe.setLoadConfig_GuardCFDispatchFunctionPointer((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardCFFunctionTable:
                            pe.setLoadConfig_GuardCFFunctionTable((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardCFFunctionCount:
                            pe.setLoadConfig_GuardCFFunctionCount((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardFlags:
                            pe.setLoadConfig_GuardFlags((quint32)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::CodeIntegrity_Flags:
                            pe.setLoadConfig_CodeIntegrity_Flags((quint16)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::CodeIntegrity_Catalog:
                            pe.setLoadConfig_CodeIntegrity_Catalog((quint16)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::CodeIntegrity_CatalogOffset:
                            pe.setLoadConfig_CodeIntegrity_CatalogOffset((quint32)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::CodeIntegrity_Reserved:
                            pe.setLoadConfig_CodeIntegrity_Reserved((quint32)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryTable:
                            pe.setLoadConfig_GuardAddressTakenIatEntryTable((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryCount:
                            pe.setLoadConfig_GuardAddressTakenIatEntryCount((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardLongJumpTargetTable:
                            pe.setLoadConfig_GuardLongJumpTargetTable((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardLongJumpTargetCount:
                            pe.setLoadConfig_GuardLongJumpTargetCount((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::DynamicValueRelocTable:
                            pe.setLoadConfig_DynamicValueRelocTable((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::CHPEMetadataPointer:
                            pe.setLoadConfig_CHPEMetadataPointer((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardRFFailureRoutine:
                            pe.setLoadConfig_GuardRFFailureRoutine((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardRFFailureRoutineFunctionPointer:
                            pe.setLoadConfig_GuardRFFailureRoutineFunctionPointer((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::DynamicValueRelocTableOffset:
                            pe.setLoadConfig_DynamicValueRelocTableOffset((quint32)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::DynamicValueRelocTableSection:
                            pe.setLoadConfig_DynamicValueRelocTableSection((quint16)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::Reserved2:
                            pe.setLoadConfig_Reserved2((quint16)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardRFVerifyStackPointerFunctionPointer:
                            pe.setLoadConfig_GuardRFVerifyStackPointerFunctionPointer((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::HotPatchTableOffset:
                            pe.setLoadConfig_HotPatchTableOffset((quint32)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::Reserved3:
                            pe.setLoadConfig_Reserved3((quint32)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::EnclaveConfigurationPointer:
                            pe.setLoadConfig_EnclaveConfigurationPointer((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::VolatileMetadataPointer:
                            pe.setLoadConfig_VolatileMetadataPointer((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardEHContinuationTable:
                            pe.setLoadConfig_GuardEHContinuationTable((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardEHContinuationCount:
                            pe.setLoadConfig_GuardEHContinuationCount((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardXFGCheckFunctionPointer:
                            pe.setLoadConfig_GuardXFGCheckFunctionPointer((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardXFGDispatchFunctionPointer:
                            pe.setLoadConfig_GuardXFGDispatchFunctionPointer((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardXFGTableDispatchFunctionPointer:
                            pe.setLoadConfig_GuardXFGTableDispatchFunctionPointer((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::CastGuardOsDeterminedFailureMode:
                            pe.setLoadConfig_CastGuardOsDeterminedFailureMode((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer:
                            pe.setLoadConfig_GuardMemcpyFunctionPointer((quint64)nValue);
                            break;
                    }
                    ui->widgetHex_LoadConfig->reload();
                    break;
            }

            switch (nStype) {
                case SPE::TYPE_IMAGE_FILE_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_FILE_HEADER::SizeOfOptionalHeader:
                            addComment(ui->tableWidget_IMAGE_FILE_HEADER, N_IMAGE_FILE_HEADER::SizeOfOptionalHeader, HEADER_COLUMN_COMMENT,
                                       XBinary::bytesCountToString(nValue));
                            break;
                    }
                    break;

                case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfCode:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfCode, HEADER_COLUMN_COMMENT, XBinary::bytesCountToString(nValue));
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfInitializedData:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfInitializedData, HEADER_COLUMN_COMMENT,
                                       XBinary::bytesCountToString(nValue));
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfUninitializedData:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfUninitializedData, HEADER_COLUMN_COMMENT,
                                       XBinary::bytesCountToString(nValue));
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::SectionAlignment:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SectionAlignment, HEADER_COLUMN_COMMENT,
                                       XBinary::bytesCountToString(nValue));
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::FileAlignment:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::FileAlignment, HEADER_COLUMN_COMMENT,
                                       XBinary::bytesCountToString(nValue));
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfImage:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfImage, HEADER_COLUMN_COMMENT,
                                       XBinary::bytesCountToString(nValue));
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfHeaders:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfHeaders, HEADER_COLUMN_COMMENT,
                                       XBinary::bytesCountToString(nValue));
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfStackReserve:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfStackReserve, HEADER_COLUMN_COMMENT,
                                       XBinary::bytesCountToString(nValue));
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfStackCommit:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfStackCommit, HEADER_COLUMN_COMMENT,
                                       XBinary::bytesCountToString(nValue));
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfHeapReserve:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfHeapReserve, HEADER_COLUMN_COMMENT,
                                       XBinary::bytesCountToString(nValue));
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfHeapCommit:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfHeapCommit, HEADER_COLUMN_COMMENT,
                                       XBinary::bytesCountToString(nValue));
                            break;

                        case N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint, HEADER_COLUMN_COMMENT,
                                       pe.getMemoryRecordInfoByRelAddress(nValue));
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfCode:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::BaseOfCode, HEADER_COLUMN_COMMENT,
                                       pe.getMemoryRecordInfoByRelAddress(nValue));
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfData:
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::BaseOfData, HEADER_COLUMN_COMMENT,
                                       pe.getMemoryRecordInfoByRelAddress(nValue));
                            break;
                    }
                    break;

                case SPE::TYPE_EXPORT:
                    switch (nNdata) {
                        case N_IMAGE_EXPORT::Name:
                            addComment(ui->tableWidget_ExportHeader, N_IMAGE_EXPORT::Name, HEADER_COLUMN_COMMENT, pe.read_ansiString(pe.relAddressToOffset((quint32)nValue)));
                            break;
                        case N_IMAGE_EXPORT::AddressOfFunctions:
                            addComment(ui->tableWidget_ExportHeader, N_IMAGE_EXPORT::AddressOfFunctions, HEADER_COLUMN_COMMENT,
                                       pe.getMemoryRecordInfoByRelAddress((quint32)nValue));
                            break;
                        case N_IMAGE_EXPORT::AddressOfNames:
                            addComment(ui->tableWidget_ExportHeader, N_IMAGE_EXPORT::AddressOfNames, HEADER_COLUMN_COMMENT,
                                       pe.getMemoryRecordInfoByRelAddress((quint32)nValue));
                            break;
                        case N_IMAGE_EXPORT::AddressOfNameOrdinals:
                            addComment(ui->tableWidget_ExportHeader, N_IMAGE_EXPORT::AddressOfNameOrdinals, HEADER_COLUMN_COMMENT,
                                       pe.getMemoryRecordInfoByRelAddress((quint32)nValue));
                            break;
                    }
                    break;

                case SPE::TYPE_TLS:
                    switch (nNdata) {
                        case N_IMAGE_TLS::StartAddressOfRawData:
                            addComment(ui->tableWidget_TLS, N_IMAGE_TLS::StartAddressOfRawData, HEADER_COLUMN_COMMENT, pe.getMemoryRecordInfoByAddress((quint64)nValue));
                            break;
                        case N_IMAGE_TLS::EndAddressOfRawData:
                            addComment(ui->tableWidget_TLS, N_IMAGE_TLS::EndAddressOfRawData, HEADER_COLUMN_COMMENT, pe.getMemoryRecordInfoByAddress((quint64)nValue));
                            break;
                        case N_IMAGE_TLS::AddressOfIndex:
                            addComment(ui->tableWidget_TLS, N_IMAGE_TLS::AddressOfIndex, HEADER_COLUMN_COMMENT, pe.getMemoryRecordInfoByAddress((quint64)nValue));
                            break;
                        case N_IMAGE_TLS::AddressOfCallBacks:
                            addComment(ui->tableWidget_TLS, N_IMAGE_TLS::AddressOfCallBacks, HEADER_COLUMN_COMMENT, pe.getMemoryRecordInfoByAddress((quint64)nValue));
                            break;
                    }
                    break;

                case SPE::TYPE_RESOURCES_VERSION:
                    switch (nNdata) {
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwStrucVersion:
                            addComment(ui->tableWidget_Resources_Version, N_IMAGE_RESOURCE_FIXEDFILEINFO::dwStrucVersion, HEADER_COLUMN_COMMENT,
                                       XBinary::versionDwordToString((quint32)nValue));
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionMS:
                            addComment(ui->tableWidget_Resources_Version, N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionMS, HEADER_COLUMN_COMMENT,
                                       XBinary::versionDwordToString((quint32)nValue));
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionLS:
                            addComment(ui->tableWidget_Resources_Version, N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionLS, HEADER_COLUMN_COMMENT,
                                       XBinary::versionDwordToString((quint32)nValue));
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionMS:
                            addComment(ui->tableWidget_Resources_Version, N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionMS, HEADER_COLUMN_COMMENT,
                                       XBinary::versionDwordToString((quint32)nValue));
                            break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionLS:
                            addComment(ui->tableWidget_Resources_Version, N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionLS, HEADER_COLUMN_COMMENT,
                                       XBinary::versionDwordToString((quint32)nValue));
                            break;
                    }
                    break;

                case SPE::TYPE_LOADCONFIG:
                    switch (nNdata) {
                        case N_IMAGE_LOADCONFIG::Size:
                            addComment(ui->tableWidget_LoadConfig, N_IMAGE_LOADCONFIG::Size, HEADER_COLUMN_COMMENT, XBinary::bytesCountToString((quint64)nValue));
                            break;
                    }
                    break;
            }

            result = SV_EDITED;
        }
    }

    blockSignals(false);

    return result;
}

void PEWidget::setReadonly(bool bState)
{
    if (ui->checkBoxReadonly->isChecked() != bState) {
        const bool bBlocked1 = ui->checkBoxReadonly->blockSignals(true);
        ui->checkBoxReadonly->setChecked(bState);
        ui->checkBoxReadonly->blockSignals(bBlocked1);
    }

    setLineEditsReadOnly(g_lineEdit_IMAGE_DOS_HEADER, N_IMAGE_DOS_HEADER::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_IMAGE_NT_HEADERS, N_IMAGE_NT_HEADERS::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_IMAGE_FILE_HEADER, N_IMAGE_FILE_HEADER::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_TLS, N_IMAGE_TLS::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_Version_FixedFileInfo, N_IMAGE_RESOURCE_FIXEDFILEINFO::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_LoadConfig, N_IMAGE_LOADCONFIG::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_EXPORT, N_IMAGE_EXPORT::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_NetHeader, N_IMAGE_NETHEADER::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_Net_Metadata, N_IMAGE_NET_METADATA::__data_size, bState);
    //    setLineEditsReadOnly(lineEdit_IMAGE_DIRECTORY_ADDRESS,N_IMAGE_DIRECORIES::__data_size,bState);
    //    setLineEditsReadOnly(lineEdit_IMAGE_DIRECTORY_SIZE,N_IMAGE_DIRECORIES::__data_size,bState);

    setComboBoxesReadOnly(g_comboBox, __CB_size, bState);
    setPushButtonReadOnly(g_pushButton, __PB_size, bState);
    setDateTimeEditReadOnly(g_dateTimeEdit, __TD_size, bState);

    ui->widgetHex->setReadonly(bState);
    ui->widgetStrings->setReadonly(bState);
    ui->widgetDisasm->setReadonly(bState);
    ui->widgetHex_DosStub->setReadonly(bState);
    ui->widgetDisasm_DosStub->setReadonly(bState);
    ui->widgetHex_Section->setReadonly(bState);
    ui->widgetHex_Overlay->setReadonly(bState);
    ui->widgetHex_Resources->setReadonly(bState);
    ui->widgetHex_Debug->setReadonly(bState);
    ui->widgetHex_Exception->setReadonly(bState);
    ui->widgetHex_LoadConfig->setReadonly(bState);
    ui->widgetHex_NetHeader->setReadonly(bState);
    ui->widgetHex_Net_Metadata->setReadonly(bState);
    ui->widgetHex_Net_Metadata_Stream->setReadonly(bState);
    ui->widgetHex_Certificate->setReadonly(bState);
    ui->widgetHex_IMAGE_DIRECTORY_ENTRIES->setReadonly(bState);
    ui->widgetHex_IMAGE_DOS_HEADER->setReadonly(bState);
    ui->widgetHex_IMAGE_FILE_HEADER->setReadonly(bState);
    ui->widgetHex_IMAGE_NT_HEADERS->setReadonly(bState);
    ui->widgetHex_IMAGE_OPTIONAL_HEADER->setReadonly(bState);
    ui->widgetHex_TLS->setReadonly(bState);

    ui->widgetTools->setReadonly(bState);
}

void PEWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_lineEdit_IMAGE_DOS_HEADER, N_IMAGE_DOS_HEADER::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_IMAGE_NT_HEADERS, N_IMAGE_NT_HEADERS::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_IMAGE_FILE_HEADER, N_IMAGE_FILE_HEADER::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_TLS, N_IMAGE_TLS::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_Version_FixedFileInfo, N_IMAGE_RESOURCE_FIXEDFILEINFO::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_LoadConfig, N_IMAGE_LOADCONFIG::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_EXPORT, N_IMAGE_EXPORT::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_NetHeader, N_IMAGE_NETHEADER::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_Net_Metadata, N_IMAGE_NET_METADATA::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_Resources, N_IMAGE_RESOURCES::__data_size, bState);

    //    _blockSignals((QObject
    //    **)lineEdit_IMAGE_DIRECTORY_ADDRESS,N_IMAGE_DIRECORIES::__data_size,bState);
    //    _blockSignals((QObject
    //    **)lineEdit_IMAGE_DIRECTORY_SIZE,N_IMAGE_DIRECORIES::__data_size,bState);

    _blockSignals((QObject **)g_comboBox, __CB_size, bState);
    _blockSignals((QObject **)g_pushButton, __PB_size, bState);
    _blockSignals((QObject **)g_dateTimeEdit, __TD_size, bState);
}

void PEWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious)
{
    Q_UNUSED(pItemPrevious)

    if (pItemCurrent) {
        reloadData();
        addPage(pItemCurrent);
        ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}

void PEWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget = qobject_cast<QWidget *>(sender());
    int nStype = pWidget->property("STYPE").toInt();
    int nNdata = pWidget->property("NDATA").toInt();

    switch (nStype) {
        case SPE::TYPE_IMAGE_DOS_HEADER:
            switch (nNdata) {
                case N_IMAGE_DOS_HEADER::e_magic:
                    g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_magic]->setValue((quint16)nValue);
                    break;
            }
            break;

        case SPE::TYPE_IMAGE_NT_HEADERS:
            switch (nNdata) {
                case N_IMAGE_NT_HEADERS::Signature:
                    g_lineEdit_IMAGE_NT_HEADERS[N_IMAGE_NT_HEADERS::Signature]->setValue((quint32)nValue);
                    break;
            }
            break;

        case SPE::TYPE_IMAGE_FILE_HEADER:
            switch (nNdata) {
                case N_IMAGE_FILE_HEADER::Machine:
                    g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Machine]->setValue((quint16)nValue);
                    break;
                case N_IMAGE_FILE_HEADER::TimeDateStamp:
                    g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::TimeDateStamp]->setValue((quint32)nValue);
                    break;
                case N_IMAGE_FILE_HEADER::Characteristics:
                    g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Characteristics]->setValue((quint16)nValue);
                    break;
            }
            break;

        case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
            switch (nNdata) {
                case N_IMAGE_OPTIONAL_HEADER::Magic:
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Magic]->setValue((quint16)nValue);
                    break;
                case N_IMAGE_OPTIONAL_HEADER::Subsystem:
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Subsystem]->setValue((quint16)nValue);
                    break;
                case N_IMAGE_OPTIONAL_HEADER::DllCharacteristics:
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::DllCharacteristics]->setValue((quint16)nValue);
                    break;
                // Extra
                case N_IMAGE_OPTIONAL_HEADER::OperatingSystemVersion:
                    XBinary::XDWORD xdword = XBinary::make_xdword((quint32)nValue);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion]->setValue((quint16)xdword.nValue1);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion]->setValue((quint16)xdword.nValue2);
            }
            break;

        case SPE::TYPE_EXPORT:
            switch (nNdata) {
                case N_IMAGE_EXPORT::TimeDateStamp:
                    g_dateTimeEdit[TD_IMAGE_EXPORT_TimeDateStamp]->setValue(nValue);
                    break;
            }
            break;

        case SPE::TYPE_NETHEADER:
            switch (nNdata) {
                case N_IMAGE_NETHEADER::Flags:
                    g_lineEdit_NetHeader[N_IMAGE_NETHEADER::Flags]->setValue((quint32)nValue);
                    break;
            }
            break;

        case SPE::TYPE_RESOURCES_VERSION:
            switch (nNdata) {
                case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature:
                    g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature]->setValue((quint32)nValue);
                    break;
                case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags:
                    g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags]->setValue((quint32)nValue);
                    break;
                case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS:
                    g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS]->setValue((quint32)nValue);
                    break;
                case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType:
                    g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType]->setValue((quint32)nValue);
                    break;
            }
            break;
    }
}

void PEWidget::widgetAction()
{
    QWidget *pWidget = qobject_cast<QWidget *>(sender());
    int nStype = pWidget->property("STYPE").toInt();
    int nNdata = pWidget->property("NDATA").toInt();

    switch (nStype) {
        case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
            switch (nNdata) {
                case N_IMAGE_OPTIONAL_HEADER::CheckSum:
                    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

                    if (pe.isValid()) {
                        quint32 nCheckSum = pe.calculateCheckSum();
                        g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::CheckSum]->setValue(nCheckSum);
                    }
                    break;
            }
            break;
    }
}

void PEWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void PEWidget::editSectionHeader()
{
    showSectionHeader(SPE::TYPE_IMAGE_SECTION_HEADER, ui->tableView_Sections);
}

void PEWidget::sectionHex()
{
    showSectionHex(ui->tableView_Sections);
}

void PEWidget::sectionDisasm()
{
    showSectionDisasm(ui->tableView_Sections);
}

void PEWidget::sectionEntropy()
{
    showSectionEntropy(ui->tableView_Sections);
}

void PEWidget::sectionDump()
{
    dumpSection(ui->tableView_Sections);
}

void PEWidget::resourceHex()
{
    showSectionHex(ui->tableView_Resources);
}

void PEWidget::resourceDisasm()
{
    showSectionDisasm(ui->tableView_Resources);
}

void PEWidget::resourceEntropy()
{
    showSectionEntropy(ui->tableView_Resources);
}

void PEWidget::resourceDump()
{
    dumpSection(ui->tableView_Resources);
}

void PEWidget::reloadData()
{
    int nType = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_TYPE).toInt();
    qint64 nDataOffset = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nDataSize = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();

    QString sInit = getInitString(ui->treeWidgetNavi->currentItem());

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (pe.isValid()) {
        bool bIs64 = pe.is64();

        if (nType == SPE::TYPE_INFO) {
            if (!isInitPresent(sInit)) {
                ui->widgetInfo->setData(getDevice(), pe.getFileType(), "Info", true);
            }
        } else if (nType == SPE::TYPE_VIRUSTOTAL) {
            if (!isInitPresent(sInit)) {
                ui->widgetVirusTotal->setData(getDevice());
            }
        } else if (nType == SPE::TYPE_HEX) {
            if (!isInitPresent(sInit)) {
                XHexView::OPTIONS options = {};
                options.bMenu_Disasm = true;
                options.bMenu_MemoryMap = true;
                ui->widgetHex->setData(getDevice(), options);
                ui->widgetHex->setBackupDevice(getBackupDevice());
                // TODO save directory
                // ui->widgetHex->enableReadOnly(false);

                ui->widgetHex->reload();
            }
        } else if (nType == SPE::TYPE_DISASM) {
            if (!isInitPresent(sInit)) {
                XMultiDisasmWidget::OPTIONS options = {};
                options.fileType = pe.getFileType();
                options.nInitAddress = getDisasmInitAddress();
                options.bMenu_Hex = true;

                ui->widgetDisasm->setData(getDevice(), options, getXInfoDB());
                ui->widgetDisasm->setBackupDevice(getBackupDevice());

                setDisasmInitAddress(-1);
            }
        } else if (nType == SPE::TYPE_HASH) {
            if (!isInitPresent(sInit)) {
                ui->widgetHash->setData(getDevice(), pe.getFileType(), 0, -1, true);
            }
        } else if (nType == SPE::TYPE_STRINGS) {
            if (!isInitPresent(sInit)) {
                SearchStringsWidget::OPTIONS stringsOptions = {};
                stringsOptions.bMenu_Hex = true;
                stringsOptions.bMenu_Demangle = true;
                stringsOptions.bAnsi = true;
                stringsOptions.bUTF8 = false;
                stringsOptions.bUnicode = true;
                stringsOptions.bCStrings = false;

                ui->widgetStrings->setData(getDevice(), stringsOptions, true);
            }
        } else if (nType == SPE::TYPE_SIGNATURES) {
            if (!isInitPresent(sInit)) {
                SearchSignaturesWidget::OPTIONS signaturesOptions = {};
                signaturesOptions.bMenu_Hex = true;

                ui->widgetSignatures->setData(getDevice(), pe.getFileType(), signaturesOptions, false);
            }
        } else if (nType == SPE::TYPE_MEMORYMAP) {
            if (!isInitPresent(sInit)) {
                ui->widgetMemoryMap->setData(getDevice(), pe.getFileType());
            }
        } else if (nType == SPE::TYPE_ENTROPY) {
            if (!isInitPresent(sInit)) {
                ui->widgetEntropy->setData(getDevice(), 0, getDevice()->size(), pe.getFileType(),
                                           true);  // TODO save last directory
            }
        } else if (nType == SPE::TYPE_HEURISTICSCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetHeuristicScan->setData(getDevice(), true, pe.getFileType());
            }
        } else if (nType == SPE::TYPE_EXTRACTOR) {
            if (!isInitPresent(sInit)) {
                XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();
                extractorOptions.bMenu_Hex = true;

                ui->widgetExtractor->setData(getDevice(), extractorOptions, true);
            }
        } else if (nType == SPE::TYPE_TOOLS) {
            if (!isInitPresent(sInit)) {
                ui->widgetTools->setData(getDevice(), getOptions().bIsImage, getOptions().nImageBase);
            }
        } else if (nType == SPE::TYPE_IMAGE_DOS_HEADER) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SPE::TYPE_IMAGE_DOS_HEADER, ui->tableWidget_IMAGE_DOS_HEADER, N_IMAGE_DOS_HEADER::records, g_lineEdit_IMAGE_DOS_HEADER,
                                  N_IMAGE_DOS_HEADER::__data_size, 0);
                g_comboBox[CB_IMAGE_DOS_HEADER_e_magic] = createComboBox(ui->tableWidget_IMAGE_DOS_HEADER, XPE::getImageMagicsS(), SPE::TYPE_IMAGE_DOS_HEADER,
                                                                         N_IMAGE_DOS_HEADER::e_magic, XComboBoxEx::CBTYPE_LIST);
                g_invWidget[INV_IMAGE_DOS_HEADER_e_lfanew] =
                    createInvWidget(ui->tableWidget_IMAGE_DOS_HEADER, SPE::TYPE_IMAGE_DOS_HEADER, N_IMAGE_DOS_HEADER::e_lfanew, InvWidget::TYPE_HEX);

                blockSignals(true);

                XMSDOS_DEF::IMAGE_DOS_HEADEREX msdosheaderex = pe.getDosHeaderEx();

                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_magic]->setValue(msdosheaderex.e_magic);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_cblp]->setValue(msdosheaderex.e_cblp);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_cp]->setValue(msdosheaderex.e_cp);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_crlc]->setValue(msdosheaderex.e_crlc);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_cparhdr]->setValue(msdosheaderex.e_cparhdr);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_minalloc]->setValue(msdosheaderex.e_minalloc);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_maxalloc]->setValue(msdosheaderex.e_maxalloc);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_ss]->setValue(msdosheaderex.e_ss);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_sp]->setValue(msdosheaderex.e_sp);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_csum]->setValue(msdosheaderex.e_csum);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_ip]->setValue(msdosheaderex.e_ip);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_cs]->setValue(msdosheaderex.e_cs);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_lfarlc]->setValue(msdosheaderex.e_lfarlc);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_ovno]->setValue(msdosheaderex.e_ovno);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res_0]->setValue(msdosheaderex.e_res[0]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res_1]->setValue(msdosheaderex.e_res[1]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res_2]->setValue(msdosheaderex.e_res[2]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res_3]->setValue(msdosheaderex.e_res[3]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_oemid]->setValue(msdosheaderex.e_oemid);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_oeminfo]->setValue(msdosheaderex.e_oeminfo);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_0]->setValue(msdosheaderex.e_res2[0]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_1]->setValue(msdosheaderex.e_res2[1]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_2]->setValue(msdosheaderex.e_res2[2]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_3]->setValue(msdosheaderex.e_res2[3]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_4]->setValue(msdosheaderex.e_res2[4]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_5]->setValue(msdosheaderex.e_res2[5]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_6]->setValue(msdosheaderex.e_res2[6]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_7]->setValue(msdosheaderex.e_res2[7]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_8]->setValue(msdosheaderex.e_res2[8]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_9]->setValue(msdosheaderex.e_res2[9]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_lfanew]->setValue(msdosheaderex.e_lfanew);

                g_comboBox[CB_IMAGE_DOS_HEADER_e_magic]->setValue(msdosheaderex.e_magic);

                g_invWidget[INV_IMAGE_DOS_HEADER_e_lfanew]->setOffsetAndSize(&pe, msdosheaderex.e_lfanew, 0);

                qint64 nOffset = pe.getDosHeaderExOffset();
                qint64 nSize = pe.getDosHeaderExSize();
                qint64 nAddress = pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_IMAGE_DOS_HEADER], ui->widgetHex_IMAGE_DOS_HEADER);

                blockSignals(false);
            }
        } else if (nType == SPE::TYPE_DOS_STUB) {
            if (!isInitPresent(sInit)) {
                qint64 nOffset = pe.getDosStubOffset();
                qint64 nSize = pe.getDosStubSize();
                qint64 nAddress = pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_DOS_STUB], ui->widgetHex_DosStub);

                XBinary binary(g_subDevice[SPE::TYPE_DOS_STUB], true, nAddress);
                binary.setFileType(XBinary::FT_REGION);
                binary.setArch("8086");

                XDisasmView::OPTIONS options = {};
                options.nInitAddress = -1;  // TODO Check MSDOS
                options.memoryMapRegion = binary.getMemoryMap();

                ui->widgetDisasm_DosStub->setData(g_subDevice[SPE::TYPE_DOS_STUB], options);
                ui->widgetDisasm_DosStub->setBackupDevice(getBackupDevice());
            }
        } else if (nType == SPE::TYPE_IMAGE_NT_HEADERS) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SPE::TYPE_IMAGE_NT_HEADERS, ui->tableWidget_IMAGE_NT_HEADERS, N_IMAGE_NT_HEADERS::records, g_lineEdit_IMAGE_NT_HEADERS,
                                  N_IMAGE_NT_HEADERS::__data_size, 0);
                g_comboBox[CB_IMAGE_NT_HEADERS_Signature] = createComboBox(ui->tableWidget_IMAGE_NT_HEADERS, XPE::getImageNtHeadersSignaturesS(), SPE::TYPE_IMAGE_NT_HEADERS,
                                                                           N_IMAGE_NT_HEADERS::Signature, XComboBoxEx::CBTYPE_LIST);

                blockSignals(true);

                quint32 nSignature = pe.getNtHeaders_Signature();

                g_lineEdit_IMAGE_NT_HEADERS[N_IMAGE_NT_HEADERS::Signature]->setValue(nSignature);
                g_comboBox[CB_IMAGE_NT_HEADERS_Signature]->setValue(nSignature);

                qint64 nOffset = pe.getNtHeadersOffset();
                qint64 nSize = 4;
                qint64 nAddress = pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_IMAGE_NT_HEADERS], ui->widgetHex_IMAGE_NT_HEADERS);

                blockSignals(false);
            }
        } else if (nType == SPE::TYPE_IMAGE_FILE_HEADER) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SPE::TYPE_IMAGE_FILE_HEADER, ui->tableWidget_IMAGE_FILE_HEADER, N_IMAGE_FILE_HEADER::records, g_lineEdit_IMAGE_FILE_HEADER,
                                  N_IMAGE_FILE_HEADER::__data_size, 0);
                g_comboBox[CB_IMAGE_FILE_HEADER_Machine] = createComboBox(ui->tableWidget_IMAGE_FILE_HEADER, XPE::getImageFileHeaderMachinesS(), SPE::TYPE_IMAGE_FILE_HEADER,
                                                                          N_IMAGE_FILE_HEADER::Machine, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_IMAGE_FILE_HEADER_Characteristics] =
                    createComboBox(ui->tableWidget_IMAGE_FILE_HEADER, XPE::getImageFileHeaderCharacteristicsS(), SPE::TYPE_IMAGE_FILE_HEADER,
                                   N_IMAGE_FILE_HEADER::Characteristics, XComboBoxEx::CBTYPE_FLAGS);

                g_dateTimeEdit[TD_IMAGE_FILE_HEADER_TimeDateStamp] =
                    createTimeDateEdit(ui->tableWidget_IMAGE_FILE_HEADER, SPE::TYPE_IMAGE_FILE_HEADER, N_IMAGE_FILE_HEADER::TimeDateStamp, XDateTimeEditX::DT_TYPE_POSIX);
                g_invWidget[INV_IMAGE_FILE_HEADER_PointerToSymbolTable] =
                    createInvWidget(ui->tableWidget_IMAGE_FILE_HEADER, SPE::TYPE_IMAGE_FILE_HEADER, N_IMAGE_FILE_HEADER::PointerToSymbolTable, InvWidget::TYPE_HEX);

                blockSignals(true);

                XPE_DEF::IMAGE_FILE_HEADER fileheader = pe.getFileHeader();

                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Machine]->setValue(fileheader.Machine);
                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::NumberOfSections]->setValue(fileheader.NumberOfSections);
                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::TimeDateStamp]->setValue(fileheader.TimeDateStamp);
                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::PointerToSymbolTable]->setValue(fileheader.PointerToSymbolTable);
                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::NumberOfSymbols]->setValue(fileheader.NumberOfSymbols);
                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::SizeOfOptionalHeader]->setValue(fileheader.SizeOfOptionalHeader);
                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Characteristics]->setValue(fileheader.Characteristics);

                g_comboBox[CB_IMAGE_FILE_HEADER_Machine]->setValue(fileheader.Machine);
                g_comboBox[CB_IMAGE_FILE_HEADER_Characteristics]->setValue(fileheader.Characteristics);
                g_dateTimeEdit[TD_IMAGE_FILE_HEADER_TimeDateStamp]->setValue(fileheader.TimeDateStamp);

                addComment(ui->tableWidget_IMAGE_FILE_HEADER, N_IMAGE_FILE_HEADER::SizeOfOptionalHeader, HEADER_COLUMN_COMMENT,
                           XBinary::bytesCountToString(fileheader.SizeOfOptionalHeader));

                g_invWidget[INV_IMAGE_FILE_HEADER_PointerToSymbolTable]->setAddressAndSize(&pe, pe.getBaseAddress() + fileheader.PointerToSymbolTable, 0);

                qint64 nOffset = pe.getFileHeaderOffset();
                qint64 nSize = pe.getFileHeaderSize();
                qint64 nAddress = pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_IMAGE_FILE_HEADER], ui->widgetHex_IMAGE_FILE_HEADER);

                blockSignals(false);
            }
        } else if (nType == SPE::TYPE_IMAGE_OPTIONAL_HEADER) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SPE::TYPE_IMAGE_OPTIONAL_HEADER, ui->tableWidget_IMAGE_OPTIONAL_HEADER,
                                  pe.is64() ? (N_IMAGE_OPTIONAL_HEADER::records64) : (N_IMAGE_OPTIONAL_HEADER::records32), g_lineEdit_IMAGE_OPTIONAL_HEADER,
                                  N_IMAGE_OPTIONAL_HEADER::__data_size, 0);
                g_comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic] = createComboBox(ui->tableWidget_IMAGE_OPTIONAL_HEADER, XPE::getImageOptionalHeaderMagicS(),
                                                                            SPE::TYPE_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::Magic, XComboBoxEx::CBTYPE_LIST);

                g_comboBox[CB_IMAGE_OPTIONAL_HEADER_OperationSystemVersion] =
                    createComboBox(ui->tableWidget_IMAGE_OPTIONAL_HEADER, XPE::getOperatingSystemVersions(pe.getOsInfo().osName), SPE::TYPE_IMAGE_OPTIONAL_HEADER,
                                   N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion, XComboBoxEx::CBTYPE_LIST, 0, N_IMAGE_OPTIONAL_HEADER::OperatingSystemVersion);

                g_comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem] = createComboBox(ui->tableWidget_IMAGE_OPTIONAL_HEADER, XPE::getImageOptionalHeaderSubsystemS(),
                                                                                SPE::TYPE_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::Subsystem, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics] =
                    createComboBox(ui->tableWidget_IMAGE_OPTIONAL_HEADER, XPE::getImageOptionalHeaderDllCharacteristicsS(), SPE::TYPE_IMAGE_OPTIONAL_HEADER,
                                   N_IMAGE_OPTIONAL_HEADER::DllCharacteristics, XComboBoxEx::CBTYPE_FLAGS);

                g_invWidget[INV_IMAGE_OPTIONAL_HEADER_AddressOfEntryPoint] = createInvWidget(ui->tableWidget_IMAGE_OPTIONAL_HEADER, SPE::TYPE_IMAGE_OPTIONAL_HEADER,
                                                                                             N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint, InvWidget::TYPE_DISASM);
                g_invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfCode] =
                    createInvWidget(ui->tableWidget_IMAGE_OPTIONAL_HEADER, SPE::TYPE_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::BaseOfCode, InvWidget::TYPE_HEX);

                if (!bIs64) {
                    g_invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfData] =
                        createInvWidget(ui->tableWidget_IMAGE_OPTIONAL_HEADER, SPE::TYPE_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::BaseOfData, InvWidget::TYPE_HEX);
                }

                g_pushButton[PB_CalculateChecksum] =
                    createPushButton(ui->tableWidget_IMAGE_OPTIONAL_HEADER, SPE::TYPE_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::CheckSum, tr("Calculate"));

                blockSignals(true);

                XBinary::_MEMORY_MAP memoryMap = pe.getMemoryMap();

                if (bIs64) {
                    XPE_DEF::IMAGE_OPTIONAL_HEADER64S oh64 = pe.getOptionalHeader64S();
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Magic]->setValue(oh64.Magic);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorLinkerVersion]->setValue(oh64.MajorLinkerVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorLinkerVersion]->setValue(oh64.MinorLinkerVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfCode]->setValue(oh64.SizeOfCode);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfInitializedData]->setValue(oh64.SizeOfInitializedData);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfUninitializedData]->setValue(oh64.SizeOfUninitializedData);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint]->setValue(oh64.AddressOfEntryPoint);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::BaseOfCode]->setValue(oh64.BaseOfCode);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::ImageBase]->setValue(oh64.ImageBase);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SectionAlignment]->setValue(oh64.SectionAlignment);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::FileAlignment]->setValue(oh64.FileAlignment);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion]->setValue(oh64.MajorOperatingSystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion]->setValue(oh64.MinorOperatingSystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorImageVersion]->setValue(oh64.MajorImageVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorImageVersion]->setValue(oh64.MinorImageVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorSubsystemVersion]->setValue(oh64.MajorSubsystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorSubsystemVersion]->setValue(oh64.MinorSubsystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Win32VersionValue]->setValue(oh64.Win32VersionValue);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfImage]->setValue(oh64.SizeOfImage);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeaders]->setValue(oh64.SizeOfHeaders);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::CheckSum]->setValue(oh64.CheckSum);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Subsystem]->setValue(oh64.Subsystem);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::DllCharacteristics]->setValue(oh64.DllCharacteristics);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfStackReserve]->setValue(oh64.SizeOfStackReserve);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfStackCommit]->setValue(oh64.SizeOfStackCommit);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeapReserve]->setValue(oh64.SizeOfHeapReserve);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeapCommit]->setValue(oh64.SizeOfHeapCommit);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::LoaderFlags]->setValue(oh64.LoaderFlags);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::NumberOfRvaAndSizes]->setValue(oh64.NumberOfRvaAndSizes);

                    g_comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic]->setValue(oh64.Magic);
                    g_comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem]->setValue(oh64.Subsystem);
                    g_comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics]->setValue(oh64.DllCharacteristics);

                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfCode, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh64.SizeOfCode));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfInitializedData, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh64.SizeOfInitializedData));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfUninitializedData, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh64.SizeOfUninitializedData));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SectionAlignment, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh64.SectionAlignment));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::FileAlignment, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh64.FileAlignment));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfImage, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh64.SizeOfImage));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfHeaders, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh64.SizeOfHeaders));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfStackReserve, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh64.SizeOfStackReserve));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfStackCommit, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh64.SizeOfStackCommit));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfHeapReserve, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh64.SizeOfHeapReserve));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfHeapCommit, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh64.SizeOfHeapCommit));
                    // TODO more
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByRelAddress(&memoryMap, oh64.AddressOfEntryPoint));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::BaseOfCode, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByRelAddress(&memoryMap, oh64.BaseOfCode));

                    g_invWidget[INV_IMAGE_OPTIONAL_HEADER_AddressOfEntryPoint]->setAddressAndSize(&pe, pe.getBaseAddress() + oh64.AddressOfEntryPoint, 0);
                    g_invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfCode]->setAddressAndSize(&pe, pe.getBaseAddress() + oh64.BaseOfCode, 0);
                } else {
                    XPE_DEF::IMAGE_OPTIONAL_HEADER32S oh32 = pe.getOptionalHeader32S();
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Magic]->setValue(oh32.Magic);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorLinkerVersion]->setValue(oh32.MajorLinkerVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorLinkerVersion]->setValue(oh32.MinorLinkerVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfCode]->setValue(oh32.SizeOfCode);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfInitializedData]->setValue(oh32.SizeOfInitializedData);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfUninitializedData]->setValue(oh32.SizeOfUninitializedData);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint]->setValue(oh32.AddressOfEntryPoint);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::BaseOfCode]->setValue(oh32.BaseOfCode);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::BaseOfData]->setValue(oh32.BaseOfData);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::ImageBase]->setValue(oh32.ImageBase);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SectionAlignment]->setValue(oh32.SectionAlignment);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::FileAlignment]->setValue(oh32.FileAlignment);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion]->setValue(oh32.MajorOperatingSystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion]->setValue(oh32.MinorOperatingSystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorImageVersion]->setValue(oh32.MajorImageVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorImageVersion]->setValue(oh32.MinorImageVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorSubsystemVersion]->setValue(oh32.MajorSubsystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorSubsystemVersion]->setValue(oh32.MinorSubsystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Win32VersionValue]->setValue(oh32.Win32VersionValue);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfImage]->setValue(oh32.SizeOfImage);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeaders]->setValue(oh32.SizeOfHeaders);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::CheckSum]->setValue(oh32.CheckSum);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Subsystem]->setValue(oh32.Subsystem);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::DllCharacteristics]->setValue(oh32.DllCharacteristics);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfStackReserve]->setValue(oh32.SizeOfStackReserve);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfStackCommit]->setValue(oh32.SizeOfStackCommit);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeapReserve]->setValue(oh32.SizeOfHeapReserve);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeapCommit]->setValue(oh32.SizeOfHeapCommit);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::LoaderFlags]->setValue(oh32.LoaderFlags);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::NumberOfRvaAndSizes]->setValue(oh32.NumberOfRvaAndSizes);

                    g_comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic]->setValue(oh32.Magic);
                    g_comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem]->setValue(oh32.Subsystem);
                    g_comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics]->setValue(oh32.DllCharacteristics);

                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfCode, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh32.SizeOfCode));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfInitializedData, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh32.SizeOfInitializedData));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfUninitializedData, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh32.SizeOfUninitializedData));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SectionAlignment, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh32.SectionAlignment));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::FileAlignment, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh32.FileAlignment));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfImage, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh32.SizeOfImage));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfHeaders, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh32.SizeOfHeaders));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfStackReserve, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh32.SizeOfStackReserve));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfStackCommit, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh32.SizeOfStackCommit));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfHeapReserve, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh32.SizeOfHeapReserve));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfHeapCommit, HEADER_COLUMN_COMMENT,
                               XBinary::bytesCountToString(oh32.SizeOfHeapCommit));
                    // TODO more
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByRelAddress(&memoryMap, oh32.AddressOfEntryPoint));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::BaseOfCode, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByRelAddress(&memoryMap, oh32.BaseOfCode));
                    addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::BaseOfData, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByRelAddress(&memoryMap, oh32.BaseOfData));

                    g_invWidget[INV_IMAGE_OPTIONAL_HEADER_AddressOfEntryPoint]->setAddressAndSize(&pe, pe.getBaseAddress() + oh32.AddressOfEntryPoint, 0);
                    g_invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfCode]->setAddressAndSize(&pe, pe.getBaseAddress() + oh32.BaseOfCode, 0);
                    g_invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfData]->setAddressAndSize(&pe, pe.getBaseAddress() + oh32.BaseOfData, 0);
                }

                g_comboBox[CB_IMAGE_OPTIONAL_HEADER_OperationSystemVersion]->setValue(pe.getOperatingSystemVersion());

                qint64 nOffset = pe.getOptionalHeaderOffset();
                qint64 nSize = pe.getOptionalHeaderSize();
                qint64 nAddress = pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_IMAGE_OPTIONAL_HEADER], ui->widgetHex_IMAGE_OPTIONAL_HEADER);

                blockSignals(false);
            }
        } else if (nType == SPE::TYPE_IMAGE_DIRECTORY_ENTRIES) {
            if (!isInitPresent(sInit)) {
                createSectionTable(SPE::TYPE_IMAGE_DIRECTORY_ENTRIES, ui->tableWidget_IMAGE_DIRECTORY_ENTRIES, N_IMAGE_DATA_DIRECTORY::records,
                                   N_IMAGE_DATA_DIRECTORY::__data_size);

                blockSignals(true);

                qint32 nNumberOfRvaAndSizes = (qint32)pe.getOptionalHeader_NumberOfRvaAndSizes();

                ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setRowCount(16);

                XBinary::_MEMORY_MAP memoryMap = pe.getMemoryMap();
                QMap<quint64, QString> mapDataDirectories = XPE::getImageOptionalHeaderDataDirectoryS();

                for (qint32 i = 0; i < 16; i++) {
                    ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i, 4, new QTableWidgetItem());  // Comment
                    ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i, 5, new QTableWidgetItem());  // Comment

                    XPE_DEF::IMAGE_DATA_DIRECTORY dd = pe.getOptionalHeader_DataDirectory((quint32)i);

                    QTableWidgetItem *pItemNumber = new QTableWidgetItem(QString::number(i));

                    pItemNumber->setData(Qt::UserRole + FW_DEF::SECTION_DATA_SIZE, dd.Size);
                    pItemNumber->setData(Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET, pe.getDataDirectoryHeaderOffset(i));

                    addComment(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES, i, 4, XBinary::bytesCountToString(dd.Size));

                    if (i != XPE_DEF::S_IMAGE_DIRECTORY_ENTRY_SECURITY) {
                        pItemNumber->setData(Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS, dd.VirtualAddress);
                        pItemNumber->setData(Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET, pe.addressToOffset(&memoryMap, memoryMap.nModuleAddress + dd.VirtualAddress));

                        if ((dd.VirtualAddress) && (pe.isRelAddressValid(&memoryMap, dd.VirtualAddress))) {
                            addComment(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES, i, 5, pe.getMemoryRecordInfoByRelAddress(&memoryMap, dd.VirtualAddress));
                        }
                    } else {
                        pItemNumber->setData(Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS, dd.VirtualAddress);
                        pItemNumber->setData(Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET, dd.VirtualAddress);

                        if ((dd.VirtualAddress) && (pe.isOffsetValid(&memoryMap, dd.VirtualAddress))) {
                            addComment(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES, i, 5, pe.getMemoryRecordInfoByOffset(&memoryMap, dd.VirtualAddress));
                        }
                    }

                    ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i, 0, pItemNumber);

                    ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i, 1, new QTableWidgetItem(mapDataDirectories.value(i)));
                    ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i, 2, new QTableWidgetItem(XBinary::valueToHex(dd.VirtualAddress)));

                    QTableWidgetItem *pItem = new QTableWidgetItem(XBinary::valueToHex(dd.Size));
                    ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i, 3, pItem);

                    setItemEnable(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(i, 0), i < nNumberOfRvaAndSizes);
                    setItemEnable(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(i, 1), i < nNumberOfRvaAndSizes);
                    setItemEnable(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(i, 2), i < nNumberOfRvaAndSizes);
                    setItemEnable(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(i, 3), i < nNumberOfRvaAndSizes);

                    // TODO !!!
                    //                ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->setItem(i,3,new
                    //                QTableWidgetItem(XBinary::valueToHex(dd.Size)));
                    //
                    //                lineEdit_IMAGE_DIRECTORY_ADDRESS[i]->setValue(dd.VirtualAddress);
                    //                lineEdit_IMAGE_DIRECTORY_SIZE[i]->setValue(dd.Size);

                    //                lineEdit_IMAGE_DIRECTORY_ADDRESS[i]->setEnabled(i<(int)nNumberOfRvaAndSizes);
                    //                lineEdit_IMAGE_DIRECTORY_SIZE[i]->setEnabled(i<(int)nNumberOfRvaAndSizes);
                }

                if (ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->currentRow() == 0) {
                    loadDirectory(0);
                } else {
                    ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->selectRow(0);
                }

                blockSignals(false);
            }
        } else if (nType == SPE::TYPE_RICH) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_RICH, &g_tvModel[SPE::TYPE_RICH], &pe, 0, 0, 0);

                ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_RICH], ui->tableView_RICH, nullptr, true);
            }
        } else if (nType == SPE::TYPE_SECTIONS) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_SECTIONS, &g_tvModel[SPE::TYPE_SECTIONS], &pe, 0, 0, 0);

                ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_SECTIONS], ui->tableView_Sections, nullptr, false);

                connect(ui->tableView_Sections->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Sections_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SPE::TYPE_SECTIONS]->rowCount()) {
                    ui->tableView_Sections->setCurrentIndex(ui->tableView_Sections->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_EXPORT) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SPE::TYPE_EXPORT, ui->tableWidget_ExportHeader, N_IMAGE_EXPORT::records, g_lineEdit_EXPORT, N_IMAGE_EXPORT::__data_size, 0);

                g_invWidget[INV_IMAGE_EXPORT_Name] = createInvWidget(ui->tableWidget_ExportHeader, SPE::TYPE_EXPORT, N_IMAGE_EXPORT::Name, InvWidget::TYPE_HEX);
                g_invWidget[INV_IMAGE_EXPORT_AddressOfFunctions] =
                    createInvWidget(ui->tableWidget_ExportHeader, SPE::TYPE_EXPORT, N_IMAGE_EXPORT::AddressOfFunctions, InvWidget::TYPE_HEX);
                g_invWidget[INV_IMAGE_EXPORT_AddressOfNameOrdinals] =
                    createInvWidget(ui->tableWidget_ExportHeader, SPE::TYPE_EXPORT, N_IMAGE_EXPORT::AddressOfNameOrdinals, InvWidget::TYPE_HEX);
                g_invWidget[INV_IMAGE_EXPORT_AddressOfNames] =
                    createInvWidget(ui->tableWidget_ExportHeader, SPE::TYPE_EXPORT, N_IMAGE_EXPORT::AddressOfNames, InvWidget::TYPE_HEX);

                g_dateTimeEdit[TD_IMAGE_EXPORT_TimeDateStamp] =
                    createTimeDateEdit(ui->tableWidget_ExportHeader, SPE::TYPE_EXPORT, N_IMAGE_EXPORT::TimeDateStamp, XDateTimeEditX::DT_TYPE_POSIX);

                blockSignals(true);

                XPE::EXPORT_HEADER eh = pe.getExport();

                g_lineEdit_EXPORT[N_IMAGE_EXPORT::Characteristics]->setValue(eh.directory.Characteristics);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::TimeDateStamp]->setValue(eh.directory.TimeDateStamp);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::MajorVersion]->setValue(eh.directory.MajorVersion);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::MinorVersion]->setValue(eh.directory.MinorVersion);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::Name]->setValue(eh.directory.Name);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::Base]->setValue(eh.directory.Base);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::NumberOfFunctions]->setValue(eh.directory.NumberOfFunctions);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::NumberOfNames]->setValue(eh.directory.NumberOfNames);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::AddressOfFunctions]->setValue(eh.directory.AddressOfFunctions);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::AddressOfNames]->setValue(eh.directory.AddressOfNames);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::AddressOfNameOrdinals]->setValue(eh.directory.AddressOfNameOrdinals);

                g_dateTimeEdit[TD_IMAGE_EXPORT_TimeDateStamp]->setValue(eh.directory.TimeDateStamp);

                g_invWidget[INV_IMAGE_EXPORT_Name]->setAddressAndSize(&pe, pe.getBaseAddress() + eh.directory.Name, 0);
                g_invWidget[INV_IMAGE_EXPORT_AddressOfFunctions]->setAddressAndSize(&pe, pe.getBaseAddress() + eh.directory.AddressOfFunctions, 0);
                g_invWidget[INV_IMAGE_EXPORT_AddressOfNameOrdinals]->setAddressAndSize(&pe, pe.getBaseAddress() + eh.directory.AddressOfNameOrdinals, 0);
                g_invWidget[INV_IMAGE_EXPORT_AddressOfNames]->setAddressAndSize(&pe, pe.getBaseAddress() + eh.directory.AddressOfNames, 0);

                XBinary::_MEMORY_MAP memoryMap = pe.getMemoryMap();

                addComment(ui->tableWidget_ExportHeader, N_IMAGE_EXPORT::Name, HEADER_COLUMN_COMMENT,
                           pe.read_ansiString(pe.relAddressToOffset(&memoryMap, eh.directory.Name)));

                addComment(ui->tableWidget_ExportHeader, N_IMAGE_EXPORT::AddressOfFunctions, HEADER_COLUMN_COMMENT,
                           pe.getMemoryRecordInfoByRelAddress(&memoryMap, eh.directory.AddressOfFunctions));
                addComment(ui->tableWidget_ExportHeader, N_IMAGE_EXPORT::AddressOfNames, HEADER_COLUMN_COMMENT,
                           pe.getMemoryRecordInfoByRelAddress(&memoryMap, eh.directory.AddressOfNames));
                addComment(ui->tableWidget_ExportHeader, N_IMAGE_EXPORT::AddressOfNameOrdinals, HEADER_COLUMN_COMMENT,
                           pe.getMemoryRecordInfoByRelAddress(&memoryMap, eh.directory.AddressOfNameOrdinals));

                blockSignals(false);

                PEProcessData peProcessData(SPE::TYPE_EXPORT_FUNCTION, &g_tvModel[SPE::TYPE_EXPORT_FUNCTION], &pe, 0, 0, 0, ui->checkBoxExportShowValid->isChecked());

                ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_EXPORT_FUNCTION], ui->tableView_ExportFunctions);

                if (g_tvModel[SPE::TYPE_EXPORT_FUNCTION]->rowCount()) {
                    ui->tableView_ExportFunctions->setCurrentIndex(ui->tableView_ExportFunctions->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_IMPORT) {
            if (!isInitPresent(sInit)) {
                XBinary::_MEMORY_MAP memoryMap = pe.getMemoryMap();
                QList<XPE::IMPORT_RECORD> listImportRecords = pe.getImportRecords(&memoryMap);

                ui->lineEditHash64->setValue(pe.getImportHash64(&listImportRecords));
                ui->lineEditHash32->setValue(pe.getImportHash32(&listImportRecords));

                PEProcessData peProcessData(SPE::TYPE_IMPORT, &g_tvModel[SPE::TYPE_IMPORT], &pe, 0, 0, 0);

                ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_IMPORT], ui->tableView_ImportLibraries);

                connect(ui->tableView_ImportLibraries->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_ImportLibraries_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SPE::TYPE_IMPORT]->rowCount()) {
                    ui->tableView_ImportLibraries->setCurrentIndex(ui->tableView_ImportLibraries->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_RESOURCES) {
            if (!isInitPresent(sInit)) {
                // Table
                PEProcessData peProcessData(SPE::TYPE_RESOURCES, &g_tvModel[SPE::TYPE_RESOURCES], &pe, 0, 0, 0, false);

                ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_RESOURCES], ui->tableView_Resources, nullptr, false);

                connect(ui->tableView_Resources->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Resources_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SPE::TYPE_RESOURCES]->rowCount()) {
                    ui->tableView_Resources->setCurrentIndex(ui->tableView_Resources->model()->index(0, 0));
                }

                // Tree
                createListTable(SPE::TYPE_RESOURCES, ui->tableWidget_Resources, N_IMAGE_RESOURCES::records, g_lineEdit_Resources, N_IMAGE_RESOURCES::__data_size);

                PEProcessData peProcessDataTree(SPE::TYPE_RESOURCES, &g_tvModel[SPE::TYPE_RESOURCES_TREE], &pe, 0, 0, 0, true);

                ajustTreeView(&peProcessDataTree, &g_tvModel[SPE::TYPE_RESOURCES_TREE], ui->treeView_Resources);

                connect(ui->treeView_Resources->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTreeView_Resources_currentRowChanged(QModelIndex, QModelIndex)));
            }
        } else if (nType == SPE::TYPE_RESOURCES_STRINGTABLE) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_RESOURCES_STRINGTABLE, &g_tvModel[SPE::TYPE_RESOURCES_STRINGTABLE], &pe, 0, 0, 0, false);

                ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_RESOURCES_STRINGTABLE], ui->tableView_Resources_StringTable, nullptr, true);

                //                connect(ui->tableView_Resources_StringTable->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_Resources_StringTable_currentRowChanged(QModelIndex,QModelIndex)));

                //                if(g_tvModel[SPE::TYPE_RESOURCES_STRINGTABLE]->rowCount())
                //                {
                //                    ui->tableView_Resources_StringTable->setCurrentIndex(ui->tableView_Resources_StringTable->model()->index(0,0));
                //                }
            }
        } else if (nType == SPE::TYPE_RESOURCES_VERSION) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SPE::TYPE_RESOURCES_VERSION, ui->tableWidget_Resources_Version, N_IMAGE_RESOURCE_FIXEDFILEINFO::records, g_lineEdit_Version_FixedFileInfo,
                                  N_IMAGE_RESOURCE_FIXEDFILEINFO::__data_size, 0);

                g_comboBox[CB_RESOURCES_VERSION_dwSignature] =
                    createComboBox(ui->tableWidget_Resources_Version, XPE::getResourcesFixedFileInfoSignaturesS(), SPE::TYPE_RESOURCES_VERSION,
                                   N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_RESOURCES_VERSION_dwFileFlags] =
                    createComboBox(ui->tableWidget_Resources_Version, XPE::getResourcesFixedFileInfoFileFlagsS(), SPE::TYPE_RESOURCES_VERSION,
                                   N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags, XComboBoxEx::CBTYPE_FLAGS);
                g_comboBox[CB_RESOURCES_VERSION_dwFileOS] = createComboBox(ui->tableWidget_Resources_Version, XPE::getResourcesFixedFileInfoFileOsesS(),
                                                                           SPE::TYPE_RESOURCES_VERSION, N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_RESOURCES_VERSION_dwFileType] =
                    createComboBox(ui->tableWidget_Resources_Version, XPE::getResourcesFixedFileInfoFileTypesS(), SPE::TYPE_RESOURCES_VERSION,
                                   N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType, XComboBoxEx::CBTYPE_LIST);

                blockSignals(true);

                XPE::RESOURCES_VERSION resourceVersion = pe.getResourcesVersion();

                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature]->setValue(resourceVersion.fileInfo.dwSignature);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwStrucVersion]->setValue(resourceVersion.fileInfo.dwStrucVersion);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionMS]->setValue(resourceVersion.fileInfo.dwFileVersionMS);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionLS]->setValue(resourceVersion.fileInfo.dwFileVersionLS);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionMS]->setValue(resourceVersion.fileInfo.dwProductVersionMS);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionLS]->setValue(resourceVersion.fileInfo.dwProductVersionLS);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlagsMask]->setValue(resourceVersion.fileInfo.dwFileFlagsMask);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags]->setValue(resourceVersion.fileInfo.dwFileFlags);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS]->setValue(resourceVersion.fileInfo.dwFileOS);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType]->setValue(resourceVersion.fileInfo.dwFileType);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileSubtype]->setValue(resourceVersion.fileInfo.dwFileSubtype);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileDateMS]->setValue(resourceVersion.fileInfo.dwFileDateMS);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileDateLS]->setValue(resourceVersion.fileInfo.dwFileDateLS);

                g_comboBox[CB_RESOURCES_VERSION_dwSignature]->setValue(resourceVersion.fileInfo.dwSignature);
                g_comboBox[CB_RESOURCES_VERSION_dwFileFlags]->setValue(resourceVersion.fileInfo.dwFileFlags);
                g_comboBox[CB_RESOURCES_VERSION_dwFileOS]->setValue(resourceVersion.fileInfo.dwFileOS);
                g_comboBox[CB_RESOURCES_VERSION_dwFileType]->setValue(resourceVersion.fileInfo.dwFileType);
                // TODO COMBOBOX dwFileSubtype
                addComment(ui->tableWidget_Resources_Version, N_IMAGE_RESOURCE_FIXEDFILEINFO::dwStrucVersion, HEADER_COLUMN_COMMENT,
                           XBinary::versionDwordToString(resourceVersion.fileInfo.dwStrucVersion));
                addComment(ui->tableWidget_Resources_Version, N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionMS, HEADER_COLUMN_COMMENT,
                           XBinary::versionDwordToString(resourceVersion.fileInfo.dwFileVersionMS));
                addComment(ui->tableWidget_Resources_Version, N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionLS, HEADER_COLUMN_COMMENT,
                           XBinary::versionDwordToString(resourceVersion.fileInfo.dwFileVersionLS));
                addComment(ui->tableWidget_Resources_Version, N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionMS, HEADER_COLUMN_COMMENT,
                           XBinary::versionDwordToString(resourceVersion.fileInfo.dwProductVersionMS));
                addComment(ui->tableWidget_Resources_Version, N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionLS, HEADER_COLUMN_COMMENT,
                           XBinary::versionDwordToString(resourceVersion.fileInfo.dwProductVersionLS));

                ui->textEditResources_Version->clear();

                qint32 nNumberOfResources = resourceVersion.listRecords.count();

                QString sVersion;

                for (qint32 i = 0; i < nNumberOfResources; i++) {
                    sVersion += resourceVersion.listRecords.at(i) + QString("\r\n");
                }

                ui->textEditResources_Version->setText(sVersion);

                blockSignals(false);
            }
        } else if (nType == SPE::TYPE_RESOURCES_MANIFEST) {
            if (!isInitPresent(sInit)) {
                formatXML();
            }
        } else if (nType == SPE::TYPE_EXCEPTION) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_EXCEPTION, &g_tvModel[SPE::TYPE_EXCEPTION], &pe, 0, 0, 0);

                ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_EXCEPTION], ui->tableView_Exceptions, nullptr, false);

                connect(ui->tableView_Exceptions->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Exceptions_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SPE::TYPE_EXCEPTION]->rowCount()) {
                    ui->tableView_Exceptions->setCurrentIndex(ui->tableView_Exceptions->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_RELOCS) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_RELOCS, &g_tvModel[SPE::TYPE_RELOCS], &pe, 0, 0, 0);

                ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_RELOCS], ui->tableView_Relocs, nullptr, false);

                connect(ui->tableView_Relocs->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Relocs_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SPE::TYPE_RELOCS]->rowCount()) {
                    ui->tableView_Relocs->setCurrentIndex(ui->tableView_Relocs->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_DEBUG) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_DEBUG, &g_tvModel[SPE::TYPE_DEBUG], &pe, 0, 0, 0);

                ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_DEBUG], ui->tableView_Debug, nullptr, false);

                connect(ui->tableView_Debug->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Debug_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SPE::TYPE_DEBUG]->rowCount()) {
                    ui->tableView_Debug->setCurrentIndex(ui->tableView_Debug->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_TLS) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SPE::TYPE_TLS, ui->tableWidget_TLS, pe.is64() ? (N_IMAGE_TLS::records64) : (N_IMAGE_TLS::records32), g_lineEdit_TLS,
                                  N_IMAGE_TLS::__data_size, 0);

                g_invWidget[INV_IMAGE_TLS_AddressOfCallBacks] = createInvWidget(ui->tableWidget_TLS, SPE::TYPE_TLS, N_IMAGE_TLS::AddressOfCallBacks, InvWidget::TYPE_HEX);
                g_invWidget[INV_IMAGE_TLS_AddressOfIndex] = createInvWidget(ui->tableWidget_TLS, SPE::TYPE_TLS, N_IMAGE_TLS::AddressOfIndex, InvWidget::TYPE_HEX);
                g_invWidget[INV_IMAGE_TLS_EndAddressOfRawData] = createInvWidget(ui->tableWidget_TLS, SPE::TYPE_TLS, N_IMAGE_TLS::EndAddressOfRawData, InvWidget::TYPE_HEX);
                g_invWidget[INV_IMAGE_TLS_StartAddressOfRawData] =
                    createInvWidget(ui->tableWidget_TLS, SPE::TYPE_TLS, N_IMAGE_TLS::StartAddressOfRawData, InvWidget::TYPE_HEX);

                blockSignals(true);

                XBinary::_MEMORY_MAP memoryMap = pe.getMemoryMap();

                if (bIs64) {
                    XPE_DEF::S_IMAGE_TLS_DIRECTORY64 tls64 = pe.getTLSDirectory64();
                    g_lineEdit_TLS[N_IMAGE_TLS::StartAddressOfRawData]->setValue(tls64.StartAddressOfRawData);
                    g_lineEdit_TLS[N_IMAGE_TLS::EndAddressOfRawData]->setValue(tls64.EndAddressOfRawData);
                    g_lineEdit_TLS[N_IMAGE_TLS::AddressOfIndex]->setValue(tls64.AddressOfIndex);
                    g_lineEdit_TLS[N_IMAGE_TLS::AddressOfCallBacks]->setValue(tls64.AddressOfCallBacks);
                    g_lineEdit_TLS[N_IMAGE_TLS::SizeOfZeroFill]->setValue(tls64.SizeOfZeroFill);
                    g_lineEdit_TLS[N_IMAGE_TLS::Characteristics]->setValue(tls64.Characteristics);

                    g_invWidget[INV_IMAGE_TLS_AddressOfCallBacks]->setAddressAndSize(&pe, tls64.AddressOfCallBacks, 0);
                    g_invWidget[INV_IMAGE_TLS_AddressOfIndex]->setAddressAndSize(&pe, tls64.AddressOfIndex, 0);
                    g_invWidget[INV_IMAGE_TLS_EndAddressOfRawData]->setAddressAndSize(&pe, tls64.EndAddressOfRawData, 0);
                    g_invWidget[INV_IMAGE_TLS_StartAddressOfRawData]->setAddressAndSize(&pe, tls64.StartAddressOfRawData, 0);

                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::AddressOfCallBacks, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByAddress(&memoryMap, tls64.AddressOfCallBacks));
                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::AddressOfIndex, HEADER_COLUMN_COMMENT, pe.getMemoryRecordInfoByAddress(&memoryMap, tls64.AddressOfIndex));
                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::EndAddressOfRawData, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByAddress(&memoryMap, tls64.EndAddressOfRawData));
                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::StartAddressOfRawData, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByAddress(&memoryMap, tls64.StartAddressOfRawData));
                } else {
                    XPE_DEF::S_IMAGE_TLS_DIRECTORY32 tls32 = pe.getTLSDirectory32();
                    g_lineEdit_TLS[N_IMAGE_TLS::StartAddressOfRawData]->setValue(tls32.StartAddressOfRawData);
                    g_lineEdit_TLS[N_IMAGE_TLS::EndAddressOfRawData]->setValue(tls32.EndAddressOfRawData);
                    g_lineEdit_TLS[N_IMAGE_TLS::AddressOfIndex]->setValue(tls32.AddressOfIndex);
                    g_lineEdit_TLS[N_IMAGE_TLS::AddressOfCallBacks]->setValue(tls32.AddressOfCallBacks);
                    g_lineEdit_TLS[N_IMAGE_TLS::SizeOfZeroFill]->setValue(tls32.SizeOfZeroFill);
                    g_lineEdit_TLS[N_IMAGE_TLS::Characteristics]->setValue(tls32.Characteristics);

                    g_invWidget[INV_IMAGE_TLS_AddressOfCallBacks]->setAddressAndSize(&pe, tls32.AddressOfCallBacks, 0);
                    g_invWidget[INV_IMAGE_TLS_AddressOfIndex]->setAddressAndSize(&pe, tls32.AddressOfIndex, 0);
                    g_invWidget[INV_IMAGE_TLS_EndAddressOfRawData]->setAddressAndSize(&pe, tls32.EndAddressOfRawData, 0);
                    g_invWidget[INV_IMAGE_TLS_StartAddressOfRawData]->setAddressAndSize(&pe, tls32.StartAddressOfRawData, 0);

                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::AddressOfCallBacks, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByAddress(&memoryMap, tls32.AddressOfCallBacks));
                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::AddressOfIndex, HEADER_COLUMN_COMMENT, pe.getMemoryRecordInfoByAddress(&memoryMap, tls32.AddressOfIndex));
                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::EndAddressOfRawData, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByAddress(&memoryMap, tls32.EndAddressOfRawData));
                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::StartAddressOfRawData, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByAddress(&memoryMap, tls32.StartAddressOfRawData));
                }

                qint64 nOffset = pe.getTLSHeaderOffset();
                qint64 nSize = pe.getTLSHeaderSize();
                qint64 nAddress = pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_TLS], ui->widgetHex_TLS);

                blockSignals(false);
            }
        } else if (nType == SPE::TYPE_TLSCALLBACKS) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_TLSCALLBACKS, &g_tvModel[SPE::TYPE_TLSCALLBACKS], &pe, 0, 0, 0);

                ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_TLSCALLBACKS], ui->tableView_TLSCallbacks, nullptr, true);

                // connect(ui->tableView_Sections->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_Sections_currentRowChanged(QModelIndex,QModelIndex)));

                if (g_tvModel[SPE::TYPE_TLSCALLBACKS]->rowCount()) {
                    ui->tableView_TLSCallbacks->setCurrentIndex(ui->tableView_TLSCallbacks->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_LOADCONFIG) {
            if (!isInitPresent(sInit)) {
                qint32 nHeaderSize = pe.getLoadConfig_Size();

                qint32 nRecordSize = N_IMAGE_LOADCONFIG::__data_size;

                const FW_DEF::HEADER_RECORD *pHeaderRecords = (bIs64 ? (N_IMAGE_LOADCONFIG::records64) : (N_IMAGE_LOADCONFIG::records32));

                for (qint32 i = 0; i < N_IMAGE_LOADCONFIG::__data_size; i++) {
                    if (nHeaderSize > (pHeaderRecords[i].nOffset)) {
                        nRecordSize = i + 1;
                    }
                }

                createHeaderTable(SPE::TYPE_LOADCONFIG, ui->tableWidget_LoadConfig, bIs64 ? (N_IMAGE_LOADCONFIG::records64) : (N_IMAGE_LOADCONFIG::records32),
                                  g_lineEdit_LoadConfig, nRecordSize, 0);

                if (nRecordSize > N_IMAGE_LOADCONFIG::SecurityCookie)
                    g_invWidget[INV_IMAGE_LOADCONFIG_SecurityCookie] =
                        createInvWidget(ui->tableWidget_LoadConfig, SPE::TYPE_LOADCONFIG, N_IMAGE_LOADCONFIG::SecurityCookie, InvWidget::TYPE_HEX);
                if (nRecordSize > N_IMAGE_LOADCONFIG::SEHandlerTable)
                    g_invWidget[INV_IMAGE_LOADCONFIG_SEHandlerTable] =
                        createInvWidget(ui->tableWidget_LoadConfig, SPE::TYPE_LOADCONFIG, N_IMAGE_LOADCONFIG::SEHandlerTable, InvWidget::TYPE_HEX);
                if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer)
                    g_invWidget[INV_IMAGE_LOADCONFIG_GuardCFCheckFunctionPointer] =
                        createInvWidget(ui->tableWidget_LoadConfig, SPE::TYPE_LOADCONFIG, N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer, InvWidget::TYPE_HEX);
                if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer)
                    g_invWidget[INV_IMAGE_LOADCONFIG_GuardCFDispatchFunctionPointer] =
                        createInvWidget(ui->tableWidget_LoadConfig, SPE::TYPE_LOADCONFIG, N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer, InvWidget::TYPE_HEX);
                if (nRecordSize > N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer)
                    g_invWidget[INV_IMAGE_LOADCONFIG_GuardMemcpyFunctionPointer] =
                        createInvWidget(ui->tableWidget_LoadConfig, SPE::TYPE_LOADCONFIG, N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer, InvWidget::TYPE_HEX);

                blockSignals(true);

                if (bIs64) {
                    XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY64 lc64 = pe.getLoadConfigDirectory64();

                    if (nRecordSize > N_IMAGE_LOADCONFIG::Size) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Size]->setValue(lc64.Size);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::TimeDateStamp) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::TimeDateStamp]->setValue(lc64.TimeDateStamp);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::MinorVersion) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MinorVersion]->setValue(lc64.MinorVersion);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::MajorVersion) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MajorVersion]->setValue(lc64.MajorVersion);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GlobalFlagsClear) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GlobalFlagsClear]->setValue(lc64.GlobalFlagsClear);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GlobalFlagsSet) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GlobalFlagsSet]->setValue(lc64.GlobalFlagsSet);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CriticalSectionDefaultTimeout)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CriticalSectionDefaultTimeout]->setValue(lc64.CriticalSectionDefaultTimeout);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DeCommitFreeBlockThreshold)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DeCommitFreeBlockThreshold]->setValue(lc64.DeCommitFreeBlockThreshold);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DeCommitTotalFreeThreshold)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DeCommitTotalFreeThreshold]->setValue(lc64.DeCommitTotalFreeThreshold);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::LockPrefixTable) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::LockPrefixTable]->setValue(lc64.LockPrefixTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::MaximumAllocationSize)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MaximumAllocationSize]->setValue(lc64.MaximumAllocationSize);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::VirtualMemoryThreshold)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::VirtualMemoryThreshold]->setValue(lc64.VirtualMemoryThreshold);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::ProcessAffinityMask)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::ProcessAffinityMask]->setValue(lc64.ProcessAffinityMask);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CSDVersion) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CSDVersion]->setValue(lc64.CSDVersion);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DependentLoadFlags) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DependentLoadFlags]->setValue(lc64.DependentLoadFlags);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::EditList) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::EditList]->setValue(lc64.EditList);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SecurityCookie) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SecurityCookie]->setValue(lc64.SecurityCookie);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SEHandlerTable) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SEHandlerTable]->setValue(lc64.SEHandlerTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SEHandlerCount) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SEHandlerCount]->setValue(lc64.SEHandlerCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer]->setValue(lc64.GuardCFCheckFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer]->setValue(lc64.GuardCFDispatchFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFFunctionTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFFunctionTable]->setValue(lc64.GuardCFFunctionTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFFunctionCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFFunctionCount]->setValue(lc64.GuardCFFunctionCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardFlags) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardFlags]->setValue(lc64.GuardFlags);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_Flags)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_Flags]->setValue(lc64.CodeIntegrity.Flags);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_Catalog)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_Catalog]->setValue(lc64.CodeIntegrity.Catalog);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_CatalogOffset)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_CatalogOffset]->setValue(lc64.CodeIntegrity.CatalogOffset);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_Reserved)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_Reserved]->setValue(lc64.CodeIntegrity.Reserved);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryTable]->setValue(lc64.GuardAddressTakenIatEntryTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryCount]->setValue(lc64.GuardAddressTakenIatEntryCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardLongJumpTargetTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardLongJumpTargetTable]->setValue(lc64.GuardLongJumpTargetTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardLongJumpTargetCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardLongJumpTargetCount]->setValue(lc64.GuardLongJumpTargetCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DynamicValueRelocTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DynamicValueRelocTable]->setValue(lc64.DynamicValueRelocTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CHPEMetadataPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CHPEMetadataPointer]->setValue(lc64.CHPEMetadataPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardRFFailureRoutine)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardRFFailureRoutine]->setValue(lc64.GuardRFFailureRoutine);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardRFFailureRoutineFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardRFFailureRoutineFunctionPointer]->setValue(lc64.GuardRFFailureRoutineFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DynamicValueRelocTableOffset)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DynamicValueRelocTableOffset]->setValue(lc64.DynamicValueRelocTableOffset);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DynamicValueRelocTableSection)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DynamicValueRelocTableSection]->setValue(lc64.DynamicValueRelocTableSection);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::Reserved2) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Reserved2]->setValue(lc64.Reserved2);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardRFVerifyStackPointerFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardRFVerifyStackPointerFunctionPointer]->setValue(lc64.GuardRFVerifyStackPointerFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::HotPatchTableOffset)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::HotPatchTableOffset]->setValue(lc64.HotPatchTableOffset);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::Reserved3) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Reserved3]->setValue(lc64.Reserved3);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::EnclaveConfigurationPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::EnclaveConfigurationPointer]->setValue(lc64.EnclaveConfigurationPointer);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::VolatileMetadataPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::VolatileMetadataPointer]->setValue(lc64.VolatileMetadataPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardEHContinuationTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardEHContinuationTable]->setValue(lc64.GuardEHContinuationTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardEHContinuationCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardEHContinuationCount]->setValue(lc64.GuardEHContinuationCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardXFGCheckFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardXFGCheckFunctionPointer]->setValue(lc64.GuardXFGCheckFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardXFGDispatchFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardXFGDispatchFunctionPointer]->setValue(lc64.GuardXFGDispatchFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardXFGTableDispatchFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardXFGTableDispatchFunctionPointer]->setValue(lc64.GuardXFGTableDispatchFunctionPointer);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::CastGuardOsDeterminedFailureMode)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CastGuardOsDeterminedFailureMode]->setValue(lc64.CastGuardOsDeterminedFailureMode);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer]->setValue(lc64.GuardMemcpyFunctionPointer);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::Size)
                        addComment(ui->tableWidget_LoadConfig, N_IMAGE_LOADCONFIG::Size, HEADER_COLUMN_COMMENT, XBinary::bytesCountToString(lc64.Size));

                    if (nRecordSize > N_IMAGE_LOADCONFIG::SecurityCookie) g_invWidget[INV_IMAGE_LOADCONFIG_SecurityCookie]->setAddressAndSize(&pe, lc64.SecurityCookie, 0);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SEHandlerTable) g_invWidget[INV_IMAGE_LOADCONFIG_SEHandlerTable]->setAddressAndSize(&pe, lc64.SEHandlerTable, 0);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer)
                        g_invWidget[INV_IMAGE_LOADCONFIG_GuardCFCheckFunctionPointer]->setAddressAndSize(&pe, lc64.GuardCFCheckFunctionPointer, 0);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer)
                        g_invWidget[INV_IMAGE_LOADCONFIG_GuardCFDispatchFunctionPointer]->setAddressAndSize(&pe, lc64.GuardCFDispatchFunctionPointer, 0);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer)
                        g_invWidget[INV_IMAGE_LOADCONFIG_GuardMemcpyFunctionPointer]->setAddressAndSize(&pe, lc64.GuardMemcpyFunctionPointer, 0);
                } else {
                    XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32 lc32 = pe.getLoadConfigDirectory32();

                    if (nRecordSize > N_IMAGE_LOADCONFIG::Size) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Size]->setValue(lc32.Size);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::TimeDateStamp) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::TimeDateStamp]->setValue(lc32.TimeDateStamp);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::MinorVersion) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MinorVersion]->setValue(lc32.MinorVersion);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::MajorVersion) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MajorVersion]->setValue(lc32.MajorVersion);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GlobalFlagsClear) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GlobalFlagsClear]->setValue(lc32.GlobalFlagsClear);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GlobalFlagsSet) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GlobalFlagsSet]->setValue(lc32.GlobalFlagsSet);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CriticalSectionDefaultTimeout)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CriticalSectionDefaultTimeout]->setValue(lc32.CriticalSectionDefaultTimeout);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DeCommitFreeBlockThreshold)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DeCommitFreeBlockThreshold]->setValue(lc32.DeCommitFreeBlockThreshold);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DeCommitTotalFreeThreshold)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DeCommitTotalFreeThreshold]->setValue(lc32.DeCommitTotalFreeThreshold);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::LockPrefixTable) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::LockPrefixTable]->setValue(lc32.LockPrefixTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::MaximumAllocationSize)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MaximumAllocationSize]->setValue(lc32.MaximumAllocationSize);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::VirtualMemoryThreshold)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::VirtualMemoryThreshold]->setValue(lc32.VirtualMemoryThreshold);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::ProcessAffinityMask)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::ProcessAffinityMask]->setValue(lc32.ProcessAffinityMask);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CSDVersion) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CSDVersion]->setValue(lc32.CSDVersion);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DependentLoadFlags) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DependentLoadFlags]->setValue(lc32.DependentLoadFlags);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::EditList) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::EditList]->setValue(lc32.EditList);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SecurityCookie) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SecurityCookie]->setValue(lc32.SecurityCookie);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SEHandlerTable) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SEHandlerTable]->setValue(lc32.SEHandlerTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SEHandlerCount) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SEHandlerCount]->setValue(lc32.SEHandlerCount);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer]->setValue(lc32.GuardCFCheckFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer]->setValue(lc32.GuardCFDispatchFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFFunctionTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFFunctionTable]->setValue(lc32.GuardCFFunctionTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFFunctionCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFFunctionCount]->setValue(lc32.GuardCFFunctionCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardFlags) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardFlags]->setValue(lc32.GuardFlags);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_Flags)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_Flags]->setValue(lc32.CodeIntegrity.Flags);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_Catalog)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_Catalog]->setValue(lc32.CodeIntegrity.Catalog);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_CatalogOffset)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_CatalogOffset]->setValue(lc32.CodeIntegrity.CatalogOffset);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_Reserved)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_Reserved]->setValue(lc32.CodeIntegrity.Reserved);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryTable]->setValue(lc32.GuardAddressTakenIatEntryTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryCount]->setValue(lc32.GuardAddressTakenIatEntryCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardLongJumpTargetTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardLongJumpTargetTable]->setValue(lc32.GuardLongJumpTargetTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardLongJumpTargetCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardLongJumpTargetCount]->setValue(lc32.GuardLongJumpTargetCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DynamicValueRelocTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DynamicValueRelocTable]->setValue(lc32.DynamicValueRelocTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CHPEMetadataPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CHPEMetadataPointer]->setValue(lc32.CHPEMetadataPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardRFFailureRoutine)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardRFFailureRoutine]->setValue(lc32.GuardRFFailureRoutine);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardRFFailureRoutineFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardRFFailureRoutineFunctionPointer]->setValue(lc32.GuardRFFailureRoutineFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DynamicValueRelocTableOffset)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DynamicValueRelocTableOffset]->setValue(lc32.DynamicValueRelocTableOffset);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DynamicValueRelocTableSection)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DynamicValueRelocTableSection]->setValue(lc32.DynamicValueRelocTableSection);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::Reserved2) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Reserved2]->setValue(lc32.Reserved2);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardRFVerifyStackPointerFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardRFVerifyStackPointerFunctionPointer]->setValue(lc32.GuardRFVerifyStackPointerFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::HotPatchTableOffset)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::HotPatchTableOffset]->setValue(lc32.HotPatchTableOffset);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::Reserved3) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Reserved3]->setValue(lc32.Reserved3);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::EnclaveConfigurationPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::EnclaveConfigurationPointer]->setValue(lc32.EnclaveConfigurationPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::VolatileMetadataPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::VolatileMetadataPointer]->setValue(lc32.VolatileMetadataPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardEHContinuationTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardEHContinuationTable]->setValue(lc32.GuardEHContinuationTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardEHContinuationCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardEHContinuationCount]->setValue(lc32.GuardEHContinuationCount);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardXFGCheckFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardXFGCheckFunctionPointer]->setValue(lc32.GuardXFGCheckFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardXFGDispatchFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardXFGDispatchFunctionPointer]->setValue(lc32.GuardXFGDispatchFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardXFGTableDispatchFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardXFGTableDispatchFunctionPointer]->setValue(lc32.GuardXFGTableDispatchFunctionPointer);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::CastGuardOsDeterminedFailureMode)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CastGuardOsDeterminedFailureMode]->setValue(lc32.CastGuardOsDeterminedFailureMode);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer]->setValue(lc32.GuardMemcpyFunctionPointer);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::Size)
                        addComment(ui->tableWidget_LoadConfig, N_IMAGE_LOADCONFIG::Size, HEADER_COLUMN_COMMENT, XBinary::bytesCountToString(lc32.Size));

                    if (nRecordSize > N_IMAGE_LOADCONFIG::SecurityCookie) g_invWidget[INV_IMAGE_LOADCONFIG_SecurityCookie]->setAddressAndSize(&pe, lc32.SecurityCookie, 0);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SEHandlerTable) g_invWidget[INV_IMAGE_LOADCONFIG_SEHandlerTable]->setAddressAndSize(&pe, lc32.SEHandlerTable, 0);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer)
                        g_invWidget[INV_IMAGE_LOADCONFIG_GuardCFCheckFunctionPointer]->setAddressAndSize(&pe, lc32.GuardCFCheckFunctionPointer, 0);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer)
                        g_invWidget[INV_IMAGE_LOADCONFIG_GuardCFDispatchFunctionPointer]->setAddressAndSize(&pe, lc32.GuardCFDispatchFunctionPointer, 0);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer)
                        g_invWidget[INV_IMAGE_LOADCONFIG_GuardMemcpyFunctionPointer]->setAddressAndSize(&pe, lc32.GuardMemcpyFunctionPointer, 0);
                }

                qint64 nOffset = pe.getLoadConfigDirectoryOffset();
                qint64 nSize = pe.getLoadConfigDirectorySize();
                qint64 nAddress = pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_LOADCONFIG], ui->widgetHex_LoadConfig);

                blockSignals(false);
            }
        } else if (nType == SPE::TYPE_BOUNDIMPORT) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_BOUNDIMPORT, &g_tvModel[SPE::TYPE_BOUNDIMPORT], &pe, 0, 0, 0);

                ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_BOUNDIMPORT], ui->tableView_BoundImport);

                if (g_tvModel[SPE::TYPE_BOUNDIMPORT]->rowCount()) {
                    ui->tableView_BoundImport->setCurrentIndex(ui->tableView_BoundImport->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_DELAYIMPORT) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_DELAYIMPORT, &g_tvModel[SPE::TYPE_DELAYIMPORT], &pe, 0, 0, 0);

                ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_DELAYIMPORT], ui->tableView_DelayImportLibraries);

                connect(ui->tableView_DelayImportLibraries->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_DelayImportLibraries_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SPE::TYPE_DELAYIMPORT]->rowCount()) {
                    ui->tableView_DelayImportLibraries->setCurrentIndex(ui->tableView_DelayImportLibraries->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_NETHEADER) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SPE::TYPE_NETHEADER, ui->tableWidget_NetHeader, N_IMAGE_NETHEADER::records, g_lineEdit_NetHeader, N_IMAGE_NETHEADER::__data_size, 0);

                g_comboBox[CB_IMAGE_NETHEADER_FLAGS] =
                    createComboBox(ui->tableWidget_NetHeader, XPE::getComImageFlagsS(), SPE::TYPE_NETHEADER, N_IMAGE_NETHEADER::Flags, XComboBoxEx::CBTYPE_FLAGS);

                blockSignals(true);

                XPE_DEF::IMAGE_COR20_HEADER netHeader = pe.getNetHeader();

                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::cb]->setValue(netHeader.cb);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::MajorRuntimeVersion]->setValue(netHeader.MajorRuntimeVersion);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::MinorRuntimeVersion]->setValue(netHeader.MinorRuntimeVersion);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::MetaData_Address]->setValue(netHeader.MetaData.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::MetaData_Size]->setValue(netHeader.MetaData.Size);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::Flags]->setValue(netHeader.Flags);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::EntryPoint]->setValue(netHeader.EntryPointRVA);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::Resources_Address]->setValue(netHeader.Resources.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::Resources_Size]->setValue(netHeader.Resources.Size);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::StrongNameSignature_Address]->setValue(netHeader.StrongNameSignature.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::StrongNameSignature_Size]->setValue(netHeader.StrongNameSignature.Size);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::CodeManagerTable_Address]->setValue(netHeader.CodeManagerTable.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::CodeManagerTable_Size]->setValue(netHeader.CodeManagerTable.Size);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::VTableFixups_Address]->setValue(netHeader.VTableFixups.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::VTableFixups_Size]->setValue(netHeader.VTableFixups.Size);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::ExportAddressTableJumps_Address]->setValue(netHeader.ExportAddressTableJumps.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::ExportAddressTableJumps_Size]->setValue(netHeader.ExportAddressTableJumps.Size);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::ManagedNativeHeader_Address]->setValue(netHeader.ManagedNativeHeader.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::ManagedNativeHeader_Size]->setValue(netHeader.ManagedNativeHeader.Size);

                g_comboBox[CB_IMAGE_NETHEADER_FLAGS]->setValue(netHeader.Flags);

                qint64 nOffset = pe.getNetHeaderOffset();
                qint64 nSize = pe.getNetHeaderSize();
                qint64 nAddress = pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_NETHEADER], ui->widgetHex_NetHeader);

                blockSignals(false);
            }
        } else if (nType == SPE::TYPE_NET_METADATA) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SPE::TYPE_NET_METADATA, ui->tableWidget_Net_Metadata, N_IMAGE_NET_METADATA::records, g_lineEdit_Net_Metadata,
                                  N_IMAGE_NET_METADATA::__data_size, 0);

                blockSignals(true);

                XBinary::OFFSETSIZE osMetadata = pe.getNet_MetadataOffsetSize();

                XPE::CLI_METADATA_HEADER header = pe._read_MetadataHeader(osMetadata.nOffset);

                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::Signature]->setValue(header.nSignature);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::MajorVersion]->setValue(header.nMajorVersion);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::MinorVersion]->setValue(header.nMinorVersion);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::Reserved]->setValue(header.nReserved);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::VersionStringLength]->setValue(header.nVersionStringLength);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::Version]->setStringValue(header.sVersion, header.nVersionStringLength);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::Flags]->setValue(header.nFlags);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::Streams]->setValue(header.nStreams);

                updateTableRecord(ui->tableWidget_Net_Metadata, N_IMAGE_NET_METADATA::Version, 16, header.nVersionStringLength);
                updateTableRecord(ui->tableWidget_Net_Metadata, N_IMAGE_NET_METADATA::Flags, 16 + header.nVersionStringLength, 2);
                updateTableRecord(ui->tableWidget_Net_Metadata, N_IMAGE_NET_METADATA::Streams, 16 + header.nVersionStringLength + 2, 2);

                qint64 nOffset = osMetadata.nOffset;
                qint64 nSize = osMetadata.nSize;
                qint64 nAddress = pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_NET_METADATA], ui->widgetHex_Net_Metadata);

                blockSignals(false);
            }
        } else if (nType == SPE::TYPE_NET_METADATA_STREAM) {
            if (!isInitPresent(sInit)) {
                qint64 nOffset = nDataOffset;
                qint64 nSize = nDataSize;
                qint64 nAddress = pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_NET_METADATA_STREAM], ui->widgetHex_Net_Metadata_Stream);
            }
        } else if (nType == SPE::TYPE_CERTIFICATE) {
            if (!isInitPresent(sInit)) {
                // TODO

                PEProcessData peProcessData(SPE::TYPE_CERTIFICATE, &g_tvModel[SPE::TYPE_CERTIFICATE], &pe, 0, 0, 0);

                ajustTreeView(&peProcessData, &g_tvModel[SPE::TYPE_CERTIFICATE], ui->treeView_Certificate);

                connect(ui->treeView_Certificate->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTreeView_Certificate_currentRowChanged(QModelIndex, QModelIndex)));
            }
        } else if (nType == SPE::TYPE_OVERLAY) {
            if (!isInitPresent(sInit)) {
                qint64 nOverLayOffset = pe.getOverlayOffset();
                qint64 nOverlaySize = pe.getOverlaySize();

                loadHexSubdevice(nOverLayOffset, nOverlaySize, nOverLayOffset, &g_subDevice[SPE::TYPE_OVERLAY], ui->widgetHex_Overlay, true);
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    addInit(sInit);
}

void PEWidget::on_tableView_Sections_customContextMenuRequested(const QPoint &pos)
{
    int nRow = ui->tableView_Sections->currentIndex().row();

    if (nRow != -1) {
        bool bIsEnable = getTableViewItemSize(ui->tableView_Sections);

        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editSectionHeader()));
        contextMenu.addAction(&actionEdit);

        QAction actionHex(tr("Hex"), this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(sectionHex()));
        actionHex.setEnabled(bIsEnable);
        contextMenu.addAction(&actionHex);

        QAction actionDisasm(tr("Disasm"), this);
        connect(&actionDisasm, SIGNAL(triggered()), this, SLOT(sectionDisasm()));
        actionDisasm.setEnabled(bIsEnable);
        contextMenu.addAction(&actionDisasm);

        QAction actionEntropy(tr("Entropy"), this);
        connect(&actionEntropy, SIGNAL(triggered()), this, SLOT(sectionEntropy()));
        actionEntropy.setEnabled(bIsEnable);
        contextMenu.addAction(&actionEntropy);

        QAction actionDump(tr("Dump to file"), this);
        connect(&actionDump, SIGNAL(triggered()), this, SLOT(sectionDump()));
        actionDump.setEnabled(bIsEnable);
        contextMenu.addAction(&actionDump);

        contextMenu.exec(ui->tableView_Sections->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::loadImportLibrary(int nRow)
{
    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (pe.isValid()) {
        PEProcessData peProcessData(SPE::TYPE_IMPORT_FUNCTION, &g_tvModel[SPE::TYPE_IMPORT_FUNCTION], &pe, nRow, 0, 0);

        ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_IMPORT_FUNCTION], ui->tableView_ImportFunctions);

        if (g_tvModel[SPE::TYPE_IMPORT_FUNCTION]->rowCount()) {
            ui->tableView_ImportFunctions->setCurrentIndex(ui->tableView_ImportFunctions->model()->index(0, 0));
        }
    }
}

void PEWidget::loadRelocs(int nRow)
{
    QModelIndex index = ui->tableView_Relocs->model()->index(nRow, 0);

    qint64 nOffset = ui->tableView_Relocs->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();

    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (pe.isValid()) {
        PEProcessData peProcessData(SPE::TYPE_RELOCS_POSITION, &g_tvModel[SPE::TYPE_RELOCS_POSITION], &pe, 0, nOffset, 0);

        ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_RELOCS_POSITION], ui->tableView_RelocsPositions, nullptr, false);

        if (g_tvModel[SPE::TYPE_RELOCS_POSITION]->rowCount()) {
            ui->tableView_RelocsPositions->setCurrentIndex(ui->tableView_RelocsPositions->model()->index(0, 0));
        }
    }
}

void PEWidget::loadSection(int nRow)
{
    loadHexSubdeviceByTableView(nRow, SPE::TYPE_SECTIONS, ui->widgetHex_Section, ui->tableView_Sections, &g_subDevice[SPE::TYPE_SECTIONS]);
}

void PEWidget::loadException(int nRow)
{
    loadHexSubdeviceByTableView(nRow, SPE::TYPE_EXCEPTION, ui->widgetHex_Exception, ui->tableView_Exceptions, &g_subDevice[SPE::TYPE_EXCEPTION]);
}

void PEWidget::loadDirectory(int nRow)
{
    qint64 nOffset = ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(nRow, 0)->data(Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nSize = ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(nRow, 0)->data(Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();
    XADDR nAddress = ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(nRow, 0)->data(Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

    loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_IMAGE_DIRECTORY_ENTRIES], ui->widgetHex_IMAGE_DIRECTORY_ENTRIES);
}

void PEWidget::loadDebug(int nRow)
{
    loadHexSubdeviceByTableView(nRow, SPE::TYPE_DEBUG, ui->widgetHex_Debug, ui->tableView_Debug, &g_subDevice[SPE::TYPE_DEBUG]);
}

void PEWidget::loadDelayImport(int nRow)
{
    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (pe.isValid()) {
        PEProcessData peProcessData(SPE::TYPE_DELAYIMPORT_FUNCTION, &g_tvModel[SPE::TYPE_DELAYIMPORT_FUNCTION], &pe, nRow, 0, 0);

        ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_DELAYIMPORT_FUNCTION], ui->tableView_DelayImportFunctions);

        if (g_tvModel[SPE::TYPE_DELAYIMPORT]->rowCount()) {
            ui->tableView_DelayImportFunctions->setCurrentIndex(ui->tableView_DelayImportFunctions->model()->index(0, 0));
        }
    }
}

void PEWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    XBinary::MODE mode = XPE::getMode(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET, getColumnWidth(this, CW_UINT16, mode));
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE, getColumnWidth(this, CW_TYPE, mode));

    switch (nType) {
        case SPE::TYPE_IMAGE_DOS_HEADER:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_IMAGE_NT_HEADERS:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_IMAGE_FILE_HEADER:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINTMODE, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_EXPORT:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINTMODE, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_TLS:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINTMODE, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_LOADCONFIG:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINTMODE, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_NETHEADER:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;

        case SPE::TYPE_RESOURCES_VERSION:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGMID, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;
    }
}

QString PEWidget::typeIdToString(int nType)
{
    QString sResult = tr("Unknown");

    switch (nType) {
        case SPE::TYPE_IMAGE_SECTION_HEADER:
            sResult = QString("Section %1").arg(tr("Header"));
            break;
        case SPE::TYPE_IMPORT:
            sResult = QString("Import %1").arg(tr("Header"));
            break;
        case SPE::TYPE_DEBUG:
            sResult = QString("Debug %1").arg(tr("Header"));
            break;
        case SPE::TYPE_EXCEPTION:
            sResult = QString("Exception %1").arg(tr("Header"));
            break;
        case SPE::TYPE_RELOCS:
            sResult = QString("Relocs %1").arg(tr("Header"));
            break;
        case SPE::TYPE_DELAYIMPORT:
            sResult = QString("Delay import %1").arg(tr("Header"));
            break;
        case SPE::TYPE_BOUNDIMPORT:
            sResult = QString("Bound import %1").arg(tr("Header"));
            break;
    }

    return sResult;
}

void PEWidget::_showInDisasmWindowAddress(XADDR nAddress)
{
    setTreeItem(ui->treeWidgetNavi, SPE::TYPE_DISASM);
    ui->widgetDisasm->goToAddress(nAddress);
}

void PEWidget::_showInDisasmWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SPE::TYPE_DISASM);
    ui->widgetDisasm->goToOffset(nOffset);
}

void PEWidget::_showInMemoryMapWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SPE::TYPE_MEMORYMAP);
    ui->widgetMemoryMap->goToOffset(nOffset);
}

void PEWidget::_showInHexWindow(qint64 nOffset, qint64 nSize)
{
    setTreeItem(ui->treeWidgetNavi, SPE::TYPE_HEX);
    ui->widgetHex->setSelection(nOffset, nSize);
}

bool PEWidget::createSectionTable(int nType, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, int nNumberOfRecords)
{
    qint32 nSymbolWidth = XLineEditHEX::getSymbolWidth(this);

    QStringList slHeader;

    switch (nType) {
        case SPE::TYPE_IMAGE_DIRECTORY_ENTRIES:
            pTableWidget->setColumnCount(nNumberOfRecords + 4);
            pTableWidget->setColumnWidth(0, nSymbolWidth * 3);
            pTableWidget->setColumnWidth(1, nSymbolWidth * 12);
            pTableWidget->setColumnWidth(2, nSymbolWidth * 8);
            pTableWidget->setColumnWidth(3, nSymbolWidth * 8);
            pTableWidget->setColumnWidth(4, nSymbolWidth * 8);
            slHeader.append("");
            slHeader.append("");
            break;

        default:
            pTableWidget->setColumnCount(nNumberOfRecords);
    }

    pTableWidget->setRowCount(0);

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        slHeader.append(pRecords[i].sName);
    }

    switch (nType) {
        case SPE::TYPE_IMAGE_DIRECTORY_ENTRIES:
            slHeader.append("");
            slHeader.append("");
            break;
    }

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    switch (nType) {
        case SPE::TYPE_IMAGE_DIRECTORY_ENTRIES:
            pTableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
            break;
    }

    return true;
}

void PEWidget::on_pushButtonReload_clicked()
{
    ui->pushButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000, this, SLOT(enableButton()));
}

void PEWidget::enableButton()
{
    ui->pushButtonReload->setEnabled(true);
}

void PEWidget::on_tableWidget_IMAGE_DIRECTORY_ENTRIES_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    if (nCurrentRow != -1) {
        loadDirectory(nCurrentRow);
    }
}

void PEWidget::on_tableWidget_IMAGE_DOS_HEADER_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex_IMAGE_DOS_HEADER, ui->tableWidget_IMAGE_DOS_HEADER);
}

void PEWidget::on_tableWidget_IMAGE_NT_HEADERS_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex_IMAGE_NT_HEADERS, ui->tableWidget_IMAGE_NT_HEADERS);
}

void PEWidget::on_tableWidget_IMAGE_FILE_HEADER_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex_IMAGE_FILE_HEADER, ui->tableWidget_IMAGE_FILE_HEADER);
}

void PEWidget::on_tableWidget_IMAGE_OPTIONAL_HEADER_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex_IMAGE_OPTIONAL_HEADER, ui->tableWidget_IMAGE_OPTIONAL_HEADER);
}

void PEWidget::on_tableWidget_LoadConfig_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex_LoadConfig, ui->tableWidget_LoadConfig);
}

void PEWidget::on_tableWidget_NetHeader_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex_NetHeader, ui->tableWidget_NetHeader);
}

void PEWidget::on_tableWidget_TLS_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex_TLS, ui->tableWidget_TLS);
}

void PEWidget::editImportHeader()
{
    showSectionHeader(SPE::TYPE_IMPORT, ui->tableView_ImportLibraries);
}

void PEWidget::editDebugHeader()
{
    showSectionHeader(SPE::TYPE_DEBUG, ui->tableView_Debug);
}

void PEWidget::editRelocsHeader()
{
    showSectionHeader(SPE::TYPE_RELOCS, ui->tableView_Relocs);
}

void PEWidget::editExceptionHeader()
{
    showSectionHeader(SPE::TYPE_EXCEPTION, ui->tableView_Exceptions);
}

void PEWidget::editDelayImportHeader()
{
    showSectionHeader(SPE::TYPE_DELAYIMPORT, ui->tableView_DelayImportLibraries);
}

void PEWidget::editBoundImportHeader()
{
    showSectionHeader(SPE::TYPE_BOUNDIMPORT, ui->tableView_BoundImport);
}

void PEWidget::exportFunctionHex()
{
    showSectionHex(ui->tableView_ExportFunctions);
}

void PEWidget::exportFunctionDisasm()
{
    showSectionDisasm(ui->tableView_ExportFunctions);
}

void PEWidget::exportFunctionDemangle()
{
    showTableViewDemangle(ui->tableView_ExportFunctions, N_IMAGE_EXPORT_FUNCTION::Name + 1);
}

void PEWidget::importFunctionDemangle()
{
    showTableViewDemangle(ui->tableView_ImportFunctions, N_IMAGE_IMPORT_FUNCTION::Hint + 2);
}

void PEWidget::showSectionHeader(int nType, QTableView *pTableView)
{
    int nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET).toLongLong();

        PESectionHeaderWidget *pSectionHeaderWidget = new PESectionHeaderWidget(getDevice(), getOptions(), (quint32)nRow, nOffset, nType, this);

        DialogSectionHeader dsh(this);
        dsh.setWidget(pSectionHeaderWidget);
        dsh.setData(typeIdToString(nType));
        dsh.setGlobal(getShortcuts(), getGlobalOptions());

        connect(&dsh, SIGNAL(changed()), this, SLOT(setEdited()));

        dsh.exec();

        reloadData();

        pTableView->setCurrentIndex(pTableView->model()->index(nRow, 0));
    }
}

void PEWidget::on_tableView_Sections_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editSectionHeader();
}

void PEWidget::on_tableView_ImportLibraries_customContextMenuRequested(const QPoint &pos)
{
    int nRow = ui->tableView_ImportLibraries->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editImportHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_ImportLibraries->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::onTableView_Sections_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SPE::TYPE_SECTIONS, ui->widgetHex_Section, ui->tableView_Sections, &g_subDevice[SPE::TYPE_SECTIONS]);
}

void PEWidget::onTableView_ImportLibraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    int nRow = current.row();

    if (nRow != -1) {
        loadImportLibrary(nRow);
    }
}

void PEWidget::on_tableView_ImportLibraries_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editImportHeader();
}

void PEWidget::on_tableView_Relocs_customContextMenuRequested(const QPoint &pos)
{
    int nRow = ui->tableView_Relocs->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editRelocsHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_Relocs->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::onTableView_Relocs_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    int nRow = current.row();

    if (nRow != -1) {
        loadRelocs(nRow);
    }
}

void PEWidget::on_tableView_Relocs_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editRelocsHeader();
}

void PEWidget::on_tableView_Debug_customContextMenuRequested(const QPoint &pos)
{
    int nRow = ui->tableView_Debug->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editDebugHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_Debug->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::onTableView_Debug_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    int nRow = current.row();

    if (nRow != -1) {
        loadDebug(nRow);
    }
}

void PEWidget::on_tableView_Debug_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editDebugHeader();
}

void PEWidget::on_tableView_Resources_customContextMenuRequested(const QPoint &pos)
{
    int nRow = ui->tableView_Resources->currentIndex().row();

    if (nRow != -1) {
        bool bIsEnable = getTableViewItemSize(ui->tableView_Resources);

        QMenu contextMenu(this);

        QAction actionHex(tr("Hex"), this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(resourceHex()));
        actionHex.setEnabled(bIsEnable);
        contextMenu.addAction(&actionHex);

        QAction actionDisasm(tr("Disasm"), this);
        connect(&actionDisasm, SIGNAL(triggered()), this, SLOT(resourceDisasm()));
        actionDisasm.setEnabled(bIsEnable);
        contextMenu.addAction(&actionDisasm);

        QAction actionEntropy(tr("Entropy"), this);
        connect(&actionEntropy, SIGNAL(triggered()), this, SLOT(resourceEntropy()));
        actionEntropy.setEnabled(bIsEnable);
        contextMenu.addAction(&actionEntropy);

        QAction actionDump(tr("Dump to file"), this);
        connect(&actionDump, SIGNAL(triggered()), this, SLOT(resourceDump()));
        actionDump.setEnabled(bIsEnable);
        contextMenu.addAction(&actionDump);

        // TODO Add scan

        contextMenu.exec(ui->tableView_Resources->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::onTableView_Resources_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SPE::TYPE_RESOURCES, ui->widgetHex_Resources, ui->tableView_Resources, &g_subDevice[SPE::TYPE_RESOURCES]);
}

void PEWidget::on_tableView_Exceptions_customContextMenuRequested(const QPoint &pos)
{
    int nRow = ui->tableView_Exceptions->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editExceptionHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_Exceptions->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::onTableView_Exceptions_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    int nRow = current.row();

    if (nRow != -1) {
        loadException(nRow);
    }
}

void PEWidget::on_tableView_Exceptions_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editExceptionHeader();
}

void PEWidget::on_tableView_DelayImportLibraries_customContextMenuRequested(const QPoint &pos)
{
    int nRow = ui->tableView_DelayImportLibraries->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editDelayImportHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_DelayImportLibraries->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::onTableView_DelayImportLibraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    int nRow = current.row();

    if (nRow != -1) {
        loadDelayImport(nRow);
    }
}

void PEWidget::on_tableView_DelayImportLibraries_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editDelayImportHeader();
}

void PEWidget::on_tableView_BoundImport_customContextMenuRequested(const QPoint &pos)
{
    int nRow = ui->tableView_BoundImport->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editBoundImportHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_BoundImport->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::on_tableView_BoundImport_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)

    editBoundImportHeader();
}

void PEWidget::onTreeView_Resources_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    if (current.row() != -1) {
        QString sID1 = ui->treeView_Resources->model()->data(current, Qt::UserRole + FW_DEF::SECTION_DATA_VALUE1).toString();
        QString sID2 = ui->treeView_Resources->model()->data(current, Qt::UserRole + FW_DEF::SECTION_DATA_VALUE2).toString();
        QString sID3 = ui->treeView_Resources->model()->data(current, Qt::UserRole + FW_DEF::SECTION_DATA_VALUE3).toString();
        qint64 nOffset = ui->treeView_Resources->model()->data(current, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = ui->treeView_Resources->model()->data(current, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();
        XADDR nAddress = ui->treeView_Resources->model()->data(current, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        g_lineEdit_Resources[N_IMAGE_RESOURCES::ID1]->setStringValue(sID1);
        g_lineEdit_Resources[N_IMAGE_RESOURCES::ID2]->setStringValue(sID2);
        g_lineEdit_Resources[N_IMAGE_RESOURCES::ID3]->setStringValue(sID3);
        g_lineEdit_Resources[N_IMAGE_RESOURCES::ADDRESS]->setValue((quint32)nAddress);
        g_lineEdit_Resources[N_IMAGE_RESOURCES::OFFSET]->setValue((quint32)nOffset);
        g_lineEdit_Resources[N_IMAGE_RESOURCES::SIZE]->setValue((quint32)nSize);

        loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_RESOURCES], ui->widgetHex_Resources);
    }
}

void PEWidget::onTreeView_Certificate_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    if (current.row() != -1) {
        qint64 nOffset = ui->treeView_Certificate->model()->data(current, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = ui->treeView_Certificate->model()->data(current, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();

        loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SPE::TYPE_CERTIFICATE], ui->widgetHex_Certificate, true);
    }
}

void PEWidget::on_tableView_ExportFunctions_customContextMenuRequested(const QPoint &pos)
{
    int nRow = ui->tableView_ExportFunctions->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionHex(tr("Hex"), this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(exportFunctionHex()));
        contextMenu.addAction(&actionHex);

        QAction actionDisasm(tr("Disasm"), this);
        connect(&actionDisasm, SIGNAL(triggered()), this, SLOT(exportFunctionDisasm()));
        contextMenu.addAction(&actionDisasm);

        QAction actionDemangle(tr("Demangle"), this);
        connect(&actionDemangle, SIGNAL(triggered()), this, SLOT(exportFunctionDemangle()));
        contextMenu.addAction(&actionDemangle);

        contextMenu.exec(ui->tableView_ExportFunctions->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::on_pushButtonHex_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SPE::TYPE_HEX);
}

void PEWidget::on_pushButtonStrings_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SPE::TYPE_STRINGS);
}

void PEWidget::on_pushButtonMemoryMap_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SPE::TYPE_MEMORYMAP);
}

void PEWidget::on_pushButtonEntropy_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SPE::TYPE_ENTROPY);
}

void PEWidget::on_pushButtonHeuristicScan_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SPE::TYPE_HEURISTICSCAN);
}

void PEWidget::on_pushButtonDisasm_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SPE::TYPE_DISASM);
}

void PEWidget::on_tableWidget_Net_Metadata_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow)
    Q_UNUSED(nCurrentColumn)
    Q_UNUSED(nPreviousRow)
    Q_UNUSED(nPreviousColumn)

    setHeaderTableSelection(ui->widgetHex_Net_Metadata, ui->tableWidget_Net_Metadata);
}

void PEWidget::on_toolButtonPrev_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getPrevPage());
    setAddPageEnabled(true);
}

void PEWidget::on_toolButtonNext_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getNextPage());
    setAddPageEnabled(true);
}

void PEWidget::on_checkBoxExportShowValid_stateChanged(int nState)
{
    Q_UNUSED(nState)

    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (pe.isValid()) {
        PEProcessData peProcessData(SPE::TYPE_EXPORT_FUNCTION, &g_tvModel[SPE::TYPE_EXPORT_FUNCTION], &pe, 0, 0, 0, ui->checkBoxExportShowValid->isChecked());

        ajustTableView(&peProcessData, &g_tvModel[SPE::TYPE_EXPORT_FUNCTION], ui->tableView_ExportFunctions);
    }
}

void PEWidget::on_tableView_ImportFunctions_customContextMenuRequested(const QPoint &pos)
{
    int nRow = ui->tableView_ImportFunctions->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionDemangle(tr("Demangle"), this);
        connect(&actionDemangle, SIGNAL(triggered()), this, SLOT(importFunctionDemangle()));
        contextMenu.addAction(&actionDemangle);

        contextMenu.exec(ui->tableView_ImportFunctions->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::on_pushButtonCertificateCheck_clicked()
{
    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (pe.isValid()) {
        PEProcessData peProcessData(SPE::TYPE_CERTIFICATE_CHECK, &g_tvModel[SPE::TYPE_CERTIFICATE_CHECK], &pe, 0, 0, 0);

        ajustDialogModel(&peProcessData, &g_tvModel[SPE::TYPE_CERTIFICATE_CHECK], tr("Certificate"));
    }
}

void PEWidget::on_checkBox_ManifestFormat_stateChanged(int nState)
{
    Q_UNUSED(nState)

    formatXML();
}

void PEWidget::formatXML()
{
    if (getDevice()) {
        XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

        if (pe.isValid()) {
            ui->textEditResources_Manifest->clear();

            QString sManifest = pe.getResourceManifest();

            if (ui->checkBox_ManifestFormat->isChecked()) {
                sManifest = XBinary::formatXML(sManifest);
            }

            ui->textEditResources_Manifest->setText(sManifest);
        }
    }
}

void PEWidget::on_tableWidget_IMAGE_DIRECTORY_ENTRIES_customContextMenuRequested(const QPoint &pos)
{
    int nRow = ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->currentIndex().row();

    if (nRow != -1) {
        bool bIsEnable = getTableViewItemSize(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES);

        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editDirectoryHeader()));
        contextMenu.addAction(&actionEdit);

        QAction actionHex(tr("Hex"), this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(directoryHex()));
        actionHex.setEnabled(bIsEnable);
        contextMenu.addAction(&actionHex);

        QAction actionEntropy(tr("Entropy"), this);
        connect(&actionEntropy, SIGNAL(triggered()), this, SLOT(directoryEntropy()));
        actionEntropy.setEnabled(bIsEnable);
        contextMenu.addAction(&actionEntropy);

        contextMenu.exec(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::editDirectoryHeader()
{
    showSectionHeader(SPE::TYPE_IMAGE_DIRECTORY_ENTRIES, ui->tableWidget_IMAGE_DIRECTORY_ENTRIES);
}

void PEWidget::directoryHex()
{
    showSectionHex(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES);
}

void PEWidget::directoryEntropy()
{
    showSectionEntropy(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES);
}

void PEWidget::on_tableView_TLSCallbacks_customContextMenuRequested(const QPoint &pos)
{
    int nRow = ui->tableView_TLSCallbacks->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionDisasm(tr("Disasm"), this);
        connect(&actionDisasm, SIGNAL(triggered()), this, SLOT(disasmTLSCallback()));
        contextMenu.addAction(&actionDisasm);

        contextMenu.exec(ui->tableView_TLSCallbacks->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::disasmTLSCallback()
{
    showSectionDisasm(ui->tableView_TLSCallbacks);
}

void PEWidget::on_pushButtonSave_Sections_clicked()
{
    XShortcutsWidget::saveModel(ui->tableView_Sections->model(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(tr("Sections"))));
}

void PEWidget::on_tableView_Resources_StringTable_customContextMenuRequested(const QPoint &pos)
{
    int nRow = ui->tableView_Resources_StringTable->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionHex(tr("Hex"), this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(stringTableHex()));
        contextMenu.addAction(&actionHex);

        contextMenu.exec(ui->tableView_Resources_StringTable->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::stringTableHex()
{
    showSectionHex(ui->tableView_Resources_StringTable);
}

void PEWidget::on_pushButtonSave_Debug_clicked()
{
    XShortcutsWidget::saveModel(ui->tableView_Debug->model(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Debug"))));
}

void PEWidget::on_pushButtonSave_Exception_clicked()
{
    XShortcutsWidget::saveModel(ui->tableView_Exceptions->model(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Exceptions"))));
}

void PEWidget::on_pushButtonSave_IMAGE_DIRECTORY_ENTRIES_clicked()
{
    saveHeaderTable(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("IMAGE_DIRECTORY_ENTRIES"))));
}

void PEWidget::on_pushButtonExtractAllIcons_Resources_clicked()
{
    QString sDirectory = QFileDialog::getExistingDirectory(this, tr("Dump all"), XBinary::getDeviceDirectory(getDevice()));

    if (!sDirectory.isEmpty()) {
        // TODO
    }
}

void PEWidget::on_pushButtonDumpAll_Resources_clicked()
{
    dumpAll(ui->tableView_Resources);
}

void PEWidget::on_pushButtonSave_Resources_clicked()
{
    XShortcutsWidget::saveModel(ui->tableView_Resources->model(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Resources"))));
}

void PEWidget::on_pushButtonDump_Overlay_clicked()
{
    PEToolsWidget::dumpOverlay(this, getDevice(), getOptions().bIsImage, getOptions().nImageBase);
}

void PEWidget::on_pushButtonDump_DosStub_clicked()
{
    PEToolsWidget::dumpDosStub(this, getDevice(), getOptions().bIsImage, getOptions().nImageBase);
}

void PEWidget::on_pushButtonDumpAll_Sections_clicked()
{
    dumpAll(ui->tableView_Sections);
}
