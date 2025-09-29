/* Copyright (c) 2017-2025 hors<horsicq@gmail.com>
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

    XOptions::adjustToolButton(ui->toolButtonReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonNext, XOptions::ICONTYPE_FORWARD, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonPrev, XOptions::ICONTYPE_BACKWARD, Qt::ToolButtonIconOnly);

    ui->toolButtonReload->setToolTip(tr("Reload"));
    ui->toolButtonNext->setToolTip(tr("Next visited"));
    ui->toolButtonPrev->setToolTip(tr("Previous visited"));
    ui->checkBoxReadonly->setToolTip(tr("Readonly"));

    memset(g_subDevice, 0, sizeof g_subDevice);

    initWidget();
    initDisasmView(ui->widgetDisasm_DosStub);
    connect(ui->widgetTools, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(allReload(qint64, qint64)));

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
    memset(m_comboBox, 0, sizeof m_comboBox);
    memset(g_pushButton, 0, sizeof g_pushButton);
    memset(g_dateTimeEdit, 0, sizeof g_dateTimeEdit);
    memset(g_invWidget, 0, sizeof g_invWidget);

    _deleteSubdevices(g_subDevice, (sizeof g_subDevice) / (sizeof(SubDevice *)));

    resetWidget();

    ui->widgetTools->setData(0);
    XDisasmView::OPTIONS options = {};
    ui->widgetDisasm_DosStub->setData(0, options);

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

        QTreeWidgetItem *pItemInfo = createNewItem(SPE::TYPE_INFO, tr("Info"), XOptions::ICONTYPE_INFO);
        ui->treeWidgetNavi->addTopLevelItem(pItemInfo);
        pItemInfo->addChild(createNewItem(SPE::TYPE_NFDSCAN, "Nauz File Detector (NFD)", XOptions::ICONTYPE_NFD));
        pItemInfo->addChild(createNewItem(SPE::TYPE_DIESCAN, "Detect It Easy (DiE)", XOptions::ICONTYPE_DIE));
#ifdef USE_YARA
        pItemInfo->addChild(createNewItem(SPE::TYPE_YARASCAN, "Yara rules", XOptions::ICONTYPE_YARA));
#endif
        pItemInfo->addChild(createNewItem(SPE::TYPE_VIRUSTOTAL, "VirusTotal", XOptions::ICONTYPE_VIRUSTOTAL));

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_VISUALIZATION, tr("Visualization"), XOptions::ICONTYPE_VISUALIZATION));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_HEX, tr("Hex"), XOptions::ICONTYPE_HEX));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_DISASM, tr("Disasm"), XOptions::ICONTYPE_DISASM));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_HASH, tr("Hash"), XOptions::ICONTYPE_HASH));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_STRINGS, tr("Strings"), XOptions::ICONTYPE_STRING));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_SIGNATURES, tr("Signatures"), XOptions::ICONTYPE_SIGNATURE));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_MEMORYMAP, tr("Memory map"), XOptions::ICONTYPE_MEMORYMAP));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_ENTROPY, tr("Entropy"), XOptions::ICONTYPE_ENTROPY));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_EXTRACTOR, tr("Extractor"), XOptions::ICONTYPE_EXTRACTOR));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_SEARCH, tr("Search"), XOptions::ICONTYPE_SEARCH));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_TOOLS, tr("Tools"), XOptions::ICONTYPE_TOOL));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_IMAGE_DOS_HEADER, "IMAGE_DOS_HEADER", XOptions::ICONTYPE_HEADER));

        if (pe.isDosStubPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_DOS_STUB, "DOS stub", XOptions::ICONTYPE_CODE));
        }

        QTreeWidgetItem *pNtHeaders = createNewItem(SPE::TYPE_IMAGE_NT_HEADERS, "IMAGE_NT_HEADERS", XOptions::ICONTYPE_HEADER);
        ui->treeWidgetNavi->addTopLevelItem(pNtHeaders);
        pNtHeaders->addChild(createNewItem(SPE::TYPE_IMAGE_FILE_HEADER, "IMAGE_FILE_HEADER", XOptions::ICONTYPE_HEADER));
        QTreeWidgetItem *pOptionalHeader = createNewItem(SPE::TYPE_IMAGE_OPTIONAL_HEADER, "IMAGE_OPTIONAL_HEADER", XOptions::ICONTYPE_HEADER);
        pNtHeaders->addChild(pOptionalHeader);
        pOptionalHeader->addChild(createNewItem(SPE::TYPE_IMAGE_DIRECTORY_ENTRIES, "IMAGE_DIRECTORY_ENTRIES", XOptions::ICONTYPE_TABLE));

        if (pe.isRichSignaturePresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_RICH, QString("Rich %1").arg(tr("Signature")), XOptions::ICONTYPE_LIST));
        }

        if (pe.getFileHeader_NumberOfSections()) {
            QTreeWidgetItem *pItemSections = createNewItem(SPE::TYPE_SECTIONS, tr("Sections"), XOptions::ICONTYPE_SECTION);
            ui->treeWidgetNavi->addTopLevelItem(pItemSections);

            pItemSections->addChild(createNewItem(SPE::TYPE_SECTIONS_INFO, tr("Info"), XOptions::ICONTYPE_INFO));
        }

        if (pe.isExportPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_EXPORT, tr("Export"), XOptions::ICONTYPE_EXPORT));
        }

        if (pe.isImportPresent()) {
            QTreeWidgetItem *pItemImport = createNewItem(SPE::TYPE_IMPORT, tr("Import"), XOptions::ICONTYPE_IMPORT);
            ui->treeWidgetNavi->addTopLevelItem(pItemImport);

            pItemImport->addChild(createNewItem(SPE::TYPE_IMPORT_INFO, tr("Info"), XOptions::ICONTYPE_INFO));
        }

        if (pe.isResourcesPresent()) {
            QTreeWidgetItem *pResources = createNewItem(SPE::TYPE_RESOURCES, tr("Resources"), XOptions::ICONTYPE_RESOURCE);

            ui->treeWidgetNavi->addTopLevelItem(pResources);

            QList<XPE::RESOURCE_RECORD> listResources = pe.getResources(&memoryMap, 10000);

            if (pe.isResourceStringTablePresent(&listResources)) {
                pResources->addChild(createNewItem(SPE::TYPE_RESOURCES_STRINGTABLE, tr("String table"), XOptions::ICONTYPE_STRING));
            }

            if (pe.isResourceVersionPresent(&listResources)) {
                pResources->addChild(createNewItem(SPE::TYPE_RESOURCES_VERSION, tr("Version"), XOptions::ICONTYPE_VERSION));
            }

            if (pe.isResourceManifestPresent(&listResources)) {
                pResources->addChild(createNewItem(SPE::TYPE_RESOURCES_MANIFEST, tr("Manifest"), XOptions::ICONTYPE_MANIFEST));
            }
        }

        if (pe.isExceptionPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_EXCEPTION, tr("Exceptions"), XOptions::ICONTYPE_EXCEPTION));
        }

        if (pe.isSignPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_CERTIFICATE, tr("Certificate"), XOptions::ICONTYPE_CERTIFICATE));
        }

        if (pe.isRelocsPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_RELOCS, tr("Relocs"), XOptions::ICONTYPE_RELOC));
        }

        if (pe.isDebugPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_DEBUG, tr("Debug"), XOptions::ICONTYPE_DEBUG));
        }

        if (pe.isTLSPresent()) {
            QTreeWidgetItem *pTLS = createNewItem(SPE::TYPE_TLS, QString("TLS"), XOptions::ICONTYPE_TLS);

            ui->treeWidgetNavi->addTopLevelItem(pTLS);

            if (pe.isTLSCallbacksPresent(&memoryMap)) {
                pTLS->addChild(createNewItem(SPE::TYPE_TLSCALLBACKS, QString("TLS %1").arg(tr("Callbacks"))));
            }
        }

        if (pe.isLoadConfigPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_LOADCONFIG, tr("Load config"), XOptions::ICONTYPE_TABLE));
        }

        if (pe.isBoundImportPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_BOUNDIMPORT, tr("Bound import"), XOptions::ICONTYPE_IMPORT));
        }

        if (pe.isDelayImportPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_DELAYIMPORT, tr("Delay import"), XOptions::ICONTYPE_IMPORT));
        }

        if (pe.isNETPresent()) {
            QTreeWidgetItem *pNetHeader = createNewItem(SPE::TYPE_NETHEADER, QString(".NET"), XOptions::ICONTYPE_DOTNET);
            ui->treeWidgetNavi->addTopLevelItem(pNetHeader);

            XPE::CLI_INFO cliInfo = pe.getCliInfo(true);

            if (pe.isNetMetadataPresent(&cliInfo, &memoryMap)) {
                QTreeWidgetItem *pNetMetadata = createNewItem(SPE::TYPE_NET_METADATA, tr("Metadata"), XOptions::ICONTYPE_TABLE);
                pNetHeader->addChild(pNetMetadata);

                qint32 nNumberOfStreams = cliInfo.metaData.listStreams.count();

                for (qint32 i = 0; i < nNumberOfStreams; i++) {
                    QTreeWidgetItem *pNetMetadataStream =
                        createNewItem(SPE::TYPE_NET_METADATA_STREAM, cliInfo.metaData.listStreams.at(i).sName, XOptions::ICONTYPE_METADATA,
                                      cliInfo.metaData.listStreams.at(i).nOffset, cliInfo.metaData.listStreams.at(i).nSize);
                    pNetMetadata->addChild(pNetMetadataStream);

                    if ((cliInfo.metaData.listStreams.at(i).sName == "#~") || (cliInfo.metaData.listStreams.at(i).sName == "#-")) {
                        QTreeWidgetItem *pNetMetadataTable = createNewItem(SPE::TYPE_NET_METADATA_TABLE, tr("Metadata table"), XOptions::ICONTYPE_TABLE,
                                                                           cliInfo.metaData.listStreams.at(i).nOffset, cliInfo.metaData.listStreams.at(i).nSize);
                        pNetMetadataStream->addChild(pNetMetadataTable);
                    }
                }
            }

            // TODO NET Resources
            // TODO more NET
        }

        if (pe.isOverlayPresent()) {
            ui->treeWidgetNavi->addTopLevelItem(createNewItem(SPE::TYPE_OVERLAY, tr("Overlay"), XOptions::ICONTYPE_OVERLAY));
        }

        ui->treeWidgetNavi->expandAll();

        setDisasmInitAddress(pe.getEntryPointAddress());  // Optimize

        setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);
    }
}

void PEWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    ui->widgetTools->setGlobal(pShortcuts, pXOptions);
    FormatWidget::setGlobal(pShortcuts, pXOptions);
}

FormatWidget::SV PEWidget::_setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset)
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
                        case N_IMAGE_DOS_HEADER::e_magic: m_comboBox[CB_IMAGE_DOS_HEADER_e_magic]->setValue(nValue); break;
                        case N_IMAGE_DOS_HEADER::e_lfanew: g_invWidget[INV_IMAGE_DOS_HEADER_e_lfanew]->setOffsetAndSize(&pe, (quint32)nValue, 0); break;
                    }
                    break;

                case SPE::TYPE_IMAGE_NT_HEADERS:
                    switch (nNdata) {
                        case N_IMAGE_NT_HEADERS::Signature: m_comboBox[CB_IMAGE_NT_HEADERS_Signature]->setValue(nValue); break;
                    }
                    break;

                case SPE::TYPE_IMAGE_FILE_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_FILE_HEADER::Machine: m_comboBox[CB_IMAGE_FILE_HEADER_Machine]->setValue(nValue); break;
                        case N_IMAGE_FILE_HEADER::TimeDateStamp:
                            g_dateTimeEdit[TD_IMAGE_FILE_HEADER_TimeDateStamp]->setValue(nValue, XDateTimeEditX::DT_TYPE_POSIX);
                            break;
                        case N_IMAGE_FILE_HEADER::PointerToSymbolTable:
                            g_invWidget[INV_IMAGE_FILE_HEADER_PointerToSymbolTable]->setAddressAndSize(&pe, (quint32)nValue, 0);
                            break;
                        case N_IMAGE_FILE_HEADER::Characteristics: m_comboBox[CB_IMAGE_FILE_HEADER_Characteristics]->setValue(nValue); break;
                    }
                    break;

                case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_OPTIONAL_HEADER::Magic: m_comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic]->setValue(nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint:
                            g_invWidget[INV_IMAGE_OPTIONAL_HEADER_AddressOfEntryPoint]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfCode:
                            g_invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfCode]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfData:
                            g_invWidget[INV_IMAGE_OPTIONAL_HEADER_BaseOfData]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0);
                            break;
                        case N_IMAGE_OPTIONAL_HEADER::Subsystem: m_comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem]->setValue(nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::DllCharacteristics: m_comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics]->setValue(nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion:
                        case N_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion:
                        case N_IMAGE_OPTIONAL_HEADER::OperatingSystemVersion:
                            quint32 nVersion = pe.getOperatingSystemVersion();
                            m_comboBox[CB_IMAGE_OPTIONAL_HEADER_OperationSystemVersion]->setValue(nVersion);
                            break;
                    }
                    break;

                case SPE::TYPE_EXPORT:
                    switch (nNdata) {
                        case N_IMAGE_EXPORT::TimeDateStamp: g_dateTimeEdit[TD_IMAGE_EXPORT_TimeDateStamp]->setValue(nValue, XDateTimeEditX::DT_TYPE_POSIX); break;
                        case N_IMAGE_EXPORT::Name: g_invWidget[INV_IMAGE_EXPORT_Name]->setAddressAndSize(&pe, pe.getBaseAddress() + (quint32)nValue, 0); break;
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
                        case N_IMAGE_TLS::StartAddressOfRawData: g_invWidget[INV_IMAGE_TLS_StartAddressOfRawData]->setAddressAndSize(&pe, (quint64)nValue, 0); break;
                        case N_IMAGE_TLS::EndAddressOfRawData: g_invWidget[INV_IMAGE_TLS_EndAddressOfRawData]->setAddressAndSize(&pe, (quint64)nValue, 0); break;
                        case N_IMAGE_TLS::AddressOfIndex: g_invWidget[INV_IMAGE_TLS_AddressOfIndex]->setAddressAndSize(&pe, (quint64)nValue, 0); break;
                        case N_IMAGE_TLS::AddressOfCallBacks: g_invWidget[INV_IMAGE_TLS_AddressOfCallBacks]->setAddressAndSize(&pe, (quint64)nValue, 0); break;
                    }
                    break;

                case SPE::TYPE_NETHEADER:
                    switch (nNdata) {
                        case N_IMAGE_NETHEADER::Flags: m_comboBox[CB_IMAGE_NETHEADER_FLAGS]->setValue((quint32)nValue); break;
                    }
                    break;

                case SPE::TYPE_RESOURCES_VERSION:
                    switch (nNdata) {
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature: m_comboBox[CB_RESOURCES_VERSION_dwSignature]->setValue((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags: m_comboBox[CB_RESOURCES_VERSION_dwFileFlags]->setValue((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS: m_comboBox[CB_RESOURCES_VERSION_dwFileOS]->setValue((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType: m_comboBox[CB_RESOURCES_VERSION_dwFileType]->setValue((quint32)nValue); break;
                    }
                    break;

                case SPE::TYPE_LOADCONFIG:
                    switch (nNdata) {
                        case N_IMAGE_LOADCONFIG::SecurityCookie: g_invWidget[INV_IMAGE_LOADCONFIG_SecurityCookie]->setAddressAndSize(&pe, (quint64)nValue, 0); break;
                        case N_IMAGE_LOADCONFIG::SEHandlerTable: g_invWidget[INV_IMAGE_LOADCONFIG_SEHandlerTable]->setAddressAndSize(&pe, (quint64)nValue, 0); break;
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
                        case N_IMAGE_DOS_HEADER::e_magic: pe.set_e_magic((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_cblp: pe.set_e_cblp((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_cp: pe.set_e_cp((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_crlc: pe.set_e_crlc((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_cparhdr: pe.set_e_cparhdr((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_minalloc: pe.set_e_minalloc((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_maxalloc: pe.set_e_maxalloc((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_ss: pe.set_e_ss((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_sp: pe.set_e_sp((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_csum: pe.set_e_csum((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_ip: pe.set_e_ip((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_cs: pe.set_e_cs((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_lfarlc: pe.set_e_lfarlc((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_ovno: pe.set_e_ovno((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res_0: pe.set_e_res(0, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res_1: pe.set_e_res(1, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res_2: pe.set_e_res(2, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res_3: pe.set_e_res(3, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_oemid: pe.set_e_oemid((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_oeminfo: pe.set_e_oeminfo((quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res2_0: pe.set_e_res2(0, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res2_1: pe.set_e_res2(1, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res2_2: pe.set_e_res2(2, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res2_3: pe.set_e_res2(3, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res2_4: pe.set_e_res2(4, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res2_5: pe.set_e_res2(5, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res2_6: pe.set_e_res2(6, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res2_7: pe.set_e_res2(7, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res2_8: pe.set_e_res2(8, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_res2_9: pe.set_e_res2(9, (quint16)nValue); break;
                        case N_IMAGE_DOS_HEADER::e_lfanew: pe.set_e_lfanew((quint32)nValue); break;
                    }

                    ui->widgetHex_IMAGE_DOS_HEADER->reload();
                    break;

                case SPE::TYPE_IMAGE_NT_HEADERS:
                    switch (nNdata) {
                        case N_IMAGE_NT_HEADERS::Signature: pe.setNtHeaders_Signature((quint32)nValue); break;
                    }

                    ui->widgetHex_IMAGE_NT_HEADERS->reload();
                    break;

                case SPE::TYPE_IMAGE_FILE_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_FILE_HEADER::Machine: pe.setFileHeader_Machine((quint16)nValue); break;
                        case N_IMAGE_FILE_HEADER::NumberOfSections: pe.setFileHeader_NumberOfSections((quint16)nValue); break;
                        case N_IMAGE_FILE_HEADER::TimeDateStamp: pe.setFileHeader_TimeDateStamp((quint32)nValue); break;
                        case N_IMAGE_FILE_HEADER::PointerToSymbolTable: pe.setFileHeader_PointerToSymbolTable((quint32)nValue); break;
                        case N_IMAGE_FILE_HEADER::NumberOfSymbols: pe.setFileHeader_NumberOfSymbols((quint32)nValue); break;
                        case N_IMAGE_FILE_HEADER::SizeOfOptionalHeader: pe.setFileHeader_SizeOfOptionalHeader((quint16)nValue); break;
                        case N_IMAGE_FILE_HEADER::Characteristics: pe.setFileHeader_Characteristics((quint16)nValue); break;
                    }

                    ui->widgetHex_IMAGE_FILE_HEADER->reload();
                    break;

                case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
                    switch (nNdata) {
                        case N_IMAGE_OPTIONAL_HEADER::Magic: pe.setOptionalHeader_Magic((quint16)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::MajorLinkerVersion: pe.setOptionalHeader_MajorLinkerVersion((quint8)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::MinorLinkerVersion: pe.setOptionalHeader_MinorLinkerVersion((quint8)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfCode: pe.setOptionalHeader_SizeOfCode((quint32)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfInitializedData: pe.setOptionalHeader_SizeOfInitializedData((quint32)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfUninitializedData: pe.setOptionalHeader_SizeOfUninitializedData((quint32)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint: pe.setOptionalHeader_AddressOfEntryPoint((quint32)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfCode: pe.setOptionalHeader_BaseOfCode((quint32)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::BaseOfData: pe.setOptionalHeader_BaseOfData((quint32)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::ImageBase: pe.setOptionalHeader_ImageBase((quint64)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::SectionAlignment: pe.setOptionalHeader_SectionAlignment((quint32)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::FileAlignment: pe.setOptionalHeader_FileAlignment((quint32)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion: pe.setOptionalHeader_MajorOperatingSystemVersion((quint16)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion: pe.setOptionalHeader_MinorOperatingSystemVersion((quint16)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::MajorImageVersion: pe.setOptionalHeader_MajorImageVersion((quint16)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::MinorImageVersion: pe.setOptionalHeader_MinorImageVersion((quint16)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::MajorSubsystemVersion: pe.setOptionalHeader_MajorSubsystemVersion((quint16)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::MinorSubsystemVersion: pe.setOptionalHeader_MinorSubsystemVersion((quint16)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::Win32VersionValue: pe.setOptionalHeader_Win32VersionValue((quint16)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfImage: pe.setOptionalHeader_SizeOfImage((quint32)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfHeaders: pe.setOptionalHeader_SizeOfHeaders((quint32)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::CheckSum: pe.setOptionalHeader_CheckSum((quint32)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::Subsystem: pe.setOptionalHeader_Subsystem((quint16)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::DllCharacteristics: pe.setOptionalHeader_DllCharacteristics((quint16)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfStackReserve: pe.setOptionalHeader_SizeOfStackReserve((quint64)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfStackCommit: pe.setOptionalHeader_SizeOfStackCommit((quint64)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfHeapReserve: pe.setOptionalHeader_SizeOfHeapReserve((quint64)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::SizeOfHeapCommit: pe.setOptionalHeader_SizeOfHeapCommit((quint64)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::LoaderFlags: pe.setOptionalHeader_LoaderFlags((quint32)nValue); break;
                        case N_IMAGE_OPTIONAL_HEADER::NumberOfRvaAndSizes: pe.setOptionalHeader_NumberOfRvaAndSizes((quint32)nValue); break;
                        // Extra
                        case N_IMAGE_OPTIONAL_HEADER::OperatingSystemVersion: pe.setOperatingSystemVersion((quint32)nValue); break;
                    }

                    ui->widgetHex_IMAGE_OPTIONAL_HEADER->reload();
                    break;

                case SPE::TYPE_EXPORT:
                    switch (nNdata) {
                        case N_IMAGE_EXPORT::Characteristics: pe.setExportDirectory_Characteristics((quint32)nValue); break;
                        case N_IMAGE_EXPORT::TimeDateStamp: pe.setExportDirectory_TimeDateStamp((quint32)nValue); break;
                        case N_IMAGE_EXPORT::MajorVersion: pe.setExportDirectory_MajorVersion((quint16)nValue); break;
                        case N_IMAGE_EXPORT::MinorVersion: pe.setExportDirectory_MinorVersion((quint16)nValue); break;
                        case N_IMAGE_EXPORT::Name: pe.setExportDirectory_Name((quint32)nValue); break;
                        case N_IMAGE_EXPORT::Base: pe.setExportDirectory_Base((quint32)nValue); break;
                        case N_IMAGE_EXPORT::NumberOfFunctions: pe.setExportDirectory_NumberOfFunctions((quint32)nValue); break;
                        case N_IMAGE_EXPORT::NumberOfNames: pe.setExportDirectory_NumberOfNames((quint32)nValue); break;
                        case N_IMAGE_EXPORT::AddressOfFunctions: pe.setExportDirectory_AddressOfFunctions((quint32)nValue); break;
                        case N_IMAGE_EXPORT::AddressOfNames: pe.setExportDirectory_AddressOfNames((quint32)nValue); break;
                        case N_IMAGE_EXPORT::AddressOfNameOrdinals: pe.setExportDirectory_AddressOfNameOrdinals((quint32)nValue); break;
                    }
                    break;

                case SPE::TYPE_TLS:
                    switch (nNdata) {
                        case N_IMAGE_TLS::StartAddressOfRawData: pe.setTLS_StartAddressOfRawData((quint64)nValue); break;
                        case N_IMAGE_TLS::EndAddressOfRawData: pe.setTLS_EndAddressOfRawData((quint64)nValue); break;
                        case N_IMAGE_TLS::AddressOfIndex: pe.setTLS_AddressOfIndex((quint64)nValue); break;
                        case N_IMAGE_TLS::AddressOfCallBacks: pe.setTLS_AddressOfCallBacks((quint64)nValue); break;
                        case N_IMAGE_TLS::SizeOfZeroFill: pe.setTLS_SizeOfZeroFill((quint32)nValue); break;
                        case N_IMAGE_TLS::Characteristics: pe.setTLS_Characteristics((quint32)nValue); break;
                    }

                    ui->widgetHex_TLS->reload();
                    break;

                case SPE::TYPE_RESOURCES_VERSION:
                    switch (nNdata) {
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature: pe.setFixedFileInfo_dwSignature((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwStrucVersion: pe.setFixedFileInfo_dwStrucVersion((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionMS: pe.setFixedFileInfo_dwFileVersionMS((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionLS: pe.setFixedFileInfo_dwFileVersionLS((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionMS: pe.setFixedFileInfo_dwProductVersionMS((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionLS: pe.setFixedFileInfo_dwProductVersionLS((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlagsMask: pe.setFixedFileInfo_dwFileFlagsMask((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags: pe.setFixedFileInfo_dwFileFlags((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS: pe.setFixedFileInfo_dwFileOS((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType: pe.setFixedFileInfo_dwFileType((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileSubtype: pe.setFixedFileInfo_dwFileSubtype((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileDateMS: pe.setFixedFileInfo_dwFileDateMS((quint32)nValue); break;
                        case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileDateLS: pe.setFixedFileInfo_dwFileDateLS((quint32)nValue); break;
                    }
                    break;

                case SPE::TYPE_NETHEADER:
                    switch (nNdata) {
                        case N_IMAGE_NETHEADER::cb: pe.setNetHeader_cb((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::MinorRuntimeVersion: pe.setNetHeader_MinorRuntimeVersion((quint16)nValue); break;
                        case N_IMAGE_NETHEADER::MajorRuntimeVersion: pe.setNetHeader_MajorRuntimeVersion((quint16)nValue); break;
                        case N_IMAGE_NETHEADER::MetaData_Address: pe.setNetHeader_MetaData_Address((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::MetaData_Size: pe.setNetHeader_MetaData_Size((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::Flags: pe.setNetHeader_Flags((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::EntryPoint: pe.setNetHeader_EntryPoint((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::Resources_Address: pe.setNetHeader_Resources_Address((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::Resources_Size: pe.setNetHeader_Resources_Size((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::StrongNameSignature_Address: pe.setNetHeader_StrongNameSignature_Address((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::StrongNameSignature_Size: pe.setNetHeader_StrongNameSignature_Size((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::CodeManagerTable_Address: pe.setNetHeader_CodeManagerTable_Address((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::CodeManagerTable_Size: pe.setNetHeader_CodeManagerTable_Size((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::VTableFixups_Address: pe.setNetHeader_VTableFixups_Address((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::VTableFixups_Size: pe.setNetHeader_VTableFixups_Size((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::ExportAddressTableJumps_Address: pe.setNetHeader_ExportAddressTableJumps_Address((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::ExportAddressTableJumps_Size: pe.setNetHeader_ExportAddressTableJumps_Size((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::ManagedNativeHeader_Address: pe.setNetHeader_ManagedNativeHeader_Address((quint32)nValue); break;
                        case N_IMAGE_NETHEADER::ManagedNativeHeader_Size: pe.setNetHeader_ManagedNativeHeader_Size((quint32)nValue); break;
                    }

                    ui->widgetHex_NetHeader->reload();
                    break;

                case SPE::TYPE_NET_METADATA:
                    switch (nNdata) {
                        case N_IMAGE_NET_METADATA::Signature: pe.setMetadataHeader_Signature((quint32)nValue); break;
                        case N_IMAGE_NET_METADATA::MajorVersion: pe.setMetadataHeader_MajorVersion((quint16)nValue); break;
                        case N_IMAGE_NET_METADATA::MinorVersion: pe.setMetadataHeader_MinorVersion((quint16)nValue); break;
                        case N_IMAGE_NET_METADATA::Reserved: pe.setMetadataHeader_Reserved((quint32)nValue); break;
                        case N_IMAGE_NET_METADATA::VersionStringLength: pe.setMetadataHeader_VersionStringLength((quint32)nValue); break;  // TODO reload!!!
                        case N_IMAGE_NET_METADATA::Version: pe.setMetadataHeader_Version(sValue); break;
                        case N_IMAGE_NET_METADATA::Flags: pe.setMetadataHeader_Flags((quint16)nValue); break;
                        case N_IMAGE_NET_METADATA::Streams: pe.setMetadataHeader_Streams((quint16)nValue); break;
                    }

                    ui->widgetHex_Net_Metadata->reload();
                    break;

                case SPE::TYPE_LOADCONFIG:
                    switch (nNdata) {
                        case N_IMAGE_LOADCONFIG::Size: pe.setLoadConfig_Size((quint32)nValue); break;
                        case N_IMAGE_LOADCONFIG::TimeDateStamp: pe.setLoadConfig_TimeDateStamp((quint32)nValue); break;
                        case N_IMAGE_LOADCONFIG::MinorVersion: pe.setLoadConfig_MinorVersion((quint16)nValue); break;
                        case N_IMAGE_LOADCONFIG::MajorVersion: pe.setLoadConfig_MajorVersion((quint16)nValue); break;
                        case N_IMAGE_LOADCONFIG::GlobalFlagsClear: pe.setLoadConfig_GlobalFlagsClear((quint32)nValue); break;
                        case N_IMAGE_LOADCONFIG::GlobalFlagsSet: pe.setLoadConfig_GlobalFlagsSet((quint32)nValue); break;
                        case N_IMAGE_LOADCONFIG::CriticalSectionDefaultTimeout: pe.setLoadConfig_CriticalSectionDefaultTimeout((quint32)nValue); break;
                        case N_IMAGE_LOADCONFIG::DeCommitFreeBlockThreshold: pe.setLoadConfig_DeCommitFreeBlockThreshold((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::DeCommitTotalFreeThreshold: pe.setLoadConfig_DeCommitTotalFreeThreshold((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::LockPrefixTable: pe.setLoadConfig_LockPrefixTable((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::MaximumAllocationSize: pe.setLoadConfig_MaximumAllocationSize((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::VirtualMemoryThreshold: pe.setLoadConfig_VirtualMemoryThreshold((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::ProcessAffinityMask: pe.setLoadConfig_ProcessAffinityMask((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::CSDVersion: pe.setLoadConfig_CSDVersion((quint16)nValue); break;
                        case N_IMAGE_LOADCONFIG::DependentLoadFlags: pe.setLoadConfig_DependentLoadFlags((quint16)nValue); break;
                        case N_IMAGE_LOADCONFIG::EditList: pe.setLoadConfig_EditList((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::SecurityCookie: pe.setLoadConfig_SecurityCookie((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::SEHandlerTable: pe.setLoadConfig_SEHandlerTable((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::SEHandlerCount: pe.setLoadConfig_SEHandlerCount((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer: pe.setLoadConfig_GuardCFCheckFunctionPointer((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer: pe.setLoadConfig_GuardCFDispatchFunctionPointer((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardCFFunctionTable: pe.setLoadConfig_GuardCFFunctionTable((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardCFFunctionCount: pe.setLoadConfig_GuardCFFunctionCount((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardFlags: pe.setLoadConfig_GuardFlags((quint32)nValue); break;
                        case N_IMAGE_LOADCONFIG::CodeIntegrity_Flags: pe.setLoadConfig_CodeIntegrity_Flags((quint16)nValue); break;
                        case N_IMAGE_LOADCONFIG::CodeIntegrity_Catalog: pe.setLoadConfig_CodeIntegrity_Catalog((quint16)nValue); break;
                        case N_IMAGE_LOADCONFIG::CodeIntegrity_CatalogOffset: pe.setLoadConfig_CodeIntegrity_CatalogOffset((quint32)nValue); break;
                        case N_IMAGE_LOADCONFIG::CodeIntegrity_Reserved: pe.setLoadConfig_CodeIntegrity_Reserved((quint32)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryTable: pe.setLoadConfig_GuardAddressTakenIatEntryTable((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryCount: pe.setLoadConfig_GuardAddressTakenIatEntryCount((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardLongJumpTargetTable: pe.setLoadConfig_GuardLongJumpTargetTable((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardLongJumpTargetCount: pe.setLoadConfig_GuardLongJumpTargetCount((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::DynamicValueRelocTable: pe.setLoadConfig_DynamicValueRelocTable((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::CHPEMetadataPointer: pe.setLoadConfig_CHPEMetadataPointer((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardRFFailureRoutine: pe.setLoadConfig_GuardRFFailureRoutine((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardRFFailureRoutineFunctionPointer: pe.setLoadConfig_GuardRFFailureRoutineFunctionPointer((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::DynamicValueRelocTableOffset: pe.setLoadConfig_DynamicValueRelocTableOffset((quint32)nValue); break;
                        case N_IMAGE_LOADCONFIG::DynamicValueRelocTableSection: pe.setLoadConfig_DynamicValueRelocTableSection((quint16)nValue); break;
                        case N_IMAGE_LOADCONFIG::Reserved2: pe.setLoadConfig_Reserved2((quint16)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardRFVerifyStackPointerFunctionPointer:
                            pe.setLoadConfig_GuardRFVerifyStackPointerFunctionPointer((quint64)nValue);
                            break;
                        case N_IMAGE_LOADCONFIG::HotPatchTableOffset: pe.setLoadConfig_HotPatchTableOffset((quint32)nValue); break;
                        case N_IMAGE_LOADCONFIG::Reserved3: pe.setLoadConfig_Reserved3((quint32)nValue); break;
                        case N_IMAGE_LOADCONFIG::EnclaveConfigurationPointer: pe.setLoadConfig_EnclaveConfigurationPointer((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::VolatileMetadataPointer: pe.setLoadConfig_VolatileMetadataPointer((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardEHContinuationTable: pe.setLoadConfig_GuardEHContinuationTable((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardEHContinuationCount: pe.setLoadConfig_GuardEHContinuationCount((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardXFGCheckFunctionPointer: pe.setLoadConfig_GuardXFGCheckFunctionPointer((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardXFGDispatchFunctionPointer: pe.setLoadConfig_GuardXFGDispatchFunctionPointer((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardXFGTableDispatchFunctionPointer: pe.setLoadConfig_GuardXFGTableDispatchFunctionPointer((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::CastGuardOsDeterminedFailureMode: pe.setLoadConfig_CastGuardOsDeterminedFailureMode((quint64)nValue); break;
                        case N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer: pe.setLoadConfig_GuardMemcpyFunctionPointer((quint64)nValue); break;
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
                            addComment(ui->tableWidget_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::SizeOfCode, HEADER_COLUMN_COMMENT,
                                       XBinary::bytesCountToString(nValue));
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
                            addComment(ui->tableWidget_ExportHeader, N_IMAGE_EXPORT::Name, HEADER_COLUMN_COMMENT,
                                       pe.read_ansiString(pe.relAddressToOffset((quint32)nValue)));
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

    setComboBoxesReadOnly(m_comboBox, __CB_size, bState);
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

    _blockSignals((QObject **)m_comboBox, __CB_size, bState);
    _blockSignals((QObject **)g_pushButton, __PB_size, bState);
    _blockSignals((QObject **)g_dateTimeEdit, __TD_size, bState);
}

void PEWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious)
{
    Q_UNUSED(pItemPrevious)

    if (pItemCurrent) {
        reloadData(false);
        addPage(pItemCurrent);
        ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}

void PEWidget::_widgetValueChanged(QVariant vValue)
{
    QWidget *pWidget = qobject_cast<QWidget *>(sender());
    qint32 nStype = pWidget->property("STYPE").toInt();
    qint32 nNdata = pWidget->property("NDATA").toInt();

    quint64 nValue = vValue.toULongLong();

    switch (nStype) {
        case SPE::TYPE_IMAGE_DOS_HEADER:
            switch (nNdata) {
                case N_IMAGE_DOS_HEADER::e_magic: g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_magic]->setValue_uint16((quint16)nValue); break;
            }
            break;

        case SPE::TYPE_IMAGE_NT_HEADERS:
            switch (nNdata) {
                case N_IMAGE_NT_HEADERS::Signature: g_lineEdit_IMAGE_NT_HEADERS[N_IMAGE_NT_HEADERS::Signature]->setValue_uint32((quint32)nValue); break;
            }
            break;

        case SPE::TYPE_IMAGE_FILE_HEADER:
            switch (nNdata) {
                case N_IMAGE_FILE_HEADER::Machine: g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Machine]->setValue_uint16((quint16)nValue); break;
                case N_IMAGE_FILE_HEADER::TimeDateStamp: g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::TimeDateStamp]->setValue_uint32((quint32)nValue); break;
                case N_IMAGE_FILE_HEADER::Characteristics: g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Characteristics]->setValue_uint16((quint16)nValue); break;
            }
            break;

        case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
            switch (nNdata) {
                case N_IMAGE_OPTIONAL_HEADER::Magic: g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Magic]->setValue_uint16((quint16)nValue); break;
                case N_IMAGE_OPTIONAL_HEADER::Subsystem: g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Subsystem]->setValue_uint16((quint16)nValue); break;
                case N_IMAGE_OPTIONAL_HEADER::DllCharacteristics:
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::DllCharacteristics]->setValue_uint16((quint16)nValue);
                    break;
                // Extra
                case N_IMAGE_OPTIONAL_HEADER::OperatingSystemVersion: {
                    // TODO set correct
                    XBinary::XDWORD xdword = XBinary::make_xdword((quint32)nValue);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion]->setValue_uint16((quint16)xdword.nValue1);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion]->setValue_uint16((quint16)xdword.nValue2);
                }
            }
            break;

        case SPE::TYPE_EXPORT:
            switch (nNdata) {
                case N_IMAGE_EXPORT::TimeDateStamp: g_dateTimeEdit[TD_IMAGE_EXPORT_TimeDateStamp]->setValue(nValue, XDateTimeEditX::DT_TYPE_POSIX); break;
            }
            break;

        case SPE::TYPE_NETHEADER:
            switch (nNdata) {
                case N_IMAGE_NETHEADER::Flags: g_lineEdit_NetHeader[N_IMAGE_NETHEADER::Flags]->setValue_uint32((quint32)nValue); break;
            }
            break;

        case SPE::TYPE_RESOURCES_VERSION:
            switch (nNdata) {
                case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature:
                    g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature]->setValue_uint32((quint32)nValue);
                    break;
                case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags:
                    g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags]->setValue_uint32((quint32)nValue);
                    break;
                case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS:
                    g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS]->setValue_uint32((quint32)nValue);
                    break;
                case N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType:
                    g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType]->setValue_uint32((quint32)nValue);
                    break;
            }
            break;
    }
}

void PEWidget::widgetAction()
{
    QWidget *pWidget = qobject_cast<QWidget *>(sender());
    qint32 nStype = pWidget->property("STYPE").toInt();
    qint32 nNdata = pWidget->property("NDATA").toInt();

    switch (nStype) {
        case SPE::TYPE_IMAGE_OPTIONAL_HEADER:
            switch (nNdata) {
                case N_IMAGE_OPTIONAL_HEADER::CheckSum:
                    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

                    if (pe.isValid()) {
                        quint32 nCheckSum = pe.calculateCheckSum();
                        g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::CheckSum]->setValue_uint32(nCheckSum);
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

void PEWidget::reloadData(bool bSaveSelection)
{
    qint32 nType = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_TYPE).toInt();
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
        } else if (nType == SPE::TYPE_VISUALIZATION) {
            if (!isInitPresent(sInit)) {
                ui->widgetVisualization->setData(getDevice(), pe.getFileType(), true);
            }
        } else if (nType == SPE::TYPE_VIRUSTOTAL) {
            if (!isInitPresent(sInit)) {
                ui->widgetVirusTotal->setData(getDevice());
            }
        } else if (nType == SPE::TYPE_HEX) {
            ui->widgetHex->setWidgetFocus();
            if (!isInitPresent(sInit)) {
                XHexViewWidget::OPTIONS options = {};
                options.bMenu_Disasm = true;
                options.bMenu_MemoryMap = true;

                if (bSaveSelection) {
                    options.nStartSelectionOffset = -1;
                }

                ui->widgetHex->setXInfoDB(getXInfoDB());
                ui->widgetHex->setData(getDevice(), options);
                // TODO save directory
                // ui->widgetHex->enableReadOnly(false);

                ui->widgetHex->reload();
            }
        } else if (nType == SPE::TYPE_DISASM) {
            ui->widgetDisasm->setWidgetFocus();
            if (!isInitPresent(sInit)) {
                XMultiDisasmWidget::OPTIONS options = {};
                options.fileType = pe.getFileType();
                options.nInitAddress = getDisasmInitAddress();
                options.bMenu_Hex = true;

                ui->widgetDisasm->setXInfoDB(getXInfoDB());
                ui->widgetDisasm->setData(getDevice(), options);

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
                // stringsOptions.bUTF8 = false;
                stringsOptions.bUnicode = true;
                stringsOptions.bNullTerminated = false;

                ui->widgetStrings->setData(getDevice(), pe.getFileType(), stringsOptions, true);
            }
        } else if (nType == SPE::TYPE_SIGNATURES) {
            if (!isInitPresent(sInit)) {
                SearchSignaturesWidget::OPTIONS signaturesOptions = {};
                signaturesOptions.bMenu_Hex = true;
                signaturesOptions.fileType = pe.getFileType();

                ui->widgetSignatures->setData(getDevice(), signaturesOptions, false);
            }
        } else if (nType == SPE::TYPE_MEMORYMAP) {
            if (!isInitPresent(sInit)) {
                XMemoryMapWidget::OPTIONS options = {};
                options.fileType = pe.getFileType();
                options.bIsSearchEnable = true;

                ui->widgetMemoryMap->setData(getDevice(), options, getXInfoDB());
            }
        } else if (nType == SPE::TYPE_ENTROPY) {
            if (!isInitPresent(sInit)) {
                ui->widgetEntropy->setData(getDevice(), 0, getDevice()->size(), pe.getFileType(),
                                           true);  // TODO save last directory
            }
        } else if (nType == SPE::TYPE_NFDSCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetHeuristicScan->setData(getDevice(), true, pe.getFileType());
            }
        } else if (nType == SPE::TYPE_DIESCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetDIEScan->setData(getDevice(), true, pe.getFileType());
            }
#ifdef USE_YARA
        } else if (nType == SPE::TYPE_YARASCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetYARAScan->setData(XBinary::getDeviceFileName(getDevice()), true);
            }
#endif
        } else if (nType == SPE::TYPE_EXTRACTOR) {
            if (!isInitPresent(sInit)) {
                XExtractor::OPTIONS extractorOptions = XExtractor::getDefaultOptions();
                extractorOptions.fileType = pe.getFileType();
                extractorOptions.bMenu_Hex = true;

                ui->widgetExtractor->setData(getDevice(), getXInfoDB(), extractorOptions, true);
            }
        } else if (nType == SPE::TYPE_SEARCH) {
            if (!isInitPresent(sInit)) {
                SearchValuesWidget::OPTIONS options = {};
                options.fileType = pe.getFileType();
                options.bMenu_Hex = true;
                options.bMenu_Disasm = true;

                ui->widgetSearch->setData(getDevice(), options);
            }
        } else if (nType == SPE::TYPE_TOOLS) {
            if (!isInitPresent(sInit)) {
                ui->widgetTools->setData(getDevice(), getOptions().bIsImage, getOptions().nImageBase);
            }
        } else if (nType == SPE::TYPE_IMAGE_DOS_HEADER) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SPE::TYPE_IMAGE_DOS_HEADER, ui->tableWidget_IMAGE_DOS_HEADER, N_IMAGE_DOS_HEADER::records, g_lineEdit_IMAGE_DOS_HEADER,
                                  N_IMAGE_DOS_HEADER::__data_size, 0);
                m_comboBox[CB_IMAGE_DOS_HEADER_e_magic] = createComboBox(ui->tableWidget_IMAGE_DOS_HEADER, XPE::getImageMagicsS(), SPE::TYPE_IMAGE_DOS_HEADER,
                                                                         N_IMAGE_DOS_HEADER::e_magic, XComboBoxEx::CBTYPE_LIST);
                g_invWidget[INV_IMAGE_DOS_HEADER_e_lfanew] =
                    createInvWidget(ui->tableWidget_IMAGE_DOS_HEADER, SPE::TYPE_IMAGE_DOS_HEADER, N_IMAGE_DOS_HEADER::e_lfanew, InvWidget::TYPE_HEX);

                blockSignals(true);

                XMSDOS_DEF::IMAGE_DOS_HEADEREX msdosheaderex = pe.getDosHeaderEx();

                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_magic]->setValue_uint16(msdosheaderex.e_magic);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_cblp]->setValue_uint16(msdosheaderex.e_cblp);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_cp]->setValue_uint16(msdosheaderex.e_cp);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_crlc]->setValue_uint16(msdosheaderex.e_crlc);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_cparhdr]->setValue_uint16(msdosheaderex.e_cparhdr);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_minalloc]->setValue_uint16(msdosheaderex.e_minalloc);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_maxalloc]->setValue_uint16(msdosheaderex.e_maxalloc);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_ss]->setValue_uint16(msdosheaderex.e_ss);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_sp]->setValue_uint16(msdosheaderex.e_sp);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_csum]->setValue_uint16(msdosheaderex.e_csum);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_ip]->setValue_uint16(msdosheaderex.e_ip);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_cs]->setValue_uint16(msdosheaderex.e_cs);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_lfarlc]->setValue_uint16(msdosheaderex.e_lfarlc);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_ovno]->setValue_uint16(msdosheaderex.e_ovno);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res_0]->setValue_uint16(msdosheaderex.e_res[0]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res_1]->setValue_uint16(msdosheaderex.e_res[1]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res_2]->setValue_uint16(msdosheaderex.e_res[2]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res_3]->setValue_uint16(msdosheaderex.e_res[3]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_oemid]->setValue_uint16(msdosheaderex.e_oemid);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_oeminfo]->setValue_uint16(msdosheaderex.e_oeminfo);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_0]->setValue_uint16(msdosheaderex.e_res2[0]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_1]->setValue_uint16(msdosheaderex.e_res2[1]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_2]->setValue_uint16(msdosheaderex.e_res2[2]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_3]->setValue_uint16(msdosheaderex.e_res2[3]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_4]->setValue_uint16(msdosheaderex.e_res2[4]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_5]->setValue_uint16(msdosheaderex.e_res2[5]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_6]->setValue_uint16(msdosheaderex.e_res2[6]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_7]->setValue_uint16(msdosheaderex.e_res2[7]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_8]->setValue_uint16(msdosheaderex.e_res2[8]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_res2_9]->setValue_uint16(msdosheaderex.e_res2[9]);
                g_lineEdit_IMAGE_DOS_HEADER[N_IMAGE_DOS_HEADER::e_lfanew]->setValue_uint32(msdosheaderex.e_lfanew);

                m_comboBox[CB_IMAGE_DOS_HEADER_e_magic]->setValue(msdosheaderex.e_magic);

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
                // options.memoryMapRegion = binary.getMemoryMap();

                ui->widgetDisasm_DosStub->setData(g_subDevice[SPE::TYPE_DOS_STUB], options);
                ui->widgetDisasm_DosStub->setXInfoDB(getXInfoDB());
            }
        } else if (nType == SPE::TYPE_IMAGE_NT_HEADERS) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SPE::TYPE_IMAGE_NT_HEADERS, ui->tableWidget_IMAGE_NT_HEADERS, N_IMAGE_NT_HEADERS::records, g_lineEdit_IMAGE_NT_HEADERS,
                                  N_IMAGE_NT_HEADERS::__data_size, 0);
                m_comboBox[CB_IMAGE_NT_HEADERS_Signature] = createComboBox(ui->tableWidget_IMAGE_NT_HEADERS, XPE::getImageNtHeadersSignaturesS(),
                                                                           SPE::TYPE_IMAGE_NT_HEADERS, N_IMAGE_NT_HEADERS::Signature, XComboBoxEx::CBTYPE_LIST);

                blockSignals(true);

                quint32 nSignature = pe.getNtHeaders_Signature();

                g_lineEdit_IMAGE_NT_HEADERS[N_IMAGE_NT_HEADERS::Signature]->setValue_uint32(nSignature);
                m_comboBox[CB_IMAGE_NT_HEADERS_Signature]->setValue(nSignature);

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
                m_comboBox[CB_IMAGE_FILE_HEADER_Machine] = createComboBox(ui->tableWidget_IMAGE_FILE_HEADER, XPE::getImageFileHeaderMachinesS(),
                                                                          SPE::TYPE_IMAGE_FILE_HEADER, N_IMAGE_FILE_HEADER::Machine, XComboBoxEx::CBTYPE_LIST);
                m_comboBox[CB_IMAGE_FILE_HEADER_Characteristics] =
                    createComboBox(ui->tableWidget_IMAGE_FILE_HEADER, XPE::getImageFileHeaderCharacteristicsS(), SPE::TYPE_IMAGE_FILE_HEADER,
                                   N_IMAGE_FILE_HEADER::Characteristics, XComboBoxEx::CBTYPE_FLAGS);

                g_dateTimeEdit[TD_IMAGE_FILE_HEADER_TimeDateStamp] =
                    createTimeDateEdit(ui->tableWidget_IMAGE_FILE_HEADER, SPE::TYPE_IMAGE_FILE_HEADER, N_IMAGE_FILE_HEADER::TimeDateStamp);
                g_invWidget[INV_IMAGE_FILE_HEADER_PointerToSymbolTable] =
                    createInvWidget(ui->tableWidget_IMAGE_FILE_HEADER, SPE::TYPE_IMAGE_FILE_HEADER, N_IMAGE_FILE_HEADER::PointerToSymbolTable, InvWidget::TYPE_HEX);

                blockSignals(true);

                XPE_DEF::IMAGE_FILE_HEADER fileheader = pe.getFileHeader();

                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Machine]->setValue_uint16(fileheader.Machine);
                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::NumberOfSections]->setValue_uint16(fileheader.NumberOfSections);
                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::TimeDateStamp]->setValue_uint32(fileheader.TimeDateStamp);
                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::PointerToSymbolTable]->setValue_uint32(fileheader.PointerToSymbolTable);
                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::NumberOfSymbols]->setValue_uint32(fileheader.NumberOfSymbols);
                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::SizeOfOptionalHeader]->setValue_uint16(fileheader.SizeOfOptionalHeader);
                g_lineEdit_IMAGE_FILE_HEADER[N_IMAGE_FILE_HEADER::Characteristics]->setValue_uint16(fileheader.Characteristics);

                m_comboBox[CB_IMAGE_FILE_HEADER_Machine]->setValue(fileheader.Machine);
                m_comboBox[CB_IMAGE_FILE_HEADER_Characteristics]->setValue(fileheader.Characteristics);
                g_dateTimeEdit[TD_IMAGE_FILE_HEADER_TimeDateStamp]->setValue(fileheader.TimeDateStamp, XDateTimeEditX::DT_TYPE_POSIX);

                addComment(ui->tableWidget_IMAGE_FILE_HEADER, N_IMAGE_FILE_HEADER::SizeOfOptionalHeader, HEADER_COLUMN_COMMENT,
                           XBinary::bytesCountToString(fileheader.SizeOfOptionalHeader));

                g_invWidget[INV_IMAGE_FILE_HEADER_PointerToSymbolTable]->setOffsetAndSize(&pe, fileheader.PointerToSymbolTable, 0);  // TODO Check addresses

                qint64 nOffset = pe.getFileHeaderOffset();
                qint64 nSize = pe.getFileHeaderSize();
                qint64 nAddress = pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_IMAGE_FILE_HEADER], ui->widgetHex_IMAGE_FILE_HEADER);

                blockSignals(false);
            }
        } else if (nType == SPE::TYPE_IMAGE_OPTIONAL_HEADER) {
            if (!isInitPresent(sInit)) {
                XBinary::PDSTRUCT pdStructEmpty = {};

                createHeaderTable(SPE::TYPE_IMAGE_OPTIONAL_HEADER, ui->tableWidget_IMAGE_OPTIONAL_HEADER,
                                  pe.is64() ? (N_IMAGE_OPTIONAL_HEADER::records64) : (N_IMAGE_OPTIONAL_HEADER::records32), g_lineEdit_IMAGE_OPTIONAL_HEADER,
                                  N_IMAGE_OPTIONAL_HEADER::__data_size, 0);
                m_comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic] = createComboBox(ui->tableWidget_IMAGE_OPTIONAL_HEADER, XPE::getImageOptionalHeaderMagicS(),
                                                                            SPE::TYPE_IMAGE_OPTIONAL_HEADER, N_IMAGE_OPTIONAL_HEADER::Magic, XComboBoxEx::CBTYPE_LIST);

                m_comboBox[CB_IMAGE_OPTIONAL_HEADER_OperationSystemVersion] = createComboBox(
                    ui->tableWidget_IMAGE_OPTIONAL_HEADER, XPE::getOperatingSystemVersions(pe.getFileFormatInfo(&pdStructEmpty).osName), SPE::TYPE_IMAGE_OPTIONAL_HEADER,
                    N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion, XComboBoxEx::CBTYPE_LIST, 0, N_IMAGE_OPTIONAL_HEADER::OperatingSystemVersion);

                m_comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem] =
                    createComboBox(ui->tableWidget_IMAGE_OPTIONAL_HEADER, XPE::getImageOptionalHeaderSubsystemS(), SPE::TYPE_IMAGE_OPTIONAL_HEADER,
                                   N_IMAGE_OPTIONAL_HEADER::Subsystem, XComboBoxEx::CBTYPE_LIST);
                m_comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics] =
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
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Magic]->setValue_uint16(oh64.Magic);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorLinkerVersion]->setValue_uint8(oh64.MajorLinkerVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorLinkerVersion]->setValue_uint8(oh64.MinorLinkerVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfCode]->setValue_uint32(oh64.SizeOfCode);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfInitializedData]->setValue_uint32(oh64.SizeOfInitializedData);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfUninitializedData]->setValue_uint32(oh64.SizeOfUninitializedData);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint]->setValue_uint32(oh64.AddressOfEntryPoint);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::BaseOfCode]->setValue_uint32(oh64.BaseOfCode);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::ImageBase]->setValue_int64(oh64.ImageBase);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SectionAlignment]->setValue_uint32(oh64.SectionAlignment);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::FileAlignment]->setValue_uint32(oh64.FileAlignment);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion]->setValue_uint16(oh64.MajorOperatingSystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion]->setValue_uint16(oh64.MinorOperatingSystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorImageVersion]->setValue_uint16(oh64.MajorImageVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorImageVersion]->setValue_uint16(oh64.MinorImageVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorSubsystemVersion]->setValue_uint16(oh64.MajorSubsystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorSubsystemVersion]->setValue_uint16(oh64.MinorSubsystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Win32VersionValue]->setValue_uint32(oh64.Win32VersionValue);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfImage]->setValue_uint32(oh64.SizeOfImage);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeaders]->setValue_uint32(oh64.SizeOfHeaders);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::CheckSum]->setValue_uint32(oh64.CheckSum);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Subsystem]->setValue_uint16(oh64.Subsystem);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::DllCharacteristics]->setValue_uint16(oh64.DllCharacteristics);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfStackReserve]->setValue_uint64(oh64.SizeOfStackReserve);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfStackCommit]->setValue_uint64(oh64.SizeOfStackCommit);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeapReserve]->setValue_uint64(oh64.SizeOfHeapReserve);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeapCommit]->setValue_uint64(oh64.SizeOfHeapCommit);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::LoaderFlags]->setValue_uint32(oh64.LoaderFlags);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::NumberOfRvaAndSizes]->setValue_uint32(oh64.NumberOfRvaAndSizes);

                    m_comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic]->setValue(oh64.Magic);
                    m_comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem]->setValue(oh64.Subsystem);
                    m_comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics]->setValue(oh64.DllCharacteristics);

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
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Magic]->setValue_uint16(oh32.Magic);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorLinkerVersion]->setValue_uint8(oh32.MajorLinkerVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorLinkerVersion]->setValue_uint8(oh32.MinorLinkerVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfCode]->setValue_uint32(oh32.SizeOfCode);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfInitializedData]->setValue_uint32(oh32.SizeOfInitializedData);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfUninitializedData]->setValue_uint32(oh32.SizeOfUninitializedData);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::AddressOfEntryPoint]->setValue_uint32(oh32.AddressOfEntryPoint);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::BaseOfCode]->setValue_uint32(oh32.BaseOfCode);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::BaseOfData]->setValue_uint32(oh32.BaseOfData);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::ImageBase]->setValue_uint32(oh32.ImageBase);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SectionAlignment]->setValue_uint32(oh32.SectionAlignment);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::FileAlignment]->setValue_uint32(oh32.FileAlignment);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorOperatingSystemVersion]->setValue_uint16(oh32.MajorOperatingSystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorOperatingSystemVersion]->setValue_uint16(oh32.MinorOperatingSystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorImageVersion]->setValue_uint16(oh32.MajorImageVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorImageVersion]->setValue_uint16(oh32.MinorImageVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MajorSubsystemVersion]->setValue_uint16(oh32.MajorSubsystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::MinorSubsystemVersion]->setValue_uint16(oh32.MinorSubsystemVersion);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Win32VersionValue]->setValue_uint32(oh32.Win32VersionValue);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfImage]->setValue_uint32(oh32.SizeOfImage);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeaders]->setValue_uint32(oh32.SizeOfHeaders);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::CheckSum]->setValue_uint32(oh32.CheckSum);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::Subsystem]->setValue_uint16(oh32.Subsystem);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::DllCharacteristics]->setValue_uint16(oh32.DllCharacteristics);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfStackReserve]->setValue_uint32(oh32.SizeOfStackReserve);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfStackCommit]->setValue_uint32(oh32.SizeOfStackCommit);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeapReserve]->setValue_uint32(oh32.SizeOfHeapReserve);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::SizeOfHeapCommit]->setValue_uint32(oh32.SizeOfHeapCommit);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::LoaderFlags]->setValue_uint32(oh32.LoaderFlags);
                    g_lineEdit_IMAGE_OPTIONAL_HEADER[N_IMAGE_OPTIONAL_HEADER::NumberOfRvaAndSizes]->setValue_uint32(oh32.NumberOfRvaAndSizes);

                    m_comboBox[CB_IMAGE_OPTIONAL_HEADER_Magic]->setValue(oh32.Magic);
                    m_comboBox[CB_IMAGE_OPTIONAL_HEADER_Subsystem]->setValue(oh32.Subsystem);
                    m_comboBox[CB_IMAGE_OPTIONAL_HEADER_DllCharacteristics]->setValue(oh32.DllCharacteristics);

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

                m_comboBox[CB_IMAGE_OPTIONAL_HEADER_OperationSystemVersion]->setValue(pe.getOperatingSystemVersion());

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

                ajustTableView(nType, &peProcessData, &g_tvModel[SPE::TYPE_RICH], ui->tableView_RICH, true);
            }
        } else if (nType == SPE::TYPE_SECTIONS) {
            if (!isInitPresent(sInit)) {
                XBinary::OFFSETSIZE osStringTable = pe.getStringTable();

                if (pe.isOffsetAndSizeValid(osStringTable.nOffset, osStringTable.nSize)) {
                    ui->checkBoxSectionsStringTable->show();
                } else {
                    ui->checkBoxSectionsStringTable->hide();
                }

                PEProcessData peProcessData(SPE::TYPE_SECTIONS, &g_tvModel[SPE::TYPE_SECTIONS], &pe, 0, 0, 0, false);

                ajustTableView(nType, &peProcessData, &g_tvModel[SPE::TYPE_SECTIONS], ui->tableView_Sections, false);

                connect(ui->tableView_Sections->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Sections_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SPE::TYPE_SECTIONS]->rowCount()) {
                    ui->tableView_Sections->setCurrentIndex(ui->tableView_Sections->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_SECTIONS_INFO) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessDataTree(SPE::TYPE_SECTIONS_INFO, &g_tvModel[SPE::TYPE_SECTIONS_INFO], &pe, 0, 0, 0, true);

                ajustTreeView(nType, &peProcessDataTree, &g_tvModel[SPE::TYPE_SECTIONS_INFO], ui->treeView_Sections_Info);

                //                connect(ui->treeView_Sections_Info->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                //                        SLOT(onTreeView_Sections_Info_currentRowChanged(QModelIndex, QModelIndex)));
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

                g_dateTimeEdit[TD_IMAGE_EXPORT_TimeDateStamp] = createTimeDateEdit(ui->tableWidget_ExportHeader, SPE::TYPE_EXPORT, N_IMAGE_EXPORT::TimeDateStamp);

                blockSignals(true);

                XPE::EXPORT_HEADER eh = pe.getExport();

                g_lineEdit_EXPORT[N_IMAGE_EXPORT::Characteristics]->setValue_uint32(eh.directory.Characteristics);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::TimeDateStamp]->setValue_uint32(eh.directory.TimeDateStamp);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::MajorVersion]->setValue_uint16(eh.directory.MajorVersion);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::MinorVersion]->setValue_uint16(eh.directory.MinorVersion);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::Name]->setValue_uint32(eh.directory.Name);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::Base]->setValue_uint32(eh.directory.Base);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::NumberOfFunctions]->setValue_uint32(eh.directory.NumberOfFunctions);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::NumberOfNames]->setValue_uint32(eh.directory.NumberOfNames);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::AddressOfFunctions]->setValue_uint32(eh.directory.AddressOfFunctions);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::AddressOfNames]->setValue_uint32(eh.directory.AddressOfNames);
                g_lineEdit_EXPORT[N_IMAGE_EXPORT::AddressOfNameOrdinals]->setValue_uint32(eh.directory.AddressOfNameOrdinals);

                g_dateTimeEdit[TD_IMAGE_EXPORT_TimeDateStamp]->setValue(eh.directory.TimeDateStamp, XDateTimeEditX::DT_TYPE_POSIX);

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

                ajustTableView(nType, &peProcessData, &g_tvModel[SPE::TYPE_EXPORT_FUNCTION], ui->tableView_ExportFunctions);

                if (g_tvModel[SPE::TYPE_EXPORT_FUNCTION]->rowCount()) {
                    ui->tableView_ExportFunctions->setCurrentIndex(ui->tableView_ExportFunctions->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_IMPORT) {
            if (!isInitPresent(sInit)) {
                XBinary::_MEMORY_MAP memoryMap = pe.getMemoryMap();
                QList<XPE::IMPORT_RECORD> listImportRecords = pe.getImportRecords(&memoryMap);

                ui->lineEditHash64->setValue_uint64(pe.getImportHash64(&listImportRecords));
                ui->lineEditHash32->setValue_uint32(pe.getImportHash32(&listImportRecords));

                PEProcessData peProcessData(SPE::TYPE_IMPORT, &g_tvModel[SPE::TYPE_IMPORT], &pe, 0, 0, 0);

                ajustTableView(nType, &peProcessData, &g_tvModel[SPE::TYPE_IMPORT], ui->tableView_ImportLibraries);

                connect(ui->tableView_ImportLibraries->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_ImportLibraries_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SPE::TYPE_IMPORT]->rowCount()) {
                    ui->tableView_ImportLibraries->setCurrentIndex(ui->tableView_ImportLibraries->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_IMPORT_INFO) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessDataTree(SPE::TYPE_IMPORT_INFO, &g_tvModel[SPE::TYPE_IMPORT_INFO], &pe, 0, 0, 0, true);

                ajustTreeView(nType, &peProcessDataTree, &g_tvModel[SPE::TYPE_IMPORT_INFO], ui->treeView_Import_Info);

                //                connect(ui->treeView_Sections_Info->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                //                        SLOT(onTreeView_Sections_Info_currentRowChanged(QModelIndex, QModelIndex)));
            }
        } else if (nType == SPE::TYPE_RESOURCES) {
            if (!isInitPresent(sInit)) {
                // Table
                {
                    PEProcessData peProcessData(SPE::TYPE_RESOURCES, &g_tvModel[SPE::TYPE_RESOURCES], &pe, 0, 0, 0, false);

                    ajustTableView(nType, &peProcessData, &g_tvModel[SPE::TYPE_RESOURCES], ui->tableView_Resources, true);

                    connect(ui->tableView_Resources->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                            SLOT(onTableView_Resources_currentRowChanged(QModelIndex, QModelIndex)));

                    if (g_tvModel[SPE::TYPE_RESOURCES]->rowCount()) {
                        ui->tableView_Resources->setCurrentIndex(ui->tableView_Resources->model()->index(0, 0));
                    }
                }

                // Tree
                {
                    createListTable(SPE::TYPE_RESOURCES, ui->tableWidget_Resources, N_IMAGE_RESOURCES::records, g_lineEdit_Resources, N_IMAGE_RESOURCES::__data_size);

                    PEProcessData peProcessDataTree(SPE::TYPE_RESOURCES, &g_tvModel[SPE::TYPE_RESOURCES_TREE], &pe, 0, 0, 0, true);

                    ajustTreeView(nType, &peProcessDataTree, &g_tvModel[SPE::TYPE_RESOURCES_TREE], ui->treeView_Resources);

                    connect(ui->treeView_Resources->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                            SLOT(onTreeView_Resources_currentRowChanged(QModelIndex, QModelIndex)));
                }

                QList<XPE::RESOURCE_RECORD> listResources = pe.getResources(10000);

                ui->pushButtonExtractAllCursors_Resources->setEnabled(pe.isResourceGroupCursorsPresent(&listResources));
                ui->pushButtonExtractAllIcons_Resources->setEnabled(pe.isResourceGroupIconsPresent(&listResources));
            }
        } else if (nType == SPE::TYPE_RESOURCES_STRINGTABLE) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_RESOURCES_STRINGTABLE, &g_tvModel[SPE::TYPE_RESOURCES_STRINGTABLE], &pe, 0, 0, 0, false);

                ajustTableView(nType, &peProcessData, &g_tvModel[SPE::TYPE_RESOURCES_STRINGTABLE], ui->tableView_Resources_StringTable, true);

                //                connect(ui->tableView_Resources_StringTable->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_Resources_StringTable_currentRowChanged(QModelIndex,QModelIndex)));

                //                if(g_tvModel[SPE::TYPE_RESOURCES_STRINGTABLE]->rowCount())
                //                {
                //                    ui->tableView_Resources_StringTable->setCurrentIndex(ui->tableView_Resources_StringTable->model()->index(0,0));
                //                }
            }
        } else if (nType == SPE::TYPE_RESOURCES_VERSION) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SPE::TYPE_RESOURCES_VERSION, ui->tableWidget_Resources_Version, N_IMAGE_RESOURCE_FIXEDFILEINFO::records,
                                  g_lineEdit_Version_FixedFileInfo, N_IMAGE_RESOURCE_FIXEDFILEINFO::__data_size, 0);

                m_comboBox[CB_RESOURCES_VERSION_dwSignature] =
                    createComboBox(ui->tableWidget_Resources_Version, XPE::getResourcesFixedFileInfoSignaturesS(), SPE::TYPE_RESOURCES_VERSION,
                                   N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature, XComboBoxEx::CBTYPE_LIST);
                m_comboBox[CB_RESOURCES_VERSION_dwFileFlags] =
                    createComboBox(ui->tableWidget_Resources_Version, XPE::getResourcesFixedFileInfoFileFlagsS(), SPE::TYPE_RESOURCES_VERSION,
                                   N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags, XComboBoxEx::CBTYPE_FLAGS);
                m_comboBox[CB_RESOURCES_VERSION_dwFileOS] =
                    createComboBox(ui->tableWidget_Resources_Version, XPE::getResourcesFixedFileInfoFileOsesS(), SPE::TYPE_RESOURCES_VERSION,
                                   N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS, XComboBoxEx::CBTYPE_LIST);
                m_comboBox[CB_RESOURCES_VERSION_dwFileType] =
                    createComboBox(ui->tableWidget_Resources_Version, XPE::getResourcesFixedFileInfoFileTypesS(), SPE::TYPE_RESOURCES_VERSION,
                                   N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType, XComboBoxEx::CBTYPE_LIST);

                blockSignals(true);

                XPE::RESOURCES_VERSION resourceVersion = pe.getResourcesVersion();

                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwSignature]->setValue_uint32(resourceVersion.fileInfo.dwSignature);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwStrucVersion]->setValue_uint32(resourceVersion.fileInfo.dwStrucVersion);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionMS]->setValue_uint32(resourceVersion.fileInfo.dwFileVersionMS);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileVersionLS]->setValue_uint32(resourceVersion.fileInfo.dwFileVersionLS);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionMS]->setValue_uint32(resourceVersion.fileInfo.dwProductVersionMS);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwProductVersionLS]->setValue_uint32(resourceVersion.fileInfo.dwProductVersionLS);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlagsMask]->setValue_uint32(resourceVersion.fileInfo.dwFileFlagsMask);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileFlags]->setValue_uint32(resourceVersion.fileInfo.dwFileFlags);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileOS]->setValue_uint32(resourceVersion.fileInfo.dwFileOS);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileType]->setValue_uint32(resourceVersion.fileInfo.dwFileType);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileSubtype]->setValue_uint32(resourceVersion.fileInfo.dwFileSubtype);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileDateMS]->setValue_uint32(resourceVersion.fileInfo.dwFileDateMS);
                g_lineEdit_Version_FixedFileInfo[N_IMAGE_RESOURCE_FIXEDFILEINFO::dwFileDateLS]->setValue_uint32(resourceVersion.fileInfo.dwFileDateLS);

                m_comboBox[CB_RESOURCES_VERSION_dwSignature]->setValue(resourceVersion.fileInfo.dwSignature);
                m_comboBox[CB_RESOURCES_VERSION_dwFileFlags]->setValue(resourceVersion.fileInfo.dwFileFlags);
                m_comboBox[CB_RESOURCES_VERSION_dwFileOS]->setValue(resourceVersion.fileInfo.dwFileOS);
                m_comboBox[CB_RESOURCES_VERSION_dwFileType]->setValue(resourceVersion.fileInfo.dwFileType);
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

                ajustTableView(nType, &peProcessData, &g_tvModel[SPE::TYPE_EXCEPTION], ui->tableView_Exceptions, false);

                connect(ui->tableView_Exceptions->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Exceptions_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SPE::TYPE_EXCEPTION]->rowCount()) {
                    ui->tableView_Exceptions->setCurrentIndex(ui->tableView_Exceptions->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_RELOCS) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_RELOCS, &g_tvModel[SPE::TYPE_RELOCS], &pe, 0, 0, 0);

                ajustTableView(nType, &peProcessData, &g_tvModel[SPE::TYPE_RELOCS], ui->tableView_Relocs, false);

                connect(ui->tableView_Relocs->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Relocs_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SPE::TYPE_RELOCS]->rowCount()) {
                    ui->tableView_Relocs->setCurrentIndex(ui->tableView_Relocs->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_DEBUG) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_DEBUG, &g_tvModel[SPE::TYPE_DEBUG], &pe, 0, 0, 0);

                ajustTableView(nType, &peProcessData, &g_tvModel[SPE::TYPE_DEBUG], ui->tableView_Debug, false);

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
                g_invWidget[INV_IMAGE_TLS_EndAddressOfRawData] =
                    createInvWidget(ui->tableWidget_TLS, SPE::TYPE_TLS, N_IMAGE_TLS::EndAddressOfRawData, InvWidget::TYPE_HEX);
                g_invWidget[INV_IMAGE_TLS_StartAddressOfRawData] =
                    createInvWidget(ui->tableWidget_TLS, SPE::TYPE_TLS, N_IMAGE_TLS::StartAddressOfRawData, InvWidget::TYPE_HEX);

                blockSignals(true);

                XBinary::_MEMORY_MAP memoryMap = pe.getMemoryMap();

                if (bIs64) {
                    XPE_DEF::S_IMAGE_TLS_DIRECTORY64 tls64 = pe.getTLSDirectory64();
                    g_lineEdit_TLS[N_IMAGE_TLS::StartAddressOfRawData]->setValue_uint64(tls64.StartAddressOfRawData);
                    g_lineEdit_TLS[N_IMAGE_TLS::EndAddressOfRawData]->setValue_uint64(tls64.EndAddressOfRawData);
                    g_lineEdit_TLS[N_IMAGE_TLS::AddressOfIndex]->setValue_uint64(tls64.AddressOfIndex);
                    g_lineEdit_TLS[N_IMAGE_TLS::AddressOfCallBacks]->setValue_uint64(tls64.AddressOfCallBacks);
                    g_lineEdit_TLS[N_IMAGE_TLS::SizeOfZeroFill]->setValue_uint32(tls64.SizeOfZeroFill);
                    g_lineEdit_TLS[N_IMAGE_TLS::Characteristics]->setValue_uint32(tls64.Characteristics);

                    g_invWidget[INV_IMAGE_TLS_AddressOfCallBacks]->setAddressAndSize(&pe, tls64.AddressOfCallBacks, 0);
                    g_invWidget[INV_IMAGE_TLS_AddressOfIndex]->setAddressAndSize(&pe, tls64.AddressOfIndex, 0);
                    g_invWidget[INV_IMAGE_TLS_EndAddressOfRawData]->setAddressAndSize(&pe, tls64.EndAddressOfRawData, 0);
                    g_invWidget[INV_IMAGE_TLS_StartAddressOfRawData]->setAddressAndSize(&pe, tls64.StartAddressOfRawData, 0);

                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::AddressOfCallBacks, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByAddress(&memoryMap, tls64.AddressOfCallBacks));
                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::AddressOfIndex, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByAddress(&memoryMap, tls64.AddressOfIndex));
                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::EndAddressOfRawData, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByAddress(&memoryMap, tls64.EndAddressOfRawData));
                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::StartAddressOfRawData, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByAddress(&memoryMap, tls64.StartAddressOfRawData));
                } else {
                    XPE_DEF::S_IMAGE_TLS_DIRECTORY32 tls32 = pe.getTLSDirectory32();
                    g_lineEdit_TLS[N_IMAGE_TLS::StartAddressOfRawData]->setValue_uint32(tls32.StartAddressOfRawData);
                    g_lineEdit_TLS[N_IMAGE_TLS::EndAddressOfRawData]->setValue_uint32(tls32.EndAddressOfRawData);
                    g_lineEdit_TLS[N_IMAGE_TLS::AddressOfIndex]->setValue_uint32(tls32.AddressOfIndex);
                    g_lineEdit_TLS[N_IMAGE_TLS::AddressOfCallBacks]->setValue_uint32(tls32.AddressOfCallBacks);
                    g_lineEdit_TLS[N_IMAGE_TLS::SizeOfZeroFill]->setValue_uint32(tls32.SizeOfZeroFill);
                    g_lineEdit_TLS[N_IMAGE_TLS::Characteristics]->setValue_uint32(tls32.Characteristics);

                    g_invWidget[INV_IMAGE_TLS_AddressOfCallBacks]->setAddressAndSize(&pe, tls32.AddressOfCallBacks, 0);
                    g_invWidget[INV_IMAGE_TLS_AddressOfIndex]->setAddressAndSize(&pe, tls32.AddressOfIndex, 0);
                    g_invWidget[INV_IMAGE_TLS_EndAddressOfRawData]->setAddressAndSize(&pe, tls32.EndAddressOfRawData, 0);
                    g_invWidget[INV_IMAGE_TLS_StartAddressOfRawData]->setAddressAndSize(&pe, tls32.StartAddressOfRawData, 0);

                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::AddressOfCallBacks, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByAddress(&memoryMap, tls32.AddressOfCallBacks));
                    addComment(ui->tableWidget_TLS, N_IMAGE_TLS::AddressOfIndex, HEADER_COLUMN_COMMENT,
                               pe.getMemoryRecordInfoByAddress(&memoryMap, tls32.AddressOfIndex));
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

                ajustTableView(nType, &peProcessData, &g_tvModel[SPE::TYPE_TLSCALLBACKS], ui->tableView_TLSCallbacks, true);

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

                    if (nRecordSize > N_IMAGE_LOADCONFIG::Size) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Size]->setValue_uint32(lc64.Size);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::TimeDateStamp) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::TimeDateStamp]->setValue_uint32(lc64.TimeDateStamp);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::MinorVersion) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MinorVersion]->setValue_uint16(lc64.MinorVersion);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::MajorVersion) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MajorVersion]->setValue_uint16(lc64.MajorVersion);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GlobalFlagsClear)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GlobalFlagsClear]->setValue_uint32(lc64.GlobalFlagsClear);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GlobalFlagsSet) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GlobalFlagsSet]->setValue_uint32(lc64.GlobalFlagsSet);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CriticalSectionDefaultTimeout)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CriticalSectionDefaultTimeout]->setValue_uint32(lc64.CriticalSectionDefaultTimeout);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DeCommitFreeBlockThreshold)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DeCommitFreeBlockThreshold]->setValue_uint64(lc64.DeCommitFreeBlockThreshold);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DeCommitTotalFreeThreshold)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DeCommitTotalFreeThreshold]->setValue_uint64(lc64.DeCommitTotalFreeThreshold);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::LockPrefixTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::LockPrefixTable]->setValue_uint64(lc64.LockPrefixTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::MaximumAllocationSize)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MaximumAllocationSize]->setValue_uint64(lc64.MaximumAllocationSize);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::VirtualMemoryThreshold)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::VirtualMemoryThreshold]->setValue_uint64(lc64.VirtualMemoryThreshold);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::ProcessAffinityMask)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::ProcessAffinityMask]->setValue_uint64(lc64.ProcessAffinityMask);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CSDVersion) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CSDVersion]->setValue_uint16(lc64.CSDVersion);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DependentLoadFlags)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DependentLoadFlags]->setValue_uint16(lc64.DependentLoadFlags);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::EditList) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::EditList]->setValue_uint64(lc64.EditList);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SecurityCookie) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SecurityCookie]->setValue_uint64(lc64.SecurityCookie);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SEHandlerTable) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SEHandlerTable]->setValue_uint64(lc64.SEHandlerTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SEHandlerCount) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SEHandlerCount]->setValue_uint64(lc64.SEHandlerCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer]->setValue_uint64(lc64.GuardCFCheckFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer]->setValue_uint64(lc64.GuardCFDispatchFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFFunctionTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFFunctionTable]->setValue_uint64(lc64.GuardCFFunctionTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFFunctionCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFFunctionCount]->setValue_uint64(lc64.GuardCFFunctionCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardFlags) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardFlags]->setValue_uint32(lc64.GuardFlags);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_Flags)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_Flags]->setValue_uint16(lc64.CodeIntegrity.Flags);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_Catalog)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_Catalog]->setValue_uint16(lc64.CodeIntegrity.Catalog);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_CatalogOffset)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_CatalogOffset]->setValue_uint32(lc64.CodeIntegrity.CatalogOffset);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_Reserved)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_Reserved]->setValue_uint32(lc64.CodeIntegrity.Reserved);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryTable]->setValue_uint64(lc64.GuardAddressTakenIatEntryTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryCount]->setValue_uint64(lc64.GuardAddressTakenIatEntryCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardLongJumpTargetTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardLongJumpTargetTable]->setValue_uint64(lc64.GuardLongJumpTargetTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardLongJumpTargetCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardLongJumpTargetCount]->setValue_uint64(lc64.GuardLongJumpTargetCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DynamicValueRelocTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DynamicValueRelocTable]->setValue_uint64(lc64.DynamicValueRelocTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CHPEMetadataPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CHPEMetadataPointer]->setValue_uint64(lc64.CHPEMetadataPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardRFFailureRoutine)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardRFFailureRoutine]->setValue_uint64(lc64.GuardRFFailureRoutine);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardRFFailureRoutineFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardRFFailureRoutineFunctionPointer]->setValue_uint64(lc64.GuardRFFailureRoutineFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DynamicValueRelocTableOffset)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DynamicValueRelocTableOffset]->setValue_uint32(lc64.DynamicValueRelocTableOffset);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DynamicValueRelocTableSection)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DynamicValueRelocTableSection]->setValue_uint16(lc64.DynamicValueRelocTableSection);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::Reserved2) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Reserved2]->setValue_uint16(lc64.Reserved2);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardRFVerifyStackPointerFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardRFVerifyStackPointerFunctionPointer]->setValue_uint64(
                            lc64.GuardRFVerifyStackPointerFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::HotPatchTableOffset)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::HotPatchTableOffset]->setValue_uint32(lc64.HotPatchTableOffset);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::Reserved3) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Reserved3]->setValue_uint32(lc64.Reserved3);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::EnclaveConfigurationPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::EnclaveConfigurationPointer]->setValue_uint64(lc64.EnclaveConfigurationPointer);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::VolatileMetadataPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::VolatileMetadataPointer]->setValue_uint64(lc64.VolatileMetadataPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardEHContinuationTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardEHContinuationTable]->setValue_uint64(lc64.GuardEHContinuationTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardEHContinuationCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardEHContinuationCount]->setValue_uint64(lc64.GuardEHContinuationCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardXFGCheckFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardXFGCheckFunctionPointer]->setValue_uint64(lc64.GuardXFGCheckFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardXFGDispatchFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardXFGDispatchFunctionPointer]->setValue_uint64(lc64.GuardXFGDispatchFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardXFGTableDispatchFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardXFGTableDispatchFunctionPointer]->setValue_uint64(lc64.GuardXFGTableDispatchFunctionPointer);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::CastGuardOsDeterminedFailureMode)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CastGuardOsDeterminedFailureMode]->setValue_uint64(lc64.CastGuardOsDeterminedFailureMode);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer]->setValue_uint64(lc64.GuardMemcpyFunctionPointer);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::Size)
                        addComment(ui->tableWidget_LoadConfig, N_IMAGE_LOADCONFIG::Size, HEADER_COLUMN_COMMENT, XBinary::bytesCountToString(lc64.Size));

                    if (nRecordSize > N_IMAGE_LOADCONFIG::SecurityCookie)
                        g_invWidget[INV_IMAGE_LOADCONFIG_SecurityCookie]->setAddressAndSize(&pe, lc64.SecurityCookie, 0);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SEHandlerTable)
                        g_invWidget[INV_IMAGE_LOADCONFIG_SEHandlerTable]->setAddressAndSize(&pe, lc64.SEHandlerTable, 0);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer)
                        g_invWidget[INV_IMAGE_LOADCONFIG_GuardCFCheckFunctionPointer]->setAddressAndSize(&pe, lc64.GuardCFCheckFunctionPointer, 0);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer)
                        g_invWidget[INV_IMAGE_LOADCONFIG_GuardCFDispatchFunctionPointer]->setAddressAndSize(&pe, lc64.GuardCFDispatchFunctionPointer, 0);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer)
                        g_invWidget[INV_IMAGE_LOADCONFIG_GuardMemcpyFunctionPointer]->setAddressAndSize(&pe, lc64.GuardMemcpyFunctionPointer, 0);
                } else {
                    XPE_DEF::S_IMAGE_LOAD_CONFIG_DIRECTORY32 lc32 = pe.getLoadConfigDirectory32();

                    if (nRecordSize > N_IMAGE_LOADCONFIG::Size) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Size]->setValue_uint32(lc32.Size);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::TimeDateStamp) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::TimeDateStamp]->setValue_uint32(lc32.TimeDateStamp);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::MinorVersion) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MinorVersion]->setValue_uint16(lc32.MinorVersion);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::MajorVersion) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MajorVersion]->setValue_uint16(lc32.MajorVersion);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GlobalFlagsClear)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GlobalFlagsClear]->setValue_uint32(lc32.GlobalFlagsClear);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GlobalFlagsSet) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GlobalFlagsSet]->setValue_uint32(lc32.GlobalFlagsSet);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CriticalSectionDefaultTimeout)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CriticalSectionDefaultTimeout]->setValue_uint32(lc32.CriticalSectionDefaultTimeout);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DeCommitFreeBlockThreshold)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DeCommitFreeBlockThreshold]->setValue_uint32(lc32.DeCommitFreeBlockThreshold);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DeCommitTotalFreeThreshold)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DeCommitTotalFreeThreshold]->setValue_uint32(lc32.DeCommitTotalFreeThreshold);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::LockPrefixTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::LockPrefixTable]->setValue_uint32(lc32.LockPrefixTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::MaximumAllocationSize)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::MaximumAllocationSize]->setValue_uint32(lc32.MaximumAllocationSize);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::VirtualMemoryThreshold)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::VirtualMemoryThreshold]->setValue_uint32(lc32.VirtualMemoryThreshold);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::ProcessAffinityMask)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::ProcessAffinityMask]->setValue_uint32(lc32.ProcessAffinityMask);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CSDVersion) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CSDVersion]->setValue_uint16(lc32.CSDVersion);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DependentLoadFlags)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DependentLoadFlags]->setValue_uint16(lc32.DependentLoadFlags);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::EditList) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::EditList]->setValue_uint32(lc32.EditList);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SecurityCookie) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SecurityCookie]->setValue_uint32(lc32.SecurityCookie);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SEHandlerTable) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SEHandlerTable]->setValue_uint32(lc32.SEHandlerTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SEHandlerCount) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::SEHandlerCount]->setValue_uint32(lc32.SEHandlerCount);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFCheckFunctionPointer]->setValue_uint32(lc32.GuardCFCheckFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFDispatchFunctionPointer]->setValue_uint32(lc32.GuardCFDispatchFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFFunctionTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFFunctionTable]->setValue_uint32(lc32.GuardCFFunctionTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardCFFunctionCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardCFFunctionCount]->setValue_uint32(lc32.GuardCFFunctionCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardFlags) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardFlags]->setValue_uint32(lc32.GuardFlags);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_Flags)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_Flags]->setValue_uint16(lc32.CodeIntegrity.Flags);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_Catalog)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_Catalog]->setValue_uint16(lc32.CodeIntegrity.Catalog);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_CatalogOffset)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_CatalogOffset]->setValue_uint32(lc32.CodeIntegrity.CatalogOffset);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CodeIntegrity_Reserved)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CodeIntegrity_Reserved]->setValue_uint32(lc32.CodeIntegrity.Reserved);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryTable]->setValue_uint32(lc32.GuardAddressTakenIatEntryTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardAddressTakenIatEntryCount]->setValue_uint32(lc32.GuardAddressTakenIatEntryCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardLongJumpTargetTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardLongJumpTargetTable]->setValue_uint32(lc32.GuardLongJumpTargetTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardLongJumpTargetCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardLongJumpTargetCount]->setValue_uint32(lc32.GuardLongJumpTargetCount);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DynamicValueRelocTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DynamicValueRelocTable]->setValue_uint32(lc32.DynamicValueRelocTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::CHPEMetadataPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CHPEMetadataPointer]->setValue_uint32(lc32.CHPEMetadataPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardRFFailureRoutine)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardRFFailureRoutine]->setValue_uint32(lc32.GuardRFFailureRoutine);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardRFFailureRoutineFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardRFFailureRoutineFunctionPointer]->setValue_uint32(lc32.GuardRFFailureRoutineFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DynamicValueRelocTableOffset)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DynamicValueRelocTableOffset]->setValue_uint32(lc32.DynamicValueRelocTableOffset);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::DynamicValueRelocTableSection)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::DynamicValueRelocTableSection]->setValue_uint16(lc32.DynamicValueRelocTableSection);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::Reserved2) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Reserved2]->setValue_uint16(lc32.Reserved2);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardRFVerifyStackPointerFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardRFVerifyStackPointerFunctionPointer]->setValue_uint32(
                            lc32.GuardRFVerifyStackPointerFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::HotPatchTableOffset)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::HotPatchTableOffset]->setValue_uint32(lc32.HotPatchTableOffset);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::Reserved3) g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::Reserved3]->setValue_uint32(lc32.Reserved3);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::EnclaveConfigurationPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::EnclaveConfigurationPointer]->setValue_uint32(lc32.EnclaveConfigurationPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::VolatileMetadataPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::VolatileMetadataPointer]->setValue_uint32(lc32.VolatileMetadataPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardEHContinuationTable)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardEHContinuationTable]->setValue_uint32(lc32.GuardEHContinuationTable);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardEHContinuationCount)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardEHContinuationCount]->setValue_uint32(lc32.GuardEHContinuationCount);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardXFGCheckFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardXFGCheckFunctionPointer]->setValue_uint32(lc32.GuardXFGCheckFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardXFGDispatchFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardXFGDispatchFunctionPointer]->setValue_uint32(lc32.GuardXFGDispatchFunctionPointer);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardXFGTableDispatchFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardXFGTableDispatchFunctionPointer]->setValue_uint32(lc32.GuardXFGTableDispatchFunctionPointer);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::CastGuardOsDeterminedFailureMode)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::CastGuardOsDeterminedFailureMode]->setValue_uint32(lc32.CastGuardOsDeterminedFailureMode);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer)
                        g_lineEdit_LoadConfig[N_IMAGE_LOADCONFIG::GuardMemcpyFunctionPointer]->setValue_uint32(lc32.GuardMemcpyFunctionPointer);

                    if (nRecordSize > N_IMAGE_LOADCONFIG::Size)
                        addComment(ui->tableWidget_LoadConfig, N_IMAGE_LOADCONFIG::Size, HEADER_COLUMN_COMMENT, XBinary::bytesCountToString(lc32.Size));

                    if (nRecordSize > N_IMAGE_LOADCONFIG::SecurityCookie)
                        g_invWidget[INV_IMAGE_LOADCONFIG_SecurityCookie]->setAddressAndSize(&pe, lc32.SecurityCookie, 0);
                    if (nRecordSize > N_IMAGE_LOADCONFIG::SEHandlerTable)
                        g_invWidget[INV_IMAGE_LOADCONFIG_SEHandlerTable]->setAddressAndSize(&pe, lc32.SEHandlerTable, 0);

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

                ajustTableView(nType, &peProcessData, &g_tvModel[SPE::TYPE_BOUNDIMPORT], ui->tableView_BoundImport);

                if (g_tvModel[SPE::TYPE_BOUNDIMPORT]->rowCount()) {
                    ui->tableView_BoundImport->setCurrentIndex(ui->tableView_BoundImport->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_DELAYIMPORT) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_DELAYIMPORT, &g_tvModel[SPE::TYPE_DELAYIMPORT], &pe, 0, 0, 0);

                ajustTableView(nType, &peProcessData, &g_tvModel[SPE::TYPE_DELAYIMPORT], ui->tableView_DelayImportLibraries);

                connect(ui->tableView_DelayImportLibraries->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_DelayImportLibraries_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SPE::TYPE_DELAYIMPORT]->rowCount()) {
                    ui->tableView_DelayImportLibraries->setCurrentIndex(ui->tableView_DelayImportLibraries->model()->index(0, 0));
                }
            }
        } else if (nType == SPE::TYPE_NETHEADER) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SPE::TYPE_NETHEADER, ui->tableWidget_NetHeader, N_IMAGE_NETHEADER::records, g_lineEdit_NetHeader, N_IMAGE_NETHEADER::__data_size, 0);

                m_comboBox[CB_IMAGE_NETHEADER_FLAGS] =
                    createComboBox(ui->tableWidget_NetHeader, XPE::getComImageFlagsS(), SPE::TYPE_NETHEADER, N_IMAGE_NETHEADER::Flags, XComboBoxEx::CBTYPE_FLAGS);

                blockSignals(true);

                XPE_DEF::IMAGE_COR20_HEADER netHeader = pe.getNetHeader();

                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::cb]->setValue_uint32(netHeader.cb);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::MajorRuntimeVersion]->setValue_uint16(netHeader.MajorRuntimeVersion);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::MinorRuntimeVersion]->setValue_uint16(netHeader.MinorRuntimeVersion);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::MetaData_Address]->setValue_uint32(netHeader.MetaData.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::MetaData_Size]->setValue_uint32(netHeader.MetaData.Size);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::Flags]->setValue_uint32(netHeader.Flags);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::EntryPoint]->setValue_uint32(netHeader.EntryPointRVA);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::Resources_Address]->setValue_uint32(netHeader.Resources.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::Resources_Size]->setValue_uint32(netHeader.Resources.Size);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::StrongNameSignature_Address]->setValue_uint32(netHeader.StrongNameSignature.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::StrongNameSignature_Size]->setValue_uint32(netHeader.StrongNameSignature.Size);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::CodeManagerTable_Address]->setValue_uint32(netHeader.CodeManagerTable.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::CodeManagerTable_Size]->setValue_uint32(netHeader.CodeManagerTable.Size);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::VTableFixups_Address]->setValue_uint32(netHeader.VTableFixups.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::VTableFixups_Size]->setValue_uint32(netHeader.VTableFixups.Size);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::ExportAddressTableJumps_Address]->setValue_uint32(netHeader.ExportAddressTableJumps.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::ExportAddressTableJumps_Size]->setValue_uint32(netHeader.ExportAddressTableJumps.Size);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::ManagedNativeHeader_Address]->setValue_uint32(netHeader.ManagedNativeHeader.VirtualAddress);
                g_lineEdit_NetHeader[N_IMAGE_NETHEADER::ManagedNativeHeader_Size]->setValue_uint32(netHeader.ManagedNativeHeader.Size);

                m_comboBox[CB_IMAGE_NETHEADER_FLAGS]->setValue(netHeader.Flags);

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

                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::Signature]->setValue_uint32(header.nSignature);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::MajorVersion]->setValue_uint16(header.nMajorVersion);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::MinorVersion]->setValue_uint16(header.nMinorVersion);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::Reserved]->setValue_uint32(header.nReserved);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::VersionStringLength]->setValue_uint32(header.nVersionStringLength);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::Version]->setValue_String(header.sVersion, header.nVersionStringLength);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::Flags]->setValue_uint16(header.nFlags);
                g_lineEdit_Net_Metadata[N_IMAGE_NET_METADATA::Streams]->setValue_uint16(header.nStreams);

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
        } else if (nType == SPE::TYPE_NET_METADATA_TABLE) {
            if (!isInitPresent(sInit)) {
                PEProcessData peProcessData(SPE::TYPE_NET_METADATA_TABLE, &g_tvModel[SPE::TYPE_NET_METADATA_TABLE], &pe, 0, 0, 0);

                ajustTableView(nType, &peProcessData, &g_tvModel[SPE::TYPE_NET_METADATA_TABLE], ui->tableView_Net_Metadata_Table, false);

                connect(ui->tableView_Net_Metadata_Table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_Net_Metadata_Table_currentRowChanged(QModelIndex, QModelIndex)));

                if (g_tvModel[SPE::TYPE_NET_METADATA_TABLE]->rowCount()) {
                    ui->tableView_Net_Metadata_Table->setCurrentIndex(ui->tableView_Net_Metadata_Table->model()->index(0, 0));
                }

                qint64 nOffset = nDataOffset;
                qint64 nSize = nDataSize;
                qint64 nAddress = pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_NET_METADATA_TABLE], ui->widgetHex_Net_Metadata_Table);
            }
        } else if (nType == SPE::TYPE_CERTIFICATE) {
            if (!isInitPresent(sInit)) {
                // TODO

                PEProcessData peProcessData(SPE::TYPE_CERTIFICATE, &g_tvModel[SPE::TYPE_CERTIFICATE], &pe, 0, 0, 0);

                ajustTreeView(nType, &peProcessData, &g_tvModel[SPE::TYPE_CERTIFICATE], ui->treeView_Certificate);

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
    qint32 nRow = ui->tableView_Sections->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionEdit(this);
        QAction actionHex(this);
        QAction actionDisasm(this);
        QAction actionEntropy(this);
        QAction actionDump(this);

        getShortcuts()->adjustAction(&contextMenu, &actionEdit, X_ID_TABLE_EDIT, this, SLOT(editSectionHeader()));
        getShortcuts()->adjustAction(&contextMenu, &actionHex, X_ID_TABLE_SELECTION_HEX, this, SLOT(sectionHex()));
        getShortcuts()->adjustAction(&contextMenu, &actionDisasm, X_ID_TABLE_SELECTION_DISASM, this, SLOT(sectionDisasm()));
        getShortcuts()->adjustAction(&contextMenu, &actionEntropy, X_ID_TABLE_SELECTION_ENTROPY, this, SLOT(sectionEntropy()));
        getShortcuts()->adjustAction(&contextMenu, &actionDump, X_ID_TABLE_SELECTION_DUMPTOFILE, this, SLOT(sectionDump()));

        if (!getTableViewItemSize(ui->tableView_Sections)) {
            actionHex.setEnabled(false);
            actionDisasm.setEnabled(false);
            actionEntropy.setEnabled(false);
            actionDump.setEnabled(false);
        }

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_Sections);

        contextMenu.exec(ui->tableView_Sections->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::loadImportLibrary(qint32 nRow)
{
    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (pe.isValid()) {
        PEProcessData peProcessData(SPE::TYPE_IMPORT_FUNCTION, &g_tvModel[SPE::TYPE_IMPORT_FUNCTION], &pe, nRow, 0, 0);

        ajustTableView(SPE::TYPE_IMPORT_FUNCTION, &peProcessData, &g_tvModel[SPE::TYPE_IMPORT_FUNCTION], ui->tableView_ImportFunctions);

        if (g_tvModel[SPE::TYPE_IMPORT_FUNCTION]->rowCount()) {
            ui->tableView_ImportFunctions->setCurrentIndex(ui->tableView_ImportFunctions->model()->index(0, 0));
        }
    }
}

void PEWidget::loadRelocs(qint32 nRow)
{
    QModelIndex index = ui->tableView_Relocs->model()->index(nRow, 0);

    qint64 nOffset = ui->tableView_Relocs->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();

    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (pe.isValid()) {
        PEProcessData peProcessData(SPE::TYPE_RELOCS_POSITION, &g_tvModel[SPE::TYPE_RELOCS_POSITION], &pe, 0, nOffset, 0);

        ajustTableView(SPE::TYPE_RELOCS_POSITION, &peProcessData, &g_tvModel[SPE::TYPE_RELOCS_POSITION], ui->tableView_RelocsPositions, false);

        if (g_tvModel[SPE::TYPE_RELOCS_POSITION]->rowCount()) {
            ui->tableView_RelocsPositions->setCurrentIndex(ui->tableView_RelocsPositions->model()->index(0, 0));
        }
    }
}

void PEWidget::loadSection(qint32 nRow)
{
    loadHexSubdeviceByTableView(nRow, SPE::TYPE_SECTIONS, ui->widgetHex_Section, ui->tableView_Sections, &g_subDevice[SPE::TYPE_SECTIONS]);
}

void PEWidget::loadException(qint32 nRow)
{
    loadHexSubdeviceByTableView(nRow, SPE::TYPE_EXCEPTION, ui->widgetHex_Exception, ui->tableView_Exceptions, &g_subDevice[SPE::TYPE_EXCEPTION]);
}

void PEWidget::loadDirectory(qint32 nRow)
{
    qint64 nOffset = ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(nRow, 0)->data(Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nSize = ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(nRow, 0)->data(Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();
    XADDR nAddress = ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->item(nRow, 0)->data(Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

    loadHexSubdevice(nOffset, nSize, nAddress, &g_subDevice[SPE::TYPE_IMAGE_DIRECTORY_ENTRIES], ui->widgetHex_IMAGE_DIRECTORY_ENTRIES);
}

void PEWidget::loadDebug(qint32 nRow)
{
    loadHexSubdeviceByTableView(nRow, SPE::TYPE_DEBUG, ui->widgetHex_Debug, ui->tableView_Debug, &g_subDevice[SPE::TYPE_DEBUG]);
}

void PEWidget::loadDelayImport(qint32 nRow)
{
    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (pe.isValid()) {
        PEProcessData peProcessData(SPE::TYPE_DELAYIMPORT_FUNCTION, &g_tvModel[SPE::TYPE_DELAYIMPORT_FUNCTION], &pe, nRow, 0, 0);

        ajustTableView(SPE::TYPE_DELAYIMPORT_FUNCTION, &peProcessData, &g_tvModel[SPE::TYPE_DELAYIMPORT_FUNCTION], ui->tableView_DelayImportFunctions);

        if (g_tvModel[SPE::TYPE_DELAYIMPORT]->rowCount()) {
            ui->tableView_DelayImportFunctions->setCurrentIndex(ui->tableView_DelayImportFunctions->model()->index(0, 0));
        }
    }
}

void PEWidget::adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget)
{
    FormatWidget::adjustHeaderTable(nType, pTableWidget);
}

QString PEWidget::typeIdToString(qint32 nType)
{
    QString sResult = tr("Unknown");

    switch (nType) {
        case SPE::TYPE_IMAGE_SECTION_HEADER: sResult = QString("Section %1").arg(tr("Header")); break;
        case SPE::TYPE_IMPORT: sResult = QString("Import %1").arg(tr("Header")); break;
        case SPE::TYPE_DEBUG: sResult = QString("Debug %1").arg(tr("Header")); break;
        case SPE::TYPE_EXCEPTION: sResult = QString("Exception %1").arg(tr("Header")); break;
        case SPE::TYPE_RELOCS: sResult = QString("Relocs %1").arg(tr("Header")); break;
        case SPE::TYPE_DELAYIMPORT: sResult = QString("Delay import %1").arg(tr("Header")); break;
        case SPE::TYPE_BOUNDIMPORT: sResult = QString("Bound import %1").arg(tr("Header")); break;
    }

    return sResult;
}

void PEWidget::_showInDisasmWindowAddress(XADDR nAddress)
{
    setTreeItem(ui->treeWidgetNavi, SPE::TYPE_DISASM);
    ui->widgetDisasm->setLocation(nAddress, XBinary::LT_ADDRESS, 0);
}

void PEWidget::_showInDisasmWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SPE::TYPE_DISASM);
    ui->widgetDisasm->setLocation(nOffset, XBinary::LT_OFFSET, 0);
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

void PEWidget::_findValue(quint64 nValue, XBinary::ENDIAN endian)
{
    setTreeItem(ui->treeWidgetNavi, SPE::TYPE_SEARCH);
    ui->widgetSearch->findValue(nValue, endian);
}

bool PEWidget::createSectionTable(qint32 nType, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, int nNumberOfRecords)
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

        default: pTableWidget->setColumnCount(nNumberOfRecords);
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
        case SPE::TYPE_IMAGE_DIRECTORY_ENTRIES: pTableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch); break;
    }

    return true;
}

void PEWidget::on_toolButtonReload_clicked()
{
    ui->toolButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000, this, SLOT(enableButton()));
}

void PEWidget::enableButton()
{
    ui->toolButtonReload->setEnabled(true);
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

void PEWidget::showSectionHeader(qint32 nType, QTableView *pTableView)
{
    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET).toLongLong();

        PESectionHeaderWidget *pSectionHeaderWidget = new PESectionHeaderWidget(getDevice(), getOptions(), (quint32)nRow, nOffset, nType, this);

        DialogSectionHeader dsh(this);
        dsh.setGlobal(getShortcuts(), getGlobalOptions());
        dsh.setWidget(pSectionHeaderWidget);
        dsh.setData(typeIdToString(nType));

        connect(&dsh, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(setEdited(qint64, qint64)));

        dsh.exec();

        reloadData(true);

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
    qint32 nRow = ui->tableView_ImportLibraries->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editImportHeader()));
        contextMenu.addAction(&actionEdit);

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_ImportLibraries);

        contextMenu.exec(ui->tableView_ImportLibraries->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::onTableView_Sections_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SPE::TYPE_SECTIONS, ui->widgetHex_Section, ui->tableView_Sections, &g_subDevice[SPE::TYPE_SECTIONS]);
}

void PEWidget::onTableView_Net_Metadata_Table_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    setHexSubdeviceByTableView(current.row(), SPE::TYPE_NET_METADATA_TABLE, ui->widgetHex_Net_Metadata_Table, ui->tableView_Net_Metadata_Table);
}

void PEWidget::onTableView_ImportLibraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    qint32 nRow = current.row();

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
    qint32 nRow = ui->tableView_Relocs->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editRelocsHeader()));
        contextMenu.addAction(&actionEdit);

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_Relocs);

        contextMenu.exec(ui->tableView_Relocs->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::onTableView_Relocs_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    qint32 nRow = current.row();

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
    qint32 nRow = ui->tableView_Debug->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editDebugHeader()));
        contextMenu.addAction(&actionEdit);

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_Debug);

        contextMenu.exec(ui->tableView_Debug->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::onTableView_Debug_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    qint32 nRow = current.row();

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
    qint32 nRow = ui->tableView_Resources->currentIndex().row();

    if (nRow != -1) {
        bool bIsEnable = getTableViewItemSize(ui->tableView_Resources);

        QMenu contextMenu(this);  // TODO

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

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_Resources);

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
    qint32 nRow = ui->tableView_Exceptions->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editExceptionHeader()));
        contextMenu.addAction(&actionEdit);

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_Exceptions);

        contextMenu.exec(ui->tableView_Exceptions->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::onTableView_Exceptions_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    qint32 nRow = current.row();

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
    qint32 nRow = ui->tableView_DelayImportLibraries->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editDelayImportHeader()));
        contextMenu.addAction(&actionEdit);

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_DelayImportLibraries);

        contextMenu.exec(ui->tableView_DelayImportLibraries->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::onTableView_DelayImportLibraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    qint32 nRow = current.row();

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
    qint32 nRow = ui->tableView_BoundImport->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editBoundImportHeader()));
        contextMenu.addAction(&actionEdit);

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_BoundImport);

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

        g_lineEdit_Resources[N_IMAGE_RESOURCES::ID1]->setValue_String(sID1);
        g_lineEdit_Resources[N_IMAGE_RESOURCES::ID2]->setValue_String(sID2);
        g_lineEdit_Resources[N_IMAGE_RESOURCES::ID3]->setValue_String(sID3);
        g_lineEdit_Resources[N_IMAGE_RESOURCES::ADDRESS]->setValue_uint32((quint32)nAddress);
        g_lineEdit_Resources[N_IMAGE_RESOURCES::OFFSET]->setValue_uint32((quint32)nOffset);
        g_lineEdit_Resources[N_IMAGE_RESOURCES::SIZE]->setValue_uint32((quint32)nSize);

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
    qint32 nRow = ui->tableView_ExportFunctions->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionHex(tr("Hex"), this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(exportFunctionHex()));
        contextMenu.addAction(&actionHex);

        QAction actionDisasm(tr("Disasm"), this);
        connect(&actionDisasm, SIGNAL(triggered()), this, SLOT(exportFunctionDisasm()));
        contextMenu.addAction(&actionDisasm);

        QAction actionDemangle(tr("Demangle"), this);
        connect(&actionDemangle, SIGNAL(triggered()), this, SLOT(exportFunctionDemangle()));
        contextMenu.addAction(&actionDemangle);

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_ExportFunctions);

        contextMenu.exec(ui->tableView_ExportFunctions->viewport()->mapToGlobal(pos));
    }
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

        ajustTableView(SPE::TYPE_EXPORT_FUNCTION, &peProcessData, &g_tvModel[SPE::TYPE_EXPORT_FUNCTION], ui->tableView_ExportFunctions);
    }
}

void PEWidget::on_tableView_ImportFunctions_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_ImportFunctions->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionDemangle(tr("Demangle"), this);
        connect(&actionDemangle, SIGNAL(triggered()), this, SLOT(importFunctionDemangle()));
        contextMenu.addAction(&actionDemangle);

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_ImportFunctions);

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
    qint32 nRow = ui->tableWidget_IMAGE_DIRECTORY_ENTRIES->currentIndex().row();

    if (nRow != -1) {
        bool bIsEnable = getTableViewItemSize(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES);

        QMenu contextMenu(this);  // TODO

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

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableWidget_IMAGE_DIRECTORY_ENTRIES);

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
    qint32 nRow = ui->tableView_TLSCallbacks->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionDisasm(tr("Disasm"), this);
        connect(&actionDisasm, SIGNAL(triggered()), this, SLOT(disasmTLSCallback()));
        contextMenu.addAction(&actionDisasm);

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_TLSCallbacks);

        contextMenu.exec(ui->tableView_TLSCallbacks->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::disasmTLSCallback()
{
    showSectionDisasm(ui->tableView_TLSCallbacks);
}

void PEWidget::on_pushButtonSave_Sections_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_Sections->getProxyModel(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(tr("Sections"))));
}

void PEWidget::on_tableView_Resources_StringTable_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_Resources_StringTable->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO

        QAction actionHex(tr("Hex"), this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(stringTableHex()));
        contextMenu.addAction(&actionHex);

        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_Resources_StringTable);

        contextMenu.exec(ui->tableView_Resources_StringTable->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::stringTableHex()
{
    showSectionHex(ui->tableView_Resources_StringTable);
}

void PEWidget::on_pushButtonSave_Debug_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_Debug->getProxyModel(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Debug"))));
}

void PEWidget::on_pushButtonSave_Exception_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_Exceptions->getProxyModel(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Exceptions"))));
}

void PEWidget::on_pushButtonSave_IMAGE_DIRECTORY_ENTRIES_clicked()
{
    saveHeaderTable(ui->tableWidget_IMAGE_DIRECTORY_ENTRIES, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("IMAGE_DIRECTORY_ENTRIES"))));
}

void PEWidget::on_pushButtonExtractAllIcons_Resources_clicked()
{
    QString sDirectory = QFileDialog::getExistingDirectory(this, tr("Extract all icons"), XBinary::getDeviceDirectory(getDevice()));

    if (!sDirectory.isEmpty()) {
        XFormats::saveAllPEIconsToDirectory(getDevice(), sDirectory);
    }
}

void PEWidget::on_pushButtonExtractAllCursors_Resources_clicked()
{
    QString sDirectory = QFileDialog::getExistingDirectory(this, tr("Extract all cursors"), XBinary::getDeviceDirectory(getDevice()));

    if (!sDirectory.isEmpty()) {
        XFormats::saveAllPECursorsToDirectory(getDevice(), sDirectory);
    }
}

void PEWidget::on_pushButtonDumpAll_Resources_clicked()
{
    dumpAll(ui->tableView_Resources);
}

void PEWidget::on_pushButtonSave_Resources_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_Resources->getProxyModel(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Resources"))));
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

void PEWidget::on_pushButtonSave_IMAGE_DOS_HEADER_clicked()
{
    saveHeaderTable(ui->tableWidget_IMAGE_DOS_HEADER, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("IMAGE_DOS_HEADER"))));
}

void PEWidget::on_pushButtonSave_IMAGE_OPTIONAL_HEADER_clicked()
{
    saveHeaderTable(ui->tableWidget_IMAGE_OPTIONAL_HEADER, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("IMAGE_OPTIONAL_HEADER"))));
}

void PEWidget::on_pushButtonSave_IMAGE_NT_HEADERS_clicked()
{
    saveHeaderTable(ui->tableWidget_IMAGE_NT_HEADERS, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("IMAGE_NT_HEADERS"))));
}

void PEWidget::on_pushButtonSave_IMAGE_FILE_HEADER_clicked()
{
    saveHeaderTable(ui->tableWidget_IMAGE_FILE_HEADER, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("IMAGE_FILE_HEADER"))));
}

void PEWidget::on_pushButtonSave_BoundImport_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_BoundImport->getProxyModel(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("BoundImport"))));
}

void PEWidget::on_pushButtonSave_Certificate_clicked()
{
    XShortcutsWidget::saveTreeModel(ui->treeView_Certificate->model(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Certificate"))));
}

void PEWidget::on_pushButtonSave_Sections_Info_clicked()
{
    XShortcutsWidget::saveTreeModel(ui->treeView_Sections_Info->model(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Sections_Info"))));
}

void PEWidget::on_pushButtonSave_DelayImportLibraries_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_DelayImportLibraries->getProxyModel(),
                                     XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("DelayImportLibraries"))));
}

void PEWidget::on_pushButtonSave_DelayImportFunctions_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_DelayImportFunctions->getProxyModel(),
                                     XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("DelayImportFunctions"))));
}

void PEWidget::on_pushButtonSave_ImportLibraries_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_ImportLibraries->getProxyModel(),
                                     XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("ImportLibraries"))));
}

void PEWidget::on_pushButtonSave_ImportFunctions_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_ImportFunctions->getProxyModel(),
                                     XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("ImportFunctions"))));
}

void PEWidget::on_pushButtonSave_RICH_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_RICH->getProxyModel(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("RICH"))));
}

void PEWidget::on_pushButtonSave_Relocs_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_Relocs->getProxyModel(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Relocs"))));
}

void PEWidget::on_pushButtonSave_RelocsPositions_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_RelocsPositions->getProxyModel(),
                                     XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("RelocsPositions"))));
}

void PEWidget::on_pushButtonSave_LoadConfig_clicked()
{
    saveHeaderTable(ui->tableWidget_LoadConfig, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("LoadConfig"))));
}

void PEWidget::on_pushButtonExpand_Sections_Info_clicked()
{
    ui->treeView_Sections_Info->expandAll();
}

void PEWidget::on_pushButtonCollapse_Sections_Info_clicked()
{
    ui->treeView_Sections_Info->collapseAll();
}

void PEWidget::on_treeView_Certificate_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->treeView_Certificate->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO
        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->treeView_Certificate);

        contextMenu.exec(ui->treeView_Certificate->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::on_tableView_DelayImportFunctions_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_DelayImportFunctions->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO
        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_DelayImportFunctions);

        contextMenu.exec(ui->tableView_DelayImportFunctions->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::on_tableView_RICH_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_RICH->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO
        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_RICH);

        contextMenu.exec(ui->tableView_RICH->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::on_tableView_RelocsPositions_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_RelocsPositions->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO
        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_RelocsPositions);

        contextMenu.exec(ui->tableView_RelocsPositions->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::on_treeView_Resources_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->treeView_Resources->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO
        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->tableView_Resources);

        contextMenu.exec(ui->treeView_Resources->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::on_treeView_Sections_Info_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->treeView_Sections_Info->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO
        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->treeView_Sections_Info);

        contextMenu.exec(ui->treeView_Sections_Info->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::on_pushButtonSave_TLSCallbacks_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_TLSCallbacks->getProxyModel(),
                                     XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("TLSCallbacks"))));
}

void PEWidget::on_pushButtonSave_TLS_clicked()
{
    saveHeaderTable(ui->tableWidget_TLS, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("TLS"))));
}

void PEWidget::on_pushButtonSave_Resources_Version_Header_clicked()
{
    saveHeaderTable(ui->tableWidget_Resources_Version, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Resources_Version_Header"))));
}

void PEWidget::on_pushButtonSave_Resources_Version_clicked()
{
    XShortcutsWidget::saveTextEdit(ui->textEditResources_Version, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Resources_Version"))));
}

void PEWidget::on_pushButtonSave_Resources_StringTable_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_Resources_StringTable->getProxyModel(),
                                     XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Resources_StringTable"))));
}

void PEWidget::on_pushButtonSave_Manifest_clicked()
{
    XShortcutsWidget::saveTextEdit(ui->textEditResources_Manifest, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Resources_Manifest"))));
}

void PEWidget::on_pushButtonSave_ExportHeader_clicked()
{
    saveHeaderTable(ui->tableWidget_ExportHeader, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("ExportHeader"))));
}

void PEWidget::on_pushButtonSave_ExportFunctions_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableView_ExportFunctions->getProxyModel(),
                                     XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("ExportFunctions"))));
}

void PEWidget::on_pushButtonSave_NetHeader_clicked()
{
    saveHeaderTable(ui->tableWidget_NetHeader, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("NetHeader"))));
}

void PEWidget::on_pushButtonSave_Net_Metadata_clicked()
{
    saveHeaderTable(ui->tableWidget_Net_Metadata, XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Net_Metadata"))));
}

void PEWidget::on_pushButtonSave_Import_Info_clicked()
{
    XShortcutsWidget::saveTreeModel(ui->treeView_Import_Info->model(), XBinary::getResultFileName(getDevice(), QString("%1.txt").arg(QString("Import_Info"))));
}

void PEWidget::on_pushButtonExpand_Import_Info_clicked()
{
    ui->treeView_Import_Info->expandAll();
}

void PEWidget::on_pushButtonCollapse_Import_Info_clicked()
{
    ui->treeView_Import_Info->collapseAll();
}

void PEWidget::on_treeView_Import_Info_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->treeView_Import_Info->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);  // TODO
        QMenu menuCopy(this);

        getShortcuts()->adjustRowCopyMenu(&contextMenu, &menuCopy, ui->treeView_Import_Info);
        contextMenu.exec(ui->treeView_Import_Info->viewport()->mapToGlobal(pos));
    }
}

void PEWidget::on_checkBoxSectionsStringTable_stateChanged(int nState)
{
    Q_UNUSED(nState)

    XPE pe(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (pe.isValid()) {
        PEProcessData peProcessData(SPE::TYPE_SECTIONS, &g_tvModel[SPE::TYPE_SECTIONS], &pe, 0, 0, 0, ui->checkBoxSectionsStringTable->isChecked());

        ajustTableView(SPE::TYPE_SECTIONS, &peProcessData, &g_tvModel[SPE::TYPE_SECTIONS], ui->tableView_Sections, false);

        connect(ui->tableView_Sections->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                SLOT(onTableView_Sections_currentRowChanged(QModelIndex, QModelIndex)));
    }
}
