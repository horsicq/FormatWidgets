/* Copyright (c) 2020-2022 hors<horsicq@gmail.com>
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

FormatsWidget::FormatsWidget(QWidget *pParent)
    : XShortcutsWidget(pParent), ui(new Ui::FormatsWidget) {
    ui->setupUi(this);

    const bool bBlocked1 = ui->comboBoxScanEngine->blockSignals(true);

    ui->comboBoxScanEngine->addItem(tr("Automatic"), SE_AUTO);
    ui->comboBoxScanEngine->addItem(QString("Detect It Easy(DiE)"), SE_DIE);
    ui->comboBoxScanEngine->addItem(QString("Nauz File Detector(NFD)"), SE_NFD);

    ui->stackedWidgetMain->setCurrentIndex(TABINFO_BINARY);

    adjustScanTab(SE_AUTO);

    connect(ui->pageScanNFD, SIGNAL(scanStarted()), this,
            SLOT(onScanStarted()));
    connect(ui->pageScanNFD, SIGNAL(scanFinished()), this,
            SLOT(onScanFinished()));
    connect(ui->pageScanDIE, SIGNAL(scanStarted()), this,
            SLOT(onScanStarted()));
    connect(ui->pageScanDIE, SIGNAL(scanFinished()), this,
            SLOT(onScanFinished()));

    ui->comboBoxScanEngine->blockSignals(bBlocked1);
}

void FormatsWidget::setFileName(QString sFileName, bool bScan) {
    this->g_sFileName = sFileName;
    this->g_bScan = bScan;

    XFormats::setFileTypeComboBox(g_sFileName, ui->comboBoxFileType);

    reload();
}

void FormatsWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions) {
    ui->pageScanDIE->setGlobal(pShortcuts, pXOptions);
    ui->pageScanNFD->setGlobal(pShortcuts, pXOptions);

    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void FormatsWidget::adjustView() {
    QString sScanEngine = getGlobalOptions()->getScanEngine();

    if (sScanEngine == "die") {
        ui->comboBoxScanEngine->setCurrentIndex(SE_DIE);
    } else if (sScanEngine == "nfd") {
        ui->comboBoxScanEngine->setCurrentIndex(SE_NFD);
    }

    ui->pageScanDIE->adjustView();
    ui->pageScanNFD->adjustView();

    XShortcutsWidget::adjustView();
}

FormatsWidget::~FormatsWidget() { delete ui; }

void FormatsWidget::on_comboBoxFileType_currentIndexChanged(int nIndex) {
    Q_UNUSED(nIndex)

    reload();
}

void FormatsWidget::reload() {
    adjustScanTab(getScanEngine((SE)ui->comboBoxScanEngine->currentIndex()));

    XBinary::FT fileType = getCurrentFileType();

    QFile file;
    file.setFileName(g_sFileName);

    if (file.open(QIODevice::ReadOnly)) {
        ui->lineEditFileSize->setText(XBinary::bytesCountToString(file.size()));

        XBinary::_MEMORY_MAP memoryMap =
            XFormats::getMemoryMap(fileType, &file);

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
            ui->lineEditBaseAddress->setValue((quint8)memoryMap.nModuleAddress);
        } else if (mode == XBinary::MODE_16) {
            if (memoryMap.fileType == XBinary::FT_MSDOS) {
                ui->lineEditBaseAddress->setValue(
                    (quint32)memoryMap.nModuleAddress);
            } else {
                ui->lineEditBaseAddress->setValue(
                    (quint16)memoryMap.nModuleAddress);
            }
        } else if ((mode == XBinary::MODE_16SEG) ||
                   (mode == XBinary::MODE_32)) {
            ui->lineEditBaseAddress->setValue(
                (quint32)memoryMap.nModuleAddress);
        } else if (mode == XBinary::MODE_64) {
            ui->lineEditBaseAddress->setValue(
                (quint64)memoryMap.nModuleAddress);
        }

        ui->lineEditEndianness->setText(
            XBinary::endiannessToString(memoryMap.bIsBigEndian));
        ui->lineEditArch->setText(memoryMap.sArch);
        ui->lineEditMode->setText(XBinary::modeIdToString(memoryMap.mode));
        ui->lineEditType->setText(memoryMap.sType);

        if (fileType == XBinary::FT_BINARY) {
            XBinary binary(&file);

            ui->stackedWidgetMain->setCurrentIndex(TABINFO_BINARY);

            ui->lineEditEntryPoint->setValue(
                (quint32)binary.getEntryPointAddress());
        } else if (fileType == XBinary::FT_COM) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_COM);

            XCOM com(&file);

            if (com.isValid()) {
                ui->lineEditEntryPoint->setValue(
                    (quint16)com.getEntryPointAddress());
            }
        } else if (fileType == XBinary::FT_ZIP) {
            // TODO Set name on button
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_ZIP);

            XZip xzip(&file);

            if (xzip.isValid()) {
                ui->lineEditEntryPoint->setValue(xzip.getEntryPointAddress());
            }
        } else if (fileType == XBinary::FT_DEX) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_DEX);

            XDEX dex(&file);

            if (dex.isValid()) {
                ui->lineEditEntryPoint->setValue(
                    (quint16)dex.getEntryPointAddress());
            }
        } else if (fileType == XBinary::FT_MSDOS) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_MSDOS);

            XMSDOS msdos(&file);

            if (msdos.isValid()) {
                ui->lineEditEntryPoint->setValue(
                    (quint32)msdos.getEntryPointAddress());

                ui->pushButtonMSDOSOverlay->setEnabled(
                    msdos.isOverlayPresent());
            }
        } else if ((fileType == XBinary::FT_LE) ||
                   (fileType == XBinary::FT_LX))  // mb TODO LX
        {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_LE);

            XLE le(&file);

            if (le.isValid()) {
                ui->lineEditEntryPoint->setValue(
                    (quint32)le.getEntryPointAddress());
            }
        } else if (fileType == XBinary::FT_NE) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_NE);

            XNE ne(&file);

            if (ne.isValid()) {
                ui->lineEditEntryPoint->setValue(
                    (quint32)ne.getEntryPointAddress());
            }
        } else if ((fileType == XBinary::FT_PE32) ||
                   (fileType == XBinary::FT_PE64)) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_PE);

            XPE pe(&file);

            if (pe.isValid()) {
                if (pe.is64()) {
                    ui->lineEditEntryPoint->setValue(
                        (quint64)pe.getEntryPointAddress());
                } else {
                    ui->lineEditEntryPoint->setValue(
                        (quint32)pe.getEntryPointAddress());
                }

                bool bIsResourcesPresent = pe.isResourcesPresent();

                ui->lineEditPESections->setValue(
                    pe.getFileHeader_NumberOfSections());
                ui->groupBoxPESections->setEnabled(pe.isSectionsTablePresent());

                ui->pushButtonPEExport->setEnabled(pe.isExportPresent());
                ui->pushButtonPEImport->setEnabled(pe.isImportPresent());
                ui->pushButtonPEResources->setEnabled(bIsResourcesPresent);
                ui->pushButtonPENET->setEnabled(pe.isNETPresent());
                ui->pushButtonPETLS->setEnabled(pe.isTLSPresent());

                ui->groupBoxPEResources->setEnabled(bIsResourcesPresent);
                ui->pushButtonPEManifest->setEnabled(
                    pe.isResourceManifestPresent());
                ui->pushButtonPEVersion->setEnabled(
                    pe.isResourceVersionPresent());

                ui->pushButtonPEOverlay->setEnabled(pe.isOverlayPresent());

                ui->lineEditPETimeDateStamp->setText(XBinary::valueToTimeString(
                    pe.getFileHeader_TimeDateStamp(), XBinary::DT_TYPE_POSIX));
                ui->lineEditPESizeOfImage->setValue(
                    pe.getOptionalHeader_SizeOfImage());
            }
        } else if ((fileType == XBinary::FT_ELF32) ||
                   (fileType == XBinary::FT_ELF64)) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_ELF);

            XELF elf(&file);

            if (elf.isValid()) {
                if (elf.is64()) {
                    ui->lineEditEntryPoint->setValue(
                        (quint64)elf.getEntryPointAddress());
                } else {
                    ui->lineEditEntryPoint->setValue(
                        (quint32)elf.getEntryPointAddress());
                }
            }

            ui->lineEditELFPrograms->setEnabled(elf.isProgramsTablePresent());
            ui->lineEditELFSections->setEnabled(elf.isSectionsTablePresent());
            ui->lineEditELFPrograms->setValue(elf.getNumberOfPrograms());
            ui->lineEditELFSections->setValue(elf.getNumberOfSections());
        } else if ((fileType == XBinary::FT_MACHO32) ||
                   (fileType == XBinary::FT_MACHO64)) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_MACH);

            XMACH mach(&file);

            if (mach.isValid()) {
                if (mach.is64()) {
                    ui->lineEditEntryPoint->setValue(
                        (quint64)mach.getEntryPointAddress());
                } else {
                    ui->lineEditEntryPoint->setValue(
                        (quint32)mach.getEntryPointAddress());
                }

                QList<XMACH::COMMAND_RECORD> listCommandRecords =
                    mach.getCommandRecords();
                QList<XMACH::SECTION_RECORD> listSectionRecords =
                    mach.getSectionRecords(&listCommandRecords);
                QList<XMACH::SEGMENT_RECORD> listSegmentRecords =
                    mach.getSegmentRecords(&listCommandRecords);
                QList<XMACH::LIBRARY_RECORD> listLibraryRecords =
                    mach.getLibraryRecords(&listCommandRecords);

                ui->lineEditMACHCommands->setEnabled(
                    listCommandRecords.count());
                ui->lineEditMACHSections->setEnabled(
                    listSectionRecords.count());
                ui->lineEditMACHSegments->setEnabled(
                    listSegmentRecords.count());
                ui->lineEditMACHLibraries->setEnabled(
                    listLibraryRecords.count());

                ui->lineEditMACHCommands->setValue(
                    (quint16)listCommandRecords.count());
                ui->lineEditMACHSections->setValue(
                    (quint16)listSectionRecords.count());
                ui->lineEditMACHSegments->setValue(
                    (quint16)listSegmentRecords.count());
                ui->lineEditMACHLibraries->setValue(
                    (quint16)listLibraryRecords.count());
            }
        } else if (fileType == XBinary::FT_MACHOFAT) {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_MACHOFAT);

            XMACHOFat machofat(&file);

            if (machofat.isValid()) {
                ui->lineEditEntryPoint->setValue(
                    machofat.getEntryPointAddress());
            }
        }

        file.close();

        scan();
    } else {
        // TODO Error message
    }
}

void FormatsWidget::scan() {
    int nIndex = ui->comboBoxScanEngine->currentIndex();

    nIndex = getScanEngine((SE)nIndex);

    adjustScanTab((SE)nIndex);

    if (g_sFileName != "") {
        if (nIndex == SE_DIE) {
            ui->pageScanDIE->setData(g_sFileName, g_bScan,
                                     getCurrentFileType());
        } else if (nIndex == SE_NFD) {
            ui->pageScanNFD->setData(g_sFileName, g_bScan,
                                     getCurrentFileType());
        }
    }

    // TODO YARA
}

void FormatsWidget::on_pushButtonDisasm_clicked() {
    if (g_sFileName != "") {
        QFile file;
        file.setFileName(g_sFileName);

        if (file.open(QIODevice::ReadOnly)) {
            DialogMultiDisasm dialogDisasm(this);  // TODO File_Type

            XMultiDisasmWidget::OPTIONS options = {};
            options.fileType = getCurrentFileType();
            options.nInitAddress = ui->lineEditEntryPoint->getValue();

            dialogDisasm.setData(&file, options);
            dialogDisasm.setGlobal(getShortcuts(), getGlobalOptions());

            dialogDisasm.exec();

            file.close();
        }
    }
}

void FormatsWidget::on_pushButtonHexEntryPoint_clicked() {
    if (g_sFileName != "") {
        QFile file;
        file.setFileName(g_sFileName);

        if (XBinary::tryToOpen(&file)) {
            XHexView::OPTIONS hexOptions = {};
            //        hexOptions.sBackupFileName=XBinary::getBackupName(&file);
            hexOptions.nStartSelectionOffset =
                XFormats::getEntryPointOffset(getCurrentFileType(), &file);

            DialogHexView dialogHex(this, &file, hexOptions, &file);
            dialogHex.setGlobal(getShortcuts(), getGlobalOptions());

            dialogHex.exec();

            file.close();
        }
    }
}

void FormatsWidget::on_pushButtonMemoryMap_clicked() {
    if (g_sFileName != "") {
        QFile file;
        file.setFileName(g_sFileName);

        if (file.open(QIODevice::ReadOnly)) {
            DialogMemoryMap dialogMemoryMap(this, &file, getCurrentFileType());
            dialogMemoryMap.setGlobal(getShortcuts(), getGlobalOptions());

            dialogMemoryMap.exec();

            file.close();
        }
    }
}

void FormatsWidget::on_pushButtonPEExport_clicked() {
    showPE(SPE::TYPE_EXPORT);
}

void FormatsWidget::on_pushButtonPEImport_clicked() {
    showPE(SPE::TYPE_IMPORT);
}

void FormatsWidget::on_pushButtonPEResources_clicked() {
    showPE(SPE::TYPE_RESOURCES);
}

void FormatsWidget::on_pushButtonPEOverlay_clicked() {
    showPE(SPE::TYPE_OVERLAY);
}

void FormatsWidget::on_pushButtonPE_clicked() {
    showPE(SPE::TYPE_IMAGE_FILE_HEADER);
}

void FormatsWidget::on_pushButtonPESections_clicked() {
    showPE(SPE::TYPE_SECTIONS);
}

void FormatsWidget::on_pushButtonPEManifest_clicked() {
    showPE(SPE::TYPE_RESOURCES_MANIFEST);
}

void FormatsWidget::on_pushButtonPEVersion_clicked() {
    showPE(SPE::TYPE_RESOURCES_VERSION);
}

void FormatsWidget::on_pushButtonPENET_clicked() {
    showPE(SPE::TYPE_NETHEADER);
}

void FormatsWidget::on_pushButtonMACH_clicked() {
    showMACH(SMACH::TYPE_mach_header);
}

void FormatsWidget::on_pushButtonMACHSegments_clicked() {
    showMACH(SMACH::TYPE_mach_segments);
}

void FormatsWidget::on_pushButtonMACHSections_clicked() {
    showMACH(SMACH::TYPE_mach_sections);
}

void FormatsWidget::on_pushButtonMACHCommands_clicked() {
    showMACH(SMACH::TYPE_mach_commands);
}

void FormatsWidget::on_pushButtonMACHLibraries_clicked() {
    showMACH(SMACH::TYPE_mach_libraries);
}

void FormatsWidget::showMSDOS(SMSDOS::TYPE type) {
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogMSDOS dialogMSDOS(this);

        dialogMSDOS.setData(&file, options);
        dialogMSDOS.setGlobal(getShortcuts(), getGlobalOptions());

        dialogMSDOS.exec();

        file.close();
    }
}

void FormatsWidget::showLE(SLE::TYPE type) {
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogLE dialogLE(this);

        dialogLE.setData(&file, options);
        dialogLE.setGlobal(getShortcuts(), getGlobalOptions());

        dialogLE.exec();

        file.close();
    }
}

void FormatsWidget::showNE(SNE::TYPE type) {
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogNE dialogNE(this);

        dialogNE.setData(&file, options);
        dialogNE.setGlobal(getShortcuts(), getGlobalOptions());

        dialogNE.exec();

        file.close();
    }
}

void FormatsWidget::showPE(SPE::TYPE type) {
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogPE dialogPE(this);

        dialogPE.setData(&file, options);
        dialogPE.setGlobal(getShortcuts(), getGlobalOptions());

        dialogPE.exec();

        file.close();
    }
}

void FormatsWidget::showELF(SELF::TYPE type) {
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogELF dialogELF(this);

        dialogELF.setData(&file, options);
        dialogELF.setGlobal(getShortcuts(), getGlobalOptions());

        dialogELF.exec();

        file.close();
    }
}

void FormatsWidget::showMACH(SMACH::TYPE type) {
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogMACH dialogMACH(this);

        dialogMACH.setData(&file, options);
        dialogMACH.setGlobal(getShortcuts(), getGlobalOptions());

        dialogMACH.exec();

        file.close();
    }
}

void FormatsWidget::showDEX(SDEX::TYPE type) {
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        FW_DEF::OPTIONS options = {};

        options.nStartType = type;
        options.nImageBase = -1;

        DialogDEX dialogDEX(this);

        dialogDEX.setData(&file, options);
        dialogDEX.setGlobal(getShortcuts(), getGlobalOptions());

        dialogDEX.exec();

        file.close();
    }
}

XBinary::FT FormatsWidget::getCurrentFileType() {
    XBinary::FT fileType =
        (XBinary::FT)(ui->comboBoxFileType->currentData().toInt());

    return fileType;
}

void FormatsWidget::on_pushButtonMSDOSOverlay_clicked() {
    showMSDOS(SMSDOS::TYPE_OVERLAY);
}

void FormatsWidget::on_pushButtonMSDOS_clicked() {
    showMSDOS(SMSDOS::TYPE_DOS_HEADER);
}

void FormatsWidget::on_pushButtonPETLS_clicked() { showPE(SPE::TYPE_TLS); }

void FormatsWidget::on_pushButtonELF_clicked() { showELF(SELF::TYPE_Elf_Ehdr); }

void FormatsWidget::on_pushButtonELFSections_clicked() {
    showELF(SELF::TYPE_Elf_Shdr);
}

void FormatsWidget::on_pushButtonELFPrograms_clicked() {
    showELF(SELF::TYPE_Elf_Phdr);
}

void FormatsWidget::on_comboBoxScanEngine_currentIndexChanged(int nIndex) {
    adjustScanTab(getScanEngine((SE)nIndex));

    scan();
}

void FormatsWidget::on_pushButtonLE_clicked() { showLE(SLE::TYPE_VXD_HEADER); }

void FormatsWidget::on_pushButtonNE_clicked() { showNE(SNE::TYPE_OS2_HEADER); }

void FormatsWidget::on_pushButtonDEX_clicked() { showDEX(SDEX::TYPE_HEADER); }

void FormatsWidget::on_pushButtonZIP_clicked() {
    DialogArchive dialogArchive(this);

    FW_DEF::OPTIONS options = {};

    dialogArchive.setFileName(g_sFileName, options, QSet<XBinary::FT>());
    dialogArchive.setGlobal(getShortcuts(), getGlobalOptions());

    dialogArchive.exec();
}

void FormatsWidget::on_pushButtonMACHOFAT_clicked() {
    QFile file;
    file.setFileName(g_sFileName);

    if (XBinary::tryToOpen(&file)) {
        DialogMACHOFAT dialogMACHOFAT(this);

        FW_DEF::OPTIONS options = {};
        options.nImageBase = -1;

        dialogMACHOFAT.setData(&file, options);
        dialogMACHOFAT.setGlobal(getShortcuts(), getGlobalOptions());

        dialogMACHOFAT.exec();

        file.close();
    }
}

FormatsWidget::SE FormatsWidget::getScanEngine(FormatsWidget::SE seIndex) {
    SE tabResult = seIndex;

    if (seIndex == SE_AUTO) {
        tabResult = SE_DIE;

        XBinary::FT fileType = getCurrentFileType();

        // TODO !!!
        if ((fileType == XBinary::FT_DEX) || (fileType == XBinary::FT_ELF32) ||
            (fileType == XBinary::FT_ELF64) ||
            (fileType == XBinary::FT_MACHO32) ||
            (fileType == XBinary::FT_MACHO64) ||
            (fileType == XBinary::FT_MACHOFAT) ||
            (fileType == XBinary::FT_ZIP) || (fileType == XBinary::FT_JAR) ||
            (fileType == XBinary::FT_APK) || (fileType == XBinary::FT_APKS) ||
            (fileType == XBinary::FT_IPA)) {
            tabResult = SE_NFD;
        }
    }

    return tabResult;
}

void FormatsWidget::adjustScanTab(FormatsWidget::SE seIndex) {
    if (seIndex == SE_DIE) {
        ui->stackedWidgetScan->setCurrentIndex(0);
    } else if (seIndex == SE_NFD) {
        ui->stackedWidgetScan->setCurrentIndex(1);
    }
}

void FormatsWidget::onScanStarted() {
    ui->stackedWidgetMain->setEnabled(false);
    ui->groupBoxFileType->setEnabled(false);
    ui->groupBoxScanEngine->setEnabled(false);
    ui->groupBoxEntryPoint->setEnabled(false);
    ui->groupBoxBaseAddress->setEnabled(false);
}

void FormatsWidget::onScanFinished() {
    ui->stackedWidgetMain->setEnabled(true);
    ui->groupBoxFileType->setEnabled(true);
    ui->groupBoxScanEngine->setEnabled(true);
    ui->groupBoxEntryPoint->setEnabled(true);
    ui->groupBoxBaseAddress->setEnabled(true);
}

void FormatsWidget::registerShortcuts(bool bState) { Q_UNUSED(bState) }

void FormatsWidget::on_pushButtonFileInfo_clicked() {
    QString sFileName = g_sFileName;

    if (sFileName != "") {
        QFile file;
        file.setFileName(sFileName);

        if (file.open(QIODevice::ReadOnly)) {
            DialogXFileInfo dialogFileInfo(this);
            dialogFileInfo.setGlobal(getShortcuts(), getGlobalOptions());

            dialogFileInfo.setData(&file, XBinary::FT_UNKNOWN, "Info", true);

            dialogFileInfo.exec();

            file.close();
        }
    }
}

void FormatsWidget::on_pushButtonMIME_clicked() {
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

void FormatsWidget::on_pushButtonHash_clicked() {
    QString sFileName = g_sFileName;

    if (sFileName != "") {
        QFile file;
        file.setFileName(sFileName);

        if (file.open(QIODevice::ReadOnly)) {
            DialogHash dialogHash(this);
            dialogHash.setData(&file, XBinary::FT_UNKNOWN);
            dialogHash.setGlobal(getShortcuts(), getGlobalOptions());

            dialogHash.exec();

            file.close();
        }
    }
}

void FormatsWidget::on_pushButtonStrings_clicked() {
    QString sFileName = g_sFileName;

    if (sFileName != "") {
        QFile file;
        file.setFileName(sFileName);

        if (file.open(QIODevice::ReadOnly)) {
            SearchStringsWidget::OPTIONS stringsOptions = {};
            stringsOptions.bAnsi = true;
            stringsOptions.bUTF8 = false;
            stringsOptions.bUnicode = true;
            stringsOptions.bCStrings = true;

            DialogSearchStrings dialogSearchStrings(this);
            dialogSearchStrings.setData(&file, stringsOptions, true);
            dialogSearchStrings.setGlobal(getShortcuts(), getGlobalOptions());

            dialogSearchStrings.exec();

            file.close();
        }
    }
}

void FormatsWidget::on_pushButtonSignatures_clicked() {
    QString sFileName = g_sFileName;

    if (sFileName != "") {
        QFile file;
        file.setFileName(sFileName);

        if (file.open(QIODevice::ReadOnly)) {
            SearchSignaturesWidget::OPTIONS signaturesOptions = {};

            DialogSearchSignatures dialogSearchSignatures(this);
            dialogSearchSignatures.setData(&file, XBinary::FT_UNKNOWN,
                                           signaturesOptions);
            dialogSearchSignatures.setGlobal(getShortcuts(),
                                             getGlobalOptions());

            dialogSearchSignatures.exec();

            file.close();
        }
    }
}

void FormatsWidget::on_pushButtonHex_clicked() {
    QString sFileName = g_sFileName;

    if (sFileName != "") {
        QFile file;
        file.setFileName(sFileName);

        if (XBinary::tryToOpen(&file)) {
            XHexView::OPTIONS hexOptions = {};

            DialogHexView dialogHex(this, &file, hexOptions, &file);
            dialogHex.setGlobal(getShortcuts(), getGlobalOptions());

            dialogHex.exec();

            file.close();
        }
    }
}

void FormatsWidget::on_pushButtonEntropy_clicked() {
    QString sFileName = g_sFileName;

    if (sFileName != "") {
        QFile file;
        file.setFileName(sFileName);

        if (file.open(QIODevice::ReadOnly)) {
            DialogEntropy dialogEntropy(this);
            dialogEntropy.setData(&file);
            dialogEntropy.setGlobal(getShortcuts(), getGlobalOptions());

            dialogEntropy.exec();

            file.close();
        }
    }
}

void FormatsWidget::on_pushButtonVirusTotal_clicked() {
    QString sFileName = g_sFileName;

    if (sFileName != "") {
        QFile file;
        file.setFileName(sFileName);

        if (file.open(QIODevice::ReadOnly)) {
            if (getGlobalOptions()->getVirusTotalApiKey() != "") {
                DialogXVirusTotal dialogVirusTotal(this);
                dialogVirusTotal.setGlobal(getShortcuts(), getGlobalOptions());
                dialogVirusTotal.setData(&file);

                dialogVirusTotal.exec();
            } else {
                QString sMD5 = XBinary::getHash(XBinary::HASH_MD5, &file);
                XVirusTotalWidget::showInBrowser(sMD5);
            }

            file.close();
        }
    }
}

void FormatsWidget::on_pushButtonExtractor_clicked() {
    QString sFileName = g_sFileName;

    if (sFileName != "") {
        QFile file;
        file.setFileName(sFileName);

        if (file.open(QIODevice::ReadOnly)) {
            DialogXExtractor dialogExtractor(this);
            dialogExtractor.setData(&file);
            dialogExtractor.setGlobal(getShortcuts(), getGlobalOptions());

            dialogExtractor.exec();

            file.close();
        }
    }
}
