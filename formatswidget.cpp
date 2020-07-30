// Copyright (c) 2020 hors<horsicq@gmail.com>
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
#include "formatswidget.h"
#include "ui_formatswidget.h"

FormatsWidget::FormatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormatsWidget)
{
    ui->setupUi(this);

    const QSignalBlocker blocker(ui->comboBoxScanEngine);

    ui->comboBoxScanEngine->addItem(QString("Detect It Easy(DiE)"),TABSE_DIE);
    ui->comboBoxScanEngine->addItem(QString("Nauz File Detector(NFD)"),TABSE_NFD);

    ui->stackedWidgetMain->setCurrentIndex(TABINFO_BINARY);
    ui->stackedWidgetScan->setCurrentIndex(TABSE_DIE);
}

void FormatsWidget::setData(QString sFileName, bool bScan)
{
    const QSignalBlocker blocker(ui->comboBoxFileType);

    this->sFileName=sFileName;
    this->bScan=bScan;

    ui->comboBoxFileType->clear();

    QSet<XBinary::FT> stTypes=XBinary::getFileTypes(sFileName);

    XFormats::filterFileTypes(&stTypes);

    QList<XBinary::FT> listFileTypes=XBinary::_getFileTypeListFromSet(stTypes);

    int nCount=listFileTypes.count();

    for(int i=0;i<nCount;i++)
    {
        XBinary::FT ft=listFileTypes.at(i);
        ui->comboBoxFileType->addItem(XBinary::fileTypeIdToString(ft),ft);
    }

    if(nCount)
    {
        if(listFileTypes.at(0)==XBinary::FT_BINARY)
        {
            ui->comboBoxFileType->setCurrentIndex(0);
        }
        else
        {
            ui->comboBoxFileType->setCurrentIndex(nCount-1);
        }

        reload();
    }
}

void FormatsWidget::setScanEngine(QString sScanEngine)
{
    if(sScanEngine=="die")
    {
        ui->comboBoxScanEngine->setCurrentIndex(TABSE_DIE);
    }
    else if(sScanEngine=="nfd")
    {
        ui->comboBoxScanEngine->setCurrentIndex(TABSE_NFD);
    }
}

void FormatsWidget::setBackupFileName(QString sBackupFileName)
{
    this->sBackupFilename=sBackupFileName;
}

FormatsWidget::~FormatsWidget()
{
    delete ui;
}

void FormatsWidget::setDIEDatabasePath(QString sDatabasePath)
{
    ui->pageScanDIE->setDatabasePath(sDatabasePath);
}

void FormatsWidget::setDIEInfoPath(QString sDatabasePath)
{
    ui->pageScanDIE->setInfoPath(sDatabasePath);
}

void FormatsWidget::on_comboBoxFileType_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    reload();
}

void FormatsWidget::reload()
{
    XBinary::FT ft=getCurrentFileType();

    QFile file;
    file.setFileName(sFileName);

    if(file.open(QIODevice::ReadOnly))
    {
        if(ft==XBinary::FT_BINARY)
        {
            ui->groupBoxEntryPoint->hide();
            ui->groupBoxBaseAddress->hide();
        }
        else
        {
            ui->groupBoxEntryPoint->show();
            ui->groupBoxBaseAddress->show();
        }

        XBinary::_MEMORY_MAP memoryMap=XFormats::getMemoryMap(ft,&file);

        if(memoryMap.mode==XBinary::MODE_16)
        {
            ui->lineEditBaseAddress->setValue((quint16)memoryMap.nBaseAddress);
        }
        else if((memoryMap.mode==XBinary::MODE_16SEG)||(memoryMap.mode==XBinary::MODE_32))
        {
            ui->lineEditBaseAddress->setValue((quint32)memoryMap.nBaseAddress);
        }
        else if(memoryMap.mode==XBinary::MODE_64)
        {
            ui->lineEditBaseAddress->setValue((quint64)memoryMap.nBaseAddress);
        }

        ui->labelEndianness->setText(XBinary::endiannessToString(memoryMap.bIsBigEndian));
        ui->labelArch->setText(memoryMap.sArch);
        ui->labelMode->setText(XBinary::modeIdToString(memoryMap.mode));
        ui->labelType->setText(memoryMap.sType);

        ui->pushButtonDisasm->setEnabled(XBinary::isX86asm(memoryMap.sArch));

        if(ft==XBinary::FT_BINARY)
        {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_BINARY);
        }
        else if(ft==XBinary::FT_COM)
        {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_COM);
        }
        else if(ft==XBinary::FT_MSDOS)
        {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_MSDOS);

            XMSDOS msdos(&file);

            if(msdos.isValid())
            {
                ui->lineEditEntryPoint->setValue((quint16)msdos.getEntryPointAddress());

                ui->pushButtonMSDOSOverlay->setEnabled(msdos.isOverlayPresent());
            }
        }
        else if(ft==XBinary::FT_LE)
        {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_LE);

            XLE le(&file);

            if(le.isValid())
            {
                ui->lineEditEntryPoint->setValue((quint32)le.getEntryPointAddress());
            }
        }
        else if(ft==XBinary::FT_NE)
        {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_NE);

            XNE ne(&file);

            if(ne.isValid())
            {
                ui->lineEditEntryPoint->setValue((quint32)ne.getEntryPointAddress());
            }
        }
        else if((ft==XBinary::FT_PE32)||(ft==XBinary::FT_PE64))
        {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_PE);

            XPE pe(&file);

            if(pe.isValid())
            {
                if(pe.is64())
                {
                    ui->lineEditEntryPoint->setValue((quint64)pe.getEntryPointAddress());
                }
                else
                {
                    ui->lineEditEntryPoint->setValue((quint32)pe.getEntryPointAddress());
                }

                bool bIsResourcesPresent=pe.isResourcesPresent();

                ui->lineEditPESections->setValue(pe.getFileHeader_NumberOfSections());
                ui->groupBoxPESections->setEnabled(pe.isSectionsTablePresent());

                ui->pushButtonPEExport->setEnabled(pe.isExportPresent());
                ui->pushButtonPEImport->setEnabled(pe.isImportPresent());
                ui->pushButtonPEResources->setEnabled(bIsResourcesPresent);
                ui->pushButtonPENET->setEnabled(pe.isNETPresent());
                ui->pushButtonPETLS->setEnabled(pe.isTLSPresent());

                ui->groupBoxPEResources->setEnabled(bIsResourcesPresent);
                ui->pushButtonPEManifest->setEnabled(pe.isResourceManifestPresent());
                ui->pushButtonPEVersion->setEnabled(pe.isResourceVersionPresent());

                ui->pushButtonPEOverlay->setEnabled(pe.isOverlayPresent());

                ui->lineEditPETimeDateStamp->setText(XBinary::valueToTimeString(pe.getFileHeader_TimeDateStamp(),XBinary::DT_TYPE_POSIX));
                ui->lineEditPESizeOfImage->setValue(pe.getOptionalHeader_SizeOfImage());
            }
        }
        else if((ft==XBinary::FT_ELF32)||(ft==XBinary::FT_ELF64))
        {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_ELF);

            XELF elf(&file);

            if(elf.isValid())
            {
                if(elf.is64())
                {
                    ui->lineEditEntryPoint->setValue((quint64)elf.getEntryPointAddress());
                }
                else
                {
                    ui->lineEditEntryPoint->setValue((quint32)elf.getEntryPointAddress());
                }
            }

            ui->lineEditELFPrograms->setEnabled(elf.isProgramsTablePresent());
            ui->lineEditELFSections->setEnabled(elf.isSectionsTablePresent());
            ui->lineEditELFPrograms->setValue(elf.getNumberOfPrograms());
            ui->lineEditELFSections->setValue(elf.getNumberOfSections());
        }
        else if((ft==XBinary::FT_MACH32)||(ft==XBinary::FT_MACH64))
        {
            ui->stackedWidgetMain->setCurrentIndex(TABINFO_MACH);

            XMACH mach(&file);

            if(mach.isValid())
            {
                if(mach.is64())
                {
                    ui->lineEditEntryPoint->setValue((quint64)mach.getEntryPointAddress());
                }
                else
                {
                    ui->lineEditEntryPoint->setValue((quint32)mach.getEntryPointAddress());
                }

                QList<XMACH::COMMAND_RECORD> listCR=mach.getCommandRecords();
                QList<XMACH::SECTION_RECORD> listSections=mach.getSectionRecords(&listCR);
                QList<XMACH::SEGMENT_RECORD> listSegments=mach.getSegmentRecords(&listCR);
                QList<XMACH::LIBRARY_RECORD> listLibraries=mach.getLibraryRecords(&listCR);

                ui->lineEditMACHCommands->setEnabled(listCR.count());
                ui->lineEditMACHSections->setEnabled(listSections.count());
                ui->lineEditMACHSegments->setEnabled(listSegments.count());
                ui->lineEditMACHLibraries->setEnabled(listLibraries.count());

                ui->lineEditMACHCommands->setValue((quint16)listCR.count());
                ui->lineEditMACHSections->setValue((quint16)listSections.count());
                ui->lineEditMACHSegments->setValue((quint16)listSegments.count());
                ui->lineEditMACHLibraries->setValue((quint16)listLibraries.count());
            }
        }

        file.close();

        scan();
    }
    else
    {
        // TODO Error message
    }
}

void FormatsWidget::scan()
{
    // TODO FT
    int nIndex=ui->comboBoxScanEngine->currentIndex();

    if(nIndex==TABSE_DIE)
    {
        ui->pageScanDIE->setData(sFileName,bScan,getCurrentFileType());
    }
    else if(nIndex==TABSE_NFD)
    {
        ui->pageScanNFD->setData(sFileName,bScan,getCurrentFileType());
    }
    // TODO YARA
}

void FormatsWidget::on_pushButtonDisasm_clicked()
{
    QFile file;
    file.setFileName(sFileName);

    if(file.open(QIODevice::ReadOnly))
    {
        XDisasmModel::SHOWOPTIONS showOptions={};
        XDisasm::OPTIONS disasmOptions={};
        disasmOptions.ft=getCurrentFileType();

        DialogDisasm dialogDisasm(this,&file,&showOptions,&disasmOptions); // TODO File_Type

        dialogDisasm.exec();

        file.close();
    }
}

void FormatsWidget::on_pushButtonHexEntryPoint_clicked()
{
    QFile file;
    file.setFileName(sFileName);

    if(XBinary::tryToOpen(&file))
    {
        QHexView::OPTIONS hexOptions={};
        hexOptions.sBackupFileName=XBinary::getBackupName(&file);
        hexOptions.nStartAddress=XFormats::getEntryPointOffset(getCurrentFileType(),&file);

        DialogHex dialogHex(this,&file,&hexOptions);

        dialogHex.exec();

        file.close();
    }
}

void FormatsWidget::on_pushButtonMemoryMap_clicked()
{
    QFile file;
    file.setFileName(sFileName);

    if(file.open(QIODevice::ReadOnly))
    {
        DialogMemoryMap dialogMemoryMap(this,&file,getCurrentFileType());

        dialogMemoryMap.exec();

        file.close();
    }
}

void FormatsWidget::on_pushButtonPEExport_clicked()
{
    showPE(SPE::TYPE_EXPORT);
}

void FormatsWidget::on_pushButtonPEImport_clicked()
{
    showPE(SPE::TYPE_IMPORT);
}

void FormatsWidget::on_pushButtonPEResources_clicked()
{
    showPE(SPE::TYPE_RESOURCES);
}

void FormatsWidget::on_pushButtonPEOverlay_clicked()
{
    showPE(SPE::TYPE_OVERLAY);
}

void FormatsWidget::on_pushButtonPE_clicked()
{
    showPE(SPE::TYPE_IMAGE_FILE_HEADER);
}

void FormatsWidget::on_pushButtonPESections_clicked()
{
    showPE(SPE::TYPE_SECTIONS);
}

void FormatsWidget::on_pushButtonPEManifest_clicked()
{
    showPE(SPE::TYPE_RESOURCE_MANIFEST);
}

void FormatsWidget::on_pushButtonPEVersion_clicked()
{
    showPE(SPE::TYPE_RESOURCE_VERSION);
}

void FormatsWidget::on_pushButtonPENET_clicked()
{
    showPE(SPE::TYPE_NETHEADER);
}

void FormatsWidget::on_pushButtonMACH_clicked()
{
    showMACH(SMACH::TYPE_mach_header);
}

void FormatsWidget::on_pushButtonMACHSegments_clicked()
{
    showMACH(SMACH::TYPE_mach_segments);
}

void FormatsWidget::on_pushButtonMACHSections_clicked()
{
    showMACH(SMACH::TYPE_mach_sections);
}

void FormatsWidget::showMSDOS(SMSDOS::TYPE type)
{
    QFile file;
    file.setFileName(sFileName);

    if(XBinary::tryToOpen(&file))
    {
        FW_DEF::OPTIONS options={};

        if(sBackupFilename!="")
        {
            options.sBackupFileName=sBackupFilename;
        }

        options.nStartType=type;

        DialogMSDOS dialogMSDOS(this);

        dialogMSDOS.setData(&file,&options);

        dialogMSDOS.exec();

        file.close();
    }
}

void FormatsWidget::showLE(SLE::TYPE type)
{
    QFile file;
    file.setFileName(sFileName);

    if(XBinary::tryToOpen(&file))
    {
        FW_DEF::OPTIONS options={};

        if(sBackupFilename!="")
        {
            options.sBackupFileName=sBackupFilename;
        }

        options.nStartType=type;

        DialogLE dialogLE(this);

        dialogLE.setData(&file,&options);

        dialogLE.exec();

        file.close();
    }
}

void FormatsWidget::showNE(SNE::TYPE type)
{
    QFile file;
    file.setFileName(sFileName);

    if(XBinary::tryToOpen(&file))
    {
        FW_DEF::OPTIONS options={};

        if(sBackupFilename!="")
        {
            options.sBackupFileName=sBackupFilename;
        }

        options.nStartType=type;

        DialogNE dialogNE(this);

        dialogNE.setData(&file,&options);

        dialogNE.exec();

        file.close();
    }
}

void FormatsWidget::showPE(SPE::TYPE type)
{
    QFile file;
    file.setFileName(sFileName);

    if(XBinary::tryToOpen(&file))
    {
        FW_DEF::OPTIONS options={};

        if(sBackupFilename!="")
        {
            options.sBackupFileName=sBackupFilename;
        }

        options.nStartType=type;

        DialogPE dialogPE(this);

        dialogPE.setData(&file,&options);

        dialogPE.exec();

        file.close();
    }
}

void FormatsWidget::showELF(SELF::TYPE type)
{
    QFile file;
    file.setFileName(sFileName);

    if(XBinary::tryToOpen(&file))
    {
        FW_DEF::OPTIONS options={};

        if(sBackupFilename!="")
        {
            options.sBackupFileName=sBackupFilename;
        }

        options.nStartType=type;

        DialogELF dialogELF(this);

        dialogELF.setData(&file,&options);

        dialogELF.exec();

        file.close();
    }
}

void FormatsWidget::showMACH(SMACH::TYPE type)
{
    QFile file;
    file.setFileName(sFileName);

    if(XBinary::tryToOpen(&file))
    {
        FW_DEF::OPTIONS options={};

        if(sBackupFilename!="")
        {
            options.sBackupFileName=sBackupFilename;
        }

        options.nStartType=type;

        DialogMACH dialogMACH(this);

        dialogMACH.setData(&file,&options);

        dialogMACH.exec();

        file.close();
    }
}

XBinary::FT FormatsWidget::getCurrentFileType()
{
    return (XBinary::FT)(ui->comboBoxFileType->currentData().toInt());
}

void FormatsWidget::on_pushButtonMSDOSOverlay_clicked()
{
    showMSDOS(SMSDOS::TYPE_OVERLAY);
}

void FormatsWidget::on_pushButtonMSDOS_clicked()
{
    showMSDOS(SMSDOS::TYPE_DOS_HEADER);
}

void FormatsWidget::on_pushButtonPETLS_clicked()
{
    showPE(SPE::TYPE_TLS);
}

void FormatsWidget::on_pushButtonELF_clicked()
{
    showELF(SELF::TYPE_Elf_Ehdr);
}

void FormatsWidget::on_pushButtonELFSections_clicked()
{
    showELF(SELF::TYPE_Elf_Shdr);
}

void FormatsWidget::on_pushButtonELFPrograms_clicked()
{
    showELF(SELF::TYPE_Elf_Phdr);
}

void FormatsWidget::on_comboBoxScanEngine_currentIndexChanged(int nIndex)
{
    ui->stackedWidgetScan->setCurrentIndex(nIndex);

    scan();
}

void FormatsWidget::on_pushButtonLE_clicked()
{
    showLE(SLE::TYPE_VXD_HEADER);
}

void FormatsWidget::on_pushButtonNE_clicked()
{
    showNE(SNE::TYPE_OS2_HEADER);
}
