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
#include "machwidget.h"

#include "ui_machwidget.h"

MACHWidget::MACHWidget(QWidget *pParent) : FormatWidget(pParent), ui(new Ui::MACHWidget)
{
    ui->setupUi(this);

    memset(g_subDevice, 0, sizeof g_subDevice);

    initWidget();
}

MACHWidget::MACHWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, QWidget *pParent) : MACHWidget(pParent)
{
    MACHWidget::setData(pDevice, options, 0, 0, 0);
    MACHWidget::reload();
}

MACHWidget::~MACHWidget()
{
    delete ui;
}

void MACHWidget::clear()
{
    MACHWidget::reset();

    memset(g_lineEdit_mach_header, 0, sizeof g_lineEdit_mach_header);
    memset(g_lineEdit_mach_dyld_info_only, 0, sizeof g_lineEdit_mach_dyld_info_only);
    memset(g_lineEdit_mach_uuid, 0, sizeof g_lineEdit_mach_uuid);
    memset(g_lineEdit_mach_dylinker, 0, sizeof g_lineEdit_mach_dylinker);
    memset(g_lineEdit_mach_rpath, 0, sizeof g_lineEdit_mach_rpath);
    memset(g_lineEdit_mach_symtab, 0, sizeof g_lineEdit_mach_symtab);
    memset(g_lineEdit_mach_dysymtab, 0, sizeof g_lineEdit_mach_dysymtab);
    memset(g_lineEdit_mach_version_min, 0, sizeof g_lineEdit_mach_version_min);
    memset(g_lineEdit_mach_build_version, 0, sizeof g_lineEdit_mach_build_version);
    memset(g_lineEdit_mach_source_version, 0, sizeof g_lineEdit_mach_source_version);
    memset(g_lineEdit_mach_encryption_info, 0, sizeof g_lineEdit_mach_encryption_info);
    memset(g_lineEdit_mach_function_starts, 0, sizeof g_lineEdit_mach_function_starts);
    memset(g_lineEdit_mach_data_in_code, 0, sizeof g_lineEdit_mach_data_in_code);
    memset(g_lineEdit_mach_code_signature, 0, sizeof g_lineEdit_mach_code_signature);
    memset(g_lineEdit_mach_SuperBlob, 0, sizeof g_lineEdit_mach_SuperBlob);
    memset(g_lineEdit_mach_main, 0, sizeof g_lineEdit_mach_main);
    memset(g_lineEdit_mach_unix_thread, 0, sizeof g_lineEdit_mach_unix_thread);
    memset(g_lineEdit_mach_unix_thread_x86_32, 0, sizeof g_lineEdit_mach_unix_thread_x86_32);
    memset(g_lineEdit_mach_unix_thread_x86_64, 0, sizeof g_lineEdit_mach_unix_thread_x86_64);
    memset(g_lineEdit_mach_unix_thread_arm_32, 0, sizeof g_lineEdit_mach_unix_thread_arm_32);
    memset(g_lineEdit_mach_unix_thread_arm_64, 0, sizeof g_lineEdit_mach_unix_thread_arm_64);
    memset(g_lineEdit_mach_unix_thread_m68k_32, 0, sizeof g_lineEdit_mach_unix_thread_m68k_32);
    memset(g_lineEdit_mach_unix_thread_ppc_32, 0, sizeof g_lineEdit_mach_unix_thread_ppc_32);

    memset(g_comboBox, 0, sizeof g_comboBox);
    memset(g_invWidget, 0, sizeof g_invWidget);

    _deleteSubdevices(g_subDevice, (sizeof g_subDevice) / (sizeof(SubDevice *)));

    resetWidget();

    ui->checkBoxReadonly->setChecked(true);

    ui->treeWidgetNavi->clear();
}

void MACHWidget::cleanup()
{
    MACHWidget::clear();
}

void MACHWidget::reload()
{
    MACHWidget::clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    XMACH mach(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (mach.isValid()) {
        setFileType(mach.getFileType());

        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_INFO, tr("Info")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_VIRUSTOTAL, "VirusTotal"));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_HEX, tr("Hex")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_DISASM, tr("Disasm")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_HASH, tr("Hash")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_STRINGS, tr("Strings")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_SIGNATURES, tr("Signatures")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_MEMORYMAP, tr("Memory map")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_ENTROPY, tr("Entropy")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_HEURISTICSCAN, tr("Heuristic scan")));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_mach_header, mach.is64() ? ("mach_header_64") : ("mach_header")));

        QList<XMACH::COMMAND_RECORD> listCommandRecords = mach.getCommandRecords();

        if (listCommandRecords.count()) {
            QTreeWidgetItem *pItemCommands = createNewItem(SMACH::TYPE_mach_commands, tr("Commands"));

            ui->treeWidgetNavi->addTopLevelItem(pItemCommands);

            QList<XMACH::SEGMENT_RECORD> listSegmentRecords = mach.getSegmentRecords(&listCommandRecords);

            if (listSegmentRecords.count()) {
                QTreeWidgetItem *pItemSegments = createNewItem(SMACH::TYPE_mach_segments, tr("Segments"));  // TODO rename

                pItemCommands->addChild(pItemSegments);

                QList<XMACH::SECTION_RECORD> listSectionRecords = mach.getSectionRecords(&listCommandRecords);

                if (listSectionRecords.count()) {
                    QTreeWidgetItem *pItemSections = createNewItem(SMACH::TYPE_mach_sections,
                                                                   tr("Sections"));  // TODO rename

                    pItemSegments->addChild(pItemSections);
                }
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_ID_DYLIB, &listCommandRecords)) {
                QTreeWidgetItem *pItemIdDylib = createNewItem(SMACH::TYPE_mach_id_library, QString("LC_ID_DYLIB"));

                pItemCommands->addChild(pItemIdDylib);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_LOAD_DYLIB, &listCommandRecords)) {
                QTreeWidgetItem *pItemLoadDylib = createNewItem(SMACH::TYPE_mach_libraries, QString("LC_LOAD_DYLIB"));

                pItemCommands->addChild(pItemLoadDylib);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_LOADFVMLIB, &listCommandRecords)) {
                QTreeWidgetItem *pLoadFVMLib = createNewItem(SMACH::TYPE_mach_LOADFVMLIB, QString("LC_LOADFVMLIB"));

                pItemCommands->addChild(pLoadFVMLib);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_IDFVMLIB, &listCommandRecords)) {
                QTreeWidgetItem *pIdFMVLib = createNewItem(SMACH::TYPE_mach_IDFVMLIB, QString("LC_IDFVMLIB"));

                pItemCommands->addChild(pIdFMVLib);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_LOAD_WEAK_DYLIB, &listCommandRecords)) {
                QTreeWidgetItem *pItemLibraries = createNewItem(SMACH::TYPE_mach_weak_libraries, QString("LC_LOAD_WEAK_DYLIB"));

                pItemCommands->addChild(pItemLibraries);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_DYLD_INFO_ONLY, &listCommandRecords)) {
                qint64 _nOffset = mach.getCommandRecordOffset(XMACH_DEF::S_LC_DYLD_INFO_ONLY, 0, &listCommandRecords);

                QTreeWidgetItem *pItemDyldInfo = createNewItem(SMACH::TYPE_mach_dyld_info_only, QString("LC_DYLD_INFO_ONLY"), _nOffset);  // TODO rename

                pItemCommands->addChild(pItemDyldInfo);

                XMACH_DEF::dyld_info_command dyld_info = mach._read_dyld_info_command(_nOffset);

                if (mach.isOffsetValid(dyld_info.rebase_off) && (dyld_info.rebase_size)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_DYLD_INFO_rebase, tr("Rebase"), dyld_info.rebase_off,
                                                           dyld_info.rebase_size);  // TODO rename

                    pItemDyldInfo->addChild(pItem);
                }

                if (mach.isOffsetValid(dyld_info.bind_off) && (dyld_info.bind_size)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_DYLD_INFO_bind, tr("Binding"), dyld_info.bind_off,
                                                           dyld_info.bind_size);  // TODO rename

                    pItemDyldInfo->addChild(pItem);
                }

                if (mach.isOffsetValid(dyld_info.weak_bind_off) && (dyld_info.weak_bind_size)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_DYLD_INFO_weak_bind, tr("Weak binding"), dyld_info.weak_bind_off,
                                                           dyld_info.weak_bind_size);  // TODO rename

                    pItemDyldInfo->addChild(pItem);
                }

                if (mach.isOffsetValid(dyld_info.lazy_bind_off) && (dyld_info.lazy_bind_size)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_DYLD_INFO_lazy_bind, tr("Lazy binding"), dyld_info.lazy_bind_off,
                                                           dyld_info.lazy_bind_size);  // TODO rename

                    pItemDyldInfo->addChild(pItem);
                }

                if (mach.isOffsetValid(dyld_info.export_off) && (dyld_info.export_size)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_DYLD_INFO_export, tr("Export"), dyld_info.export_off,
                                                           dyld_info.export_size);  // TODO rename

                    pItemDyldInfo->addChild(pItem);
                }
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_UUID, &listCommandRecords)) {
                QTreeWidgetItem *pItemUuid = createNewItem(SMACH::TYPE_mach_uuid, QString("LC_UUID"),
                                                           mach.getCommandRecordOffset(XMACH_DEF::S_LC_UUID, 0,
                                                                                       &listCommandRecords));  // TODO rename

                pItemCommands->addChild(pItemUuid);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_SYMTAB, &listCommandRecords)) {
                qint64 _nOffset = mach.getCommandRecordOffset(XMACH_DEF::S_LC_SYMTAB, 0, &listCommandRecords);

                QTreeWidgetItem *pItemSymtab = createNewItem(SMACH::TYPE_mach_symtab, QString("LC_SYMTAB"),
                                                             _nOffset);  // TODO rename

                pItemCommands->addChild(pItemSymtab);

                XMACH_DEF::symtab_command symtab = mach._read_symtab_command(_nOffset);

                if (mach.isOffsetValid(symtab.stroff) && (symtab.strsize)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_STRINGTABLE, tr("String table"), symtab.stroff, symtab.strsize);  // TODO rename

                    pItemSymtab->addChild(pItem);
                }

                if (mach.isOffsetValid(symtab.symoff) && (symtab.nsyms)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_SYMBOLTABLE, tr("Symbol table"), symtab.symoff, symtab.nsyms);  // TODO rename

                    pItemSymtab->addChild(pItem);
                }
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_DYSYMTAB, &listCommandRecords)) {
                qint64 _nOffset = mach.getCommandRecordOffset(XMACH_DEF::S_LC_DYSYMTAB, 0, &listCommandRecords);

                QTreeWidgetItem *pItemDysymtab = createNewItem(SMACH::TYPE_mach_dysymtab, QString("LC_DYSYMTAB"),
                                                               _nOffset);  // TODO rename

                pItemCommands->addChild(pItemDysymtab);

                XMACH_DEF::dysymtab_command dysymtab = mach._read_dysymtab_command(_nOffset);

                if (mach.isOffsetValid(dysymtab.tocoff) && (dysymtab.ntoc)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_DYSYMTAB_toc, tr("Table of contents"), dysymtab.tocoff, dysymtab.ntoc);  // TODO rename

                    pItemDysymtab->addChild(pItem);
                }

                if (mach.isOffsetValid(dysymtab.modtaboff) && (dysymtab.nmodtab)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_DYSYMTAB_modtab, tr("Modules"), dysymtab.modtaboff, dysymtab.nmodtab);  // TODO rename

                    pItemDysymtab->addChild(pItem);
                }

                if (mach.isOffsetValid(dysymtab.extrefsymoff) && (dysymtab.nextrefsyms)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_DYSYMTAB_extrefsyms, tr("External references"), dysymtab.extrefsymoff,
                                                           dysymtab.nextrefsyms);  // TODO rename

                    pItemDysymtab->addChild(pItem);
                }

                if (mach.isOffsetValid(dysymtab.indirectsymoff) && (dysymtab.nindirectsyms)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_DYSYMTAB_indirectsyms, tr("Indirect symbols"), dysymtab.indirectsymoff,
                                                           dysymtab.nindirectsyms);  // TODO rename

                    pItemDysymtab->addChild(pItem);
                }

                if (mach.isOffsetValid(dysymtab.extreloff) && (dysymtab.nextrel)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_DYSYMTAB_extrel, QString("External relocation"), dysymtab.extreloff,
                                                           dysymtab.nextrel);  // TODO rename

                    pItemDysymtab->addChild(pItem);
                }

                if (mach.isOffsetValid(dysymtab.locreloff) && (dysymtab.nlocrel)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_DYSYMTAB_locrel, tr("Local relocation"), dysymtab.locreloff, dysymtab.nlocrel);  // TODO rename

                    pItemDysymtab->addChild(pItem);
                }
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_VERSION_MIN_MACOSX, &listCommandRecords)) {
                QTreeWidgetItem *pItemVersionMin = createNewItem(SMACH::TYPE_mach_version_min, QString("LC_VERSION_MIN_MACOSX"),
                                                                 mach.getCommandRecordOffset(XMACH_DEF::S_LC_VERSION_MIN_MACOSX, 0,
                                                                                             &listCommandRecords));  // TODO rename

                pItemCommands->addChild(pItemVersionMin);
            } else if (mach.isCommandPresent(XMACH_DEF::S_LC_VERSION_MIN_IPHONEOS, &listCommandRecords)) {
                QTreeWidgetItem *pItemVersionMin = createNewItem(SMACH::TYPE_mach_version_min, QString("LC_VERSION_MIN_IPHONEOS"),
                                                                 mach.getCommandRecordOffset(XMACH_DEF::S_LC_VERSION_MIN_IPHONEOS, 0,
                                                                                             &listCommandRecords));  // TODO rename

                pItemCommands->addChild(pItemVersionMin);
            } else if (mach.isCommandPresent(XMACH_DEF::S_LC_VERSION_MIN_TVOS, &listCommandRecords)) {
                QTreeWidgetItem *pItemVersionMin = createNewItem(SMACH::TYPE_mach_version_min, QString("LC_VERSION_MIN_TVOS"),
                                                                 mach.getCommandRecordOffset(XMACH_DEF::S_LC_VERSION_MIN_TVOS, 0,
                                                                                             &listCommandRecords));  // TODO rename

                pItemCommands->addChild(pItemVersionMin);
            } else if (mach.isCommandPresent(XMACH_DEF::S_LC_VERSION_MIN_WATCHOS, &listCommandRecords)) {
                QTreeWidgetItem *pItemVersionMin = createNewItem(SMACH::TYPE_mach_version_min, QString("LC_VERSION_MIN_WATCHOS"),
                                                                 mach.getCommandRecordOffset(XMACH_DEF::S_LC_VERSION_MIN_WATCHOS, 0,
                                                                                             &listCommandRecords));  // TODO rename

                pItemCommands->addChild(pItemVersionMin);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_BUILD_VERSION, &listCommandRecords)) {
                QTreeWidgetItem *pItemBuildVersion = createNewItem(SMACH::TYPE_mach_build_version, QString("LC_BUILD_VERSION"),
                                                                   mach.getCommandRecordOffset(XMACH_DEF::S_LC_BUILD_VERSION, 0,
                                                                                               &listCommandRecords));  // TODO rename

                pItemCommands->addChild(pItemBuildVersion);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_LOAD_DYLINKER, &listCommandRecords)) {
                QTreeWidgetItem *pItemDylinker = createNewItem(SMACH::TYPE_mach_dylinker, QString("LC_LOAD_DYLINKER"),
                                                               mach.getCommandRecordOffset(XMACH_DEF::S_LC_LOAD_DYLINKER, 0,
                                                                                           &listCommandRecords));  // TODO rename

                pItemCommands->addChild(pItemDylinker);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_RPATH, &listCommandRecords)) {
                QTreeWidgetItem *pItemRPath = createNewItem(SMACH::TYPE_mach_rpath, QString("LC_RPATH"),
                                                            mach.getCommandRecordOffset(XMACH_DEF::S_LC_RPATH, 0,
                                                                                        &listCommandRecords));  // TODO rename

                pItemCommands->addChild(pItemRPath);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_SOURCE_VERSION, &listCommandRecords)) {
                QTreeWidgetItem *pItemSourceVersion = createNewItem(SMACH::TYPE_mach_source_version, QString("LC_SOURCE_VERSION"),
                                                                    mach.getCommandRecordOffset(XMACH_DEF::S_LC_SOURCE_VERSION, 0,
                                                                                                &listCommandRecords));  // TODO rename

                pItemCommands->addChild(pItemSourceVersion);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_ENCRYPTION_INFO, &listCommandRecords)) {
                QTreeWidgetItem *pItemEncryptionInfo = createNewItem(SMACH::TYPE_mach_encryption_info, QString("LC_ENCRYPTION_INFO"),
                                                                     mach.getCommandRecordOffset(XMACH_DEF::S_LC_ENCRYPTION_INFO, 0, &listCommandRecords),
                                                                     mach.get_encryption_info_command_size());  // TODO rename

                pItemCommands->addChild(pItemEncryptionInfo);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_ENCRYPTION_INFO_64, &listCommandRecords)) {
                QTreeWidgetItem *pItemEncryptionInfo = createNewItem(SMACH::TYPE_mach_encryption_info, QString("LC_ENCRYPTION_INFO_64"),
                                                                     mach.getCommandRecordOffset(XMACH_DEF::S_LC_ENCRYPTION_INFO_64, 0, &listCommandRecords),
                                                                     mach.get_encryption_info_command_64_size());  // TODO rename

                pItemCommands->addChild(pItemEncryptionInfo);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_FUNCTION_STARTS, &listCommandRecords)) {
                qint64 _nOffset = mach.getCommandRecordOffset(XMACH_DEF::S_LC_FUNCTION_STARTS, 0, &listCommandRecords);

                QTreeWidgetItem *pItemFunctionStarts = createNewItem(SMACH::TYPE_mach_function_starts, QString("LC_FUNCTION_STARTS"), _nOffset);  // TODO rename

                pItemCommands->addChild(pItemFunctionStarts);

                XMACH_DEF::linkedit_data_command linkedit_data = mach._read_linkedit_data_command(_nOffset);

                if (mach.isOffsetValid(linkedit_data.dataoff) && (linkedit_data.datasize)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_FUNCTIONS, tr("Functions"), linkedit_data.dataoff,
                                                           linkedit_data.datasize);  // TODO rename

                    pItemFunctionStarts->addChild(pItem);
                }
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_DATA_IN_CODE, &listCommandRecords)) {
                qint64 _nOffset = mach.getCommandRecordOffset(XMACH_DEF::S_LC_DATA_IN_CODE, 0, &listCommandRecords);

                QTreeWidgetItem *pItemDataInCode = createNewItem(SMACH::TYPE_mach_data_in_code, QString("LC_DATA_IN_CODE"),
                                                                 _nOffset);  // TODO rename

                pItemCommands->addChild(pItemDataInCode);

                XMACH_DEF::linkedit_data_command linkedit_data = mach._read_linkedit_data_command(_nOffset);

                if (mach.isOffsetValid(linkedit_data.dataoff) && (linkedit_data.datasize)) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_DICE, tr("Data in code"), linkedit_data.dataoff,
                                                           linkedit_data.datasize);  // TODO rename

                    pItemDataInCode->addChild(pItem);
                }
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_CODE_SIGNATURE, &listCommandRecords)) {
                qint64 _nOffset = mach.getCommandRecordOffset(XMACH_DEF::S_LC_CODE_SIGNATURE, 0, &listCommandRecords);

                QTreeWidgetItem *pItemCodeSignature = createNewItem(SMACH::TYPE_mach_code_signature, QString("LC_CODE_SIGNATURE"), _nOffset);  // TODO rename

                pItemCommands->addChild(pItemCodeSignature);

                XMACH_DEF::linkedit_data_command linkedit_data = mach._read_linkedit_data_command(_nOffset);

                if (mach.isOffsetValid(linkedit_data.dataoff) && (linkedit_data.datasize)) {
                    QTreeWidgetItem *pItemSuperBlob = createNewItem(SMACH::TYPE_mach_SuperBlob, QString("__SC_SuperBlob"),
                                                                    linkedit_data.dataoff);  // TODO rename

                    pItemCodeSignature->addChild(pItemSuperBlob);
                }
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_MAIN, &listCommandRecords)) {
                QTreeWidgetItem *pItemMain = createNewItem(SMACH::TYPE_mach_main, QString("LC_MAIN"),
                                                           mach.getCommandRecordOffset(XMACH_DEF::S_LC_MAIN, 0,
                                                                                       &listCommandRecords));  // TODO rename

                pItemCommands->addChild(pItemMain);
            }

            if (mach.isCommandPresent(XMACH_DEF::S_LC_UNIXTHREAD, &listCommandRecords)) {
                qint64 _nOffset = mach.getCommandRecordOffset(XMACH_DEF::S_LC_UNIXTHREAD, 0, &listCommandRecords);

                QTreeWidgetItem *pItemUnixThread = createNewItem(SMACH::TYPE_mach_unix_thread, QString("LC_UNIXTHREAD"),
                                                                 _nOffset);  // TODO rename

                pItemCommands->addChild(pItemUnixThread);

                //                XMACH_DEF::unix_thread_command
                //                unix_thread=mach._read_unix_thread_command(_nOffset);

                quint32 nMachine = mach.getHeader_cputype();

                _nOffset += sizeof(XMACH_DEF::unix_thread_command);

                if (nMachine == XMACH_DEF::S_CPU_TYPE_I386) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_mach_unix_thread_x86_32, QString("x86_thread_state32_t"),
                                                           _nOffset);  // TODO rename

                    pItemUnixThread->addChild(pItem);
                } else if (nMachine == XMACH_DEF::S_CPU_TYPE_X86_64) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_mach_unix_thread_x86_64, QString("x86_thread_state64_t"),
                                                           _nOffset);  // TODO rename

                    pItemUnixThread->addChild(pItem);
                } else if (nMachine == XMACH_DEF::S_CPU_TYPE_ARM) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_mach_unix_thread_arm_32, QString("arm_thread_state32_t"),
                                                           _nOffset);  // TODO rename

                    pItemUnixThread->addChild(pItem);
                } else if (nMachine == XMACH_DEF::S_CPU_TYPE_ARM64) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_mach_unix_thread_arm_64, QString("arm_thread_state64_t"),
                                                           _nOffset);  // TODO rename

                    pItemUnixThread->addChild(pItem);
                } else if (nMachine == XMACH_DEF::S_CPU_TYPE_MC680x0) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_mach_unix_thread_m68k_32, QString("m68k_thread_state32_t"),
                                                           _nOffset);  // TODO rename

                    pItemUnixThread->addChild(pItem);
                } else if (nMachine == XMACH_DEF::S_CPU_TYPE_POWERPC) {
                    QTreeWidgetItem *pItem = createNewItem(SMACH::TYPE_mach_unix_thread_ppc_32, QString("ppc_thread_state32_t"),
                                                           _nOffset);  // TODO rename

                    pItemUnixThread->addChild(pItem);
                }
            }
        }

        ui->treeWidgetNavi->expandAll();

        setTreeItem(ui->treeWidgetNavi, getOptions().nStartType);

        setDisasmInitAddress(mach.getEntryPointAddress());  // Optimize
    }
}

FormatWidget::SV MACHWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition, qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nPosition)

    SV result = SV_NONE;

    blockSignals(true);

    quint64 nValue = vValue.toULongLong();
    QString sValue = vValue.toString();

    if (getDevice()->isWritable()) {
        XMACH mach(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

        if (mach.isValid()) {
            switch (nStype) {
                case SMACH::TYPE_mach_header:
                    switch (nNdata) {
                        case N_mach_header::magic:
                            g_comboBox[CB_mach_header_magic]->setValue(nValue);
                            break;
                        case N_mach_header::cputype:
                            g_comboBox[CB_mach_header_cputype]->setValue(nValue);
                            break;
                        case N_mach_header::cpusubtype:
                            g_comboBox[CB_mach_header_cpusubtype]->setValue(nValue);
                            break;
                        case N_mach_header::filetype:
                            g_comboBox[CB_mach_header_filetype]->setValue(nValue);
                            break;
                        case N_mach_header::flags:
                            g_comboBox[CB_mach_header_flags]->setValue(nValue);
                            break;
                    }
                    break;

                case SMACH::TYPE_mach_dyld_info_only:
                    switch (nNdata) {
                        case N_mach_dyld_info::rebase_off:
                            g_invWidget[INV_rebase_off]->setOffsetAndSize(&mach, nValue, mach.get_dyld_info().rebase_size, true);
                            break;
                        case N_mach_dyld_info::rebase_size:
                            g_invWidget[INV_rebase_off]->setOffsetAndSize(&mach, mach.get_dyld_info().rebase_off, nValue, true);
                            break;
                        case N_mach_dyld_info::bind_off:
                            g_invWidget[INV_bind_off]->setOffsetAndSize(&mach, nValue, mach.get_dyld_info().bind_size, true);
                            break;
                        case N_mach_dyld_info::bind_size:
                            g_invWidget[INV_bind_off]->setOffsetAndSize(&mach, mach.get_dyld_info().bind_off, nValue, true);
                            break;
                        case N_mach_dyld_info::weak_bind_off:
                            g_invWidget[INV_weak_bind_off]->setOffsetAndSize(&mach, nValue, mach.get_dyld_info().weak_bind_size, true);
                            break;
                        case N_mach_dyld_info::weak_bind_size:
                            g_invWidget[INV_weak_bind_off]->setOffsetAndSize(&mach, mach.get_dyld_info().weak_bind_off, nValue, true);
                            break;
                        case N_mach_dyld_info::lazy_bind_off:
                            g_invWidget[INV_lazy_bind_off]->setOffsetAndSize(&mach, nValue, mach.get_dyld_info().lazy_bind_size, true);
                            break;
                        case N_mach_dyld_info::lazy_bind_size:
                            g_invWidget[INV_lazy_bind_off]->setOffsetAndSize(&mach, mach.get_dyld_info().lazy_bind_off, nValue, true);
                            break;
                        case N_mach_dyld_info::export_off:
                            g_invWidget[INV_export_off]->setOffsetAndSize(&mach, nValue, mach.get_dyld_info().export_size, true);
                            break;
                        case N_mach_dyld_info::export_size:
                            g_invWidget[INV_export_off]->setOffsetAndSize(&mach, mach.get_dyld_info().export_off, nValue, true);
                            break;
                    }
                    break;

                case SMACH::TYPE_mach_symtab:
                    switch (nNdata) {
                        case N_mach_symtab::symoff:
                            g_invWidget[INV_symoff]->setOffsetAndSize(&mach, nValue, mach.get_symtab().nsyms * mach.get_nlist_MODE_size(), true);
                            break;
                        case N_mach_symtab::nsyms:
                            g_invWidget[INV_symoff]->setOffsetAndSize(&mach, mach.get_symtab().symoff, nValue * mach.get_nlist_MODE_size(), true);
                            break;
                        case N_mach_symtab::stroff:
                            g_invWidget[INV_stroff]->setOffsetAndSize(&mach, nValue, mach.get_symtab().strsize, true);
                            break;
                        case N_mach_symtab::strsize:
                            g_invWidget[INV_stroff]->setOffsetAndSize(&mach, mach.get_symtab().stroff, nValue, true);
                            break;
                    }
                    break;

                case SMACH::TYPE_mach_dysymtab:
                    switch (nNdata) {
                        case N_mach_dysymtab::tocoff:
                            g_invWidget[INV_tocoff]->setOffsetAndSize(&mach, nValue, 0, true);
                            break;  // TODO Size
                        case N_mach_dysymtab::modtaboff:
                            g_invWidget[INV_modtaboff]->setOffsetAndSize(&mach, nValue, 0, true);
                            break;  // TODO Size
                        case N_mach_dysymtab::extrefsymoff:
                            g_invWidget[INV_extrefsymoff]->setOffsetAndSize(&mach, nValue, 0, true);
                            break;  // TODO Size
                        case N_mach_dysymtab::indirectsymoff:
                            g_invWidget[INV_indirectsymoff]->setOffsetAndSize(&mach, nValue, 0, true);
                            break;  // TODO Size
                        case N_mach_dysymtab::extreloff:
                            g_invWidget[INV_extreloff]->setOffsetAndSize(&mach, nValue, 0, true);
                            break;  // TODO Size
                        case N_mach_dysymtab::locreloff:
                            g_invWidget[INV_locreloff]->setOffsetAndSize(&mach, nValue, 0, true);
                            break;  // TODO Size
                    }
                    break;

                case SMACH::TYPE_mach_encryption_info:
                    if (mach.is64()) {
                        switch (nNdata) {
                            case N_mach_encryption_info::cryptoff:
                                g_invWidget[INV_cryptoff]->setOffsetAndSize(&mach, nValue, mach.get_encryption_info_64().cryptsize, true);
                                break;
                            case N_mach_encryption_info::cryptsize:
                                g_invWidget[INV_cryptoff]->setOffsetAndSize(&mach, mach.get_encryption_info_64().cryptoff, nValue, true);
                                break;
                        }
                    } else {
                        switch (nNdata) {
                            case N_mach_encryption_info::cryptoff:
                                g_invWidget[INV_cryptoff]->setOffsetAndSize(&mach, nValue, mach.get_encryption_info().cryptsize, true);
                                break;
                            case N_mach_encryption_info::cryptsize:
                                g_invWidget[INV_cryptoff]->setOffsetAndSize(&mach, mach.get_encryption_info().cryptoff, nValue, true);
                                break;
                        }
                    }

                    break;

                case SMACH::TYPE_mach_data_in_code:
                    switch (nNdata) {
                        case N_mach_linkedit_data::dataoff:
                            g_invWidget[INV_DATA_IN_CODE_dataoff]->setOffsetAndSize(&mach, nValue, mach.get_linkedit_data(XMACH_DEF::S_LC_DATA_IN_CODE).datasize, true);
                            break;
                        case N_mach_linkedit_data::datasize:
                            g_invWidget[INV_DATA_IN_CODE_dataoff]->setOffsetAndSize(&mach, mach.get_linkedit_data(XMACH_DEF::S_LC_DATA_IN_CODE).dataoff, nValue, true);
                            break;
                    }
                    break;

                case SMACH::TYPE_mach_function_starts:
                    switch (nNdata) {
                        case N_mach_linkedit_data::dataoff:
                            g_invWidget[INV_FUNCTION_STARTS_dataoff]->setOffsetAndSize(&mach, nValue, mach.get_linkedit_data(XMACH_DEF::S_LC_FUNCTION_STARTS).datasize, true);
                            break;
                        case N_mach_linkedit_data::datasize:
                            g_invWidget[INV_FUNCTION_STARTS_dataoff]->setOffsetAndSize(&mach, mach.get_linkedit_data(XMACH_DEF::S_LC_FUNCTION_STARTS).dataoff, nValue, true);
                            break;
                    }
                    break;

                case SMACH::TYPE_mach_code_signature:
                    switch (nNdata) {
                        case N_mach_linkedit_data::dataoff:
                            g_invWidget[INV_CODE_SIGNATURE_dataoff]->setOffsetAndSize(&mach, nValue, mach.get_linkedit_data(XMACH_DEF::S_LC_CODE_SIGNATURE).datasize, true);
                            break;
                        case N_mach_linkedit_data::datasize:
                            g_invWidget[INV_CODE_SIGNATURE_dataoff]->setOffsetAndSize(&mach, mach.get_linkedit_data(XMACH_DEF::S_LC_CODE_SIGNATURE).dataoff, nValue, true);
                            break;
                    }
                    break;

                case SMACH::TYPE_mach_main:
                    switch (nNdata) {
                        case N_mach_main::entryoff:
                            g_invWidget[INV_MAIN_entryoff]->setOffsetAndSize(&mach, nValue, 0, true);
                            break;
                    }
                    break;
            }

            switch (nStype) {
                case SMACH::TYPE_mach_header:
                    switch (nNdata) {
                        case N_mach_header::magic:
                            mach.setHeader_magic((quint32)nValue);
                            break;
                        case N_mach_header::cputype:
                            mach.setHeader_cputype((qint32)nValue);
                            break;
                        case N_mach_header::cpusubtype:
                            mach.setHeader_cpusubtype((qint32)nValue);
                            break;
                        case N_mach_header::filetype:
                            mach.setHeader_filetype((quint32)nValue);
                            break;
                        case N_mach_header::ncmds:
                            mach.setHeader_ncmds((quint32)nValue);
                            break;
                        case N_mach_header::sizeofcmds:
                            mach.setHeader_sizeofcmds((quint32)nValue);
                            break;
                        case N_mach_header::flags:
                            mach.setHeader_flags((quint32)nValue);
                            break;
                        case N_mach_header::reserved:
                            mach.setHeader_reserved((quint32)nValue);
                            break;
                    }

                    ui->widgetHex_mach_header->reload();

                    break;

                case SMACH::TYPE_mach_dyld_info_only:
                    switch (nNdata) {
                        case N_mach_dyld_info::rebase_off:
                            mach._set_dyld_info_command_rebase_off(nOffset, nValue);
                            break;
                        case N_mach_dyld_info::rebase_size:
                            mach._set_dyld_info_command_rebase_size(nOffset, nValue);
                            break;
                        case N_mach_dyld_info::bind_off:
                            mach._set_dyld_info_command_bind_off(nOffset, nValue);
                            break;
                        case N_mach_dyld_info::bind_size:
                            mach._set_dyld_info_command_bind_size(nOffset, nValue);
                            break;
                        case N_mach_dyld_info::weak_bind_off:
                            mach._set_dyld_info_command_weak_bind_off(nOffset, nValue);
                            break;
                        case N_mach_dyld_info::weak_bind_size:
                            mach._set_dyld_info_command_weak_bind_size(nOffset, nValue);
                            break;
                        case N_mach_dyld_info::lazy_bind_off:
                            mach._set_dyld_info_command_lazy_bind_off(nOffset, nValue);
                            break;
                        case N_mach_dyld_info::lazy_bind_size:
                            mach._set_dyld_info_command_lazy_bind_size(nOffset, nValue);
                            break;
                        case N_mach_dyld_info::export_off:
                            mach._set_dyld_info_command_export_off(nOffset, nValue);
                            break;
                        case N_mach_dyld_info::export_size:
                            mach._set_dyld_info_command_export_size(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_dyld_info_only->reload();

                    break;

                case SMACH::TYPE_mach_uuid:
                    switch (nNdata) {
                        case N_mach_uuid::uuid:
                            mach.setModuleUUID(sValue);
                            break;
                    }

                    break;

                case SMACH::TYPE_mach_dylinker:
                    switch (nNdata) {
                        case N_mach_dylinker::dylinker:
                            mach.setLoadDylinker(sValue);
                            break;
                    }

                    break;

                case SMACH::TYPE_mach_rpath:
                    switch (nNdata) {
                        case N_mach_rpath::path:
                            mach.setRPath(sValue);
                            break;
                    }

                    break;

                case SMACH::TYPE_mach_source_version:
                    switch (nNdata) {
                        case N_mach_source_version::version:
                            mach.setSourceVersion(nValue);
                            break;
                    }

                    ui->widgetHex_source_version->reload();

                    break;

                case SMACH::TYPE_mach_symtab:
                    switch (nNdata) {
                        case N_mach_symtab::symoff:
                            mach._set_symtab_command_symoff(nOffset, nValue);
                            break;
                        case N_mach_symtab::nsyms:
                            mach._set_symtab_command_nsyms(nOffset, nValue);
                            break;
                        case N_mach_symtab::stroff:
                            mach._set_symtab_command_stroff(nOffset, nValue);
                            break;
                        case N_mach_symtab::strsize:
                            mach._set_symtab_command_strsize(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_symtab->reload();

                    break;

                case SMACH::TYPE_mach_dysymtab:
                    switch (nNdata) {
                        case N_mach_dysymtab::ilocalsym:
                            mach._set_dysymtab_command_ilocalsym(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::nlocalsym:
                            mach._set_dysymtab_command_nlocalsym(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::iextdefsym:
                            mach._set_dysymtab_command_iextdefsym(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::nextdefsym:
                            mach._set_dysymtab_command_nextdefsym(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::iundefsym:
                            mach._set_dysymtab_command_iundefsym(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::nundefsym:
                            mach._set_dysymtab_command_nundefsym(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::tocoff:
                            mach._set_dysymtab_command_tocoff(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::ntoc:
                            mach._set_dysymtab_command_ntoc(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::modtaboff:
                            mach._set_dysymtab_command_modtaboff(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::nmodtab:
                            mach._set_dysymtab_command_nmodtab(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::extrefsymoff:
                            mach._set_dysymtab_command_extrefsymoff(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::nextrefsyms:
                            mach._set_dysymtab_command_nextrefsyms(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::indirectsymoff:
                            mach._set_dysymtab_command_indirectsymoff(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::nindirectsyms:
                            mach._set_dysymtab_command_nindirectsyms(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::extreloff:
                            mach._set_dysymtab_command_extreloff(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::nextrel:
                            mach._set_dysymtab_command_nextrel(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::locreloff:
                            mach._set_dysymtab_command_locreloff(nOffset, nValue);
                            break;
                        case N_mach_dysymtab::nlocrel:
                            mach._set_dysymtab_command_nlocrel(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_dysymtab->reload();

                    break;

                case SMACH::TYPE_mach_version_min:
                    switch (nNdata) {
                        case N_mach_version_min::version:
                            mach._set_version_min_command_version(nOffset, nValue);
                            break;
                        case N_mach_version_min::sdk:
                            mach._set_version_min_command_sdk(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_version_min->reload();

                    break;

                case SMACH::TYPE_mach_build_version:
                    switch (nNdata) {
                        case N_mach_build_version::platform:
                            mach._set_build_version_command_platform(nOffset, nValue);
                            break;
                        case N_mach_build_version::minos:
                            mach._set_build_version_command_minos(nOffset, nValue);
                            break;
                        case N_mach_build_version::sdk:
                            mach._set_build_version_command_sdk(nOffset, nValue);
                            break;
                        case N_mach_build_version::ntools:
                            mach._set_build_version_command_ntools(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_build_version->reload();

                    break;

                case SMACH::TYPE_mach_encryption_info:
                    switch (nNdata) {
                        case N_mach_encryption_info::cryptoff:
                            mach._set_encryption_info_command_64_cryptoff(nOffset, nValue);
                            break;
                        case N_mach_encryption_info::cryptsize:
                            mach._set_encryption_info_command_64_cryptsize(nOffset, nValue);
                            break;
                        case N_mach_encryption_info::cryptid:
                            mach._set_encryption_info_command_64_cryptid(nOffset, nValue);
                            break;
                        case N_mach_encryption_info::pad:
                            mach._set_encryption_info_command_64_pad(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_encryption_info->reload();

                    break;

                case SMACH::TYPE_mach_function_starts:
                    switch (nNdata) {
                        case N_mach_linkedit_data::dataoff:
                            mach._set_linkedit_data_command_dataoff(nOffset, nValue);
                            break;
                        case N_mach_linkedit_data::datasize:
                            mach._set_linkedit_data_command_datasize(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_function_starts->reload();

                    break;

                case SMACH::TYPE_mach_data_in_code:
                    switch (nNdata) {
                        case N_mach_linkedit_data::dataoff:
                            mach._set_linkedit_data_command_dataoff(nOffset, nValue);
                            break;
                        case N_mach_linkedit_data::datasize:
                            mach._set_linkedit_data_command_datasize(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_data_in_code->reload();

                    break;

                case SMACH::TYPE_mach_code_signature:
                    switch (nNdata) {
                        case N_mach_linkedit_data::dataoff:
                            mach._set_linkedit_data_command_dataoff(nOffset, nValue);
                            break;
                        case N_mach_linkedit_data::datasize:
                            mach._set_linkedit_data_command_datasize(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_code_signature->reload();

                    break;

                case SMACH::TYPE_mach_SuperBlob:
                    switch (nNdata) {
                        case N_mach_SuperBlob::magic:
                            mach._set_SC_SuperBlob_magic(nOffset, nValue);
                            break;
                        case N_mach_SuperBlob::length:
                            mach._set_SC_SuperBlob_length(nOffset, nValue);
                            break;
                        case N_mach_SuperBlob::count:
                            mach._set_SC_SuperBlob_count(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_SuperBlob->reload();

                    break;

                case SMACH::TYPE_mach_main:
                    switch (nNdata) {
                        case N_mach_main::entryoff:
                            mach._set_entry_point_command_entryoff(nOffset, nValue);
                            break;
                        case N_mach_main::stacksize:
                            mach._set_entry_point_command_stacksize(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_main->reload();

                    break;

                case SMACH::TYPE_mach_unix_thread:
                    switch (nNdata) {
                        case N_mach_unix_thread::flavor:
                            mach._set_unix_thread_command_flavor(nOffset, nValue);
                            break;
                        case N_mach_unix_thread::count:
                            mach._set_unix_thread_command_count(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_unix_thread->reload();

                    break;

                case SMACH::TYPE_mach_unix_thread_x86_32:
                    switch (nNdata) {
                        case N_mach_unix_thread_x86_32::eax:
                            mach._set_x86_thread_state32_t_eax(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::ebx:
                            mach._set_x86_thread_state32_t_ebx(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::ecx:
                            mach._set_x86_thread_state32_t_ecx(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::edx:
                            mach._set_x86_thread_state32_t_edx(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::edi:
                            mach._set_x86_thread_state32_t_edi(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::esi:
                            mach._set_x86_thread_state32_t_esi(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::ebp:
                            mach._set_x86_thread_state32_t_ebp(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::esp:
                            mach._set_x86_thread_state32_t_esp(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::ss:
                            mach._set_x86_thread_state32_t_ss(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::eflags:
                            mach._set_x86_thread_state32_t_eflags(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::eip:
                            mach._set_x86_thread_state32_t_eip(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::cs:
                            mach._set_x86_thread_state32_t_cs(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::ds:
                            mach._set_x86_thread_state32_t_ds(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::es:
                            mach._set_x86_thread_state32_t_es(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::fs:
                            mach._set_x86_thread_state32_t_fs(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_32::gs:
                            mach._set_x86_thread_state32_t_gs(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_unix_thread_x86_32->reload();

                    break;

                case SMACH::TYPE_mach_unix_thread_x86_64:
                    switch (nNdata) {
                        case N_mach_unix_thread_x86_64::rax:
                            mach._set_x86_thread_state64_t_rax(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::rbx:
                            mach._set_x86_thread_state64_t_rbx(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::rcx:
                            mach._set_x86_thread_state64_t_rcx(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::rdx:
                            mach._set_x86_thread_state64_t_rdx(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::rdi:
                            mach._set_x86_thread_state64_t_rdi(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::rsi:
                            mach._set_x86_thread_state64_t_rsi(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::rbp:
                            mach._set_x86_thread_state64_t_rbp(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::rsp:
                            mach._set_x86_thread_state64_t_rsp(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::r8:
                            mach._set_x86_thread_state64_t_r8(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::r9:
                            mach._set_x86_thread_state64_t_r9(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::r10:
                            mach._set_x86_thread_state64_t_r10(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::r11:
                            mach._set_x86_thread_state64_t_r11(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::r12:
                            mach._set_x86_thread_state64_t_r12(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::r13:
                            mach._set_x86_thread_state64_t_r13(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::r14:
                            mach._set_x86_thread_state64_t_r14(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::r15:
                            mach._set_x86_thread_state64_t_r15(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::rip:
                            mach._set_x86_thread_state64_t_rip(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::rflags:
                            mach._set_x86_thread_state64_t_rflags(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::cs:
                            mach._set_x86_thread_state64_t_cs(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::fs:
                            mach._set_x86_thread_state64_t_fs(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_x86_64::gs:
                            mach._set_x86_thread_state64_t_gs(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_unix_thread_x86_64->reload();

                    break;

                case SMACH::TYPE_mach_unix_thread_arm_32:
                    switch (nNdata) {
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
                        case N_mach_unix_thread_arm_32::r12:
                            mach._set_arm_thread_state32_t_r(nOffset, nValue, nNdata - N_mach_unix_thread_arm_32::r0);
                            break;
                        case N_mach_unix_thread_arm_32::sp:
                            mach._set_arm_thread_state32_t_sp(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_arm_32::lr:
                            mach._set_arm_thread_state32_t_lr(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_arm_32::pc:
                            mach._set_arm_thread_state32_t_pc(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_arm_32::cpsr:
                            mach._set_arm_thread_state32_t_cpsr(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_unix_thread_arm_32->reload();

                    break;

                case SMACH::TYPE_mach_unix_thread_arm_64:
                    switch (nNdata) {
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
                        case N_mach_unix_thread_arm_64::x28:
                            mach._set_arm_thread_state64_t_x(nOffset, nValue, nNdata - N_mach_unix_thread_arm_64::x0);
                            break;
                        case N_mach_unix_thread_arm_64::fp:
                            mach._set_arm_thread_state64_t_fp(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_arm_64::lr:
                            mach._set_arm_thread_state64_t_lr(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_arm_64::sp:
                            mach._set_arm_thread_state64_t_sp(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_arm_64::pc:
                            mach._set_arm_thread_state64_t_pc(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_arm_64::cpsr:
                            mach._set_arm_thread_state64_t_cpsr(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_arm_64::pad:
                            mach._set_arm_thread_state64_t_pad(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_unix_thread_arm_64->reload();

                    break;

                case SMACH::TYPE_mach_unix_thread_m68k_32:
                    switch (nNdata) {
                        case N_mach_unix_thread_m68k_32::dreg0:
                        case N_mach_unix_thread_m68k_32::dreg1:
                        case N_mach_unix_thread_m68k_32::dreg2:
                        case N_mach_unix_thread_m68k_32::dreg3:
                        case N_mach_unix_thread_m68k_32::dreg4:
                        case N_mach_unix_thread_m68k_32::dreg5:
                        case N_mach_unix_thread_m68k_32::dreg6:
                        case N_mach_unix_thread_m68k_32::dreg7:
                            mach._set_m68k_thread_state32_t_dreg(nOffset, nValue, nNdata - N_mach_unix_thread_m68k_32::dreg0);
                            break;
                        case N_mach_unix_thread_m68k_32::areg0:
                        case N_mach_unix_thread_m68k_32::areg1:
                        case N_mach_unix_thread_m68k_32::areg2:
                        case N_mach_unix_thread_m68k_32::areg3:
                        case N_mach_unix_thread_m68k_32::areg4:
                        case N_mach_unix_thread_m68k_32::areg5:
                        case N_mach_unix_thread_m68k_32::areg6:
                        case N_mach_unix_thread_m68k_32::areg7:
                            mach._set_m68k_thread_state32_t_areg(nOffset, nValue, nNdata - N_mach_unix_thread_m68k_32::areg0);
                            break;
                        case N_mach_unix_thread_m68k_32::pad0:
                            mach._set_m68k_thread_state32_t_pad0(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_m68k_32::sr:
                            mach._set_m68k_thread_state32_t_sr(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_m68k_32::pc:
                            mach._set_m68k_thread_state32_t_pc(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_unix_thread_arm_32->reload();

                    break;

                case SMACH::TYPE_mach_unix_thread_ppc_32:
                    switch (nNdata) {
                        case N_mach_unix_thread_ppc_32::srr0:
                            mach._set_ppc_thread_state32_t_srr0(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_ppc_32::srr1:
                            mach._set_ppc_thread_state32_t_srr1(nOffset, nValue);
                            break;
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
                        case N_mach_unix_thread_ppc_32::r31:
                            mach._set_ppc_thread_state32_t_r(nOffset, nValue, nNdata - N_mach_unix_thread_ppc_32::r0);
                            break;
                        case N_mach_unix_thread_ppc_32::ct:
                            mach._set_ppc_thread_state32_t_ct(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_ppc_32::xer:
                            mach._set_ppc_thread_state32_t_xer(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_ppc_32::lr:
                            mach._set_ppc_thread_state32_t_lr(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_ppc_32::ctr:
                            mach._set_ppc_thread_state32_t_ctr(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_ppc_32::mq:
                            mach._set_ppc_thread_state32_t_mq(nOffset, nValue);
                            break;
                        case N_mach_unix_thread_ppc_32::vrsave:
                            mach._set_ppc_thread_state32_t_vrsave(nOffset, nValue);
                            break;
                    }

                    ui->widgetHex_unix_thread_ppc_32->reload();

                    break;
            }

            switch (nStype) {
                case SMACH::TYPE_mach_version_min:
                    switch (nNdata) {
                        case N_mach_version_min::version:
                            addComment(ui->tableWidget_version_min, N_mach_version_min::version, HEADER_COLUMN_COMMENT, XBinary::fullVersionDwordToString((quint32)nValue));
                            break;
                        case N_mach_version_min::sdk:
                            addComment(ui->tableWidget_version_min, N_mach_version_min::sdk, HEADER_COLUMN_COMMENT, XBinary::fullVersionDwordToString((quint32)nValue));
                            break;
                    }

                    break;

                case SMACH::TYPE_mach_build_version:
                    switch (nNdata) {
                        case N_mach_build_version::minos:
                            addComment(ui->tableWidget_build_version, N_mach_build_version::minos, HEADER_COLUMN_COMMENT, XBinary::fullVersionDwordToString((quint32)nValue));
                            break;
                        case N_mach_build_version::sdk:
                            addComment(ui->tableWidget_build_version, N_mach_build_version::sdk, HEADER_COLUMN_COMMENT, XBinary::fullVersionDwordToString((quint32)nValue));
                            break;
                    }

                    break;
            }

            result = SV_EDITED;

            switch (nStype) {
                case SMACH::TYPE_mach_header:
                    switch (nNdata) {
                        case N_mach_header::magic:
                            result = SV_RELOAD;
                            break;
                        case N_mach_header::cputype:
                            result = SV_RELOADDATA;
                            break;
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
    if (ui->checkBoxReadonly->isChecked() != bState) {
        const bool bBlocked1 = ui->checkBoxReadonly->blockSignals(true);
        ui->checkBoxReadonly->setChecked(bState);
        ui->checkBoxReadonly->blockSignals(bBlocked1);
    }

    setLineEditsReadOnly(g_lineEdit_mach_header, N_mach_header::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_dyld_info_only, N_mach_dyld_info::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_uuid, N_mach_uuid::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_dylinker, N_mach_dylinker::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_rpath, N_mach_rpath::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_symtab, N_mach_symtab::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_dysymtab, N_mach_dysymtab::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_version_min, N_mach_version_min::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_build_version, N_mach_build_version::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_source_version, N_mach_source_version::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_encryption_info, N_mach_encryption_info::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_function_starts, N_mach_linkedit_data::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_data_in_code, N_mach_linkedit_data::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_code_signature, N_mach_linkedit_data::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_SuperBlob, N_mach_SuperBlob::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_main, N_mach_main::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_unix_thread, N_mach_unix_thread::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_unix_thread_x86_32, N_mach_unix_thread_x86_32::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_unix_thread_x86_64, N_mach_unix_thread_x86_64::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_unix_thread_arm_32, N_mach_unix_thread_arm_32::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_unix_thread_arm_64, N_mach_unix_thread_arm_64::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_unix_thread_m68k_32, N_mach_unix_thread_m68k_32::__data_size, bState);
    setLineEditsReadOnly(g_lineEdit_mach_unix_thread_ppc_32, N_mach_unix_thread_ppc_32::__data_size, bState);
    setComboBoxesReadOnly(g_comboBox, __CB_size, bState);

    ui->widgetHex->setReadonly(bState);
    ui->widgetDisasm->setReadonly(bState);
    ui->widgetStrings->setReadonly(bState);
    ui->widgetHex_build_version->setReadonly(bState);
    ui->widgetHex_code_signature->setReadonly(bState);
    ui->widgetHex_commands->setReadonly(bState);
    ui->widgetHex_data_in_code->setReadonly(bState);
    ui->widgetHex_DYLD_INFO_bind->setReadonly(bState);
    ui->widgetHex_DYLD_INFO_export->setReadonly(bState);
    ui->widgetHex_DYLD_INFO_lazy_bind->setReadonly(bState);
    ui->widgetHex_dyld_info_only->setReadonly(bState);
    ui->widgetHex_DYLD_INFO_rebase->setReadonly(bState);
    ui->widgetHex_DYLD_INFO_weak_bind->setReadonly(bState);
    ui->widgetHex_dysymtab->setReadonly(bState);
    ui->widgetHex_encryption_info->setReadonly(bState);
    ui->widgetHex_function_starts->setReadonly(bState);
    ui->widgetHex_IDFVMLIB->setReadonly(bState);
    ui->widgetHex_id_library->setReadonly(bState);
    ui->widgetHex_libraries->setReadonly(bState);
    ui->widgetHex_LOADFVMLIB->setReadonly(bState);
    ui->widgetHex_mach_header->setReadonly(bState);
    ui->widgetHex_main->setReadonly(bState);
    ui->widgetHex_sections->setReadonly(bState);
    ui->widgetHex_segments->setReadonly(bState);
    ui->widgetHex_source_version->setReadonly(bState);
    ui->widgetHex_StringTable->setReadonly(bState);
    ui->widgetHex_SuperBlob->setReadonly(bState);
    ui->widgetHex_symtab->setReadonly(bState);
    ui->widgetHex_unix_thread->setReadonly(bState);
    ui->widgetHex_unix_thread_arm_32->setReadonly(bState);
    ui->widgetHex_unix_thread_arm_64->setReadonly(bState);
    ui->widgetHex_unix_thread_m68k_32->setReadonly(bState);
    ui->widgetHex_unix_thread_ppc_32->setReadonly(bState);
    ui->widgetHex_unix_thread_x86_32->setReadonly(bState);
    ui->widgetHex_unix_thread_x86_64->setReadonly(bState);
    ui->widgetHex_version_min->setReadonly(bState);
    ui->widgetHex_weak_libraries->setReadonly(bState);
}

void MACHWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)g_lineEdit_mach_header, N_mach_header::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_dyld_info_only, N_mach_dyld_info::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_uuid, N_mach_uuid::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_dylinker, N_mach_dylinker::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_rpath, N_mach_rpath::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_symtab, N_mach_symtab::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_dysymtab, N_mach_dysymtab::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_version_min, N_mach_version_min::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_build_version, N_mach_build_version::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_source_version, N_mach_source_version::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_encryption_info, N_mach_encryption_info::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_function_starts, N_mach_linkedit_data::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_data_in_code, N_mach_linkedit_data::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_code_signature, N_mach_linkedit_data::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_SuperBlob, N_mach_SuperBlob::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_main, N_mach_main::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_unix_thread, N_mach_unix_thread::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_unix_thread_x86_32, N_mach_unix_thread_x86_32::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_unix_thread_x86_64, N_mach_unix_thread_x86_64::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_unix_thread_arm_32, N_mach_unix_thread_arm_32::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_unix_thread_arm_64, N_mach_unix_thread_arm_64::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_unix_thread_m68k_32, N_mach_unix_thread_m68k_32::__data_size, bState);
    _blockSignals((QObject **)g_lineEdit_mach_unix_thread_ppc_32, N_mach_unix_thread_ppc_32::__data_size, bState);
    _blockSignals((QObject **)g_comboBox, __CB_size, bState);
}

void MACHWidget::adjustHeaderTable(int nType, QTableWidget *pTableWidget)
{
    XBinary::MODE mode = XMACH::getMode(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET, getColumnWidth(this, CW_UINT16, mode));
    pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE, getColumnWidth(this, CW_TYPE, mode));

    switch (nType) {
        case SMACH::TYPE_mach_header:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGMID, mode));
            break;
        case SMACH::TYPE_mach_dyld_info_only:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_source_version:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT64, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_symtab:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_dysymtab:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_version_min:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_encryption_info:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_function_starts:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_data_in_code:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_code_signature:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_main:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT64, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_unix_thread:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_unix_thread_x86_32:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_unix_thread_x86_64:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT64, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_unix_thread_arm_32:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_unix_thread_arm_64:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT64, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_unix_thread_m68k_32:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
        case SMACH::TYPE_mach_unix_thread_ppc_32:
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME, getColumnWidth(this, CW_STRINGSHORT, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, getColumnWidth(this, CW_UINT32, mode));
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, getColumnWidth(this, CW_STRINGSHORT, mode));
            break;
    }
}

QString MACHWidget::typeIdToString(int nType)
{
    QString sResult = tr("Unknown");

    switch (nType) {
        case SMACH::TYPE_mach_commands:
            sResult = QString("Command %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_mach_segments:
            sResult = QString("Segment %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_mach_sections:
            sResult = QString("Section %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_mach_libraries:
            sResult = QString("Library %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_mach_weak_libraries:
            sResult = QString("Library %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_mach_id_library:
            sResult = QString("Library %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_SYMBOLTABLE:
            sResult = QString("Symbol %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_DICE:
            sResult = QString("DICE %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_DYSYMTAB_modtab:
            sResult = QString("MODTAB %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_DYSYMTAB_toc:
            sResult = QString("TOC %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_DYSYMTAB_extrel:
            sResult = QString("Reloc %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_DYSYMTAB_locrel:
            sResult = QString("Reloc %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_DYSYMTAB_indirectsyms:
            sResult = QString("Indirect symbol %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_mach_IDFVMLIB:
            sResult = QString("IDFVMLIB %1").arg(tr("Header"));
            break;
        case SMACH::TYPE_mach_LOADFVMLIB:
            sResult = QString("IDFVMLIB %1").arg(tr("Header"));
            break;
    }

    return sResult;
}

void MACHWidget::_showInDisasmWindowAddress(XADDR nAddress)
{
    setTreeItem(ui->treeWidgetNavi, SMACH::TYPE_DISASM);
    ui->widgetDisasm->goToAddress(nAddress);
}

void MACHWidget::_showInDisasmWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SMACH::TYPE_DISASM);
    ui->widgetDisasm->goToOffset(nOffset);
}

void MACHWidget::_showInMemoryMapWindowOffset(qint64 nOffset)
{
    setTreeItem(ui->treeWidgetNavi, SMACH::TYPE_MEMORYMAP);
    ui->widgetMemoryMap->goToOffset(nOffset);
}

void MACHWidget::_showInHexWindow(qint64 nOffset, qint64 nSize)
{
    setTreeItem(ui->treeWidgetNavi, SMACH::TYPE_HEX);
    ui->widgetHex->setSelection(nOffset, nSize);
}

void MACHWidget::reloadData()
{
    qint32 nType = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_TYPE).toInt();
    qint64 nDataOffset = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nDataSize = ui->treeWidgetNavi->currentItem()->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();

    QString sInit = getInitString(ui->treeWidgetNavi->currentItem());

    ui->stackedWidgetInfo->setCurrentIndex(nType);

    XMACH mach(getDevice(), getOptions().bIsImage, getOptions().nImageBase);

    if (mach.isValid()) {
        if (nType == SMACH::TYPE_INFO) {
            if (!isInitPresent(sInit)) {
                ui->widgetInfo->setData(getDevice(), mach.getFileType(), "Info", true);
            }
        } else if (nType == SMACH::TYPE_VIRUSTOTAL) {
            if (!isInitPresent(sInit)) {
                ui->widgetVirusTotal->setData(getDevice());
            }
        } else if (nType == SMACH::TYPE_HEX) {
            if (!isInitPresent(sInit)) {
                XHexView::OPTIONS options = {};
                options.bMenu_Disasm = true;
                options.bMenu_MemoryMap = true;
                ui->widgetHex->setData(getDevice(), options);
                ui->widgetHex->setBackupDevice(getBackupDevice());
                //                ui->widgetHex->enableReadOnly(false);
            }
        } else if (nType == SMACH::TYPE_DISASM) {
            if (!isInitPresent(sInit)) {
                XMultiDisasmWidget::OPTIONS options = {};
                options.fileType = mach.getFileType();
                options.nInitAddress = getDisasmInitAddress();
                options.bMenu_Hex = true;

                ui->widgetDisasm->setData(getDevice(), options, getXInfoDB());
                ui->widgetDisasm->setBackupDevice(getBackupDevice());

                setDisasmInitAddress(-1);
            }
        } else if (nType == SMACH::TYPE_HASH) {
            if (!isInitPresent(sInit)) {
                ui->widgetHash->setData(getDevice(), mach.getFileType(), 0, -1, true);
            }
        } else if (nType == SMACH::TYPE_STRINGS) {
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
        } else if (nType == SMACH::TYPE_SIGNATURES) {
            if (!isInitPresent(sInit)) {
                SearchSignaturesWidget::OPTIONS signaturesOptions = {};
                signaturesOptions.bMenu_Hex = true;

                ui->widgetSignatures->setData(getDevice(), mach.getFileType(), signaturesOptions, false);
            }
        } else if (nType == SMACH::TYPE_MEMORYMAP) {
            if (!isInitPresent(sInit)) {
                ui->widgetMemoryMap->setData(getDevice(), mach.getFileType());
            }
        } else if (nType == SMACH::TYPE_ENTROPY) {
            if (!isInitPresent(sInit)) {
                ui->widgetEntropy->setData(getDevice(), 0, getDevice()->size(), mach.getFileType(), true);
            }
        } else if (nType == SMACH::TYPE_HEURISTICSCAN) {
            if (!isInitPresent(sInit)) {
                ui->widgetHeuristicScan->setData(getDevice(), true, mach.getFileType());
            }
        } else if (nType == SMACH::TYPE_mach_header) {
            if (!isInitPresent(sInit)) {
                if (!mach.is64()) {
                    createHeaderTable(SMACH::TYPE_mach_header, ui->tableWidget_mach_header, N_mach_header::records32, g_lineEdit_mach_header, N_mach_header::__data_size - 1,
                                      0);
                } else {
                    createHeaderTable(SMACH::TYPE_mach_header, ui->tableWidget_mach_header, N_mach_header::records64, g_lineEdit_mach_header, N_mach_header::__data_size, 0);
                }

                g_comboBox[CB_mach_header_magic] =
                    createComboBox(ui->tableWidget_mach_header, XMACH::getHeaderMagicsS(), SMACH::TYPE_mach_header, N_mach_header::magic, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_mach_header_cputype] =
                    createComboBox(ui->tableWidget_mach_header, XMACH::getHeaderCpuTypesS(), SMACH::TYPE_mach_header, N_mach_header::cputype, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_mach_header_cpusubtype] = createComboBox(ui->tableWidget_mach_header, XMACH::getHeaderCpuSubTypesS(mach.getHeader_cputype()),
                                                                       SMACH::TYPE_mach_header, N_mach_header::cpusubtype, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_mach_header_filetype] =
                    createComboBox(ui->tableWidget_mach_header, XMACH::getHeaderFileTypesS(), SMACH::TYPE_mach_header, N_mach_header::filetype, XComboBoxEx::CBTYPE_LIST);
                g_comboBox[CB_mach_header_flags] =
                    createComboBox(ui->tableWidget_mach_header, XMACH::getHeaderFlagsS(), SMACH::TYPE_mach_header, N_mach_header::flags, XComboBoxEx::CBTYPE_FLAGS);

                blockSignals(true);

                g_lineEdit_mach_header[N_mach_header::magic]->setValue(mach.getHeader_magic());
                g_lineEdit_mach_header[N_mach_header::cputype]->setValue(mach.getHeader_cputype());
                g_lineEdit_mach_header[N_mach_header::cpusubtype]->setValue(mach.getHeader_cpusubtype());
                g_lineEdit_mach_header[N_mach_header::filetype]->setValue(mach.getHeader_filetype());
                g_lineEdit_mach_header[N_mach_header::ncmds]->setValue(mach.getHeader_ncmds());
                g_lineEdit_mach_header[N_mach_header::sizeofcmds]->setValue(mach.getHeader_sizeofcmds());
                g_lineEdit_mach_header[N_mach_header::flags]->setValue(mach.getHeader_flags());

                if (mach.is64()) {
                    g_lineEdit_mach_header[N_mach_header::reserved]->setValue(mach.getHeader_reserved());
                }

                g_comboBox[CB_mach_header_magic]->setValue(mach.getHeader_magic());
                g_comboBox[CB_mach_header_cputype]->setValue((quint32)mach.getHeader_cputype());
                g_comboBox[CB_mach_header_cpusubtype]->setValue((quint32)mach.getHeader_cpusubtype());
                g_comboBox[CB_mach_header_filetype]->setValue((quint32)mach.getHeader_filetype());
                g_comboBox[CB_mach_header_flags]->setValue((quint32)mach.getHeader_flags());

                qint64 nOffset = mach.getHeaderOffset();
                qint64 nSize = mach.getHeaderSize();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_header], ui->widgetHex_mach_header);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_commands) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_mach_commands, &tvModel[SMACH::TYPE_mach_commands], &mach, 0, 0);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_mach_commands], ui->tableView_commands, nullptr, true);

                connect(ui->tableView_commands->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_commands_currentRowChanged(QModelIndex, QModelIndex)));

                if (tvModel[SMACH::TYPE_mach_commands]->rowCount()) {
                    ui->tableView_commands->setCurrentIndex(ui->tableView_commands->model()->index(0, 0));
                }
            }
        } else if (nType == SMACH::TYPE_mach_segments) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_mach_segments, &tvModel[SMACH::TYPE_mach_segments], &mach, 0, 0);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_mach_segments], ui->tableView_segments, nullptr, false);

                connect(ui->tableView_segments->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_segments_currentRowChanged(QModelIndex, QModelIndex)));

                if (tvModel[SMACH::TYPE_mach_segments]->rowCount()) {
                    ui->tableView_segments->setCurrentIndex(ui->tableView_segments->model()->index(0, 0));
                }
            }
        } else if (nType == SMACH::TYPE_mach_sections) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_mach_sections, &tvModel[SMACH::TYPE_mach_sections], &mach, 0, 0);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_mach_sections], ui->tableView_sections, nullptr, false);

                connect(ui->tableView_sections->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_sections_currentRowChanged(QModelIndex, QModelIndex)));

                if (tvModel[SMACH::TYPE_mach_sections]->rowCount()) {
                    ui->tableView_sections->setCurrentIndex(ui->tableView_sections->model()->index(0, 0));
                }
            }
        } else if (nType == SMACH::TYPE_mach_libraries) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_mach_libraries, &tvModel[SMACH::TYPE_mach_libraries], &mach, 0, 0);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_mach_libraries], ui->tableView_libraries, nullptr, true);

                connect(ui->tableView_libraries->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_libraries_currentRowChanged(QModelIndex, QModelIndex)));

                if (tvModel[SMACH::TYPE_mach_libraries]->rowCount()) {
                    ui->tableView_libraries->setCurrentIndex(ui->tableView_libraries->model()->index(0, 0));
                }
            }
        } else if (nType == SMACH::TYPE_mach_weak_libraries) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_mach_weak_libraries, &tvModel[SMACH::TYPE_mach_weak_libraries], &mach, 0, 0);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_mach_weak_libraries], ui->tableView_weak_libraries, nullptr, true);

                connect(ui->tableView_weak_libraries->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_weak_libraries_currentRowChanged(QModelIndex, QModelIndex)));

                if (tvModel[SMACH::TYPE_mach_weak_libraries]->rowCount()) {
                    ui->tableView_weak_libraries->setCurrentIndex(ui->tableView_weak_libraries->model()->index(0, 0));
                }
            }
        } else if (nType == SMACH::TYPE_mach_id_library) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_mach_id_library, &tvModel[SMACH::TYPE_mach_id_library], &mach, 0, 0);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_mach_id_library], ui->tableView_id_library, nullptr, true);

                connect(ui->tableView_id_library->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_id_library_currentRowChanged(QModelIndex, QModelIndex)));

                if (tvModel[SMACH::TYPE_mach_id_library]->rowCount()) {
                    ui->tableView_id_library->setCurrentIndex(ui->tableView_id_library->model()->index(0, 0));
                }
            }
        } else if (nType == SMACH::TYPE_mach_LOADFVMLIB) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_mach_LOADFVMLIB, &tvModel[SMACH::TYPE_mach_LOADFVMLIB], &mach, 0, 0);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_mach_LOADFVMLIB], ui->tableView_LOADFVMLIB, nullptr, true);

                connect(ui->tableView_LOADFVMLIB->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_LOADFVMLIB_currentRowChanged(QModelIndex, QModelIndex)));

                if (tvModel[SMACH::TYPE_mach_LOADFVMLIB]->rowCount()) {
                    ui->tableView_LOADFVMLIB->setCurrentIndex(ui->tableView_LOADFVMLIB->model()->index(0, 0));
                }
            }
        } else if (nType == SMACH::TYPE_mach_IDFVMLIB) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_mach_IDFVMLIB, &tvModel[SMACH::TYPE_mach_IDFVMLIB], &mach, 0, 0);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_mach_IDFVMLIB], ui->tableView_IDFVMLIB, nullptr, true);

                connect(ui->tableView_IDFVMLIB->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_IDFVMLIB_currentRowChanged(QModelIndex, QModelIndex)));

                if (tvModel[SMACH::TYPE_mach_IDFVMLIB]->rowCount()) {
                    ui->tableView_IDFVMLIB->setCurrentIndex(ui->tableView_IDFVMLIB->model()->index(0, 0));
                }
            }
        } else if (nType == SMACH::TYPE_mach_dyld_info_only) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_dyld_info_only, ui->tableWidget_dyld_info_only, N_mach_dyld_info::records, g_lineEdit_mach_dyld_info_only,
                                  N_mach_dyld_info::__data_size, 0, nDataOffset);

                g_invWidget[INV_rebase_off] =
                    createInvWidget(ui->tableWidget_dyld_info_only, SMACH::TYPE_mach_dyld_info_only, N_mach_dyld_info::rebase_off, InvWidget::TYPE_HEX);
                g_invWidget[INV_bind_off] = createInvWidget(ui->tableWidget_dyld_info_only, SMACH::TYPE_mach_dyld_info_only, N_mach_dyld_info::bind_off, InvWidget::TYPE_HEX);
                g_invWidget[INV_weak_bind_off] =
                    createInvWidget(ui->tableWidget_dyld_info_only, SMACH::TYPE_mach_dyld_info_only, N_mach_dyld_info::weak_bind_off, InvWidget::TYPE_HEX);
                g_invWidget[INV_lazy_bind_off] =
                    createInvWidget(ui->tableWidget_dyld_info_only, SMACH::TYPE_mach_dyld_info_only, N_mach_dyld_info::lazy_bind_off, InvWidget::TYPE_HEX);
                g_invWidget[INV_export_off] =
                    createInvWidget(ui->tableWidget_dyld_info_only, SMACH::TYPE_mach_dyld_info_only, N_mach_dyld_info::export_off, InvWidget::TYPE_HEX);

                blockSignals(true);

                XMACH_DEF::dyld_info_command dyld_info = mach._read_dyld_info_command(nDataOffset);

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

                g_invWidget[INV_rebase_off]->setOffsetAndSize(&mach, dyld_info.rebase_off, dyld_info.rebase_size, true);
                g_invWidget[INV_bind_off]->setOffsetAndSize(&mach, dyld_info.bind_off, dyld_info.bind_size, true);
                g_invWidget[INV_weak_bind_off]->setOffsetAndSize(&mach, dyld_info.weak_bind_off, dyld_info.weak_bind_size, true);
                g_invWidget[INV_lazy_bind_off]->setOffsetAndSize(&mach, dyld_info.lazy_bind_off, dyld_info.lazy_bind_size, true);
                g_invWidget[INV_export_off]->setOffsetAndSize(&mach, dyld_info.export_off, dyld_info.export_size, true);

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_dyld_info_command_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_dyld_info_only], ui->widgetHex_dyld_info_only);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_uuid) {
            if (!isInitPresent(sInit)) {
                createListTable(SMACH::TYPE_mach_uuid, ui->tableWidget_uuid, N_mach_uuid::records, g_lineEdit_mach_uuid, N_mach_uuid::__data_size);

                blockSignals(true);

                g_lineEdit_mach_uuid[N_mach_uuid::uuid]->setUUID(mach.getModuleUUID());

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_dylinker) {
            if (!isInitPresent(sInit)) {
                createListTable(SMACH::TYPE_mach_dylinker, ui->tableWidget_dylinker, N_mach_dylinker::records, g_lineEdit_mach_dylinker, N_mach_dylinker::__data_size);

                blockSignals(true);

                g_lineEdit_mach_dylinker[N_mach_dylinker::dylinker]->setStringValue(mach.getLoadDylinker());  // TODO Max Size!!!

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_rpath) {
            if (!isInitPresent(sInit)) {
                createListTable(SMACH::TYPE_mach_rpath, ui->tableWidget_rpath, N_mach_rpath::records, g_lineEdit_mach_rpath, N_mach_rpath::__data_size);

                blockSignals(true);

                g_lineEdit_mach_rpath[N_mach_rpath::path]->setStringValue(mach.getRPath());  // TODO Max Size!!!

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_symtab) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_symtab, ui->tableWidget_symtab, N_mach_symtab::records, g_lineEdit_mach_symtab, N_mach_symtab::__data_size, 0, nDataOffset);

                g_invWidget[INV_symoff] = createInvWidget(ui->tableWidget_symtab, SMACH::TYPE_mach_symtab, N_mach_symtab::symoff, InvWidget::TYPE_HEX);
                g_invWidget[INV_stroff] = createInvWidget(ui->tableWidget_symtab, SMACH::TYPE_mach_symtab, N_mach_symtab::stroff, InvWidget::TYPE_HEX);

                blockSignals(true);

                XMACH_DEF::symtab_command symtab = mach._read_symtab_command(nDataOffset);

                g_lineEdit_mach_symtab[N_mach_symtab::symoff]->setValue(symtab.symoff);
                g_lineEdit_mach_symtab[N_mach_symtab::nsyms]->setValue(symtab.nsyms);
                g_lineEdit_mach_symtab[N_mach_symtab::stroff]->setValue(symtab.stroff);
                g_lineEdit_mach_symtab[N_mach_symtab::strsize]->setValue(symtab.strsize);

                g_invWidget[INV_symoff]->setOffsetAndSize(&mach, symtab.symoff, symtab.nsyms * mach.get_nlist_MODE_size(), true);
                g_invWidget[INV_stroff]->setOffsetAndSize(&mach, symtab.stroff, symtab.strsize, true);

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_symtab_command_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_symtab], ui->widgetHex_symtab);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_dysymtab) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_dysymtab, ui->tableWidget_dysymtab, N_mach_dysymtab::records, g_lineEdit_mach_dysymtab, N_mach_dysymtab::__data_size, 0,
                                  nDataOffset);

                g_invWidget[INV_tocoff] = createInvWidget(ui->tableWidget_dysymtab, SMACH::TYPE_mach_dysymtab, N_mach_dysymtab::tocoff, InvWidget::TYPE_HEX);
                g_invWidget[INV_modtaboff] = createInvWidget(ui->tableWidget_dysymtab, SMACH::TYPE_mach_dysymtab, N_mach_dysymtab::modtaboff, InvWidget::TYPE_HEX);
                g_invWidget[INV_extrefsymoff] = createInvWidget(ui->tableWidget_dysymtab, SMACH::TYPE_mach_dysymtab, N_mach_dysymtab::extrefsymoff, InvWidget::TYPE_HEX);
                g_invWidget[INV_indirectsymoff] = createInvWidget(ui->tableWidget_dysymtab, SMACH::TYPE_mach_dysymtab, N_mach_dysymtab::indirectsymoff, InvWidget::TYPE_HEX);
                g_invWidget[INV_extreloff] = createInvWidget(ui->tableWidget_dysymtab, SMACH::TYPE_mach_dysymtab, N_mach_dysymtab::extreloff, InvWidget::TYPE_HEX);
                g_invWidget[INV_locreloff] = createInvWidget(ui->tableWidget_dysymtab, SMACH::TYPE_mach_dysymtab, N_mach_dysymtab::locreloff, InvWidget::TYPE_HEX);

                blockSignals(true);

                XMACH_DEF::dysymtab_command dysymtab = mach._read_dysymtab_command(nDataOffset);

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

                g_invWidget[INV_tocoff]->setOffsetAndSize(&mach, dysymtab.tocoff, 0, true);
                g_invWidget[INV_modtaboff]->setOffsetAndSize(&mach, dysymtab.modtaboff, 0, true);
                g_invWidget[INV_extrefsymoff]->setOffsetAndSize(&mach, dysymtab.extrefsymoff, 0, true);
                g_invWidget[INV_indirectsymoff]->setOffsetAndSize(&mach, dysymtab.indirectsymoff, 0, true);
                g_invWidget[INV_extreloff]->setOffsetAndSize(&mach, dysymtab.extreloff, 0, true);
                g_invWidget[INV_locreloff]->setOffsetAndSize(&mach, dysymtab.locreloff, 0, true);

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_dysymtab_command_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_dysymtab], ui->widgetHex_dysymtab);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_version_min) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_version_min, ui->tableWidget_version_min, N_mach_version_min::records, g_lineEdit_mach_version_min,
                                  N_mach_version_min::__data_size, 0, nDataOffset);

                blockSignals(true);

                XMACH_DEF::version_min_command version_min = mach._read_version_min_command(nDataOffset);

                g_lineEdit_mach_version_min[N_mach_version_min::version]->setValue(version_min.version);
                g_lineEdit_mach_version_min[N_mach_version_min::sdk]->setValue(version_min.sdk);

                addComment(ui->tableWidget_version_min, N_mach_version_min::version, HEADER_COLUMN_COMMENT, XBinary::fullVersionDwordToString(version_min.version));
                addComment(ui->tableWidget_version_min, N_mach_version_min::sdk, HEADER_COLUMN_COMMENT, XBinary::fullVersionDwordToString(version_min.sdk));

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_version_min_command_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_version_min], ui->widgetHex_version_min);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_build_version) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_build_version, ui->tableWidget_build_version, N_mach_build_version::records, g_lineEdit_mach_build_version,
                                  N_mach_build_version::__data_size, 0, nDataOffset);

                blockSignals(true);

                XMACH_DEF::build_version_command build_version = mach._read_build_version_command(nDataOffset);

                g_lineEdit_mach_build_version[N_mach_build_version::platform]->setValue(build_version.platform);
                g_lineEdit_mach_build_version[N_mach_build_version::minos]->setValue(build_version.minos);
                g_lineEdit_mach_build_version[N_mach_build_version::sdk]->setValue(build_version.sdk);
                g_lineEdit_mach_build_version[N_mach_build_version::ntools]->setValue(build_version.ntools);

                addComment(ui->tableWidget_build_version, N_mach_build_version::minos, HEADER_COLUMN_COMMENT, XBinary::fullVersionDwordToString(build_version.minos));
                addComment(ui->tableWidget_build_version, N_mach_build_version::sdk, HEADER_COLUMN_COMMENT, XBinary::fullVersionDwordToString(build_version.sdk));

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_build_version_command_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_build_version], ui->widgetHex_build_version);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_source_version) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_source_version, ui->tableWidget_source_version, N_mach_source_version::records, g_lineEdit_mach_source_version,
                                  N_mach_source_version::__data_size, 0, nDataOffset);

                blockSignals(true);

                XMACH_DEF::source_version_command source_version = mach._read_source_version_command(nDataOffset);

                g_lineEdit_mach_source_version[N_mach_source_version::version]->setValue(source_version.version);

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_source_version_command_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_source_version], ui->widgetHex_source_version);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_encryption_info) {
            if (!isInitPresent(sInit)) {
                if (nDataSize == mach.get_encryption_info_command_64_size()) {
                    createHeaderTable(SMACH::TYPE_mach_encryption_info, ui->tableWidget_encryption_info, N_mach_encryption_info::records64, g_lineEdit_mach_encryption_info,
                                      N_mach_encryption_info::__data_size, 0, nDataOffset);

                    g_invWidget[INV_cryptoff] =
                        createInvWidget(ui->tableWidget_encryption_info, SMACH::TYPE_mach_encryption_info, N_mach_encryption_info::cryptoff, InvWidget::TYPE_HEX);

                    blockSignals(true);

                    XMACH_DEF::encryption_info_command_64 encryption_info = mach._read_encryption_info_command_64(nDataOffset);

                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::cryptoff]->setValue(encryption_info.cryptoff);
                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::cryptsize]->setValue(encryption_info.cryptsize);
                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::cryptid]->setValue(encryption_info.cryptid);
                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::pad]->setValue(encryption_info.pad);

                    g_invWidget[INV_cryptoff]->setOffsetAndSize(&mach, encryption_info.cryptoff, encryption_info.cryptsize, true);
                } else {
                    createHeaderTable(SMACH::TYPE_mach_encryption_info, ui->tableWidget_encryption_info, N_mach_encryption_info::records32, g_lineEdit_mach_encryption_info,
                                      N_mach_encryption_info::__data_size - 1, 0, nDataOffset);

                    g_invWidget[INV_cryptoff] =
                        createInvWidget(ui->tableWidget_encryption_info, SMACH::TYPE_mach_encryption_info, N_mach_encryption_info::cryptoff, InvWidget::TYPE_HEX);

                    blockSignals(true);

                    XMACH_DEF::encryption_info_command encryption_info = mach._read_encryption_info_command(nDataOffset);

                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::cryptoff]->setValue(encryption_info.cryptoff);
                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::cryptsize]->setValue(encryption_info.cryptsize);
                    g_lineEdit_mach_encryption_info[N_mach_encryption_info::cryptid]->setValue(encryption_info.cryptid);

                    g_invWidget[INV_cryptoff]->setOffsetAndSize(&mach, encryption_info.cryptoff, encryption_info.cryptsize, true);
                }

                qint64 nOffset = nDataOffset;
                qint64 nSize = nDataSize;

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_encryption_info], ui->widgetHex_encryption_info);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_function_starts) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_function_starts, ui->tableWidget_function_starts, N_mach_linkedit_data::records, g_lineEdit_mach_function_starts,
                                  N_mach_linkedit_data::__data_size, 0, nDataOffset);

                g_invWidget[INV_FUNCTION_STARTS_dataoff] =
                    createInvWidget(ui->tableWidget_function_starts, SMACH::TYPE_mach_function_starts, N_mach_linkedit_data::dataoff, InvWidget::TYPE_HEX);

                blockSignals(true);

                XMACH_DEF::linkedit_data_command linkedit_data = mach._read_linkedit_data_command(nDataOffset);

                g_lineEdit_mach_function_starts[N_mach_linkedit_data::dataoff]->setValue(linkedit_data.dataoff);
                g_lineEdit_mach_function_starts[N_mach_linkedit_data::datasize]->setValue(linkedit_data.datasize);

                g_invWidget[INV_FUNCTION_STARTS_dataoff]->setOffsetAndSize(&mach, linkedit_data.dataoff, linkedit_data.datasize, true);

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_linkedit_data_command_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_function_starts], ui->widgetHex_function_starts);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_data_in_code) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_data_in_code, ui->tableWidget_data_in_code, N_mach_linkedit_data::records, g_lineEdit_mach_data_in_code,
                                  N_mach_linkedit_data::__data_size, 0, nDataOffset);

                g_invWidget[INV_DATA_IN_CODE_dataoff] =
                    createInvWidget(ui->tableWidget_data_in_code, SMACH::TYPE_mach_data_in_code, N_mach_linkedit_data::dataoff, InvWidget::TYPE_HEX);

                blockSignals(true);

                XMACH_DEF::linkedit_data_command linkedit_data = mach._read_linkedit_data_command(nDataOffset);

                g_lineEdit_mach_data_in_code[N_mach_linkedit_data::dataoff]->setValue(linkedit_data.dataoff);
                g_lineEdit_mach_data_in_code[N_mach_linkedit_data::datasize]->setValue(linkedit_data.datasize);

                g_invWidget[INV_DATA_IN_CODE_dataoff]->setOffsetAndSize(&mach, linkedit_data.dataoff, linkedit_data.datasize, true);

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_linkedit_data_command_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_data_in_code], ui->widgetHex_data_in_code);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_code_signature) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_code_signature, ui->tableWidget_code_signature, N_mach_linkedit_data::records, g_lineEdit_mach_code_signature,
                                  N_mach_linkedit_data::__data_size, 0, nDataOffset);

                g_invWidget[INV_CODE_SIGNATURE_dataoff] =
                    createInvWidget(ui->tableWidget_code_signature, SMACH::TYPE_mach_code_signature, N_mach_linkedit_data::dataoff, InvWidget::TYPE_HEX);

                blockSignals(true);

                XMACH_DEF::linkedit_data_command linkedit_data = mach._read_linkedit_data_command(nDataOffset);

                g_lineEdit_mach_code_signature[N_mach_linkedit_data::dataoff]->setValue(linkedit_data.dataoff);
                g_lineEdit_mach_code_signature[N_mach_linkedit_data::datasize]->setValue(linkedit_data.datasize);

                g_invWidget[INV_CODE_SIGNATURE_dataoff]->setOffsetAndSize(&mach, linkedit_data.dataoff, linkedit_data.datasize, true);

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_linkedit_data_command_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_code_signature], ui->widgetHex_code_signature);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_SuperBlob) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_SuperBlob, ui->tableWidget_SuperBlob, N_mach_SuperBlob::records, g_lineEdit_mach_SuperBlob, N_mach_SuperBlob::__data_size,
                                  0, nDataOffset);

                blockSignals(true);

                XMACH_DEF::__SC_SuperBlob super_blob = mach._read_SC_SuperBlob(nDataOffset);

                g_lineEdit_mach_SuperBlob[N_mach_SuperBlob::magic]->setValue(super_blob.magic);
                g_lineEdit_mach_SuperBlob[N_mach_SuperBlob::length]->setValue(super_blob.length);
                g_lineEdit_mach_SuperBlob[N_mach_SuperBlob::count]->setValue(super_blob.count);

                qint64 nOffset = nDataOffset;
                qint64 nSize = super_blob.length;

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_SuperBlob], ui->widgetHex_SuperBlob);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_main) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_main, ui->tableWidget_main, N_mach_main::records, g_lineEdit_mach_main, N_mach_main::__data_size, 0, nDataOffset);

                g_invWidget[INV_MAIN_entryoff] = createInvWidget(ui->tableWidget_main, SMACH::TYPE_mach_main, N_mach_main::entryoff, InvWidget::TYPE_DISASM);

                blockSignals(true);

                XMACH_DEF::entry_point_command entry_point = mach._read_entry_point_command(nDataOffset);

                g_lineEdit_mach_main[N_mach_main::entryoff]->setValue(entry_point.entryoff);
                g_lineEdit_mach_main[N_mach_main::stacksize]->setValue(entry_point.stacksize);

                g_invWidget[INV_MAIN_entryoff]->setOffsetAndSize(&mach, entry_point.entryoff, 0, true);

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_entry_point_command_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_main], ui->widgetHex_main);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_unix_thread) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_unix_thread, ui->tableWidget_unix_thread, N_mach_unix_thread::records, g_lineEdit_mach_unix_thread,
                                  N_mach_unix_thread::__data_size, 0, nDataOffset);

                blockSignals(true);

                XMACH_DEF::unix_thread_command unix_thread = mach._read_unix_thread_command(nDataOffset);

                g_lineEdit_mach_unix_thread[N_mach_unix_thread::flavor]->setValue(unix_thread.flavor);
                g_lineEdit_mach_unix_thread[N_mach_unix_thread::count]->setValue(unix_thread.count);

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_unix_thread_command_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_unix_thread], ui->widgetHex_unix_thread);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_unix_thread_x86_32) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_unix_thread_x86_32, ui->tableWidget_unix_thread_x86_32, N_mach_unix_thread_x86_32::records,
                                  g_lineEdit_mach_unix_thread_x86_32, N_mach_unix_thread_x86_32::__data_size, 0, nDataOffset);

                blockSignals(true);

                XMACH_DEF::x86_thread_state32_t state = mach._read_x86_thread_state32_t(nDataOffset);

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

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_x86_thread_state32_t_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_unix_thread_x86_32], ui->widgetHex_unix_thread_x86_32);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_unix_thread_x86_64) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_unix_thread_x86_64, ui->tableWidget_unix_thread_x86_64, N_mach_unix_thread_x86_64::records,
                                  g_lineEdit_mach_unix_thread_x86_64, N_mach_unix_thread_x86_64::__data_size, 0, nDataOffset);

                blockSignals(true);

                XMACH_DEF::x86_thread_state64_t state = mach._read_x86_thread_state64_t(nDataOffset);

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

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_x86_thread_state64_t_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_unix_thread_x86_64], ui->widgetHex_unix_thread_x86_64);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_unix_thread_arm_32) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_unix_thread_arm_32, ui->tableWidget_unix_thread_arm_32, N_mach_unix_thread_arm_32::records,
                                  g_lineEdit_mach_unix_thread_arm_32, N_mach_unix_thread_arm_32::__data_size, 0, nDataOffset);

                blockSignals(true);

                XMACH_DEF::arm_thread_state32_t state = mach._read_arm_thread_state32_t(nDataOffset);

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

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_arm_thread_state32_t_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_unix_thread_arm_32], ui->widgetHex_unix_thread_arm_32);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_unix_thread_arm_64) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_unix_thread_arm_64, ui->tableWidget_unix_thread_arm_64, N_mach_unix_thread_arm_64::records,
                                  g_lineEdit_mach_unix_thread_arm_64, N_mach_unix_thread_arm_64::__data_size, 0, nDataOffset);

                blockSignals(true);

                XMACH_DEF::arm_thread_state64_t state = mach._read_arm_thread_state64_t(nDataOffset);

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

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_arm_thread_state64_t_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_unix_thread_arm_64], ui->widgetHex_unix_thread_arm_64);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_unix_thread_m68k_32) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_unix_thread_m68k_32, ui->tableWidget_unix_thread_m68k_32, N_mach_unix_thread_m68k_32::records,
                                  g_lineEdit_mach_unix_thread_m68k_32, N_mach_unix_thread_m68k_32::__data_size, 0, nDataOffset);

                blockSignals(true);

                XMACH_DEF::m68k_thread_state32_t state = mach._read_m68k_thread_state32_t(nDataOffset);

                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::dreg0]->setValue(state.dreg[0]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::dreg1]->setValue(state.dreg[1]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::dreg2]->setValue(state.dreg[2]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::dreg3]->setValue(state.dreg[3]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::dreg4]->setValue(state.dreg[4]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::dreg5]->setValue(state.dreg[5]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::dreg6]->setValue(state.dreg[6]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::dreg7]->setValue(state.dreg[7]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::areg0]->setValue(state.areg[0]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::areg1]->setValue(state.areg[1]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::areg2]->setValue(state.areg[2]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::areg3]->setValue(state.areg[3]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::areg4]->setValue(state.areg[4]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::areg5]->setValue(state.areg[5]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::areg6]->setValue(state.areg[6]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::areg7]->setValue(state.areg[7]);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::pad0]->setValue(state.pad0);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::sr]->setValue(state.sr);
                g_lineEdit_mach_unix_thread_m68k_32[N_mach_unix_thread_m68k_32::pc]->setValue(state.pc);

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_m68k_thread_state32_t_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_unix_thread_m68k_32], ui->widgetHex_unix_thread_m68k_32);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_mach_unix_thread_ppc_32) {
            if (!isInitPresent(sInit)) {
                createHeaderTable(SMACH::TYPE_mach_unix_thread_ppc_32, ui->tableWidget_unix_thread_ppc_32, N_mach_unix_thread_ppc_32::records,
                                  g_lineEdit_mach_unix_thread_ppc_32, N_mach_unix_thread_ppc_32::__data_size, 0, nDataOffset);

                blockSignals(true);

                XMACH_DEF::ppc_thread_state32_t state = mach._read_ppc_thread_state32_t(nDataOffset);

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

                qint64 nOffset = nDataOffset;
                qint64 nSize = mach.get_ppc_thread_state32_t_size();

                loadHexSubdevice(nOffset, nSize, nOffset, &g_subDevice[SMACH::TYPE_mach_unix_thread_arm_32], ui->widgetHex_unix_thread_arm_32);

                blockSignals(false);
            }
        } else if (nType == SMACH::TYPE_STRINGTABLE) {
            if (!isInitPresent(sInit)) {
                loadHexSubdevice(nDataOffset, nDataSize, nDataOffset, &g_subDevice[SMACH::TYPE_STRINGTABLE], ui->widgetHex_StringTable);
            }
        } else if (nType == SMACH::TYPE_SYMBOLTABLE) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_SYMBOLTABLE, &tvModel[SMACH::TYPE_SYMBOLTABLE], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_SYMBOLTABLE], ui->tableView_SymbolTable, nullptr, true);
            }
        } else if (nType == SMACH::TYPE_FUNCTIONS) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_FUNCTIONS, &tvModel[SMACH::TYPE_FUNCTIONS], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_FUNCTIONS], ui->tableView_Functions, nullptr, false);
            }
        } else if (nType == SMACH::TYPE_DICE) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_DICE, &tvModel[SMACH::TYPE_DICE], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_DICE], ui->tableView_data_in_code_entry, nullptr, false);
            }
        } else if (nType == SMACH::TYPE_DYLD_INFO_rebase) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_DYLD_INFO_rebase, &tvModel[SMACH::TYPE_DYLD_INFO_rebase], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_DYLD_INFO_rebase], ui->tableView_DYLD_INFO_rebase, nullptr, true);

                connect(ui->tableView_DYLD_INFO_rebase->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_DYLD_INFO_rebase_currentRowChanged(QModelIndex, QModelIndex)));

                loadHexSubdevice(nDataOffset, nDataSize, nDataOffset, &g_subDevice[SMACH::TYPE_DYLD_INFO_rebase], ui->widgetHex_DYLD_INFO_rebase);
            }
        } else if (nType == SMACH::TYPE_DYLD_INFO_bind) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_DYLD_INFO_bind, &tvModel[SMACH::TYPE_DYLD_INFO_bind], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_DYLD_INFO_bind], ui->tableView_DYLD_INFO_bind, nullptr, true);

                connect(ui->tableView_DYLD_INFO_bind->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_DYLD_INFO_bind_currentRowChanged(QModelIndex, QModelIndex)));

                loadHexSubdevice(nDataOffset, nDataSize, nDataOffset, &g_subDevice[SMACH::TYPE_DYLD_INFO_bind], ui->widgetHex_DYLD_INFO_bind);
            }
        } else if (nType == SMACH::TYPE_DYLD_INFO_weak_bind) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_DYLD_INFO_weak_bind, &tvModel[SMACH::TYPE_DYLD_INFO_weak_bind], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_DYLD_INFO_weak_bind], ui->tableView_DYLD_INFO_weak_bind, nullptr, true);

                connect(ui->tableView_DYLD_INFO_weak_bind->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_DYLD_INFO_weak_bind_currentRowChanged(QModelIndex, QModelIndex)));

                loadHexSubdevice(nDataOffset, nDataSize, nDataOffset, &g_subDevice[SMACH::TYPE_DYLD_INFO_weak_bind], ui->widgetHex_DYLD_INFO_weak_bind);
            }
        } else if (nType == SMACH::TYPE_DYLD_INFO_lazy_bind) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_DYLD_INFO_lazy_bind, &tvModel[SMACH::TYPE_DYLD_INFO_lazy_bind], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_DYLD_INFO_lazy_bind], ui->tableView_DYLD_INFO_lazy_bind, nullptr, true);

                connect(ui->tableView_DYLD_INFO_lazy_bind->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_DYLD_INFO_lazy_bind_currentRowChanged(QModelIndex, QModelIndex)));

                loadHexSubdevice(nDataOffset, nDataSize, nDataOffset, &g_subDevice[SMACH::TYPE_DYLD_INFO_lazy_bind], ui->widgetHex_DYLD_INFO_lazy_bind);
            }
        } else if (nType == SMACH::TYPE_DYLD_INFO_export) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_DYLD_INFO_export, &tvModel[SMACH::TYPE_DYLD_INFO_export], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_DYLD_INFO_export], ui->tableView_DYLD_INFO_export, nullptr, true);

                connect(ui->tableView_DYLD_INFO_export->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this,
                        SLOT(onTableView_DYLD_INFO_export_currentRowChanged(QModelIndex, QModelIndex)));

                loadHexSubdevice(nDataOffset, nDataSize, nDataOffset, &g_subDevice[SMACH::TYPE_DYLD_INFO_export], ui->widgetHex_DYLD_INFO_export);
            }
        } else if (nType == SMACH::TYPE_DYSYMTAB_toc) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_DYSYMTAB_toc, &tvModel[SMACH::TYPE_DYSYMTAB_toc], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_DYSYMTAB_toc], ui->tableView_DYSYMTAB_toc, nullptr, false);
            }
        } else if (nType == SMACH::TYPE_DYSYMTAB_modtab) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_DYSYMTAB_modtab, &tvModel[SMACH::TYPE_DYSYMTAB_modtab], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_DYSYMTAB_modtab], ui->tableView_DYSYMTAB_modtab, nullptr, false);
            }
        } else if (nType == SMACH::TYPE_DYSYMTAB_extrefsyms) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_DYSYMTAB_extrefsyms, &tvModel[SMACH::TYPE_DYSYMTAB_extrefsyms], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_DYSYMTAB_extrefsyms], ui->tableView_DYSYMTAB_extrefsyms, nullptr, false);
            }
        } else if (nType == SMACH::TYPE_DYSYMTAB_indirectsyms) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_DYSYMTAB_indirectsyms, &tvModel[SMACH::TYPE_DYSYMTAB_indirectsyms], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_DYSYMTAB_indirectsyms], ui->tableView_DYSYMTAB_indirectsyms, nullptr, true);
            }
        } else if (nType == SMACH::TYPE_DYSYMTAB_extrel) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_DYSYMTAB_extrel, &tvModel[SMACH::TYPE_DYSYMTAB_extrel], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_DYSYMTAB_extrel], ui->tableView_DYSYMTAB_extrel, nullptr, false);
            }
        } else if (nType == SMACH::TYPE_DYSYMTAB_locrel) {
            if (!isInitPresent(sInit)) {
                MACHProcessData machProcessData(SMACH::TYPE_DYSYMTAB_locrel, &tvModel[SMACH::TYPE_DYSYMTAB_locrel], &mach, nDataOffset, nDataSize);

                ajustTableView(&machProcessData, &tvModel[SMACH::TYPE_DYSYMTAB_locrel], ui->tableView_DYSYMTAB_locrel, nullptr, false);
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }

    addInit(sInit);
}

void MACHWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget = qobject_cast<QWidget *>(sender());
    qint32 nStype = pWidget->property("STYPE").toInt();
    qint32 nNdata = pWidget->property("NDATA").toInt();

    switch (nStype) {
        case SMACH::TYPE_mach_header:
            switch (nNdata) {
                case N_mach_header::magic:
                    g_lineEdit_mach_header[N_mach_header::magic]->setValue((quint32)nValue);
                    break;
                case N_mach_header::cputype:
                    g_lineEdit_mach_header[N_mach_header::cputype]->setValue((quint32)nValue);
                    break;
                case N_mach_header::filetype:
                    g_lineEdit_mach_header[N_mach_header::filetype]->setValue((quint32)nValue);
                    break;
                case N_mach_header::flags:
                    g_lineEdit_mach_header[N_mach_header::flags]->setValue((quint32)nValue);
                    break;
            }

            break;
    }
}

void MACHWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *pItemCurrent, QTreeWidgetItem *pItemPrevious)
{
    Q_UNUSED(pItemPrevious)

    if (pItemCurrent) {
        reloadData();
        addPage(pItemCurrent);
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

    QTimer::singleShot(1000, this, SLOT(enableButton()));
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

    setHeaderTableSelection(ui->widgetHex_mach_header, ui->tableWidget_mach_header);
}

void MACHWidget::on_tableWidget_dyld_info_only_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_dyld_info_only, ui->tableWidget_dyld_info_only);
}

void MACHWidget::on_tableWidget_symtab_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_symtab, ui->tableWidget_symtab);
}

void MACHWidget::on_tableWidget_dysymtab_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_dysymtab, ui->tableWidget_dysymtab);
}

void MACHWidget::on_tableWidget_version_min_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_version_min, ui->tableWidget_version_min);
}

void MACHWidget::on_tableWidget_build_version_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_build_version, ui->tableWidget_build_version);
}

void MACHWidget::on_tableWidget_source_version_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_source_version, ui->tableWidget_source_version);
}

void MACHWidget::on_tableWidget_encryption_info_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_encryption_info, ui->tableWidget_encryption_info);
}

void MACHWidget::on_tableWidget_function_starts_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_function_starts, ui->tableWidget_function_starts);
}

void MACHWidget::on_tableWidget_data_in_code_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_data_in_code, ui->tableWidget_data_in_code);
}

void MACHWidget::on_tableWidget_code_signature_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_code_signature, ui->tableWidget_code_signature);
}

void MACHWidget::on_tableWidget_SuperBlob_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_SuperBlob, ui->tableWidget_SuperBlob);
}

void MACHWidget::on_tableWidget_main_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_main, ui->tableWidget_main);
}

void MACHWidget::on_tableWidget_unix_thread_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_unix_thread, ui->tableWidget_unix_thread);
}

void MACHWidget::on_tableWidget_unix_thread_x86_32_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_unix_thread_x86_32, ui->tableWidget_unix_thread_x86_32);
}

void MACHWidget::on_tableWidget_unix_thread_x86_64_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_unix_thread_x86_64, ui->tableWidget_unix_thread_x86_64);
}

void MACHWidget::on_tableWidget_unix_thread_arm_32_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_unix_thread_arm_32, ui->tableWidget_unix_thread_arm_32);
}

void MACHWidget::on_tableWidget_unix_thread_arm_64_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_unix_thread_arm_64, ui->tableWidget_unix_thread_arm_64);
}

void MACHWidget::on_tableWidget_unix_thread_ppc_32_currentCellChanged(int nCurrentRow, int nCurrentColumn, int nPreviousRow, int nPreviousColumn)
{
    Q_UNUSED(nCurrentRow);
    Q_UNUSED(nCurrentColumn);
    Q_UNUSED(nPreviousRow);
    Q_UNUSED(nPreviousColumn);

    setHeaderTableSelection(ui->widgetHex_unix_thread_ppc_32, ui->tableWidget_unix_thread_ppc_32);
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

    loadHexSubdeviceByTableView(current.row(), SMACH::TYPE_mach_commands, ui->widgetHex_commands, ui->tableView_commands, &g_subDevice[SMACH::TYPE_mach_commands]);
}

void MACHWidget::onTableView_segments_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SMACH::TYPE_mach_segments, ui->widgetHex_segments, ui->tableView_segments, &g_subDevice[SMACH::TYPE_mach_segments]);
}

void MACHWidget::onTableView_sections_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SMACH::TYPE_mach_sections, ui->widgetHex_sections, ui->tableView_sections, &g_subDevice[SMACH::TYPE_mach_sections]);
}

void MACHWidget::onTableView_libraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SMACH::TYPE_mach_libraries, ui->widgetHex_libraries, ui->tableView_libraries, &g_subDevice[SMACH::TYPE_mach_libraries]);
}

void MACHWidget::onTableView_weak_libraries_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SMACH::TYPE_mach_weak_libraries, ui->widgetHex_weak_libraries, ui->tableView_weak_libraries,
                                &g_subDevice[SMACH::TYPE_mach_weak_libraries]);
}

void MACHWidget::onTableView_id_library_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SMACH::TYPE_mach_id_library, ui->widgetHex_id_library, ui->tableView_id_library, &g_subDevice[SMACH::TYPE_mach_id_library]);
}

void MACHWidget::onTableView_LOADFVMLIB_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SMACH::TYPE_mach_LOADFVMLIB, ui->widgetHex_LOADFVMLIB, ui->tableView_LOADFVMLIB, &g_subDevice[SMACH::TYPE_mach_LOADFVMLIB]);
}

void MACHWidget::onTableView_IDFVMLIB_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    loadHexSubdeviceByTableView(current.row(), SMACH::TYPE_mach_IDFVMLIB, ui->widgetHex_IDFVMLIB, ui->tableView_IDFVMLIB, &g_subDevice[SMACH::TYPE_mach_IDFVMLIB]);
}

void MACHWidget::on_tableView_commands_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editCommandHeader();
}

void MACHWidget::on_tableView_commands_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_commands->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
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
    qint32 nRow = ui->tableView_segments->currentIndex().row();

    if (nRow != -1) {
        bool bIsEnable = getTableViewItemSize(ui->tableView_segments);

        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editSegmentHeader()));
        contextMenu.addAction(&actionEdit);

        QAction actionHex(tr("Hex"), this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(segmentHex()));
        actionHex.setEnabled(bIsEnable);
        contextMenu.addAction(&actionHex);

        QAction actionDisasm(tr("Disasm"), this);
        connect(&actionDisasm, SIGNAL(triggered()), this, SLOT(segmentDisasm()));
        actionDisasm.setEnabled(bIsEnable);
        contextMenu.addAction(&actionDisasm);

        QAction actionEntropy(tr("Entropy"), this);
        connect(&actionEntropy, SIGNAL(triggered()), this, SLOT(segmentEntropy()));
        actionEntropy.setEnabled(bIsEnable);
        contextMenu.addAction(&actionEntropy);

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
    qint32 nRow = ui->tableView_sections->currentIndex().row();

    if (nRow != -1) {
        bool bIsEnable = getTableViewItemSize(ui->tableView_sections);

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
    qint32 nRow = ui->tableView_libraries->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
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
    qint32 nRow = ui->tableView_weak_libraries->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
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
    qint32 nRow = ui->tableView_id_library->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editIdLibraryHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_id_library->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_LOADFVMLIB_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editLOADFVMLIBHeader();
}

void MACHWidget::on_tableView_LOADFVMLIB_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_LOADFVMLIB->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editLOADFVMLIBHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_LOADFVMLIB->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_IDFVMLIB_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editIDFVMLIBHeader();
}

void MACHWidget::on_tableView_IDFVMLIB_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_IDFVMLIB->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editIDFVMLIBHeader()));
        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_IDFVMLIB->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_SymbolTable_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editSymbolHeader();
}

void MACHWidget::on_tableView_SymbolTable_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_SymbolTable->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editSymbolHeader()));

        QAction actionDemangle(tr("Demangle"), this);
        connect(&actionDemangle, SIGNAL(triggered()), this, SLOT(symbolDemangle()));

        contextMenu.addAction(&actionEdit);
        contextMenu.addAction(&actionDemangle);

        contextMenu.exec(ui->tableView_SymbolTable->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_Functions_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    functionDisasm();
}

void MACHWidget::on_tableView_Functions_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_Functions->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionHex(tr("Hex"), this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(functionHex()));

        QAction actionDisasm(tr("Disasm"), this);
        connect(&actionDisasm, SIGNAL(triggered()), this, SLOT(functionDisasm()));

        QAction actionDemangle(tr("Demangle"), this);
        connect(&actionDemangle, SIGNAL(triggered()), this, SLOT(functionDemangle()));

        contextMenu.addAction(&actionHex);
        contextMenu.addAction(&actionDisasm);
        contextMenu.addAction(&actionDemangle);

        contextMenu.exec(ui->tableView_Functions->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_data_in_code_entry_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editDiceHeader();
}

void MACHWidget::on_tableView_data_in_code_entry_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_data_in_code_entry->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editDiceHeader()));

        QAction actionHex(tr("Hex"), this);
        connect(&actionHex, SIGNAL(triggered()), this, SLOT(diceHex()));

        contextMenu.addAction(&actionEdit);
        contextMenu.addAction(&actionHex);

        contextMenu.exec(ui->tableView_data_in_code_entry->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_DYSYMTAB_modtab_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editModTabHeader();
}

void MACHWidget::on_tableView_DYSYMTAB_modtab_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_DYSYMTAB_modtab->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editModTabHeader()));

        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_DYSYMTAB_modtab->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_DYSYMTAB_toc_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editTocHeader();
}

void MACHWidget::on_tableView_DYSYMTAB_toc_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_DYSYMTAB_toc->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editTocHeader()));

        QAction actionDemangle(tr("Demangle"), this);
        connect(&actionDemangle, SIGNAL(triggered()), this, SLOT(tocDemangle()));

        contextMenu.addAction(&actionEdit);
        contextMenu.addAction(&actionDemangle);

        contextMenu.exec(ui->tableView_DYSYMTAB_toc->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_DYSYMTAB_extrel_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editExtrelHeader();
}

void MACHWidget::on_tableView_DYSYMTAB_extrel_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_DYSYMTAB_extrel->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editExtrelHeader()));

        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_DYSYMTAB_extrel->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_DYSYMTAB_locrel_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editLocrelHeader();
}

void MACHWidget::on_tableView_DYSYMTAB_locrel_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_DYSYMTAB_locrel->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editLocrelHeader()));

        contextMenu.addAction(&actionEdit);

        contextMenu.exec(ui->tableView_DYSYMTAB_locrel->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_DYSYMTAB_indirectsyms_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editIndirectSymbolHeader();
}

void MACHWidget::on_tableView_DYSYMTAB_indirectsyms_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_DYSYMTAB_indirectsyms->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editIndirectSymbolHeader()));

        QAction actionDemangle(tr("Demangle"), this);
        connect(&actionDemangle, SIGNAL(triggered()), this, SLOT(indirectsymsDemangle()));

        contextMenu.addAction(&actionEdit);
        contextMenu.addAction(&actionDemangle);

        contextMenu.exec(ui->tableView_DYSYMTAB_indirectsyms->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::on_tableView_DYSYMTAB_extrefsyms_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    editExtRefSymbolHeader();
}

void MACHWidget::on_tableView_DYSYMTAB_extrefsyms_customContextMenuRequested(const QPoint &pos)
{
    qint32 nRow = ui->tableView_DYSYMTAB_extrefsyms->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QAction actionEdit(tr("Edit"), this);
        connect(&actionEdit, SIGNAL(triggered()), this, SLOT(editExtRefSymbolHeader()));

        QAction actionDemangle(tr("Demangle"), this);
        connect(&actionDemangle, SIGNAL(triggered()), this, SLOT(extrefsymsDemangle()));

        contextMenu.addAction(&actionEdit);
        contextMenu.addAction(&actionDemangle);

        contextMenu.exec(ui->tableView_DYSYMTAB_extrefsyms->viewport()->mapToGlobal(pos));
    }
}

void MACHWidget::onTableView_DYLD_INFO_rebase_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    setHexSubdeviceByTableView(current.row(), SMACH::TYPE_DYLD_INFO_rebase, ui->widgetHex_DYLD_INFO_rebase, ui->tableView_DYLD_INFO_rebase);
}

void MACHWidget::onTableView_DYLD_INFO_bind_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    setHexSubdeviceByTableView(current.row(), SMACH::TYPE_DYLD_INFO_bind, ui->widgetHex_DYLD_INFO_bind, ui->tableView_DYLD_INFO_bind);
}

void MACHWidget::onTableView_DYLD_INFO_weak_bind_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    setHexSubdeviceByTableView(current.row(), SMACH::TYPE_DYLD_INFO_weak_bind, ui->widgetHex_DYLD_INFO_weak_bind, ui->tableView_DYLD_INFO_weak_bind);
}

void MACHWidget::onTableView_DYLD_INFO_lazy_bind_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    setHexSubdeviceByTableView(current.row(), SMACH::TYPE_DYLD_INFO_lazy_bind, ui->widgetHex_DYLD_INFO_lazy_bind, ui->tableView_DYLD_INFO_lazy_bind);
}

void MACHWidget::onTableView_DYLD_INFO_export_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    setHexSubdeviceByTableView(current.row(), SMACH::TYPE_DYLD_INFO_export, ui->widgetHex_DYLD_INFO_export, ui->tableView_DYLD_INFO_export);
}

void MACHWidget::editCommandHeader()
{
    showSectionHeader(SMACH::TYPE_mach_commands, ui->tableView_commands);
}

void MACHWidget::editSegmentHeader()
{
    showSectionHeader(SMACH::TYPE_mach_segments, ui->tableView_segments);
}

void MACHWidget::segmentHex()
{
    showSectionHex(ui->tableView_segments);
}

void MACHWidget::segmentDisasm()
{
    showSectionDisasm(ui->tableView_segments);
}

void MACHWidget::segmentEntropy()
{
    showSectionEntropy(ui->tableView_segments);
}

void MACHWidget::editSectionHeader()
{
    showSectionHeader(SMACH::TYPE_mach_sections, ui->tableView_sections);
}

void MACHWidget::sectionHex()
{
    showSectionHex(ui->tableView_sections);
}

void MACHWidget::sectionDisasm()
{
    showSectionDisasm(ui->tableView_sections);
}

void MACHWidget::sectionEntropy()
{
    showSectionEntropy(ui->tableView_sections);
}

void MACHWidget::editLibraryHeader()
{
    showSectionHeader(SMACH::TYPE_mach_libraries, ui->tableView_libraries);
}

void MACHWidget::editWeakLibraryHeader()
{
    showSectionHeader(SMACH::TYPE_mach_weak_libraries, ui->tableView_weak_libraries);
}

void MACHWidget::editIdLibraryHeader()
{
    showSectionHeader(SMACH::TYPE_mach_id_library, ui->tableView_id_library);
}

void MACHWidget::editIDFVMLIBHeader()
{
    showSectionHeader(SMACH::TYPE_mach_IDFVMLIB, ui->tableView_IDFVMLIB);
}

void MACHWidget::editLOADFVMLIBHeader()
{
    showSectionHeader(SMACH::TYPE_mach_LOADFVMLIB, ui->tableView_LOADFVMLIB);
}

void MACHWidget::editSymbolHeader()
{
    showSectionHeader(SMACH::TYPE_SYMBOLTABLE, ui->tableView_SymbolTable);
}

void MACHWidget::functionHex()
{
    showSectionHex(ui->tableView_Functions);
}

void MACHWidget::functionDisasm()
{
    showSectionDisasm(ui->tableView_Functions);
}

void MACHWidget::functionDemangle()
{
    showTableViewDemangle(ui->tableView_Functions, 3);
}

void MACHWidget::editDiceHeader()
{
    showSectionHeader(SMACH::TYPE_DICE, ui->tableView_data_in_code_entry);
}

void MACHWidget::editModTabHeader()
{
    showSectionHeader(SMACH::TYPE_DYSYMTAB_modtab, ui->tableView_DYSYMTAB_modtab);
}

void MACHWidget::editTocHeader()
{
    showSectionHeader(SMACH::TYPE_DYSYMTAB_toc, ui->tableView_DYSYMTAB_toc);
}

void MACHWidget::editExtrelHeader()
{
    showSectionHeader(SMACH::TYPE_DYSYMTAB_extrel, ui->tableView_DYSYMTAB_extrel);
}

void MACHWidget::editLocrelHeader()
{
    showSectionHeader(SMACH::TYPE_DYSYMTAB_locrel, ui->tableView_DYSYMTAB_locrel);
}

void MACHWidget::editIndirectSymbolHeader()
{
    showSectionHeader(SMACH::TYPE_DYSYMTAB_indirectsyms, ui->tableView_DYSYMTAB_indirectsyms);
}

void MACHWidget::editExtRefSymbolHeader()
{
    showSectionHeader(SMACH::TYPE_DYSYMTAB_extrefsyms, ui->tableView_DYSYMTAB_extrefsyms);
}

void MACHWidget::diceHex()
{
    showSectionHex(ui->tableView_data_in_code_entry);
}

void MACHWidget::showSectionHeader(int nType, QTableView *pTableView)
{
    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_HEADEROFFSET).toLongLong();

        MACHSectionHeaderWidget *pSectionHeaderWidget = new MACHSectionHeaderWidget(getDevice(), getOptions(), (quint32)nRow, nOffset, nType, this);

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

void MACHWidget::symbolDemangle()
{
    showTableViewDemangle(ui->tableView_SymbolTable, N_mach_nlist::n_value + 2);
}

void MACHWidget::indirectsymsDemangle()
{
    showTableViewDemangle(ui->tableView_DYSYMTAB_indirectsyms, 2);
}

void MACHWidget::tocDemangle()
{
    showTableViewDemangle(ui->tableView_DYSYMTAB_toc, 3);
}

void MACHWidget::extrefsymsDemangle()
{
    showTableViewDemangle(ui->tableView_DYSYMTAB_extrefsyms, 3);
}

void MACHWidget::on_pushButtonHex_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SMACH::TYPE_HEX);
}

void MACHWidget::on_pushButtonDisasm_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SMACH::TYPE_DISASM);
}

void MACHWidget::on_pushButtonStrings_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SMACH::TYPE_STRINGS);
}

void MACHWidget::on_pushButtonMemoryMap_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SMACH::TYPE_MEMORYMAP);
}

void MACHWidget::on_pushButtonEntropy_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SMACH::TYPE_ENTROPY);
}

void MACHWidget::on_pushButtonHeuristicScan_clicked()
{
    setTreeItem(ui->treeWidgetNavi, SMACH::TYPE_HEURISTICSCAN);
}
