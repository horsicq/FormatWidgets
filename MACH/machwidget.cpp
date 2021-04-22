// copyright (c) 2017-2021 hors<horsicq@gmail.com>
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
#include "machwidget.h"
#include "ui_machwidget.h"

MACHWidget::MACHWidget(QWidget *pParent) :
    FormatWidget(pParent),
    ui(new Ui::MACHWidget)
{
    ui->setupUi(this);

    initHexViewWidget(ui->widgetHex);
    initSearchSignaturesWidget(ui->widgetSignatures);
    initSearchStringsWidget(ui->widgetStrings);
    initToolsWidget(ui->widgetHex_commands);
    initToolsWidget(ui->widgetHex_mach_header);
    initToolsWidget(ui->widgetHex_sections);
    initToolsWidget(ui->widgetHex_segments);
}

MACHWidget::MACHWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent) :
    MACHWidget(pParent)
{
    MACHWidget::setData(pDevice,options,0,0,0);
    MACHWidget::reload();
}

MACHWidget::~MACHWidget()
{
    delete ui;
}

void MACHWidget::setShortcuts(XShortcuts *pShortcuts)
{
    ui->widgetHex->setShortcuts(pShortcuts);
    ui->widgetDisasm->setShortcuts(pShortcuts);
    ui->widgetStrings->setShortcuts(pShortcuts);
    ui->widgetEntropy->setShortcuts(pShortcuts);
    ui->widgetHeuristicScan->setShortcuts(pShortcuts);
    ui->widgetMemoryMap->setShortcuts(pShortcuts);
    ui->widgetHex_commands->setShortcuts(pShortcuts);
    ui->widgetHex_mach_header->setShortcuts(pShortcuts);
    ui->widgetHex_sections->setShortcuts(pShortcuts);
    ui->widgetHex_segments->setShortcuts(pShortcuts);

    FormatWidget::setShortcuts(pShortcuts);
}

void MACHWidget::clear()
{
    MACHWidget::reset();

    memset(g_lineEdit_mach_header,0,sizeof g_lineEdit_mach_header);
    memset(g_lineEdit_mach_dyld_info_only,0,sizeof g_lineEdit_mach_dyld_info_only);
    memset(g_lineEdit_mach_uuid,0,sizeof g_lineEdit_mach_uuid);
    memset(g_lineEdit_mach_dylinker,0,sizeof g_lineEdit_mach_dylinker);
    memset(g_lineEdit_mach_rpath,0,sizeof g_lineEdit_mach_rpath);
    memset(g_lineEdit_mach_symtab,0,sizeof g_lineEdit_mach_symtab);
    memset(g_lineEdit_mach_dysymtab,0,sizeof g_lineEdit_mach_dysymtab);
    memset(g_lineEdit_mach_version_min,0,sizeof g_lineEdit_mach_version_min);
    memset(g_lineEdit_mach_source_version,0,sizeof g_lineEdit_mach_source_version);
    memset(g_lineEdit_mach_encryption_info,0,sizeof g_lineEdit_mach_encryption_info);
    memset(g_lineEdit_mach_function_starts,0,sizeof g_lineEdit_mach_function_starts);
    memset(g_lineEdit_mach_data_in_code,0,sizeof g_lineEdit_mach_data_in_code);
    memset(g_lineEdit_mach_code_signature,0,sizeof g_lineEdit_mach_code_signature);
    memset(g_lineEdit_mach_main,0,sizeof g_lineEdit_mach_main);
    memset(g_lineEdit_mach_unix_thread,0,sizeof g_lineEdit_mach_unix_thread);

    memset(g_comboBox,0,sizeof g_comboBox);
    memset(g_subDevice,0,sizeof g_subDevice);

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void MACHWidget::cleanup()
{

}

void MACHWidget::reset()
{
    g_stInit.clear();
}

void MACHWidget::reload()
{
    MACHWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XMACH mach(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

    if(mach.isValid())
    {
        setFileType(mach.getFileType());

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_HEX,tr("Hex")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_HASH,tr("Hash")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_DISASM,tr("Disasm")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_STRINGS,tr("Strings")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_SIGNATURES,tr("Signatures")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_MEMORYMAP,tr("Memory map")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_ENTROPY,tr("Entropy")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_HEURISTICSCAN,tr("Heuristic scan")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_mach_header,mach.is64()?("mach_header_64"):("mach_header")));

        QList<XMACH::COMMAND_RECORD> listCommandRecords=mach.getCommandRecords();

        if(listCommandRecords.count())
        {
            QTreeWidgetItem *pItemCommands=createNewItem(SMACH::TYPE_mach_commands,tr("Commands"));

            ui->treeWidgetNavi->addTopLevelItem(pItemCommands);

            QList<XMACH::SEGMENT_RECORD> listSegmentRecords=mach.getSegmentRecords(&listCommandRecords);

            if(listSegmentRecords.count())
            {
                QTreeWidgetItem *pItemSegments=createNewItem(SMACH::TYPE_mach_segments,tr("Segments")); // TODO rename

                pItemCommands->addChild(pItemSegments);

                QList<XMACH::SECTION_RECORD> listSectionRecords=mach.getSectionRecords(&listCommandRecords);

                if(listSectionRecords.count())
                {
                    QTreeWidgetItem *pItemSections=createNewItem(SMACH::TYPE_mach_sections,tr("Sections")); // TODO rename

                    pItemSegments->addChild(pItemSections);
                }
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_ID_DYLIB,&listCommandRecords))
            {
                QTreeWidgetItem *pItemLibraries=createNewItem(SMACH::TYPE_mach_id_library,QString("LC_ID_DYLIB"));

                pItemCommands->addChild(pItemLibraries);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_LOAD_DYLIB,&listCommandRecords))
            {
                QTreeWidgetItem *pItemLibraries=createNewItem(SMACH::TYPE_mach_libraries,QString("LC_LOAD_DYLIB"));

                pItemCommands->addChild(pItemLibraries);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_LOAD_WEAK_DYLIB,&listCommandRecords))
            {
                QTreeWidgetItem *pItemLibraries=createNewItem(SMACH::TYPE_mach_weak_libraries,QString("LC_LOAD_WEAK_DYLIB"));

                pItemCommands->addChild(pItemLibraries);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_DYLD_INFO_ONLY,&listCommandRecords))
            {
                QTreeWidgetItem *pItemDyldInfo=createNewItem(SMACH::TYPE_mach_dyld_info_only,QString("LC_DYLD_INFO_ONLY"),mach.getCommandRecordOffset(XMACH_DEF::LC_DYLD_INFO_ONLY,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemDyldInfo);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_UUID,&listCommandRecords))
            {
                QTreeWidgetItem *pItemUuid=createNewItem(SMACH::TYPE_mach_uuid,QString("LC_UUID"),mach.getCommandRecordOffset(XMACH_DEF::LC_UUID,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemUuid);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_SYMTAB,&listCommandRecords))
            {
                QTreeWidgetItem *pItemSymtab=createNewItem(SMACH::TYPE_mach_symtab,QString("LC_SYMTAB"),mach.getCommandRecordOffset(XMACH_DEF::LC_SYMTAB,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemSymtab);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_DYSYMTAB,&listCommandRecords))
            {
                QTreeWidgetItem *pItemDysymtab=createNewItem(SMACH::TYPE_mach_dysymtab,QString("LC_DYSYMTAB"),mach.getCommandRecordOffset(XMACH_DEF::LC_DYSYMTAB,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemDysymtab);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_VERSION_MIN_MACOSX,&listCommandRecords))
            {
                QTreeWidgetItem *pItemVersionMin=createNewItem(SMACH::TYPE_mach_version_min,QString("LC_VERSION_MIN_MACOSX"),mach.getCommandRecordOffset(XMACH_DEF::LC_VERSION_MIN_MACOSX,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemVersionMin);
            }
            else if(mach.isCommandPresent(XMACH_DEF::LC_VERSION_MIN_IPHONEOS,&listCommandRecords))
            {
                QTreeWidgetItem *pItemVersionMin=createNewItem(SMACH::TYPE_mach_version_min,QString("LC_VERSION_MIN_IPHONEOS"),mach.getCommandRecordOffset(XMACH_DEF::LC_VERSION_MIN_IPHONEOS,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemVersionMin);
            }
            else if(mach.isCommandPresent(XMACH_DEF::LC_VERSION_MIN_TVOS,&listCommandRecords))
            {
                QTreeWidgetItem *pItemVersionMin=createNewItem(SMACH::TYPE_mach_version_min,QString("LC_VERSION_MIN_TVOS"),mach.getCommandRecordOffset(XMACH_DEF::LC_VERSION_MIN_TVOS,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemVersionMin);
            }
            else if(mach.isCommandPresent(XMACH_DEF::LC_VERSION_MIN_WATCHOS,&listCommandRecords))
            {
                QTreeWidgetItem *pItemVersionMin=createNewItem(SMACH::TYPE_mach_version_min,QString("LC_VERSION_MIN_WATCHOS"),mach.getCommandRecordOffset(XMACH_DEF::LC_VERSION_MIN_WATCHOS,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemVersionMin);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_LOAD_DYLINKER,&listCommandRecords))
            {
                QTreeWidgetItem *pItemDylinker=createNewItem(SMACH::TYPE_mach_dylinker,QString("LC_LOAD_DYLINKER"),mach.getCommandRecordOffset(XMACH_DEF::LC_LOAD_DYLINKER,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemDylinker);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_RPATH,&listCommandRecords))
            {
                QTreeWidgetItem *pItemRPath=createNewItem(SMACH::TYPE_mach_rpath,QString("LC_RPATH"),mach.getCommandRecordOffset(XMACH_DEF::LC_RPATH,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemRPath);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_SOURCE_VERSION,&listCommandRecords))
            {
                QTreeWidgetItem *pItemSourceVersion=createNewItem(SMACH::TYPE_mach_source_version,QString("LC_SOURCE_VERSION"),mach.getCommandRecordOffset(XMACH_DEF::LC_SOURCE_VERSION,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemSourceVersion);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_ENCRYPTION_INFO,&listCommandRecords))
            {
                QTreeWidgetItem *pItemEncryptionInfo=createNewItem(SMACH::TYPE_mach_encryption_info,QString("LC_ENCRYPTION_INFO"),mach.getCommandRecordOffset(XMACH_DEF::LC_ENCRYPTION_INFO,0,&listCommandRecords),mach.get_encryption_info_command_size()); // TODO rename

                pItemCommands->addChild(pItemEncryptionInfo);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_ENCRYPTION_INFO_64,&listCommandRecords))
            {
                QTreeWidgetItem *pItemEncryptionInfo=createNewItem(SMACH::TYPE_mach_encryption_info,QString("LC_ENCRYPTION_INFO_64"),mach.getCommandRecordOffset(XMACH_DEF::LC_ENCRYPTION_INFO_64,0,&listCommandRecords),mach.get_encryption_info_command_64_size()); // TODO rename

                pItemCommands->addChild(pItemEncryptionInfo);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_FUNCTION_STARTS,&listCommandRecords))
            {
                QTreeWidgetItem *pItemFunctionStarts=createNewItem(SMACH::TYPE_mach_function_starts,QString("LC_FUNCTION_STARTS"),mach.getCommandRecordOffset(XMACH_DEF::LC_FUNCTION_STARTS,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemFunctionStarts);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_DATA_IN_CODE,&listCommandRecords))
            {
                QTreeWidgetItem *pItemDataInCode=createNewItem(SMACH::TYPE_mach_data_in_code,QString("LC_DATA_IN_CODE"),mach.getCommandRecordOffset(XMACH_DEF::LC_DATA_IN_CODE,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemDataInCode);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_CODE_SIGNATURE,&listCommandRecords))
            {
                QTreeWidgetItem *pItemCodeSignature=createNewItem(SMACH::TYPE_mach_code_signature,QString("LC_CODE_SIGNATURE"),mach.getCommandRecordOffset(XMACH_DEF::LC_CODE_SIGNATURE,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemCodeSignature);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_MAIN,&listCommandRecords))
            {
                QTreeWidgetItem *pItemMain=createNewItem(SMACH::TYPE_mach_main,QString("LC_MAIN"),mach.getCommandRecordOffset(XMACH_DEF::LC_MAIN,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemMain);
            }

            if(mach.isCommandPresent(XMACH_DEF::LC_UNIXTHREAD,&listCommandRecords))
            {
                QTreeWidgetItem *pItemUnixThread=createNewItem(SMACH::TYPE_mach_unix_thread,QString("LC_UNIXTHREAD"),mach.getCommandRecordOffset(XMACH_DEF::LC_UNIXTHREAD,0,&listCommandRecords)); // TODO rename

                pItemCommands->addChild(pItemUnixThread);
            }
        }

        ui->treeWidgetNavi->expandAll();

        setTreeItem(ui->treeWidgetNavi,getOptions().nStartType);
    }
}

FormatWidget::SV MACHWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)

    SV result=SV_NONE;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();
    QString sValue=vValue.toString();

    if(getDevice()->isWritable())
    {
        XMACH mach(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

        if(mach.isValid())
        {
            switch(nStype)
            {
                case SMACH::TYPE_mach_header:
                    switch(nNdata)
                    {
                        case N_mach_header::magic:      g_comboBox[CB_mach_header_magic]->setValue(nValue);       break; // TODO reload all data
                        case N_mach_header::cputype:    g_comboBox[CB_mach_header_cputype]->setValue(nValue);     break;
                        case N_mach_header::cpusubtype: g_comboBox[CB_mach_header_cpusubtype]->setValue(nValue);  break;
                        case N_mach_header::filetype:   g_comboBox[CB_mach_header_filetype]->setValue(nValue);    break;
                        case N_mach_header::flags:      g_comboBox[CB_mach_header_flags]->setValue(nValue);       break;
                    }
                    break;
            }

            switch(nStype)
            {
                case SMACH::TYPE_mach_header:
                    switch(nNdata)
                    {
                        case N_mach_header::magic:      mach.setHeader_magic((quint32)nValue);          break;
                        case N_mach_header::cputype:    mach.setHeader_cputype((qint32)nValue);         break;
                        case N_mach_header::cpusubtype: mach.setHeader_cpusubtype((qint32)nValue);      break;
                        case N_mach_header::filetype:   mach.setHeader_filetype((quint32)nValue);       break;
                        case N_mach_header::ncmds:      mach.setHeader_ncmds((quint32)nValue);          break;
                        case N_mach_header::sizeofcmds: mach.setHeader_sizeofcmds((quint32)nValue);     break;
                        case N_mach_header::flags:      mach.setHeader_flags((quint32)nValue);          break;
                        case N_mach_header::reserved:   mach.setHeader_reserved((quint32)nValue);       break;
                    }

                    ui->widgetHex_mach_header->reload();

                    break;

                case SMACH::TYPE_mach_dyld_info_only:
                    switch(nNdata)
                    {
                        case N_mach_dyld_info::rebase_off:          mach._set_dyld_info_command_rebase_off(nOffset,nValue);           break;
                        case N_mach_dyld_info::rebase_size:         mach._set_dyld_info_command_rebase_size(nOffset,nValue);          break;
                        case N_mach_dyld_info::bind_off:            mach._set_dyld_info_command_bind_off(nOffset,nValue);             break;
                        case N_mach_dyld_info::bind_size:           mach._set_dyld_info_command_bind_size(nOffset,nValue);            break;
                        case N_mach_dyld_info::weak_bind_off:       mach._set_dyld_info_command_weak_bind_off(nOffset,nValue);        break;
                        case N_mach_dyld_info::weak_bind_size:      mach._set_dyld_info_command_weak_bind_size(nOffset,nValue);       break;
                        case N_mach_dyld_info::lazy_bind_off:       mach._set_dyld_info_command_lazy_bind_off(nOffset,nValue);        break;
                        case N_mach_dyld_info::lazy_bind_size:      mach._set_dyld_info_command_lazy_bind_size(nOffset,nValue);       break;
                        case N_mach_dyld_info::export_off:          mach._set_dyld_info_command_export_off(nOffset,nValue);           break;
                        case N_mach_dyld_info::export_size:         mach._set_dyld_info_command_export_size(nOffset,nValue);          break;
                    }

                    ui->widgetHex_dyld_info_only->reload();

                    break;

                case SMACH::TYPE_mach_uuid:
                    switch(nNdata)
                    {
                        case N_mach_uuid::uuid:             mach.setUUID(sValue);               break;
                    }

                    break;

                case SMACH::TYPE_mach_dylinker:
                    switch(nNdata)
                    {
                        case N_mach_dylinker::dylinker:     mach.setLoadDylinker(sValue);       break;
                    }

                    break;

                case SMACH::TYPE_mach_rpath:
                    switch(nNdata)
                    {
                        case N_mach_rpath::path:            mach.setRPath(sValue);              break;
                    }

                    break;

                case SMACH::TYPE_mach_source_version:
                    switch(nNdata)
                    {
                        case N_mach_source_version::version:    mach.setSourceVersion(nValue);  break;
                    }

                    ui->widgetHex_source_version->reload();

                    break;

                case SMACH::TYPE_mach_symtab:
                    switch(nNdata)
                    {
                        case N_mach_symtab::symoff:             mach._set_symtab_command_symoff(nOffset,nValue);            break;
                        case N_mach_symtab::nsyms:              mach._set_symtab_command_nsyms(nOffset,nValue);             break;
                        case N_mach_symtab::stroff:             mach._set_symtab_command_stroff(nOffset,nValue);            break;
                        case N_mach_symtab::strsize:            mach._set_symtab_command_strsize(nOffset,nValue);           break;
                    }

                    ui->widgetHex_symtab->reload();

                    break;

                case SMACH::TYPE_mach_dysymtab:
                    switch(nNdata)
                    {
                        case N_mach_dysymtab::ilocalsym:            mach._set_dysymtab_command_ilocalsym(nOffset,nValue);           break;
                        case N_mach_dysymtab::nlocalsym:            mach._set_dysymtab_command_nlocalsym(nOffset,nValue);           break;
                        case N_mach_dysymtab::iextdefsym:           mach._set_dysymtab_command_iextdefsym(nOffset,nValue);          break;
                        case N_mach_dysymtab::nextdefsym:           mach._set_dysymtab_command_nextdefsym(nOffset,nValue);          break;
                        case N_mach_dysymtab::iundefsym:            mach._set_dysymtab_command_iundefsym(nOffset,nValue);           break;
                        case N_mach_dysymtab::nundefsym:            mach._set_dysymtab_command_nundefsym(nOffset,nValue);           break;
                        case N_mach_dysymtab::tocoff:               mach._set_dysymtab_command_tocoff(nOffset,nValue);              break;
                        case N_mach_dysymtab::ntoc:                 mach._set_dysymtab_command_ntoc(nOffset,nValue);                break;
                        case N_mach_dysymtab::modtaboff:            mach._set_dysymtab_command_modtaboff(nOffset,nValue);           break;
                        case N_mach_dysymtab::nmodtab:              mach._set_dysymtab_command_nmodtab(nOffset,nValue);             break;
                        case N_mach_dysymtab::extrefsymoff:         mach._set_dysymtab_command_extrefsymoff(nOffset,nValue);        break;
                        case N_mach_dysymtab::nextrefsyms:          mach._set_dysymtab_command_nextrefsyms(nOffset,nValue);         break;
                        case N_mach_dysymtab::indirectsymoff:       mach._set_dysymtab_command_indirectsymoff(nOffset,nValue);      break;
                        case N_mach_dysymtab::nindirectsyms:        mach._set_dysymtab_command_nindirectsyms(nOffset,nValue);       break;
                        case N_mach_dysymtab::extreloff:            mach._set_dysymtab_command_extreloff(nOffset,nValue);           break;
                        case N_mach_dysymtab::nextrel:              mach._set_dysymtab_command_nextrel(nOffset,nValue);             break;
                        case N_mach_dysymtab::locreloff:            mach._set_dysymtab_command_locreloff(nOffset,nValue);           break;
                        case N_mach_dysymtab::nlocrel:              mach._set_dysymtab_command_nlocrel(nOffset,nValue);             break;
                    }

                    ui->widgetHex_dysymtab->reload();

                    break;

                case SMACH::TYPE_mach_version_min:
                    switch(nNdata)
                    {
                        case N_mach_version_min::version:           mach._set_version_min_command_version(nOffset,nValue);          break;
                        case N_mach_version_min::sdk:               mach._set_version_min_command_sdk(nOffset,nValue);              break;
                    }

                    ui->widgetHex_version_min->reload();

                    break;

                case SMACH::TYPE_mach_encryption_info:
                    switch(nNdata)
                    {
                        case N_mach_encryption_info::cryptoff:      mach._set_encryption_info_command_64_cryptoff(nOffset,nValue);      break;
                        case N_mach_encryption_info::cryptsize:     mach._set_encryption_info_command_64_cryptsize(nOffset,nValue);     break;
                        case N_mach_encryption_info::cryptid:       mach._set_encryption_info_command_64_cryptid(nOffset,nValue);       break;
                        case N_mach_encryption_info::pad:           mach._set_encryption_info_command_64_pad(nOffset,nValue);           break;
                    }

                    ui->widgetHex_encryption_info->reload();

                    break;

                case SMACH::TYPE_mach_function_starts:
                    switch(nNdata)
                    {
                        case N_mach_linkedit_data::dataoff:         mach._set_linkedit_data_command_dataoff(nOffset,nValue);                break;
                        case N_mach_linkedit_data::datasize:        mach._set_linkedit_data_command_datasize(nOffset,nValue);               break;
                    }

                    ui->widgetHex_function_starts->reload();

                    break;

                case SMACH::TYPE_mach_data_in_code:
                    switch(nNdata)
                    {
                        case N_mach_linkedit_data::dataoff:         mach._set_linkedit_data_command_dataoff(nOffset,nValue);                break;
                        case N_mach_linkedit_data::datasize:        mach._set_linkedit_data_command_datasize(nOffset,nValue);               break;
                    }

                    ui->widgetHex_data_in_code->reload();

                    break;

                case SMACH::TYPE_mach_code_signature:
                    switch(nNdata)
                    {
                        case N_mach_linkedit_data::dataoff:         mach._set_linkedit_data_command_dataoff(nOffset,nValue);                break;
                        case N_mach_linkedit_data::datasize:        mach._set_linkedit_data_command_datasize(nOffset,nValue);               break;
                    }

                    ui->widgetHex_code_signature->reload();

                    break;

                case SMACH::TYPE_mach_main:
                    switch(nNdata)
                    {
                        case N_mach_main::entryoff:                 mach._set_entry_point_command_entryoff(nOffset,nValue);                 break;
                        case N_mach_main::stacksize:                mach._set_entry_point_command_stacksize(nOffset,nValue);                break;
                    }

                    ui->widgetHex_main->reload();

                    break;

                case SMACH::TYPE_mach_unix_thread:
                    switch(nNdata)
                    {
                        case N_mach_unix_thread::flavor:            mach._set_unix_thread_command_flavor(nOffset,nValue);                   break;
                        case N_mach_unix_thread::count:             mach._set_unix_thread_command_count(nOffset,nValue);                    break;
                    }

                    ui->widgetHex_main->reload();

                    break;
            }

            result=SV_EDITED;

            switch(nStype)
            {
                case SMACH::TYPE_mach_header:
                    switch(nNdata)
                    {
                        case N_mach_header::magic:      result=SV_RELOAD;               break;
                        case N_mach_header::cputype:    result=SV_RELOADDATA;           break;
                    }

                    break;
            }
        }
    }

    blockSignals(false);

    return result;
}

void MACHWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(g_lineEdit_mach_header,N_mach_header::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_dyld_info_only,N_mach_dyld_info::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_uuid,N_mach_uuid::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_dylinker,N_mach_dylinker::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_rpath,N_mach_rpath::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_symtab,N_mach_symtab::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_dysymtab,N_mach_dysymtab::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_version_min,N_mach_version_min::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_source_version,N_mach_source_version::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_encryption_info,N_mach_encryption_info::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_function_starts,N_mach_linkedit_data::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_data_in_code,N_mach_linkedit_data::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_code_signature,N_mach_linkedit_data::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_main,N_mach_main::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_unix_thread,N_mach_unix_thread::__data_size,bState);
    setComboBoxesReadOnly(g_comboBox,__CB_size,bState);

    ui->widgetHex->setReadonly(bState);
}

void MACHWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_lineEdit_mach_header,N_mach_header::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_dyld_info_only,N_mach_dyld_info::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_uuid,N_mach_uuid::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_dylinker,N_mach_dylinker::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_rpath,N_mach_rpath::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_symtab,N_mach_symtab::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_dysymtab,N_mach_dysymtab::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_version_min,N_mach_version_min::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_source_version,N_mach_source_version::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_encryption_info,N_mach_encryption_info::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_function_starts,N_mach_linkedit_data::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_data_in_code,N_mach_linkedit_data::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_code_signature,N_mach_linkedit_data::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_main,N_mach_main::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_unix_thread,N_mach_unix_thread::__data_size,bState);
    _blockSignals((QObject **)g_comboBox,__CB_size,bState);
}

void MACHWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_NAME,QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_OFFSET,QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_TYPE,QHeaderView::ResizeToContents);

    switch(nType)
    {
        case SMACH::TYPE_mach_header:
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*26);
            break;
        default:
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*12);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*26);
            break;
    }
}

QString MACHWidget::typeIdToString(int nType)
{
    QString sResult="Unknown";

    switch(nType)
    {
        case SMACH::TYPE_mach_commands:         sResult=QString("Command %1").arg(tr("Header"));        break;
        case SMACH::TYPE_mach_segments:         sResult=QString("Segment %1").arg(tr("Header"));        break;
        case SMACH::TYPE_mach_sections:         sResult=QString("Section %1").arg(tr("Header"));        break;
        case SMACH::TYPE_mach_libraries:        sResult=QString("Library %1").arg(tr("Header"));        break;
        case SMACH::TYPE_mach_weak_libraries:   sResult=QString("Library %1").arg(tr("Header"));        break;
        case SMACH::TYPE_mach_id_library:       sResult=QString("Library %1").arg(tr("Header"));        break;
    }

    return sResult;
}

void MACHWidget::_showInDisasmWindowAddress(qint64 nAddress)
{
    setTreeItem(ui->treeWidgetNavi,SMACH::TYPE_DISASM);
    ui->widgetDisasm->goToAddress(nAddress);
}

void MACHWidget::_showInDisasmWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi,SMACH::TYPE_DISASM);
    ui->widgetDisasm->goToOffset(nOffset);
}

void MACHWidget::_showInMemoryMapWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi,SMACH::TYPE_MEMORYMAP);
    ui->widgetMemoryMap->goToOffset(nOffset);
}

void MACHWidget::_showInHexWindow(qint64 nOffset, qint64 nSize)
{
    setTreeItem(ui->treeWidgetNavi,SMACH::TYPE_HEX);
    ui->widgetHex->setSelection(nOffset,nSize);
}

void MACHWidget::reloadData()
{
    int nType=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_TYPE).toInt();
    qint64 nDataOffset=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nDataSize=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole+FW_DEF::SECTION_DATA_SIZE).toLongLong();

    QString sInit=QString("%1-%2-%3").arg(nType).arg(nDataOffset).arg(nDataSize);

    if((g_nLastType==nType)&&(sInit!=g_sLastInit))
    {
        g_stInit.remove(sInit);
    }

    g_nLastType=nType;
    g_sLastInit=sInit;

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XMACH mach(getDevice(),getOptions().bIsImage,getOptions().nImageBase);

    if(mach.isValid())
    {
        if(nType==SMACH::TYPE_HEX)
        {
            if(!g_stInit.contains(sInit))
            {
                XHexView::OPTIONS options={};
                options.bMenu_Disasm=true;
                options.bMenu_MemoryMap=true;
                options.sSignaturesPath=getOptions().sSearchSignaturesPath;
                ui->widgetHex->setData(getDevice(),options);
                ui->widgetHex->enableReadOnly(false);
            }
        }
        else if(nType==SMACH::TYPE_HASH)
        {
            if(!g_stInit.contains(sInit))
            {
                ui->widgetHash->setData(getDevice(),mach.getFileType(),0,-1,true,this);
            }
        }
        else if(nType==SMACH::TYPE_DISASM)
        {
            if(!g_stInit.contains(sInit))
            {
                XMultiDisasmWidget::OPTIONS options={};
                options.fileType=mach.getFileType();
                options.nInitAddress=mach.getEntryPointAddress();
                options.sSignaturesPath=getOptions().sSearchSignaturesPath;

                ui->widgetDisasm->setData(getDevice(),options);
            }
        }
        else if(nType==SMACH::TYPE_STRINGS)
        {
            if(!g_stInit.contains(sInit))
            {
                SearchStringsWidget::OPTIONS stringsOptions={};
                stringsOptions.bMenu_Hex=true;
                stringsOptions.bMenu_Demangle=true;
                stringsOptions.bAnsi=true;
                stringsOptions.bUnicode=true;

                ui->widgetStrings->setData(getDevice(),stringsOptions,true,this);
            }
        }
        else if(nType==SMACH::TYPE_SIGNATURES)
        {
            if(!g_stInit.contains(sInit))
            {
                SearchSignaturesWidget::OPTIONS signaturesOptions={};
                signaturesOptions.bMenu_Hex=true;
                signaturesOptions.sSignaturesPath=getOptions().sSearchSignaturesPath;

                ui->widgetSignatures->setData(getDevice(),mach.getFileType(),signaturesOptions,false,this);
            }
        }
        else if(nType==SMACH::TYPE_MEMORYMAP)
        {
            if(!g_stInit.contains(sInit))
            {
                ui->widgetMemoryMap->setData(getDevice(),mach.getFileType(),getOptions().sSearchSignaturesPath);
            }
        }
        else if(nType==SMACH::TYPE_ENTROPY)
        {
            if(!g_stInit.contains(sInit))
            {
                ui->widgetEntropy->setData(getDevice(),0,getDevice()->size(),mach.getFileType(),true,this);
            }
        }
        else if(nType==SMACH::TYPE_HEURISTICSCAN)
        {
            if(!g_stInit.contains(sInit))
            {
                ui->widgetHeuristicScan->setData(getDevice(),true,mach.getFileType(),this);
            }
        }
        else if(nType==SMACH::TYPE_mach_header)
        {
            if(!g_stInit.contains(sInit))
            {
                if(!mach.is64())
                {
                    createHeaderTable(SMACH::TYPE_mach_header,ui->tableWidget_mach_header,N_mach_header::records32,g_lineEdit_mach_header,N_mach_header::__data_size-1,0);
                }
                else
                {
                    createHeaderTable(SMACH::TYPE_mach_header,ui->tableWidget_mach_header,N_mach_header::records64,g_lineEdit_mach_header,N_mach_header::__data_size,0);
                }

                g_comboBox[CB_mach_header_magic]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderMagicsS(),SMACH::TYPE_mach_header,N_mach_header::magic,XComboBoxEx::CBTYPE_NORMAL);
                g_comboBox[CB_mach_header_cputype]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderCpuTypesS(),SMACH::TYPE_mach_header,N_mach_header::cputype,XComboBoxEx::CBTYPE_NORMAL);
                g_comboBox[CB_mach_header_cpusubtype]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderCpuSubTypes(mach.getHeader_cputype()),SMACH::TYPE_mach_header,N_mach_header::cpusubtype,XComboBoxEx::CBTYPE_NORMAL);
                g_comboBox[CB_mach_header_filetype]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderFileTypesS(),SMACH::TYPE_mach_header,N_mach_header::filetype,XComboBoxEx::CBTYPE_NORMAL);
                g_comboBox[CB_mach_header_flags]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderFlagsS(),SMACH::TYPE_mach_header,N_mach_header::flags,XComboBoxEx::CBTYPE_FLAGS);

                blockSignals(true);

                g_lineEdit_mach_header[N_mach_header::magic]->setValue(mach.getHeader_magic());
                g_lineEdit_mach_header[N_mach_header::cputype]->setValue(mach.getHeader_cputype());
                g_lineEdit_mach_header[N_mach_header::cpusubtype]->setValue(mach.getHeader_cpusubtype());
                g_lineEdit_mach_header[N_mach_header::filetype]->setValue(mach.getHeader_filetype());
                g_lineEdit_mach_header[N_mach_header::ncmds]->setValue(mach.getHeader_ncmds());
                g_lineEdit_mach_header[N_mach_header::sizeofcmds]->setValue(mach.getHeader_sizeofcmds());
                g_lineEdit_mach_header[N_mach_header::flags]->setValue(mach.getHeader_flags());

                if(mach.is64())
                {
                    g_lineEdit_mach_header[N_mach_header::reserved]->setValue(mach.getHeader_reserved());
                }

                g_comboBox[CB_mach_header_magic]->setValue(mach.getHeader_magic());
                g_comboBox[CB_mach_header_cputype]->setValue((quint32)mach.getHeader_cputype());
                g_comboBox[CB_mach_header_cpusubtype]->setValue((quint32)mach.getHeader_cpusubtype());
                g_comboBox[CB_mach_header_filetype]->setValue((quint32)mach.getHeader_filetype());
                g_comboBox[CB_mach_header_flags]->setValue((quint32)mach.getHeader_flags());

                qint64 nOffset=mach.getHeaderOffset();
                qint64 nSize=mach.getHeaderSize();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_header],ui->widgetHex_mach_header);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_commands)
        {
            if(!g_stInit.contains(sInit))
            {
                MACHProcessData machProcessData(SMACH::TYPE_mach_commands,&tvModel[SMACH::TYPE_mach_commands],&mach,0,0);

                ajustTableView(&machProcessData,&tvModel[SMACH::TYPE_mach_commands],ui->tableView_commands,nullptr,true);

                connect(ui->tableView_commands->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_commands_currentRowChanged(QModelIndex,QModelIndex)));

                if(tvModel[SMACH::TYPE_mach_commands]->rowCount())
                {
                    ui->tableView_commands->setCurrentIndex(ui->tableView_commands->model()->index(0,0));
                }
            }
        }
        else if(nType==SMACH::TYPE_mach_segments)
        {
            if(!g_stInit.contains(sInit))
            {
                MACHProcessData machProcessData(SMACH::TYPE_mach_segments,&tvModel[SMACH::TYPE_mach_segments],&mach,0,0);

                ajustTableView(&machProcessData,&tvModel[SMACH::TYPE_mach_segments],ui->tableView_segments,nullptr,false);

                connect(ui->tableView_segments->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_segments_currentRowChanged(QModelIndex,QModelIndex)));

                if(tvModel[SMACH::TYPE_mach_segments]->rowCount())
                {
                    ui->tableView_segments->setCurrentIndex(ui->tableView_segments->model()->index(0,0));
                }
            }
        }
        else if(nType==SMACH::TYPE_mach_sections)
        {
            if(!g_stInit.contains(sInit))
            {
                MACHProcessData machProcessData(SMACH::TYPE_mach_sections,&tvModel[SMACH::TYPE_mach_sections],&mach,0,0);

                ajustTableView(&machProcessData,&tvModel[SMACH::TYPE_mach_sections],ui->tableView_sections,nullptr,false);

                connect(ui->tableView_sections->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_sections_currentRowChanged(QModelIndex,QModelIndex)));

                if(tvModel[SMACH::TYPE_mach_sections]->rowCount())
                {
                    ui->tableView_sections->setCurrentIndex(ui->tableView_sections->model()->index(0,0));
                }
            }
        }
        else if(nType==SMACH::TYPE_mach_libraries)
        {
            if(!g_stInit.contains(sInit))
            {
                MACHProcessData machProcessData(SMACH::TYPE_mach_libraries,&tvModel[SMACH::TYPE_mach_libraries],&mach,0,0);

                ajustTableView(&machProcessData,&tvModel[SMACH::TYPE_mach_libraries],ui->tableView_libraries,nullptr,true);

                connect(ui->tableView_libraries->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_libraries_currentRowChanged(QModelIndex,QModelIndex)));

                if(tvModel[SMACH::TYPE_mach_libraries]->rowCount())
                {
                    ui->tableView_libraries->setCurrentIndex(ui->tableView_libraries->model()->index(0,0));
                }
            }
        }
        else if(nType==SMACH::TYPE_mach_weak_libraries)
        {
            if(!g_stInit.contains(sInit))
            {
                MACHProcessData machProcessData(SMACH::TYPE_mach_weak_libraries,&tvModel[SMACH::TYPE_mach_weak_libraries],&mach,0,0);

                ajustTableView(&machProcessData,&tvModel[SMACH::TYPE_mach_weak_libraries],ui->tableView_weak_libraries,nullptr,true);

                connect(ui->tableView_weak_libraries->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_weak_libraries_currentRowChanged(QModelIndex,QModelIndex)));

                if(tvModel[SMACH::TYPE_mach_weak_libraries]->rowCount())
                {
                    ui->tableView_weak_libraries->setCurrentIndex(ui->tableView_weak_libraries->model()->index(0,0));
                }
            }
        }
        else if(nType==SMACH::TYPE_mach_id_library)
        {
            if(!g_stInit.contains(sInit))
            {
                MACHProcessData machProcessData(SMACH::TYPE_mach_id_library,&tvModel[SMACH::TYPE_mach_id_library],&mach,0,0);

                ajustTableView(&machProcessData,&tvModel[SMACH::TYPE_mach_id_library],ui->tableView_id_library,nullptr,true);

                connect(ui->tableView_id_library->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(onTableView_id_library_currentRowChanged(QModelIndex,QModelIndex)));

                if(tvModel[SMACH::TYPE_mach_id_library]->rowCount())
                {
                    ui->tableView_id_library->setCurrentIndex(ui->tableView_id_library->model()->index(0,0));
                }
            }
        }
        else if(nType==SMACH::TYPE_mach_dyld_info_only)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_dyld_info_only,ui->tableWidget_dyld_info_only,N_mach_dyld_info::records,g_lineEdit_mach_dyld_info_only,N_mach_dyld_info::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::dyld_info_command dyld_info=mach._read_dyld_info_command(nDataOffset);

                g_lineEdit_mach_dyld_info_only[N_mach_dyld_info::rebase_off]->setValue(dyld_info.rebase_off);
                g_lineEdit_mach_dyld_info_only[N_mach_dyld_info::rebase_size]->setValue(dyld_info.rebase_size);
                g_lineEdit_mach_dyld_info_only[N_mach_dyld_info::bind_off]->setValue(dyld_info.bind_off);
                g_lineEdit_mach_dyld_info_only[N_mach_dyld_info::bind_size]->setValue(dyld_info.bind_size);
                g_lineEdit_mach_dyld_info_only[N_mach_dyld_info::weak_bind_off]->setValue(dyld_info.weak_bind_off);
                g_lineEdit_mach_dyld_info_only[N_mach_dyld_info::weak_bind_size]->setValue(dyld_info.weak_bind_size);
                g_lineEdit_mach_dyld_info_only[N_mach_dyld_info::lazy_bind_off]->setValue(dyld_info.lazy_bind_off);
                g_lineEdit_mach_dyld_info_only[N_mach_dyld_info::lazy_bind_size]->setValue(dyld_info.lazy_bind_size);
                g_lineEdit_mach_dyld_info_only[N_mach_dyld_info::export_off]->setValue(dyld_info.export_off);
                g_lineEdit_mach_dyld_info_only[N_mach_dyld_info::export_size]->setValue(dyld_info.export_size);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_dyld_info_command_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_dyld_info_only],ui->widgetHex_dyld_info_only);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_uuid)
        {
            if(!g_stInit.contains(sInit))
            {
                createListTable(SMACH::TYPE_mach_uuid,ui->tableWidget_uuid,N_mach_uuid::records,g_lineEdit_mach_uuid,N_mach_uuid::__data_size);

                blockSignals(true);

                g_lineEdit_mach_uuid[N_mach_uuid::uuid]->setUUID(mach.getUUID());

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_dylinker)
        {
            if(!g_stInit.contains(sInit))
            {
                createListTable(SMACH::TYPE_mach_dylinker,ui->tableWidget_dylinker,N_mach_dylinker::records,g_lineEdit_mach_dylinker,N_mach_dylinker::__data_size);

                blockSignals(true);

                g_lineEdit_mach_dylinker[N_mach_dylinker::dylinker]->setStringValue(mach.getLoadDylinker()); // TODO Max Size!!!

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_rpath)
        {
            if(!g_stInit.contains(sInit))
            {
                createListTable(SMACH::TYPE_mach_rpath,ui->tableWidget_rpath,N_mach_rpath::records,g_lineEdit_mach_rpath,N_mach_rpath::__data_size);

                blockSignals(true);

                g_lineEdit_mach_rpath[N_mach_rpath::path]->setStringValue(mach.getRPath()); // TODO Max Size!!!

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_symtab)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_symtab,ui->tableWidget_symtab,N_mach_symtab::records,g_lineEdit_mach_symtab,N_mach_symtab::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::symtab_command symtab=mach._read_symtab_command(nDataOffset);

                g_lineEdit_mach_symtab[N_mach_symtab::symoff]->setValue(symtab.symoff);
                g_lineEdit_mach_symtab[N_mach_symtab::nsyms]->setValue(symtab.nsyms);
                g_lineEdit_mach_symtab[N_mach_symtab::stroff]->setValue(symtab.stroff);
                g_lineEdit_mach_symtab[N_mach_symtab::strsize]->setValue(symtab.strsize);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_symtab_command_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_symtab],ui->widgetHex_symtab);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_dysymtab)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_dysymtab,ui->tableWidget_dysymtab,N_mach_dysymtab::records,g_lineEdit_mach_dysymtab,N_mach_dysymtab::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::dysymtab_command dysymtab=mach._read_dysymtab_command(nDataOffset);

                g_lineEdit_mach_dysymtab[N_mach_dysymtab::ilocalsym]->setValue(dysymtab.ilocalsym);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::nlocalsym]->setValue(dysymtab.nlocalsym);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::iextdefsym]->setValue(dysymtab.iextdefsym);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::nextdefsym]->setValue(dysymtab.nextdefsym);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::iundefsym]->setValue(dysymtab.iundefsym);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::nundefsym]->setValue(dysymtab.nundefsym);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::tocoff]->setValue(dysymtab.tocoff);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::ntoc]->setValue(dysymtab.ntoc);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::modtaboff]->setValue(dysymtab.modtaboff);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::nmodtab]->setValue(dysymtab.nmodtab);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::extrefsymoff]->setValue(dysymtab.extrefsymoff);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::nextrefsyms]->setValue(dysymtab.nextrefsyms);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::indirectsymoff]->setValue(dysymtab.indirectsymoff);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::nindirectsyms]->setValue(dysymtab.nindirectsyms);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::extreloff]->setValue(dysymtab.extreloff);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::nextrel]->setValue(dysymtab.nextrel);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::locreloff]->setValue(dysymtab.locreloff);
                g_lineEdit_mach_dysymtab[N_mach_dysymtab::nlocrel]->setValue(dysymtab.nlocrel);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_dysymtab_command_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_dysymtab],ui->widgetHex_dysymtab);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_version_min)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_version_min,ui->tableWidget_version_min,N_mach_version_min::records,g_lineEdit_mach_version_min,N_mach_version_min::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::version_min_command version_min=mach._read_version_min_command(nDataOffset);

                g_lineEdit_mach_version_min[N_mach_version_min::version]->setValue(version_min.version);
                g_lineEdit_mach_version_min[N_mach_version_min::sdk]->setValue(version_min.sdk);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_version_min_command_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_version_min],ui->widgetHex_version_min);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_source_version)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_source_version,ui->tableWidget_source_version,N_mach_source_version::records,g_lineEdit_mach_source_version,N_mach_source_version::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::source_version_command source_version=mach._read_source_version_command(nDataOffset);

                g_lineEdit_mach_source_version[N_mach_source_version::version]->setValue(source_version.version);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_source_version_command_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_source_version],ui->widgetHex_source_version);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_encryption_info)
        {
            if(!g_stInit.contains(sInit))
            {
                if(nDataSize==mach.get_encryption_info_command_64_size())
                {
                    createHeaderTable(SMACH::TYPE_mach_encryption_info,ui->tableWidget_encryption_info,N_mach_encryption_info::records64,g_lineEdit_mach_encryption_info,N_mach_encryption_info::__data_size,0,nDataOffset);

                    blockSignals(true);

                    XMACH_DEF::encryption_info_command_64 encryption_info=mach._read_encryption_info_command_64(nDataOffset);

                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::cryptoff]->setValue(encryption_info.cryptoff);
                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::cryptsize]->setValue(encryption_info.cryptsize);
                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::cryptid]->setValue(encryption_info.cryptid);
                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::pad]->setValue(encryption_info.pad);
                }
                else
                {
                    createHeaderTable(SMACH::TYPE_mach_encryption_info,ui->tableWidget_encryption_info,N_mach_encryption_info::records32,g_lineEdit_mach_encryption_info,N_mach_encryption_info::__data_size-1,0,nDataOffset);

                    blockSignals(true);

                    XMACH_DEF::encryption_info_command encryption_info=mach._read_encryption_info_command(nDataOffset);

                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::cryptoff]->setValue(encryption_info.cryptoff);
                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::cryptsize]->setValue(encryption_info.cryptsize);
                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::cryptid]->setValue(encryption_info.cryptid);
                }

                qint64 nOffset=nDataOffset;
                qint64 nSize=nDataSize;

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_encryption_info],ui->widgetHex_encryption_info);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_function_starts)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_function_starts,ui->tableWidget_function_starts,N_mach_linkedit_data::records,g_lineEdit_mach_function_starts,N_mach_linkedit_data::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::linkedit_data_command linkedit_data=mach._read_linkedit_data_command(nDataOffset);

                g_lineEdit_mach_function_starts[N_mach_linkedit_data::dataoff]->setValue(linkedit_data.dataoff);
                g_lineEdit_mach_function_starts[N_mach_linkedit_data::datasize]->setValue(linkedit_data.datasize);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_linkedit_data_command_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_function_starts],ui->widgetHex_function_starts);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_data_in_code)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_data_in_code,ui->tableWidget_data_in_code,N_mach_linkedit_data::records,g_lineEdit_mach_data_in_code,N_mach_linkedit_data::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::linkedit_data_command linkedit_data=mach._read_linkedit_data_command(nDataOffset);

                g_lineEdit_mach_data_in_code[N_mach_linkedit_data::dataoff]->setValue(linkedit_data.dataoff);
                g_lineEdit_mach_data_in_code[N_mach_linkedit_data::datasize]->setValue(linkedit_data.datasize);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_linkedit_data_command_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_data_in_code],ui->widgetHex_data_in_code);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_code_signature)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_code_signature,ui->tableWidget_code_signature,N_mach_linkedit_data::records,g_lineEdit_mach_code_signature,N_mach_linkedit_data::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::linkedit_data_command linkedit_data=mach._read_linkedit_data_command(nDataOffset);

                g_lineEdit_mach_code_signature[N_mach_linkedit_data::dataoff]->setValue(linkedit_data.dataoff);
                g_lineEdit_mach_code_signature[N_mach_linkedit_data::datasize]->setValue(linkedit_data.datasize);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_linkedit_data_command_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_code_signature],ui->widgetHex_code_signature);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_main)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_main,ui->tableWidget_main,N_mach_main::records,g_lineEdit_mach_main,N_mach_main::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::entry_point_command entry_point=mach._read_entry_point_command(nDataOffset);

                g_lineEdit_mach_main[N_mach_main::entryoff]->setValue(entry_point.entryoff);
                g_lineEdit_mach_main[N_mach_main::stacksize]->setValue(entry_point.stacksize);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_entry_point_command_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_main],ui->widgetHex_main);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_unix_thread)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_unix_thread,ui->tableWidget_unix_thread,N_mach_unix_thread::records,g_lineEdit_mach_unix_thread,N_mach_unix_thread::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::unix_thread_command unix_thread=mach._read_unix_thread_command(nDataOffset);

                g_lineEdit_mach_unix_thread[N_mach_unix_thread::flavor]->setValue(unix_thread.flavor);
                g_lineEdit_mach_unix_thread[N_mach_unix_thread::count]->setValue(unix_thread.count);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_unix_thread_command_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_unix_thread],ui->widgetHex_unix_thread);

                blockSignals(false);
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    g_stInit.insert(sInit);
}

void MACHWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
        case SMACH::TYPE_mach_header:
            switch(nNdata)
            {
                case N_mach_header::magic:      g_lineEdit_mach_header[N_mach_header::magic]->setValue((quint32)nValue);      break;
                case N_mach_header::cputype:    g_lineEdit_mach_header[N_mach_header::cputype]->setValue((quint32)nValue);    break;
                case N_mach_header::filetype:   g_lineEdit_mach_header[N_mach_header::filetype]->setValue((quint32)nValue);   break;
                case N_mach_header::flags:      g_lineEdit_mach_header[N_mach_header::flags]->setValue((quint32)nValue);      break;
            }

            break;
    }
}

void MACHWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pCurrent, QTreeWidgetItem *pPrevious)
{
    Q_UNUSED(pPrevious)

    if(pCurrent)
    {
        reloadData();
        addPage(pCurrent);
        ui->toolButtonPrev->setEnabled(isPrevPageAvailable());
        ui->toolButtonNext->setEnabled(isNextPageAvailable());
    }
}

void MACHWidget::on_checkBoxReadonly_toggled(bool bChecked)
{
    setReadonly(bChecked);
}

void MACHWidget::on_pushButtonReload_clicked()
{
    ui->pushButtonReload->setEnabled(false);
    reload();

    QTimer::singleShot(1000,this,SLOT(enableButton()));
}

void MACHWidget::enableButton()
{
    ui->pushButtonReload->setEnabled(true);
}

void MACHWidget::on_tableWidget_mach_header_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_mach_header,ui->tableWidget_mach_header);
}

void MACHWidget::on_tableWidget_dyld_info_only_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_dyld_info_only,ui->tableWidget_dyld_info_only);
}

void MACHWidget::on_tableWidget_symtab_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_symtab,ui->tableWidget_symtab);
}

void MACHWidget::on_tableWidget_dysymtab_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_dysymtab,ui->tableWidget_dysymtab);
}

void MACHWidget::on_tableWidget_version_min_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_version_min,ui->tableWidget_version_min);
}

void MACHWidget::on_tableWidget_source_version_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_source_version,ui->tableWidget_source_version);
}

void MACHWidget::on_tableWidget_encryption_info_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_encryption_info,ui->tableWidget_encryption_info);
}

void MACHWidget::on_tableWidget_function_starts_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_function_starts,ui->tableWidget_function_starts);
}

void MACHWidget::on_tableWidget_data_in_code_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_data_in_code,ui->tableWidget_data_in_code);
}

void MACHWidget::on_tableWidget_code_signature_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_code_signature,ui->tableWidget_code_signature);
}

void MACHWidget::on_tableWidget_main_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_main,ui->tableWidget_main);
}

void MACHWidget::on_tableWidget_unix_thread_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_unix_thread,ui->tableWidget_unix_thread);
}

void MACHWidget::on_toolButtonPrev_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getPrevPage());
    setAddPageEnabled(true);
}

void MACHWidget::on_toolButtonNext_clicked()
{
    setAddPageEnabled(false);
    ui->treeWidgetNavi->setCurrentItem(getNextPage());
    setAddPageEnabled(true);
}

void MACHWidget::onTableView_commands_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(),SMACH::TYPE_mach_commands,ui->widgetHex_commands,ui->tableView_commands,&g_subDevice[SMACH::TYPE_mach_commands]);
}

void MACHWidget::onTableView_segments_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(),SMACH::TYPE_mach_segments,ui->widgetHex_segments,ui->tableView_segments,&g_subDevice[SMACH::TYPE_mach_segments]);
}

void MACHWidget::onTableView_sections_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(),SMACH::TYPE_mach_sections,ui->widgetHex_sections,ui->tableView_sections,&g_subDevice[SMACH::TYPE_mach_sections]);
}

void MACHWidget::onTableView_libraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(),SMACH::TYPE_mach_libraries,ui->widgetHex_libraries,ui->tableView_libraries,&g_subDevice[SMACH::TYPE_mach_libraries]);
}

void MACHWidget::onTableView_weak_libraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(),SMACH::TYPE_mach_weak_libraries,ui->widgetHex_weak_libraries,ui->tableView_weak_libraries,&g_subDevice[SMACH::TYPE_mach_weak_libraries]);
}

void MACHWidget::onTableView_id_library_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(),SMACH::TYPE_mach_id_library,ui->widgetHex_id_library,ui->tableView_id_library,&g_subDevice[SMACH::TYPE_mach_id_library]);
}

void MACHWidget::on_tableView_commands_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editCommandHeader();
}

void MACHWidget::on_tableView_commands_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableView_commands->currentIndex().row();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editCommandHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_commands->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_segments_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editSegmentHeader();
}

void MACHWidget::on_tableView_segments_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableView_segments->currentIndex().row();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editSegmentHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_segments->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_sections_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editSectionHeader();
}

void MACHWidget::on_tableView_sections_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableView_sections->currentIndex().row();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editSectionHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_sections->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_libraries_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editLibraryHeader();
}

void MACHWidget::on_tableView_libraries_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableView_libraries->currentIndex().row();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editLibraryHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_libraries->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_weak_libraries_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editWeakLibraryHeader();
}

void MACHWidget::on_tableView_weak_libraries_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableView_weak_libraries->currentIndex().row();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editWeakLibraryHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_weak_libraries->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_id_library_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editIdLibraryHeader();
}

void MACHWidget::on_tableView_id_library_customContextMenuRequested(const QPoint &pos)
{
    int nRow=ui->tableView_id_library->currentIndex().row();

    if(nRow!=-1)
    {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"),this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editIdLibraryHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_id_library->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::editCommandHeader()
{
    showSectionHeader(SMACH::TYPE_mach_commands,ui->tableView_commands);
}

void MACHWidget::editSegmentHeader()
{
    showSectionHeader(SMACH::TYPE_mach_segments,ui->tableView_segments);
}

void MACHWidget::editSectionHeader()
{
    showSectionHeader(SMACH::TYPE_mach_sections,ui->tableView_sections);
}

void MACHWidget::editLibraryHeader()
{
    showSectionHeader(SMACH::TYPE_mach_libraries,ui->tableView_libraries);
}

void MACHWidget::editWeakLibraryHeader()
{
    showSectionHeader(SMACH::TYPE_mach_weak_libraries,ui->tableView_weak_libraries);
}

void MACHWidget::editIdLibraryHeader()
{
    showSectionHeader(SMACH::TYPE_mach_id_library,ui->tableView_id_library);
}

void MACHWidget::showSectionHeader(int nType, QTableView *pTableView)
{
    int nRow=pTableView->currentIndex().row();

    if(nRow!=-1)
    {
        QModelIndex index=pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset=pTableView->model()->data(index,Qt::UserRole+FW_DEF::SECTION_DATA_HEADEROFFSET).toLongLong();

        MACHSectionHeaderWidget *pSectionHeaderWidget=new MACHSectionHeaderWidget(getDevice(),getOptions(),(quint32)nRow,nOffset,nType,this);

        DialogSectionHeader dsh(this);
        dsh.setWidget(pSectionHeaderWidget);
        dsh.setData(typeIdToString(nType));
        dsh.setEdited(isEdited());
        dsh.setShortcuts(getShortcuts());

        connect(&dsh,SIGNAL(editState(bool)),this,SLOT(setEdited(bool)));

        dsh.exec();

        reloadData();

        pTableView->setCurrentIndex(pTableView->model()->index(nRow,0));
    }
}

void MACHWidget::on_pushButtonHex_clicked()
{
    setTreeItem(ui->treeWidgetNavi,SMACH::TYPE_HEX);
}

void MACHWidget::on_pushButtonDisasm_clicked()
{
    setTreeItem(ui->treeWidgetNavi,SMACH::TYPE_DISASM);
}

void MACHWidget::on_pushButtonStrings_clicked()
{
    setTreeItem(ui->treeWidgetNavi,SMACH::TYPE_STRINGS);
}

void MACHWidget::on_pushButtonMemoryMap_clicked()
{
    setTreeItem(ui->treeWidgetNavi,SMACH::TYPE_MEMORYMAP);
}

void MACHWidget::on_pushButtonEntropy_clicked()
{
    setTreeItem(ui->treeWidgetNavi,SMACH::TYPE_ENTROPY);
}

void MACHWidget::on_pushButtonHeuristicScan_clicked()
{
    setTreeItem(ui->treeWidgetNavi,SMACH::TYPE_HEURISTICSCAN);
}
