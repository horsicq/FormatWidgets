/* Copyright (c) 2020-2025 hors<horsicq@gmail.com>
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
#include "formatswidget.h"

#include "ui_formatswidget.h"

FormatsWidget::FormatsWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::FormatsWidget)
{
    ui->setupUi(this);

    XOptions::adjustToolButton(ui->toolButtonEntryPoint, XOptions::ICONTYPE_ENTRY, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonMemoryMap, XOptions::ICONTYPE_MEMORYMAP);
    XOptions::adjustToolButton(ui->toolButtonPEExport, XOptions::ICONTYPE_EXPORT);
    XOptions::adjustToolButton(ui->toolButtonPEImport, XOptions::ICONTYPE_IMPORT);
    XOptions::adjustToolButton(ui->toolButtonPEResources, XOptions::ICONTYPE_RESOURCE);
    XOptions::adjustToolButton(ui->toolButtonPEOverlay, XOptions::ICONTYPE_OVERLAY);
    XOptions::adjustToolButton(ui->toolButtonPE, XOptions::ICONTYPE_INFO);
    XOptions::adjustToolButton(ui->toolButtonPESections, XOptions::ICONTYPE_ENTRY, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonPEManifest, XOptions::ICONTYPE_MANIFEST);
    XOptions::adjustToolButton(ui->toolButtonPEVersion, XOptions::ICONTYPE_VERSION);
    XOptions::adjustToolButton(ui->toolButtonPENET, XOptions::ICONTYPE_DOTNET);
    XOptions::adjustToolButton(ui->toolButtonPETLS, XOptions::ICONTYPE_TLS);
    XOptions::adjustToolButton(ui->toolButtonMACH, XOptions::ICONTYPE_INFO);
    XOptions::adjustToolButton(ui->toolButtonMACHSegments, XOptions::ICONTYPE_ENTRY, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonMACHSections, XOptions::ICONTYPE_ENTRY, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonMACHCommands, XOptions::ICONTYPE_ENTRY, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonMACHLibraries, XOptions::ICONTYPE_ENTRY, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonDEX, XOptions::ICONTYPE_INFO);
    XOptions::adjustToolButton(ui->toolButtonArchive, XOptions::ICONTYPE_INFO);
    XOptions::adjustToolButton(ui->toolButtonBinary, XOptions::ICONTYPE_INFO);
    XOptions::adjustToolButton(ui->toolButtonMSDOSOverlay, XOptions::ICONTYPE_OVERLAY);
    XOptions::adjustToolButton(ui->toolButtonMSDOS, XOptions::ICONTYPE_INFO);
    XOptions::adjustToolButton(ui->toolButtonELF, XOptions::ICONTYPE_INFO);
    XOptions::adjustToolButton(ui->toolButtonELFSections, XOptions::ICONTYPE_ENTRY, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonELFPrograms, XOptions::ICONTYPE_ENTRY, Qt::ToolButtonIconOnly);
    XOptions::adjustToolButton(ui->toolButtonLE, XOptions::ICONTYPE_INFO);
    XOptions::adjustToolButton(ui->toolButtonNE, XOptions::ICONTYPE_INFO);
    XOptions::adjustToolButton(ui->toolButtonAndroidManifest, XOptions::ICONTYPE_MANIFEST);
    XOptions::adjustToolButton(ui->toolButtonMANIFESTMF, XOptions::ICONTYPE_MANIFEST);
    XOptions::adjustToolButton(ui->toolButtonDisasm, XOptions::ICONTYPE_DISASM);
    XOptions::adjustToolButton(ui->toolButtonHex, XOptions::ICONTYPE_HEX);
    XOptions::adjustToolButton(ui->toolButtonStrings, XOptions::ICONTYPE_STRING);
    XOptions::adjustToolButton(ui->toolButtonEntropy, XOptions::ICONTYPE_ENTROPY);
    XOptions::adjustToolButton(ui->toolButtonHash, XOptions::ICONTYPE_HASH);
    XOptions::adjustToolButton(ui->toolButtonSearch, XOptions::ICONTYPE_SEARCH);
    XOptions::adjustToolButton(ui->toolButtonSignatures, XOptions::ICONTYPE_SIGNATURE);
    XOptions::adjustToolButton(ui->toolButtonMemoryMap, XOptions::ICONTYPE_MEMORYMAP);
    XOptions::adjustToolButton(ui->toolButtonVisualization, XOptions::ICONTYPE_VISUALIZATION);
    XOptions::adjustToolButton(ui->toolButtonMIME, XOptions::ICONTYPE_MIME);
    XOptions::adjustToolButton(ui->toolButtonYARA, XOptions::ICONTYPE_YARA);
    XOptions::adjustToolButton(ui->toolButtonExtractor, XOptions::ICONTYPE_EXTRACTOR);
    XOptions::adjustToolButton(ui->toolButtonFileInfo, XOptions::ICONTYPE_INFO);
    XOptions::adjustToolButton(ui->toolButtonFiles, XOptions::ICONTYPE_FILE);
    XOptions::adjustToolButton(ui->toolButtonVirusTotal, XOptions::ICONTYPE_VIRUSTOTAL);

    ui->comboBoxFileType->setToolTip(tr("Type"));
    ui->comboBoxScanEngine->setToolTip(tr("Scan engine"));
    ui->toolButtonEntryPoint->setToolTip(tr("Entry point"));
    ui->toolButtonMemoryMap->setToolTip(tr("Memory map"));
    ui->toolButtonPEExport->setToolTip(tr("Export"));
    ui->toolButtonPEImport->setToolTip(tr("Import"));
    ui->toolButtonPEResources->setToolTip(tr("Resources"));
    ui->toolButtonPEOverlay->setToolTip(tr("Overlay"));
    ui->toolButtonPE->setToolTip(QString("PE"));
    ui->toolButtonPESections->setToolTip(tr("Sections"));
    ui->toolButtonPEManifest->setToolTip(tr("Manifest"));
    ui->toolButtonPEVersion->setToolTip(tr("Version"));
    ui->toolButtonPENET->setToolTip(QString(".NET"));
    ui->toolButtonPETLS->setToolTip(QString("TLS"));
    ui->toolButtonMACH->setToolTip(QString("MACH"));
    ui->toolButtonMACHSegments->setToolTip(tr("Segments"));
    ui->toolButtonMACHSections->setToolTip(tr("Sections"));
    ui->toolButtonMACHCommands->setToolTip(QString("Commands"));
    ui->toolButtonMACHLibraries->setToolTip(tr("Libraries"));
    ui->toolButtonDEX->setToolTip(QString("DEX"));
    ui->toolButtonArchive->setToolTip(tr("Archive"));
    ui->toolButtonBinary->setToolTip(tr("Binary"));
    ui->toolButtonMSDOSOverlay->setToolTip(tr("Overlay"));
    ui->toolButtonMSDOS->setToolTip(QString("MSDOS"));
    ui->toolButtonELF->setToolTip(QString("ELF"));
    ui->toolButtonELFSections->setToolTip(tr("Sections"));
    ui->toolButtonELFPrograms->setToolTip(QString("Programs"));
    ui->toolButtonLE->setToolTip(QString("LE"));
    ui->toolButtonNE->setToolTip(QString("NE"));
    ui->toolButtonAndroidManifest->setToolTip(QString("AndroidManifest"));
    ui->toolButtonMANIFESTMF->setToolTip(QString("MANIFEST.MF"));
    ui->toolButtonDisasm->setToolTip(tr("Disasm"));
    ui->toolButtonHex->setToolTip(tr("Hex"));
    ui->toolButtonStrings->setToolTip(tr("Strings"));
    ui->toolButtonEntropy->setToolTip(tr("Entropy"));
    ui->toolButtonHash->setToolTip(tr("Hash"));
    ui->toolButtonSearch->setToolTip(tr("Search"));
    ui->toolButtonSignatures->setToolTip(tr("Signatures"));
    ui->toolButtonMemoryMap->setToolTip(tr("Memory map"));
    ui->toolButtonVisualization->setToolTip(tr("Visualization"));
    ui->toolButtonMIME->setToolTip(QString("MIME"));
    ui->toolButtonYARA->setToolTip(QString("YARA"));
    ui->toolButtonExtractor->setToolTip(tr("Extractor"));
    ui->toolButtonFileInfo->setToolTip(tr("File info"));
    ui->toolButtonFiles->setToolTip(tr("Files"));
    ui->toolButtonVirusTotal->setToolTip(QString("VirusTotal"));
    ui->lineEditArch->setToolTip(tr("Architecture"));
    ui->lineEditBaseAddress->setToolTip(tr("Base address"));
    ui->lineEditEndianness->setToolTip(tr("Endianness"));
    ui->lineEditEntryPoint->setToolTip(tr("Entry point"));
    ui->lineEditFileSize->setToolTip(tr("File size"));
    ui->lineEditPESections->setToolTip(tr("Sections"));
    ui->lineEditType->setToolTip(tr("Type"));
    ui->lineEditELFPrograms->setToolTip(QString("Programs"));
    ui->lineEditELFSections->setToolTip(tr("Sections"));
    ui->lineEditMACHCommands->setToolTip(QString("Commands"));
    ui->lineEditMACHSections->setToolTip(tr("Sections"));
    ui->lineEditMACHSegments->setToolTip(tr("Segments"));
    ui->lineEditMACHLibraries->setToolTip(tr("Libraries"));
    ui->lineEditPETimeDateStamp->setToolTip(tr("Time date stamp"));
    ui->lineEditPESizeOfImage->setToolTip(tr("Size of image"));

    ui->stackedWidgetMain->setCurrentIndex(TABINFO_BINARY);

    connect(ui->pageScanDIE, SIGNAL(scanStarted()), this, SLOT(onScanStarted()));
    connect(ui->pageScanDIE, SIGNAL(scanFinished()), this, SLOT(onScanFinished()));
    connect(ui->pageScanNFD, SIGNAL(scanStarted()), this, SLOT(onScanStarted()));
    connect(ui->pageScanNFD, SIGNAL(scanFinished()), this, SLOT(onScanFinished()));
    connect(ui->pageScanYARA, SIGNAL(scanStarted()), this, SLOT(onScanStarted()));
    connect(ui->pageScanYARA, SIGNAL(scanFinished()), this, SLOT(onScanFinished()));

    connect(ui->pageScanNFD, SIGNAL(showInfo()), this, SLOT(_showNfdInfo()));
    connect(ui->pageScanYARA, SIGNAL(showInfo()), this, SLOT(_showYaraInfo()));

    connect(ui->pageScanDIE, SIGNAL(currentFileType(qint32)), this, SLOT(_currentFileType(qint32)));
    connect(ui->pageScanNFD, SIGNAL(currentFileType(qint32)), this, SLOT(_currentFileType(qint32)));

#ifndef USE_YARA
    ui->toolButtonYARA->hide();
#endif

    enableControls(false);
}

void FormatsWidget::setFileName(const QString &sFileName, bool bScan)
{
    this->g_sFileName = sFileName;
    this->g_bScan = bScan;

    enableControls(sFileName != "");

    XFormats::setFileTypeComboBox(XBinary::FT_UNKNOWN, g_sFileName, ui->comboBoxFileType, XBinary::TL_OPTION_ALL);

    reload();
}

void FormatsWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    pXOptions->setComboBox(ui->comboBoxScanEngine, XOptions::ID_SCAN_ENGINE);
    adjustScanTab("auto");

    ui->pageScanDIE->setGlobal(pShortcuts, pXOptions);
    ui->pageScanNFD->setGlobal(pShortcuts, pXOptions);
    ui->pageScanYARA->setGlobal(pShortcuts, pXOptions);

    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void FormatsWidget::adjustView()
{
    QString sScanEngine = getGlobalOptions()->getScanEngine();
    sScanEngine = getScanEngine(sScanEngine);

    if (sScanEngine == "die") {
        ui->comboBoxScanEngine->setCurrentIndex(0);
    } else if (sScanEngine == "nfd") {
        ui->comboBoxScanEngine->setCurrentIndex(1);
#ifdef USE_YARA
    } else if (sScanEngine == "yara") {
        ui->comboBoxScanEngine->setCurrentIndex(2);
#endif
    }

    ui->pageScanDIE->adjustView();
    ui->pageScanNFD->adjustView();
    ui->pageScanYARA->adjustView();
}

void FormatsWidget::setAdvanced(bool bState)
{
    if (bState) {
        ui->groupBoxTools->show();
        ui->stackedWidgetMain->show();
        ui->groupBoxBaseAddress->show();
        ui->groupBoxEntryPoint->show();
    } else {
        ui->groupBoxTools->hide();
        ui->stackedWidgetMain->hide();
        ui->groupBoxBaseAddress->hide();
        ui->groupBoxEntryPoint->hide();
    }
}

void FormatsWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)

    reload();
}

FormatsWidget::~FormatsWidget()
{
    delete ui;
}

void FormatsWidget::on_comboBoxFileType_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    reload();
}

void FormatsWidget::reload()
{
    adjustScanTab(getScanEngine(ui->comboBoxScanEngine->currentData().toString()));

    XBinary::FT fileType = getCurrentFileType();

    QFile file;
    file.setFileName(g_sFileName);

    if (file.open(QIODevice::ReadOnly)) {
        ui->lineEditFileSize->setValue_uint64(file.size(), XLineEditHEX::_MODE_SIZE);

        XBinary::_MEMORY_MAP memoryMap = XFormats::getMemoryMap(fileType, XBinary::MAPMODE_UNKNOWN, &file);

        XBinary::MODE mode = memoryMap.mode;

        if ((mode == XBinary::MODE_UNKNOWN) || (mode == XBinary::MODE_DATA)) {
            mode = XBinary::getWidthModeFromSize(file.size());
        }

        if (memoryMap.fileType == XBinary::FT_MSDOS) {
            ui->lineEditBaseAddress->setColon(true);
            ui->lineEditEntryPoint->setColon(true);
        } else {
            ui->lineEditBaseAddress->setColon(false);
            ui->lineEditEntryPoint->setColon(false);
        }

        if (mode == XBinary::MODE_8) {
            ui->lineEditBaseAddress->setValue_uint8((quint8)memoryMap.nModuleAddress);
        } else if (mode == XBinary::MODE_16) {
            if (memoryMap.fileType == XBinary::FT_MSDOS) {
                ui->lineEditBaseAddress->setValue_uint32((quint32)memoryMap.nModuleAddress);
            } else {
                ui->lineEditBaseAddress->setValue_uint16((quint16)memoryMap.nModuleAddress);
            }
        } else if ((mode == XBinary::MODE_16SEG) || (mode == XBinary::MODE_32)) {
            ui->lineEditBaseAddress->setValue_uint32((quint32)memoryMap.nModuleAddress);
        } else if (mode == XBinary::MODE_64) {
            ui->lineEditBaseAddress->setValue_uint64((quint64)memoryMap.nModuleAddress);
        }

        ui->lineEditEndianness->setText(XBinary::endianToString(memoryMap.endian));
        ui->lineEditArch->setText(memoryMap.sArch);
        ui->lineEditMode->setText(XBinary::modeIdToString(memoryMap.mode));
        ui->lineEditType->setText(memoryMap.sType);

        if (fileType == XBinary::FT_BINARY) {
            XBinary binary(&file);

            ui->stackedWidgetMain->setCurrentIndex(TABINFO_BINARY);

            ui->lineEditEntryPoint->setValue_uint32((quint32)binary.getEntryPointAddress());
        } else if (fileType == XBinary::FT_COM) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_COM);

            XCOM com(&file);

            if (com.isValid()) {
                ui->lineEditEntryPoint->setValue_uint16((quint16)com.getEntryPointAddress());
            }
        } else if (XArchives::getArchiveOpenValidFileTypes().contains(fileType)) {
            XBinary::FILEFORMATINFO fileFormatInfo = XFormats::getFileFormatInfo(fileType, &file);
            ui->toolButtonArchive->setText(XBinary::getFileFormatString(&fileFormatInfo));

            bool bMANIFESTMF = false;
            bool bAndroidManifest = false;

            if ((fileType == XBinary::FT_JAR) || (fileType == XBinary::FT_APK)) {
                XZip xzip(&file);

                if (xzip.isValid()) {
                    QList<XArchive::RECORD> listRecords = xzip.getRecords(-1, nullptr);

                    if (xzip.isArchiveRecordPresent("META-INF/MANIFEST.MF", &listRecords)) {
                        bMANIFESTMF = true;
                    }

                    if (fileType == XBinary::FT_APK) {
                        if (xzip.isArchiveRecordPresent("AndroidManifest.xml", &listRecords)) {
                            bAndroidManifest = true;
                        }
                    }
                }
            }

            if (bMANIFESTMF) {
                ui->toolButtonMANIFESTMF->show();
            } else {
                ui->toolButtonMANIFESTMF->hide();
            }

            if (bAndroidManifest) {
                ui->toolButtonAndroidManifest->show();
            } else {
                ui->toolButtonAndroidManifest->hide();
            }

            ui->stackedWidgetMain->setCurrentIndex(TABINFO_ARCHIVE);

            ui->lineEditEntryPoint->setValue_uint32(0);
        } else if (fileType == XBinary::FT_DEX) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_DEX);

            XDEX dex(&file);

            if (dex.isValid()) {
                ui->lineEditEntryPoint->setValue_uint16((quint16)dex.getEntryPointAddress());
            }
        } else if (fileType == XBinary::FT_MSDOS) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_MSDOS);

            XMSDOS msdos(&file);

            if (msdos.isValid()) {
                ui->lineEditEntryPoint->setValue_uint32((quint32)msdos.getEntryPointAddress());

                ui->toolButtonMSDOSOverlay->setEnabled(msdos.isOverlayPresent());
            }
        } else if ((fileType == XBinary::FT_LE) || (fileType == XBinary::FT_LX))  // mb TODO LX
        {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_LE);

            if (fileType == XBinary::FT_LE) {
                ui->toolButtonLE->setText("LE");
            } else if (fileType == XBinary::FT_LX) {
                ui->toolButtonLE->setText("LX");
            }

            XLE le(&file);

            if (le.isValid()) {
                ui->lineEditEntryPoint->setValue_uint32((quint32)le.getEntryPointAddress());
            }
        } else if (fileType == XBinary::FT_NE) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_NE);

            XNE ne(&file);

            if (ne.isValid()) {
                ui->lineEditEntryPoint->setValue_uint32((quint32)ne.getEntryPointAddress());
            }
        } else if ((fileType == XBinary::FT_PE32) || (fileType == XBinary::FT_PE64)) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_PE);

            XPE pe(&file);

            if (pe.isValid()) {
                if (pe.is64()) {
                    ui->lineEditEntryPoint->setValue_uint64((quint64)pe.getEntryPointAddress());
                } else {
                    ui->lineEditEntryPoint->setValue_uint32((quint32)pe.getEntryPointAddress());
                }

                bool bIsResourcesPresent = pe.isResourcesPresent();
                QList<XPE::RESOURCE_RECORD> listResources = pe.getResources(10000);

                ui->lineEditPESections->setValue_uint16(pe.getFileHeader_NumberOfSections());
                ui->groupBoxPESections->setEnabled(pe.isSectionsTablePresent());

                ui->toolButtonPEExport->setEnabled(pe.isExportPresent());
                ui->toolButtonPEImport->setEnabled(pe.isImportPresent());
                ui->toolButtonPEResources->setEnabled(bIsResourcesPresent);
                ui->toolButtonPENET->setEnabled(pe.isNETPresent());
                ui->toolButtonPETLS->setEnabled(pe.isTLSPresent());

                ui->groupBoxPEResources->setEnabled(bIsResourcesPresent);
                ui->toolButtonPEManifest->setEnabled(pe.isResourceManifestPresent(&listResources));
                ui->toolButtonPEVersion->setEnabled(pe.isResourceVersionPresent(&listResources));

                ui->toolButtonPEOverlay->setEnabled(pe.isOverlayPresent());

                quint32 nNetID = pe.getNetId();

                if (nNetID) {
                    ui->groupBoxPETimeDateStamp->setTitle(QString(".NET ID"));
                    ui->lineEditPETimeDateStamp->setValue_uint32(nNetID);
                    ui->lineEditPETimeDateStamp->setValidatorMode(XLineEditValidator::MODE_HEX_32);
                } else {
                    ui->groupBoxPETimeDateStamp->setTitle(tr("Time date stamp"));
                    ui->lineEditPETimeDateStamp->setValue_String(XBinary::valueToTimeString(pe.getFileHeader_TimeDateStamp(), XBinary::DT_TYPE_POSIX));
                    ui->lineEditPETimeDateStamp->setValidatorMode(XLineEditValidator::MODE_TEXT);
                }

                ui->lineEditPESizeOfImage->setValue_uint32(pe.getOptionalHeader_SizeOfImage());
            }
        } else if ((fileType == XBinary::FT_ELF32) || (fileType == XBinary::FT_ELF64)) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_ELF);

            XELF elf(&file);

            if (elf.isValid()) {
                if (elf.is64()) {
                    ui->lineEditEntryPoint->setValue_uint64((quint64)elf.getEntryPointAddress());
                } else {
                    ui->lineEditEntryPoint->setValue_uint32((quint32)elf.getEntryPointAddress());
                }
            }

            ui->lineEditELFPrograms->setEnabled(elf.isProgramsTablePresent());
            ui->lineEditELFSections->setEnabled(elf.isSectionsTablePresent());
            ui->lineEditELFPrograms->setValue_uint16(elf.getNumberOfPrograms());
            ui->lineEditELFSections->setValue_uint16(elf.getNumberOfSections());
        } else if ((fileType == XBinary::FT_MACHO32) || (fileType == XBinary::FT_MACHO64)) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_MACH);

            XMACH mach(&file);

            if (mach.isValid()) {
                if (mach.is64()) {
                    ui->lineEditEntryPoint->setValue_uint64((quint64)mach.getEntryPointAddress());
                } else {
                    ui->lineEditEntryPoint->setValue_uint32((quint32)mach.getEntryPointAddress());
                }

                QList<XMACH::COMMAND_RECORD> listCommandRecords = mach.getCommandRecords();
                QList<XMACH::SECTION_RECORD> listSectionRecords = mach.getSectionRecords(&listCommandRecords);
                QList<XMACH::SEGMENT_RECORD> listSegmentRecords = mach.getSegmentRecords(&listCommandRecords);
                QList<XMACH::LIBRARY_RECORD> listLibraryRecords = mach.getLibraryRecords(&listCommandRecords);

                ui->lineEditMACHCommands->setEnabled(listCommandRecords.count());
                ui->lineEditMACHSections->setEnabled(listSectionRecords.count());
                ui->lineEditMACHSegments->setEnabled(listSegmentRecords.count());
                ui->lineEditMACHLibraries->setEnabled(listLibraryRecords.count());

                ui->lineEditMACHCommands->setValue_uint16((quint16)listCommandRecords.count());
                ui->lineEditMACHSections->setValue_uint16((quint16)listSectionRecords.count());
                ui->lineEditMACHSegments->setValue_uint16((quint16)listSegmentRecords.count());
                ui->lineEditMACHLibraries->setValue_uint16((quint16)listLibraryRecords.count());
            }
        } else {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_BINARY);
        }

        file.close();

        scan();
    } else {
        // TODO Error message
    }
}

void FormatsWidget::scan()
{
    QString sData = ui->comboBoxScanEngine->currentData().toString();

    sData = getScanEngine(sData);

    adjustScanTab(sData);

    if (g_sFileName != "") {
        if (sData == "die") {
            ui->pageScanDIE->setData(g_sFileName, g_bScan, getCurrentFileType());
        } else if (sData == "nfd") {
            ui->pageScanNFD->setData(g_sFileName, g_bScan, getCurrentFileType());
#ifdef USE_YARA
        } else if (sData == "yara") {
            YARA_Widget::OPTIONS options = {};
            options.bHandleInfo = true;
            ui->pageScanYARA->setData(g_sFileName, options, g_bScan);
#endif
        }
    }
}

void FormatsWidget::enableControls(bool bState)
{
    ui->groupBoxTools->setEnabled(bState);
    ui->stackedWidgetMain->setEnabled(bState);
    ui->groupBoxBaseAddress->setEnabled(bState);
    ui->groupBoxEntryPoint->setEnabled(bState);
    ui->stackedWidgetScan->setEnabled(bState);
    ui->groupBoxFileSize->setEnabled(bState);
    ui->groupBoxFileType->setEnabled(bState);
    ui->groupBoxArch->setEnabled(bState);
    ui->groupBoxEndianness->setEnabled(bState);
    ui->groupBoxMode->setEnabled(bState);
    ui->groupBoxType->setEnabled(bState);
    ui->groupBoxScanEngine->setEnabled(bState);
}

void FormatsWidget::on_toolButtonEntryPoint_clicked()
{
    XBinary::FT fileType = getCurrentFileType();

    if (fileType == XBinary::FT_DEX) {
        showType(SBINARY::TYPE_HEX);
    } else {
        showType(SBINARY::TYPE_DISASM);
    }
}

void FormatsWidget::on_toolButtonMemoryMap_clicked()
{
    showType(SBINARY::TYPE_MEMORYMAP);
}

void FormatsWidget::on_toolButtonPEExport_clicked()
{
    showPE(SPE::TYPE_EXPORT);
}

void FormatsWidget::on_toolButtonPEImport_clicked()
{
    showPE(SPE::TYPE_IMPORT);
}

void FormatsWidget::on_toolButtonPEResources_clicked()
{
    showPE(SPE::TYPE_RESOURCES);
}

void FormatsWidget::on_toolButtonPEOverlay_clicked()
{
    showPE(SPE::TYPE_OVERLAY);
}

void FormatsWidget::on_toolButtonPE_clicked()
{
    showPE(SPE::TYPE_IMAGE_FILE_HEADER);
}

void FormatsWidget::on_toolButtonPESections_clicked()
{
    showPE(SPE::TYPE_SECTIONS);
}

void FormatsWidget::on_toolButtonPEManifest_clicked()
{
    showPE(SPE::TYPE_RESOURCES_MANIFEST);
}

void FormatsWidget::on_toolButtonPEVersion_clicked()
{
    showPE(SPE::TYPE_RESOURCES_VERSION);
}

void FormatsWidget::on_toolButtonPENET_clicked()
{
    showPE(SPE::TYPE_NETHEADER);
}

void FormatsWidget::on_toolButtonMACH_clicked()
{
    showMACH(SMACH::TYPE_mach_header);
}

void FormatsWidget::on_toolButtonMACHSegments_clicked()
{
    showMACH(SMACH::TYPE_mach_segments);
}

void FormatsWidget::on_toolButtonMACHSections_clicked()
{
    showMACH(SMACH::TYPE_mach_sections);
}

void FormatsWidget::on_toolButtonMACHCommands_clicked()
{
    showMACH(SMACH::TYPE_mach_commands);
}

void FormatsWidget::on_toolButtonMACHLibraries_clicked()
{
    showMACH(SMACH::TYPE_mach_libraries);
}

void FormatsWidget::showMSDOS(SMSDOS::TYPE type)
{
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogMSDOS dialogMSDOS(this);
        dialogMSDOS.setGlobal(getShortcuts(), getGlobalOptions());
        dialogMSDOS.setData(&file, options);

        dialogMSDOS.exec();

        file.close();
    }
}

void FormatsWidget::showLE(SLE::TYPE type)
{
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogLE dialogLE(this);
        dialogLE.setGlobal(getShortcuts(), getGlobalOptions());
        dialogLE.setData(&file, options);

        dialogLE.exec();

        file.close();
    }
}

void FormatsWidget::showNE(SNE::TYPE type)
{
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogNE dialogNE(this);
        dialogNE.setGlobal(getShortcuts(), getGlobalOptions());
        dialogNE.setData(&file, options);

        dialogNE.exec();

        file.close();
    }
}

void FormatsWidget::showPE(SPE::TYPE type)
{
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogPE dialogPE(this);
        dialogPE.setGlobal(getShortcuts(), getGlobalOptions());
        dialogPE.setData(&file, options);

        dialogPE.exec();

        file.close();
    }
}

void FormatsWidget::showELF(SELF::TYPE type)
{
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogELF dialogELF(this);
        dialogELF.setGlobal(getShortcuts(), getGlobalOptions());
        dialogELF.setData(&file, options);

        dialogELF.exec();

        file.close();
    }
}

void FormatsWidget::showMACH(SMACH::TYPE type)
{
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogMACH dialogMACH(this);
        dialogMACH.setGlobal(getShortcuts(), getGlobalOptions());
        dialogMACH.setData(&file, options);

        dialogMACH.exec();

        file.close();
    }
}

void FormatsWidget::showDEX(SDEX::TYPE type)
{
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogDEX dialogDEX(this);
        dialogDEX.setGlobal(getShortcuts(), getGlobalOptions());
        dialogDEX.setData(&file, options);

        dialogDEX.exec();

        file.close();
    }
}

void FormatsWidget::showBinary(SBINARY::TYPE type)
{
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogBinary dialogBinary(this);
        dialogBinary.setGlobal(getShortcuts(), getGlobalOptions());
        dialogBinary.setData(&file, options);

        dialogBinary.exec();

        file.close();
    }
}

XBinary::FT FormatsWidget::getCurrentFileType()
{
    return (XBinary::FT)(ui->comboBoxFileType->currentData().toInt());
}

void FormatsWidget::on_toolButtonMSDOSOverlay_clicked()
{
    showMSDOS(SMSDOS::TYPE_OVERLAY);
}

void FormatsWidget::on_toolButtonMSDOS_clicked()
{
    showMSDOS(SMSDOS::TYPE_DOS_HEADER);
}

void FormatsWidget::on_toolButtonPETLS_clicked()
{
    showPE(SPE::TYPE_TLS);
}

void FormatsWidget::on_toolButtonELF_clicked()
{
    showELF(SELF::TYPE_Elf_Ehdr);
}

void FormatsWidget::on_toolButtonELFSections_clicked()
{
    showELF(SELF::TYPE_Elf_Shdr);
}

void FormatsWidget::on_toolButtonELFPrograms_clicked()
{
    showELF(SELF::TYPE_Elf_Phdr);
}

void FormatsWidget::on_comboBoxScanEngine_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    adjustScanTab(getScanEngine(ui->comboBoxScanEngine->currentData().toString()));

    scan();
}

void FormatsWidget::on_toolButtonLE_clicked()
{
    showLE(SLE::TYPE_VXD_HEADER);
}

void FormatsWidget::on_toolButtonNE_clicked()
{
    showNE(SNE::TYPE_OS2_HEADER);
}

void FormatsWidget::on_toolButtonDEX_clicked()
{
    showDEX(SDEX::TYPE_HEADER);
}

void FormatsWidget::on_toolButtonArchive_clicked()
{
    showBinary(SBINARY::TYPE_HEX);
}

void FormatsWidget::on_toolButtonBinary_clicked()
{
    showBinary(SBINARY::TYPE_HEX);
}

void FormatsWidget::showType(SBINARY::TYPE type)
{
    XBinary::FT fileType = getCurrentFileType();
    qint32 nCurrentType = convertType(fileType, type);

    if (fileType == XBinary::FT_MSDOS) {
        showMSDOS((SMSDOS::TYPE)nCurrentType);
    } else if ((fileType == XBinary::FT_LE) || (fileType == XBinary::FT_LX)) {
        showLE((SLE::TYPE)nCurrentType);
    } else if (fileType == XBinary::FT_NE) {
        showNE((SNE::TYPE)nCurrentType);
    } else if ((fileType == XBinary::FT_PE32) || (fileType == XBinary::FT_PE64)) {
        showPE((SPE::TYPE)nCurrentType);
    } else if ((fileType == XBinary::FT_ELF32) || (fileType == XBinary::FT_ELF64)) {
        showELF((SELF::TYPE)nCurrentType);
    } else if ((fileType == XBinary::FT_MACHO32) || (fileType == XBinary::FT_MACHO64)) {
        showMACH((SMACH::TYPE)nCurrentType);
    } else if (fileType == XBinary::FT_DEX) {
        showDEX((SDEX::TYPE)nCurrentType);
    } else {
        showBinary(type);
    }
}

qint32 FormatsWidget::convertType(XBinary::FT fileType, SBINARY::TYPE type)
{
    qint32 nResult = 0;

    if (fileType == XBinary::FT_MSDOS) {
        if (type == SBINARY::TYPE_INFO) nResult = SMSDOS::TYPE_INFO;
        else if (type == SBINARY::TYPE_VISUALIZATION) nResult = SMSDOS::TYPE_VISUALIZATION;
        else if (type == SBINARY::TYPE_VIRUSTOTAL) nResult = SMSDOS::TYPE_VIRUSTOTAL;
        else if (type == SBINARY::TYPE_HEX) nResult = SMSDOS::TYPE_HEX;
        else if (type == SBINARY::TYPE_DISASM) nResult = SMSDOS::TYPE_DISASM;
        else if (type == SBINARY::TYPE_HASH) nResult = SMSDOS::TYPE_HASH;
        else if (type == SBINARY::TYPE_STRINGS) nResult = SMSDOS::TYPE_STRINGS;
        else if (type == SBINARY::TYPE_SIGNATURES) nResult = SMSDOS::TYPE_SIGNATURES;
        else if (type == SBINARY::TYPE_MEMORYMAP) nResult = SMSDOS::TYPE_MEMORYMAP;
        else if (type == SBINARY::TYPE_ENTROPY) nResult = SMSDOS::TYPE_ENTROPY;
        else if (type == SBINARY::TYPE_NFDSCAN) nResult = SMSDOS::TYPE_NFDSCAN;
        else if (type == SBINARY::TYPE_EXTRACTOR) nResult = SMSDOS::TYPE_EXTRACTOR;
        else if (type == SBINARY::TYPE_SEARCH) nResult = SMSDOS::TYPE_SEARCH;
#ifdef USE_YARA
        else if (type == SBINARY::TYPE_YARASCAN) nResult = SMSDOS::TYPE_YARASCAN;
#endif
    } else if ((fileType == XBinary::FT_LE) || (fileType == XBinary::FT_LX)) {
        if (type == SBINARY::TYPE_INFO) nResult = SLE::TYPE_INFO;
        else if (type == SBINARY::TYPE_VISUALIZATION) nResult = SLE::TYPE_VISUALIZATION;
        else if (type == SBINARY::TYPE_VIRUSTOTAL) nResult = SLE::TYPE_VIRUSTOTAL;
        else if (type == SBINARY::TYPE_HEX) nResult = SLE::TYPE_HEX;
        else if (type == SBINARY::TYPE_DISASM) nResult = SLE::TYPE_DISASM;
        else if (type == SBINARY::TYPE_HASH) nResult = SLE::TYPE_HASH;
        else if (type == SBINARY::TYPE_STRINGS) nResult = SLE::TYPE_STRINGS;
        else if (type == SBINARY::TYPE_SIGNATURES) nResult = SLE::TYPE_SIGNATURES;
        else if (type == SBINARY::TYPE_MEMORYMAP) nResult = SLE::TYPE_MEMORYMAP;
        else if (type == SBINARY::TYPE_ENTROPY) nResult = SLE::TYPE_ENTROPY;
        else if (type == SBINARY::TYPE_NFDSCAN) nResult = SLE::TYPE_NFDSCAN;
        else if (type == SBINARY::TYPE_EXTRACTOR) nResult = SLE::TYPE_EXTRACTOR;
        else if (type == SBINARY::TYPE_SEARCH) nResult = SLE::TYPE_SEARCH;
#ifdef USE_YARA
        else if (type == SBINARY::TYPE_YARASCAN) nResult = SLE::TYPE_YARASCAN;
#endif
    } else if (fileType == XBinary::FT_NE) {
        if (type == SBINARY::TYPE_INFO) nResult = SNE::TYPE_INFO;
        else if (type == SBINARY::TYPE_VISUALIZATION) nResult = SNE::TYPE_VISUALIZATION;
        else if (type == SBINARY::TYPE_VIRUSTOTAL) nResult = SNE::TYPE_VIRUSTOTAL;
        else if (type == SBINARY::TYPE_HEX) nResult = SNE::TYPE_HEX;
        else if (type == SBINARY::TYPE_DISASM) nResult = SNE::TYPE_DISASM;
        else if (type == SBINARY::TYPE_HASH) nResult = SNE::TYPE_HASH;
        else if (type == SBINARY::TYPE_STRINGS) nResult = SNE::TYPE_STRINGS;
        else if (type == SBINARY::TYPE_SIGNATURES) nResult = SNE::TYPE_SIGNATURES;
        else if (type == SBINARY::TYPE_MEMORYMAP) nResult = SNE::TYPE_MEMORYMAP;
        else if (type == SBINARY::TYPE_ENTROPY) nResult = SNE::TYPE_ENTROPY;
        else if (type == SBINARY::TYPE_NFDSCAN) nResult = SNE::TYPE_NFDSCAN;
        else if (type == SBINARY::TYPE_EXTRACTOR) nResult = SNE::TYPE_EXTRACTOR;
        else if (type == SBINARY::TYPE_SEARCH) nResult = SNE::TYPE_SEARCH;
#ifdef USE_YARA
        else if (type == SBINARY::TYPE_YARASCAN) nResult = SNE::TYPE_YARASCAN;
#endif
    } else if ((fileType == XBinary::FT_PE32) || (fileType == XBinary::FT_PE64)) {
        if (type == SBINARY::TYPE_INFO) nResult = SPE::TYPE_INFO;
        else if (type == SBINARY::TYPE_VISUALIZATION) nResult = SPE::TYPE_VISUALIZATION;
        else if (type == SBINARY::TYPE_VIRUSTOTAL) nResult = SPE::TYPE_VIRUSTOTAL;
        else if (type == SBINARY::TYPE_HEX) nResult = SPE::TYPE_HEX;
        else if (type == SBINARY::TYPE_DISASM) nResult = SPE::TYPE_DISASM;
        else if (type == SBINARY::TYPE_HASH) nResult = SPE::TYPE_HASH;
        else if (type == SBINARY::TYPE_STRINGS) nResult = SPE::TYPE_STRINGS;
        else if (type == SBINARY::TYPE_SIGNATURES) nResult = SPE::TYPE_SIGNATURES;
        else if (type == SBINARY::TYPE_MEMORYMAP) nResult = SPE::TYPE_MEMORYMAP;
        else if (type == SBINARY::TYPE_ENTROPY) nResult = SPE::TYPE_ENTROPY;
        else if (type == SBINARY::TYPE_NFDSCAN) nResult = SPE::TYPE_NFDSCAN;
        else if (type == SBINARY::TYPE_EXTRACTOR) nResult = SPE::TYPE_EXTRACTOR;
        else if (type == SBINARY::TYPE_SEARCH) nResult = SPE::TYPE_SEARCH;
#ifdef USE_YARA
        else if (type == SBINARY::TYPE_YARASCAN) nResult = SPE::TYPE_YARASCAN;
#endif
    } else if ((fileType == XBinary::FT_ELF32) || (fileType == XBinary::FT_ELF64)) {
        if (type == SBINARY::TYPE_INFO) nResult = SELF::TYPE_INFO;
        else if (type == SBINARY::TYPE_VISUALIZATION) nResult = SELF::TYPE_VISUALIZATION;
        else if (type == SBINARY::TYPE_VIRUSTOTAL) nResult = SELF::TYPE_VIRUSTOTAL;
        else if (type == SBINARY::TYPE_HEX) nResult = SELF::TYPE_HEX;
        else if (type == SBINARY::TYPE_DISASM) nResult = SELF::TYPE_DISASM;
        else if (type == SBINARY::TYPE_HASH) nResult = SELF::TYPE_HASH;
        else if (type == SBINARY::TYPE_STRINGS) nResult = SELF::TYPE_STRINGS;
        else if (type == SBINARY::TYPE_SIGNATURES) nResult = SELF::TYPE_SIGNATURES;
        else if (type == SBINARY::TYPE_MEMORYMAP) nResult = SELF::TYPE_MEMORYMAP;
        else if (type == SBINARY::TYPE_ENTROPY) nResult = SELF::TYPE_ENTROPY;
        else if (type == SBINARY::TYPE_NFDSCAN) nResult = SELF::TYPE_NFDSCAN;
        else if (type == SBINARY::TYPE_EXTRACTOR) nResult = SELF::TYPE_EXTRACTOR;
        else if (type == SBINARY::TYPE_SEARCH) nResult = SELF::TYPE_SEARCH;
#ifdef USE_YARA
        else if (type == SBINARY::TYPE_YARASCAN) nResult = SELF::TYPE_YARASCAN;
#endif
    } else if ((fileType == XBinary::FT_MACHO32) || (fileType == XBinary::FT_MACHO64)) {
        if (type == SBINARY::TYPE_INFO) nResult = SMACH::TYPE_INFO;
        else if (type == SBINARY::TYPE_VISUALIZATION) nResult = SMACH::TYPE_VISUALIZATION;
        else if (type == SBINARY::TYPE_VIRUSTOTAL) nResult = SMACH::TYPE_VIRUSTOTAL;
        else if (type == SBINARY::TYPE_HEX) nResult = SMACH::TYPE_HEX;
        else if (type == SBINARY::TYPE_DISASM) nResult = SMACH::TYPE_DISASM;
        else if (type == SBINARY::TYPE_HASH) nResult = SMACH::TYPE_HASH;
        else if (type == SBINARY::TYPE_STRINGS) nResult = SMACH::TYPE_STRINGS;
        else if (type == SBINARY::TYPE_SIGNATURES) nResult = SMACH::TYPE_SIGNATURES;
        else if (type == SBINARY::TYPE_MEMORYMAP) nResult = SMACH::TYPE_MEMORYMAP;
        else if (type == SBINARY::TYPE_ENTROPY) nResult = SMACH::TYPE_ENTROPY;
        else if (type == SBINARY::TYPE_NFDSCAN) nResult = SMACH::TYPE_NFDSCAN;
        else if (type == SBINARY::TYPE_EXTRACTOR) nResult = SMACH::TYPE_EXTRACTOR;
        else if (type == SBINARY::TYPE_SEARCH) nResult = SMACH::TYPE_SEARCH;
#ifdef USE_YARA
        else if (type == SBINARY::TYPE_YARASCAN) nResult = SMACH::TYPE_YARASCAN;
#endif
    } else if (fileType == XBinary::FT_DEX) {
        if (type == SBINARY::TYPE_INFO) nResult = SDEX::TYPE_INFO;
        else if (type == SBINARY::TYPE_VISUALIZATION) nResult = SDEX::TYPE_VISUALIZATION;
        else if (type == SBINARY::TYPE_VIRUSTOTAL) nResult = SDEX::TYPE_VIRUSTOTAL;
        else if (type == SBINARY::TYPE_HEX) nResult = SDEX::TYPE_HEX;
        //        else if (type == SBINARY::TYPE_DISASM) nResult = SDEX::TYPE_DISASM;
        else if (type == SBINARY::TYPE_HASH) nResult = SDEX::TYPE_HASH;
        else if (type == SBINARY::TYPE_STRINGS) nResult = SDEX::TYPE_STRINGS;
        else if (type == SBINARY::TYPE_SIGNATURES) nResult = SDEX::TYPE_SIGNATURES;
        else if (type == SBINARY::TYPE_MEMORYMAP) nResult = SDEX::TYPE_MEMORYMAP;
        else if (type == SBINARY::TYPE_ENTROPY) nResult = SDEX::TYPE_ENTROPY;
        else if (type == SBINARY::TYPE_NFDSCAN) nResult = SDEX::TYPE_NFDSCAN;
        else if (type == SBINARY::TYPE_EXTRACTOR) nResult = SDEX::TYPE_EXTRACTOR;
        else if (type == SBINARY::TYPE_SEARCH) nResult = SDEX::TYPE_SEARCH;
#ifdef USE_YARA
        else if (type == SBINARY::TYPE_YARASCAN) nResult = SDEX::TYPE_YARASCAN;
#endif
    } else {
        nResult = type;
    }

    return nResult;
}

QString FormatsWidget::getScanEngine(const QString &sDefault)
{
    QString sResult = sDefault;

    if (sDefault == "auto") {
        sResult = "die";

        XBinary::FT fileType = getCurrentFileType();

        // TODO !!!
        if ((fileType == XBinary::FT_DEX) || (fileType == XBinary::FT_MACHO32) || (fileType == XBinary::FT_MACHO64) || (fileType == XBinary::FT_MACHOFAT) ||
            (fileType == XBinary::FT_JAR) || (fileType == XBinary::FT_APK) || (fileType == XBinary::FT_APKS) || (fileType == XBinary::FT_IPA)) {
            sResult = "nfd";
        }
    }

    return sResult;
}

void FormatsWidget::adjustScanTab(const QString &sIndex)
{
    if (sIndex == "die") {
        ui->stackedWidgetScan->setCurrentIndex(0);
    } else if (sIndex == "nfd") {
        ui->stackedWidgetScan->setCurrentIndex(1);
#ifdef USE_YARA
    } else if (sIndex == "yara") {
        ui->stackedWidgetScan->setCurrentIndex(2);
#endif
    }
}

void FormatsWidget::onScanStarted()
{
    ui->stackedWidgetMain->setEnabled(false);
    ui->groupBoxFileType->setEnabled(false);
    ui->groupBoxTools->setEnabled(false);
    ui->groupBoxScanEngine->setEnabled(false);
    ui->groupBoxEntryPoint->setEnabled(false);
    ui->groupBoxBaseAddress->setEnabled(false);
    ui->groupBoxArch->setEnabled(false);
    ui->groupBoxFileSize->setEnabled(false);
    ui->groupBoxEndianness->setEnabled(false);
    ui->groupBoxMode->setEnabled(false);
    ui->groupBoxType->setEnabled(false);
}

void FormatsWidget::onScanFinished()
{
    ui->stackedWidgetMain->setEnabled(true);
    ui->groupBoxFileType->setEnabled(true);
    ui->groupBoxTools->setEnabled(true);
    ui->groupBoxScanEngine->setEnabled(true);
    ui->groupBoxEntryPoint->setEnabled(true);
    ui->groupBoxBaseAddress->setEnabled(true);
    ui->groupBoxArch->setEnabled(true);
    ui->groupBoxFileSize->setEnabled(true);
    ui->groupBoxEndianness->setEnabled(true);
    ui->groupBoxMode->setEnabled(true);
    ui->groupBoxType->setEnabled(true);
}

void FormatsWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

void FormatsWidget::on_toolButtonFileInfo_clicked()
{
    showType(SBINARY::TYPE_INFO);
}

void FormatsWidget::on_toolButtonMIME_clicked()
{
    QString sFileName = g_sFileName;

    if (sFileName != "") {
        QFile file;
        file.setFileName(sFileName);

        if (file.open(QIODevice::ReadOnly)) {
            DialogMIME dialogMIME(this, &file);
            dialogMIME.setGlobal(getShortcuts(), getGlobalOptions());

            dialogMIME.exec();

            file.close();
        }
    }
}

void FormatsWidget::on_toolButtonHash_clicked()
{
    showType(SBINARY::TYPE_HASH);
}

void FormatsWidget::on_toolButtonStrings_clicked()
{
    showType(SBINARY::TYPE_STRINGS);
}

void FormatsWidget::on_toolButtonSignatures_clicked()
{
    showType(SBINARY::TYPE_SIGNATURES);
}

void FormatsWidget::on_toolButtonHex_clicked()
{
    showType(SBINARY::TYPE_HEX);
}

void FormatsWidget::on_toolButtonEntropy_clicked()
{
    showType(SBINARY::TYPE_ENTROPY);
}

void FormatsWidget::on_toolButtonVirusTotal_clicked()
{
    if (getGlobalOptions()->getVirusTotalApiKey() != "") {
        showType(SBINARY::TYPE_VIRUSTOTAL);
    } else {
        QString sFileName = g_sFileName;

        if (sFileName != "") {
            QFile file;
            file.setFileName(sFileName);

            if (file.open(QIODevice::ReadOnly)) {
                QString sMD5 = XBinary::getHash(XBinary::HASH_MD5, &file);
                XVirusTotalWidget::showInBrowser(sMD5);

                file.close();
            }
        }
    }
}

void FormatsWidget::on_toolButtonExtractor_clicked()
{
    showType(SBINARY::TYPE_EXTRACTOR);
}

void FormatsWidget::on_toolButtonSearch_clicked()
{
    showType(SBINARY::TYPE_SEARCH);
}

void FormatsWidget::on_toolButtonUnpack_clicked()
{
    QString sInitDirectory = QFileInfo(g_sFileName).absolutePath();
    QString sDirectoryName = QFileDialog::getExistingDirectory(this, tr("Open directory") + QString("..."), sInitDirectory, QFileDialog::ShowDirsOnly);

    if (!sDirectoryName.isEmpty()) {
        DialogUnpackFile dialogUnpackFile(this);
        dialogUnpackFile.setGlobal(getShortcuts(), getGlobalOptions());
        dialogUnpackFile.setData(g_sFileName, sDirectoryName);

        dialogUnpackFile.showDialogDelay();
    }
}

void FormatsWidget::on_toolButtonFiles_clicked()
{
    DialogArchive dialogArchive(this);
    dialogArchive.setGlobal(getShortcuts(), getGlobalOptions());

    FW_DEF::OPTIONS options = {};

    dialogArchive.setFileName(g_sFileName, getCurrentFileType(), options, QSet<XBinary::FT>());  // TODO Set current FT

    dialogArchive.exec();
}

void FormatsWidget::on_toolButtonYARA_clicked()
{
#ifdef USE_YARA
    showType(SBINARY::TYPE_YARASCAN);
#endif
}

void FormatsWidget::on_toolButtonDisasm_clicked()
{
    showType(SBINARY::TYPE_DISASM);
}

void FormatsWidget::on_toolButtonVisualization_clicked()
{
    showType(SBINARY::TYPE_VISUALIZATION);
}

void FormatsWidget::_showNfdInfo()
{
    showType(SBINARY::TYPE_NFDSCAN);
}

void FormatsWidget::_showYaraInfo()
{
    showType(SBINARY::TYPE_YARASCAN);
}

void FormatsWidget::_currentFileType(qint32 nFT)
{
    XFormats::setComboBoxCurrent(ui->comboBoxFileType, (XBinary::FT)nFT);
}

void FormatsWidget::on_toolButtonMANIFESTMF_clicked()
{
    DialogTextInfo dialogTextInfo(this);

    dialogTextInfo.setArchive(g_sFileName, "META-INF/MANIFEST.MF");

    dialogTextInfo.exec();
}

void FormatsWidget::on_toolButtonAndroidManifest_clicked()
{
    DialogTextInfo dialogTextInfo(this);

    QByteArray baData = XArchives::decompress(g_sFileName, "AndroidManifest.xml");
    if (baData.size() > 0) {
        QString sAndroidManifest = XAndroidBinary::getDecoded(&baData, nullptr);
        dialogTextInfo.setText(sAndroidManifest);
    }

    dialogTextInfo.exec();
}
