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
    initToolsWidget(ui->widgetHex_code_signature);
    initToolsWidget(ui->widgetHex_data_in_code);
    initToolsWidget(ui->widgetHex_dyld_info_only);
    initToolsWidget(ui->widgetHex_dysymtab);
    initToolsWidget(ui->widgetHex_encryption_info);
    initToolsWidget(ui->widgetHex_function_starts);
    initToolsWidget(ui->widgetHex_id_library);
    initToolsWidget(ui->widgetHex_libraries);
    initToolsWidget(ui->widgetHex_main);
    initToolsWidget(ui->widgetHex_source_version);
    initToolsWidget(ui->widgetHex_StringTable);
    initToolsWidget(ui->widgetHex_symtab);
    initToolsWidget(ui->widgetHex_unix_thread);
    initToolsWidget(ui->widgetHex_unix_thread_arm_32);
    initToolsWidget(ui->widgetHex_unix_thread_arm_64);
    initToolsWidget(ui->widgetHex_unix_thread_ppc_32);
    initToolsWidget(ui->widgetHex_unix_thread_x86_32);
    initToolsWidget(ui->widgetHex_unix_thread_x86_64);
    initToolsWidget(ui->widgetHex_version_min);
    initToolsWidget(ui->widgetHex_weak_libraries);
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
    ui->widgetHex_code_signature->setShortcuts(pShortcuts);
    ui->widgetHex_data_in_code->setShortcuts(pShortcuts);
    ui->widgetHex_dyld_info_only->setShortcuts(pShortcuts);
    ui->widgetHex_dysymtab->setShortcuts(pShortcuts);
    ui->widgetHex_encryption_info->setShortcuts(pShortcuts);
    ui->widgetHex_function_starts->setShortcuts(pShortcuts);
    ui->widgetHex_id_library->setShortcuts(pShortcuts);
    ui->widgetHex_libraries->setShortcuts(pShortcuts);
    ui->widgetHex_main->setShortcuts(pShortcuts);
    ui->widgetHex_source_version->setShortcuts(pShortcuts);
    ui->widgetHex_StringTable->setShortcuts(pShortcuts);
    ui->widgetHex_symtab->setShortcuts(pShortcuts);
    ui->widgetHex_unix_thread->setShortcuts(pShortcuts);
    ui->widgetHex_unix_thread_arm_32->setShortcuts(pShortcuts);
    ui->widgetHex_unix_thread_arm_64->setShortcuts(pShortcuts);
    ui->widgetHex_unix_thread_ppc_32->setShortcuts(pShortcuts);
    ui->widgetHex_unix_thread_x86_32->setShortcuts(pShortcuts);
    ui->widgetHex_unix_thread_x86_64->setShortcuts(pShortcuts);
    ui->widgetHex_version_min->setShortcuts(pShortcuts);
    ui->widgetHex_weak_libraries->setShortcuts(pShortcuts);

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
    memset(g_lineEdit_mach_unix_thread_x86_32,0,sizeof g_lineEdit_mach_unix_thread_x86_32);
    memset(g_lineEdit_mach_unix_thread_x86_64,0,sizeof g_lineEdit_mach_unix_thread_x86_64);
    memset(g_lineEdit_mach_unix_thread_arm_32,0,sizeof g_lineEdit_mach_unix_thread_arm_32);
    memset(g_lineEdit_mach_unix_thread_arm_64,0,sizeof g_lineEdit_mach_unix_thread_arm_64);
    memset(g_lineEdit_mach_unix_thread_ppc_32,0,sizeof g_lineEdit_mach_unix_thread_ppc_32);

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
                qint64 _nOffset=mach.getCommandRecordOffset(XMACH_DEF::LC_SYMTAB,0,&listCommandRecords);

                QTreeWidgetItem *pItemSymtab=createNewItem(SMACH::TYPE_mach_symtab,QString("LC_SYMTAB"),_nOffset); // TODO rename

                pItemCommands->addChild(pItemSymtab);

                XMACH_DEF::symtab_command symtab=mach._read_symtab_command(_nOffset);

                if(mach.isOffsetValid(symtab.stroff)&&(symtab.strsize))
                {
                    QTreeWidgetItem *pItem=createNewItem(SMACH::TYPE_STRINGTABLE,tr("String table"),symtab.stroff,symtab.strsize); // TODO rename

                    pItemSymtab->addChild(pItem);
                }
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
                qint64 _nOffset=mach.getCommandRecordOffset(XMACH_DEF::LC_UNIXTHREAD,0,&listCommandRecords);

                QTreeWidgetItem *pItemUnixThread=createNewItem(SMACH::TYPE_mach_unix_thread,QString("LC_UNIXTHREAD"),_nOffset); // TODO rename

                pItemCommands->addChild(pItemUnixThread);

                XMACH_DEF::unix_thread_command unix_thread=mach._read_unix_thread_command(_nOffset);

                quint32 nMachine=mach.getHeader_cputype();

                _nOffset+=sizeof(XMACH_DEF::unix_thread_command);

                if((nMachine==XMACH_DEF::CPU_TYPE_I386)||(nMachine==XMACH_DEF::CPU_TYPE_X86_64))
                {
                    if(unix_thread.flavor==XMACH_DEF::x86_THREAD_STATE32)
                    {
                        QTreeWidgetItem *pItem=createNewItem(SMACH::TYPE_mach_unix_thread_x86_32,QString("x86_thread_state32_t"),_nOffset); // TODO rename

                        pItemUnixThread->addChild(pItem);
                    }
                    else if(unix_thread.flavor==XMACH_DEF::x86_THREAD_STATE64)
                    {
                        QTreeWidgetItem *pItem=createNewItem(SMACH::TYPE_mach_unix_thread_x86_64,QString("x86_thread_state64_t"),_nOffset); // TODO rename

                        pItemUnixThread->addChild(pItem);
                    }
                }
                else if((nMachine==XMACH_DEF::CPU_TYPE_ARM)||(nMachine==XMACH_DEF::CPU_TYPE_ARM64))
                {
                    if(unix_thread.flavor==XMACH_DEF::ARM_THREAD_STATE)
                    {
                        QTreeWidgetItem *pItem=createNewItem(SMACH::TYPE_mach_unix_thread_arm_32,QString("arm_thread_state32_t"),_nOffset); // TODO rename

                        pItemUnixThread->addChild(pItem);
                    }
                    else if(unix_thread.flavor==XMACH_DEF::ARM_THREAD_STATE64)
                    {
                        QTreeWidgetItem *pItem=createNewItem(SMACH::TYPE_mach_unix_thread_arm_64,QString("arm_thread_state64_t"),_nOffset); // TODO rename

                        pItemUnixThread->addChild(pItem);
                    }
                }
                else if(nMachine==XMACH_DEF::CPU_TYPE_POWERPC)
                {
                    QTreeWidgetItem *pItem=createNewItem(SMACH::TYPE_mach_unix_thread_ppc_32,QString("ppc_thread_state32_t"),_nOffset); // TODO rename

                    pItemUnixThread->addChild(pItem);
                }
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

                    ui->widgetHex_unix_thread->reload();

                    break;

                case SMACH::TYPE_mach_unix_thread_x86_32:
                    switch(nNdata)
                    {
                        case N_mach_unix_thread_x86_32::eax:        mach._set_x86_thread_state32_t_eax(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_32::ebx:        mach._set_x86_thread_state32_t_ebx(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_32::ecx:        mach._set_x86_thread_state32_t_ecx(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_32::edx:        mach._set_x86_thread_state32_t_edx(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_32::edi:        mach._set_x86_thread_state32_t_edi(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_32::esi:        mach._set_x86_thread_state32_t_esi(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_32::ebp:        mach._set_x86_thread_state32_t_ebp(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_32::esp:        mach._set_x86_thread_state32_t_esp(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_32::ss:         mach._set_x86_thread_state32_t_ss(nOffset,nValue);                  break;
                        case N_mach_unix_thread_x86_32::eflags:     mach._set_x86_thread_state32_t_eflags(nOffset,nValue);              break;
                        case N_mach_unix_thread_x86_32::eip:        mach._set_x86_thread_state32_t_eip(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_32::cs:         mach._set_x86_thread_state32_t_cs(nOffset,nValue);                  break;
                        case N_mach_unix_thread_x86_32::ds:         mach._set_x86_thread_state32_t_ds(nOffset,nValue);                  break;
                        case N_mach_unix_thread_x86_32::es:         mach._set_x86_thread_state32_t_es(nOffset,nValue);                  break;
                        case N_mach_unix_thread_x86_32::fs:         mach._set_x86_thread_state32_t_fs(nOffset,nValue);                  break;
                        case N_mach_unix_thread_x86_32::gs:         mach._set_x86_thread_state32_t_gs(nOffset,nValue);                  break;
                    }

                    ui->widgetHex_unix_thread_x86_32->reload();

                    break;

                case SMACH::TYPE_mach_unix_thread_x86_64:
                    switch(nNdata)
                    {
                        case N_mach_unix_thread_x86_64::rax:        mach._set_x86_thread_state64_t_rax(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::rbx:        mach._set_x86_thread_state64_t_rbx(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::rcx:        mach._set_x86_thread_state64_t_rcx(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::rdx:        mach._set_x86_thread_state64_t_rdx(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::rdi:        mach._set_x86_thread_state64_t_rdi(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::rsi:        mach._set_x86_thread_state64_t_rsi(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::rbp:        mach._set_x86_thread_state64_t_rbp(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::rsp:        mach._set_x86_thread_state64_t_rsp(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::r8:         mach._set_x86_thread_state64_t_r8(nOffset,nValue);                  break;
                        case N_mach_unix_thread_x86_64::r9:         mach._set_x86_thread_state64_t_r9(nOffset,nValue);                  break;
                        case N_mach_unix_thread_x86_64::r10:        mach._set_x86_thread_state64_t_r10(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::r11:        mach._set_x86_thread_state64_t_r11(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::r12:        mach._set_x86_thread_state64_t_r12(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::r13:        mach._set_x86_thread_state64_t_r13(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::r14:        mach._set_x86_thread_state64_t_r14(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::r15:        mach._set_x86_thread_state64_t_r15(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::rip:        mach._set_x86_thread_state64_t_rip(nOffset,nValue);                 break;
                        case N_mach_unix_thread_x86_64::rflags:     mach._set_x86_thread_state64_t_rflags(nOffset,nValue);              break;
                        case N_mach_unix_thread_x86_64::cs:         mach._set_x86_thread_state64_t_cs(nOffset,nValue);                  break;
                        case N_mach_unix_thread_x86_64::fs:         mach._set_x86_thread_state64_t_fs(nOffset,nValue);                  break;
                        case N_mach_unix_thread_x86_64::gs:         mach._set_x86_thread_state64_t_gs(nOffset,nValue);                  break;
                    }

                    ui->widgetHex_unix_thread_x86_64->reload();

                    break;

                case SMACH::TYPE_mach_unix_thread_arm_32:
                    switch(nNdata)
                    {
                        case N_mach_unix_thread_arm_32::r0:
                        case N_mach_unix_thread_arm_32::r1:
                        case N_mach_unix_thread_arm_32::r2:
                        case N_mach_unix_thread_arm_32::r3:
                        case N_mach_unix_thread_arm_32::r4:
                        case N_mach_unix_thread_arm_32::r5:
                        case N_mach_unix_thread_arm_32::r6:
                        case N_mach_unix_thread_arm_32::r7:
                        case N_mach_unix_thread_arm_32::r8:
                        case N_mach_unix_thread_arm_32::r9:
                        case N_mach_unix_thread_arm_32::r10:
                        case N_mach_unix_thread_arm_32::r11:
                        case N_mach_unix_thread_arm_32::r12:        mach._set_arm_thread_state32_t_r(nOffset,nValue,nNdata-N_mach_unix_thread_arm_32::r0);  break;
                        case N_mach_unix_thread_arm_32::sp:         mach._set_arm_thread_state32_t_sp(nOffset,nValue);                  break;
                        case N_mach_unix_thread_arm_32::lr:         mach._set_arm_thread_state32_t_lr(nOffset,nValue);                  break;
                        case N_mach_unix_thread_arm_32::pc:         mach._set_arm_thread_state32_t_pc(nOffset,nValue);                  break;
                        case N_mach_unix_thread_arm_32::cpsr:       mach._set_arm_thread_state32_t_cpsr(nOffset,nValue);                break;
                    }

                    ui->widgetHex_unix_thread_arm_32->reload();

                    break;

                case SMACH::TYPE_mach_unix_thread_arm_64:
                    switch(nNdata)
                    {
                        case N_mach_unix_thread_arm_64::x0:
                        case N_mach_unix_thread_arm_64::x1:
                        case N_mach_unix_thread_arm_64::x2:
                        case N_mach_unix_thread_arm_64::x3:
                        case N_mach_unix_thread_arm_64::x4:
                        case N_mach_unix_thread_arm_64::x5:
                        case N_mach_unix_thread_arm_64::x6:
                        case N_mach_unix_thread_arm_64::x7:
                        case N_mach_unix_thread_arm_64::x8:
                        case N_mach_unix_thread_arm_64::x9:
                        case N_mach_unix_thread_arm_64::x10:
                        case N_mach_unix_thread_arm_64::x11:
                        case N_mach_unix_thread_arm_64::x12:
                        case N_mach_unix_thread_arm_64::x13:
                        case N_mach_unix_thread_arm_64::x14:
                        case N_mach_unix_thread_arm_64::x15:
                        case N_mach_unix_thread_arm_64::x16:
                        case N_mach_unix_thread_arm_64::x17:
                        case N_mach_unix_thread_arm_64::x18:
                        case N_mach_unix_thread_arm_64::x19:
                        case N_mach_unix_thread_arm_64::x20:
                        case N_mach_unix_thread_arm_64::x21:
                        case N_mach_unix_thread_arm_64::x22:
                        case N_mach_unix_thread_arm_64::x23:
                        case N_mach_unix_thread_arm_64::x24:
                        case N_mach_unix_thread_arm_64::x25:
                        case N_mach_unix_thread_arm_64::x26:
                        case N_mach_unix_thread_arm_64::x27:
                        case N_mach_unix_thread_arm_64::x28:        mach._set_arm_thread_state64_t_x(nOffset,nValue,nNdata-N_mach_unix_thread_arm_64::x0);  break;
                        case N_mach_unix_thread_arm_64::fp:         mach._set_arm_thread_state64_t_fp(nOffset,nValue);                  break;
                        case N_mach_unix_thread_arm_64::lr:         mach._set_arm_thread_state64_t_lr(nOffset,nValue);                  break;
                        case N_mach_unix_thread_arm_64::sp:         mach._set_arm_thread_state64_t_sp(nOffset,nValue);                  break;
                        case N_mach_unix_thread_arm_64::pc:         mach._set_arm_thread_state64_t_pc(nOffset,nValue);                  break;
                        case N_mach_unix_thread_arm_64::cpsr:       mach._set_arm_thread_state64_t_cpsr(nOffset,nValue);                break;
                        case N_mach_unix_thread_arm_64::pad:        mach._set_arm_thread_state64_t_pad(nOffset,nValue);                 break;
                    }

                    ui->widgetHex_unix_thread_arm_64->reload();

                    break;

                case SMACH::TYPE_mach_unix_thread_ppc_32:
                    switch(nNdata)
                    {
                        case N_mach_unix_thread_ppc_32::srr0:       mach._set_ppc_thread_state32_t_srr0(nOffset,nValue);                break;
                        case N_mach_unix_thread_ppc_32::srr1:       mach._set_ppc_thread_state32_t_srr1(nOffset,nValue);                break;
                        case N_mach_unix_thread_ppc_32::r0:
                        case N_mach_unix_thread_ppc_32::r1:
                        case N_mach_unix_thread_ppc_32::r2:
                        case N_mach_unix_thread_ppc_32::r3:
                        case N_mach_unix_thread_ppc_32::r4:
                        case N_mach_unix_thread_ppc_32::r5:
                        case N_mach_unix_thread_ppc_32::r6:
                        case N_mach_unix_thread_ppc_32::r7:
                        case N_mach_unix_thread_ppc_32::r8:
                        case N_mach_unix_thread_ppc_32::r9:
                        case N_mach_unix_thread_ppc_32::r10:
                        case N_mach_unix_thread_ppc_32::r11:
                        case N_mach_unix_thread_ppc_32::r12:
                        case N_mach_unix_thread_ppc_32::r13:
                        case N_mach_unix_thread_ppc_32::r14:
                        case N_mach_unix_thread_ppc_32::r15:
                        case N_mach_unix_thread_ppc_32::r16:
                        case N_mach_unix_thread_ppc_32::r17:
                        case N_mach_unix_thread_ppc_32::r18:
                        case N_mach_unix_thread_ppc_32::r19:
                        case N_mach_unix_thread_ppc_32::r20:
                        case N_mach_unix_thread_ppc_32::r21:
                        case N_mach_unix_thread_ppc_32::r22:
                        case N_mach_unix_thread_ppc_32::r23:
                        case N_mach_unix_thread_ppc_32::r24:
                        case N_mach_unix_thread_ppc_32::r25:
                        case N_mach_unix_thread_ppc_32::r26:
                        case N_mach_unix_thread_ppc_32::r27:
                        case N_mach_unix_thread_ppc_32::r28:
                        case N_mach_unix_thread_ppc_32::r29:
                        case N_mach_unix_thread_ppc_32::r30:
                        case N_mach_unix_thread_ppc_32::r31:            mach._set_ppc_thread_state32_t_r(nOffset,nValue,nNdata-N_mach_unix_thread_ppc_32::r0);  break;
                        case N_mach_unix_thread_ppc_32::ct:             mach._set_ppc_thread_state32_t_ct(nOffset,nValue);                  break;
                        case N_mach_unix_thread_ppc_32::xer:            mach._set_ppc_thread_state32_t_xer(nOffset,nValue);                 break;
                        case N_mach_unix_thread_ppc_32::lr:             mach._set_ppc_thread_state32_t_lr(nOffset,nValue);                  break;
                        case N_mach_unix_thread_ppc_32::ctr:            mach._set_ppc_thread_state32_t_ctr(nOffset,nValue);                 break;
                        case N_mach_unix_thread_ppc_32::mq:             mach._set_ppc_thread_state32_t_mq(nOffset,nValue);                  break;
                        case N_mach_unix_thread_ppc_32::vrsave:         mach._set_ppc_thread_state32_t_vrsave(nOffset,nValue);              break;
                    }

                    ui->widgetHex_unix_thread_ppc_32->reload();

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
    setLineEditsReadOnly(g_lineEdit_mach_unix_thread_x86_32,N_mach_unix_thread_x86_32::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_unix_thread_x86_64,N_mach_unix_thread_x86_64::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_unix_thread_arm_32,N_mach_unix_thread_arm_32::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_unix_thread_arm_64,N_mach_unix_thread_arm_64::__data_size,bState);
    setLineEditsReadOnly(g_lineEdit_mach_unix_thread_ppc_32,N_mach_unix_thread_ppc_32::__data_size,bState);
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
    _blockSignals((QObject **)g_lineEdit_mach_unix_thread_x86_32,N_mach_unix_thread_x86_32::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_unix_thread_x86_64,N_mach_unix_thread_x86_64::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_unix_thread_arm_32,N_mach_unix_thread_arm_32::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_unix_thread_arm_64,N_mach_unix_thread_arm_64::__data_size,bState);
    _blockSignals((QObject **)g_lineEdit_mach_unix_thread_ppc_32,N_mach_unix_thread_ppc_32::__data_size,bState);
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
        else if(nType==SMACH::TYPE_mach_unix_thread_x86_32)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_unix_thread_x86_32,ui->tableWidget_unix_thread_x86_32,N_mach_unix_thread_x86_32::records,g_lineEdit_mach_unix_thread_x86_32,N_mach_unix_thread_x86_32::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::x86_thread_state32_t state=mach._read_x86_thread_state32_t(nDataOffset);

                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::eax]->setValue(state.eax);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::ebx]->setValue(state.ebx);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::ecx]->setValue(state.ecx);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::edx]->setValue(state.edx);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::edi]->setValue(state.edi);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::esi]->setValue(state.esi);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::ebp]->setValue(state.ebp);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::esp]->setValue(state.esp);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::ss]->setValue(state.ss);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::eflags]->setValue(state.eflags);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::eip]->setValue(state.eip);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::cs]->setValue(state.cs);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::ds]->setValue(state.ds);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::es]->setValue(state.es);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::fs]->setValue(state.fs);
                g_lineEdit_mach_unix_thread_x86_32[N_mach_unix_thread_x86_32::gs]->setValue(state.gs);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_x86_thread_state32_t_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_unix_thread_x86_32],ui->widgetHex_unix_thread_x86_32);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_unix_thread_x86_64)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_unix_thread_x86_64,ui->tableWidget_unix_thread_x86_64,N_mach_unix_thread_x86_64::records,g_lineEdit_mach_unix_thread_x86_64,N_mach_unix_thread_x86_64::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::x86_thread_state64_t state=mach._read_x86_thread_state64_t(nDataOffset);

                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::rax]->setValue(state.rax);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::rbx]->setValue(state.rbx);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::rcx]->setValue(state.rcx);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::rdx]->setValue(state.rdx);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::rdi]->setValue(state.rdi);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::rsi]->setValue(state.rsi);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::rbp]->setValue(state.rbp);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::rsp]->setValue(state.rsp);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::r8]->setValue(state.r8);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::r9]->setValue(state.r9);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::r10]->setValue(state.r10);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::r11]->setValue(state.r11);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::r12]->setValue(state.r12);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::r13]->setValue(state.r13);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::r14]->setValue(state.r14);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::r15]->setValue(state.r15);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::rip]->setValue(state.rip);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::rflags]->setValue(state.rflags);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::cs]->setValue(state.cs);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::fs]->setValue(state.fs);
                g_lineEdit_mach_unix_thread_x86_64[N_mach_unix_thread_x86_64::gs]->setValue(state.gs);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_x86_thread_state64_t_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_unix_thread_x86_64],ui->widgetHex_unix_thread_x86_64);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_unix_thread_arm_32)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_unix_thread_arm_32,ui->tableWidget_unix_thread_arm_32,N_mach_unix_thread_arm_32::records,g_lineEdit_mach_unix_thread_arm_32,N_mach_unix_thread_arm_32::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::arm_thread_state32_t state=mach._read_arm_thread_state32_t(nDataOffset);

                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::r0]->setValue(state.r[0]);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::r1]->setValue(state.r[1]);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::r2]->setValue(state.r[2]);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::r3]->setValue(state.r[3]);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::r4]->setValue(state.r[4]);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::r5]->setValue(state.r[5]);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::r6]->setValue(state.r[6]);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::r7]->setValue(state.r[7]);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::r8]->setValue(state.r[8]);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::r9]->setValue(state.r[9]);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::r10]->setValue(state.r[10]);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::r11]->setValue(state.r[11]);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::r12]->setValue(state.r[12]);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::sp]->setValue(state.sp);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::lr]->setValue(state.lr);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::pc]->setValue(state.pc);
                g_lineEdit_mach_unix_thread_arm_32[N_mach_unix_thread_arm_32::cpsr]->setValue(state.cpsr);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_arm_thread_state32_t_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_unix_thread_arm_32],ui->widgetHex_unix_thread_arm_32);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_unix_thread_arm_64)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_unix_thread_arm_64,ui->tableWidget_unix_thread_arm_64,N_mach_unix_thread_arm_64::records,g_lineEdit_mach_unix_thread_arm_64,N_mach_unix_thread_arm_64::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::arm_thread_state64_t state=mach._read_arm_thread_state64_t(nDataOffset);

                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x0]->setValue(state.x[0]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x1]->setValue(state.x[1]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x2]->setValue(state.x[2]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x3]->setValue(state.x[3]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x4]->setValue(state.x[4]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x5]->setValue(state.x[5]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x6]->setValue(state.x[6]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x7]->setValue(state.x[7]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x8]->setValue(state.x[8]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x9]->setValue(state.x[9]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x10]->setValue(state.x[10]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x11]->setValue(state.x[11]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x12]->setValue(state.x[12]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x13]->setValue(state.x[13]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x14]->setValue(state.x[14]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x15]->setValue(state.x[15]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x16]->setValue(state.x[16]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x17]->setValue(state.x[17]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x18]->setValue(state.x[18]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x19]->setValue(state.x[19]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x20]->setValue(state.x[20]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x21]->setValue(state.x[21]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x22]->setValue(state.x[22]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x23]->setValue(state.x[23]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x24]->setValue(state.x[24]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x25]->setValue(state.x[25]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x26]->setValue(state.x[26]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x27]->setValue(state.x[27]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::x28]->setValue(state.x[28]);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::fp]->setValue(state.fp);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::lr]->setValue(state.lr);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::sp]->setValue(state.sp);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::pc]->setValue(state.pc);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::cpsr]->setValue(state.cpsr);
                g_lineEdit_mach_unix_thread_arm_64[N_mach_unix_thread_arm_64::pad]->setValue(state.pad);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_arm_thread_state64_t_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_unix_thread_arm_64],ui->widgetHex_unix_thread_arm_64);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_mach_unix_thread_ppc_32)
        {
            if(!g_stInit.contains(sInit))
            {
                createHeaderTable(SMACH::TYPE_mach_unix_thread_ppc_32,ui->tableWidget_unix_thread_ppc_32,N_mach_unix_thread_ppc_32::records,g_lineEdit_mach_unix_thread_ppc_32,N_mach_unix_thread_ppc_32::__data_size,0,nDataOffset);

                blockSignals(true);

                XMACH_DEF::ppc_thread_state32_t state=mach._read_ppc_thread_state32_t(nDataOffset);

                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::srr0]->setValue(state.srr0);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::srr1]->setValue(state.srr1);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r0]->setValue(state.r[0]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r1]->setValue(state.r[1]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r2]->setValue(state.r[2]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r3]->setValue(state.r[3]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r4]->setValue(state.r[4]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r5]->setValue(state.r[5]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r6]->setValue(state.r[6]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r7]->setValue(state.r[7]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r8]->setValue(state.r[8]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r9]->setValue(state.r[9]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r10]->setValue(state.r[10]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r11]->setValue(state.r[11]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r12]->setValue(state.r[12]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r13]->setValue(state.r[13]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r14]->setValue(state.r[14]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r15]->setValue(state.r[15]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r16]->setValue(state.r[16]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r17]->setValue(state.r[17]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r18]->setValue(state.r[18]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r19]->setValue(state.r[19]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r20]->setValue(state.r[20]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r21]->setValue(state.r[21]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r22]->setValue(state.r[22]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r23]->setValue(state.r[23]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r24]->setValue(state.r[24]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r25]->setValue(state.r[25]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r26]->setValue(state.r[26]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r27]->setValue(state.r[27]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r28]->setValue(state.r[28]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r29]->setValue(state.r[29]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r30]->setValue(state.r[30]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::r31]->setValue(state.r[31]);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::ct]->setValue(state.ct);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::xer]->setValue(state.xer);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::lr]->setValue(state.lr);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::ctr]->setValue(state.ctr);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::mq]->setValue(state.mq);
                g_lineEdit_mach_unix_thread_ppc_32[N_mach_unix_thread_ppc_32::vrsave]->setValue(state.vrsave);

                qint64 nOffset=nDataOffset;
                qint64 nSize=mach.get_ppc_thread_state32_t_size();

                loadHexSubdevice(nOffset,nSize,nOffset,&g_subDevice[SMACH::TYPE_mach_unix_thread_arm_32],ui->widgetHex_unix_thread_arm_32);

                blockSignals(false);
            }
        }
        else if(nType==SMACH::TYPE_STRINGTABLE)
        {
            if(!g_stInit.contains(sInit))
            {
                loadHexSubdevice(nDataOffset,nDataSize,0,&g_subDevice[SMACH::TYPE_STRINGTABLE],ui->widgetHex_StringTable);
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

void MACHWidget::on_tableWidget_unix_thread_x86_32_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_unix_thread_x86_32,ui->tableWidget_unix_thread_x86_32);
}

void MACHWidget::on_tableWidget_unix_thread_x86_64_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_unix_thread_x86_64,ui->tableWidget_unix_thread_x86_64);
}

void MACHWidget::on_tableWidget_unix_thread_arm_32_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_unix_thread_arm_32,ui->tableWidget_unix_thread_arm_32);
}

void MACHWidget::on_tableWidget_unix_thread_arm_64_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_unix_thread_arm_64,ui->tableWidget_unix_thread_arm_64);
}

void MACHWidget::on_tableWidget_unix_thread_ppc_32_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_unix_thread_ppc_32,ui->tableWidget_unix_thread_ppc_32);
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
