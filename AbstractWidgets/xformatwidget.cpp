/* Copyright (c) 2017-2024 hors<horsicq@gmail.com>
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
#include "xformatwidget.h"

XFormatWidget::XFormatWidget(QWidget *pParent) : XShortcutsWidget(pParent)
{
    g_pDevice = nullptr;
    g_fwOptions = {};
    g_bAddPageEnable = true;
    g_nPageIndex = 0;  // TODO Check
    g_pXInfoDB = nullptr;
    g_nDisamInitAddress = -1;
    g_fileType = XBinary::FT_UNKNOWN;
    g_mode = XBinary::MODE_UNKNOWN;
    g_endian = XBinary::ENDIAN_UNKNOWN;

    g_colDisabled = QWidget::palette().color(QPalette::Window);
    g_colEnabled = QWidget::palette().color(QPalette::BrightText);

    XFormatWidget::setReadonly(true);
}

XFormatWidget::XFormatWidget(QIODevice *pDevice, XFW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent) : XFormatWidget(pParent)
{
    XFormatWidget::setData(pDevice, options, nNumber, nOffset, nType);
}

XFormatWidget::~XFormatWidget()
{
    if (g_sFileName != "") {
        QFile *pFile = dynamic_cast<QFile *>(g_pDevice);

        if (pFile) {
            pFile->close();
        }
    }
}

void XFormatWidget::setXInfoDB(XInfoDB *pXInfoDB)
{
    this->g_pXInfoDB = pXInfoDB;
}

XInfoDB *XFormatWidget::getXInfoDB()
{
    return g_pXInfoDB;
}

void XFormatWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobalChildren(this, pShortcuts, pXOptions);
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void XFormatWidget::adjustView()
{
    getGlobalOptions()->adjustWidget(this, XOptions::ID_VIEW_FONT_CONTROLS);

    {
        qint32 nNumberOfRecords = listRecWidget.count();

        for (qint32 i = 0; i < nNumberOfRecords; i++) {
            RECWIDGET recWidget = listRecWidget.at(i);

            if (recWidget.pLineEdit) {
                getGlobalOptions()->adjustWidget(recWidget.pLineEdit, XOptions::ID_VIEW_FONT_TABLEVIEWS);
            }

            if (recWidget.pWidget) {
                QList<QWidget *> listWidgets = recWidget.pWidget->findChildren<QWidget *>();

                qint32 nNumberOfWidgets = listWidgets.count();

                for (qint32 i = 0; i < nNumberOfWidgets; i++) {
                    getGlobalOptions()->adjustWidget(listWidgets.at(i), XOptions::ID_VIEW_FONT_TABLEVIEWS);
                }
            }
        }
    }
}

void XFormatWidget::setData(QIODevice *pDevice, XFW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType)
{
    g_pDevice = pDevice;

    setData(options, nNumber, nOffset, nType);
}

void XFormatWidget::setData(const QString &sFileName, XFW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType)
{
    g_sFileName = sFileName;

    QFile *pFile = new QFile(sFileName);  // TODO delete !!! or use global

    XBinary::tryToOpen(pFile);

    setData(pFile, options, nNumber, nOffset, nType);
}

void XFormatWidget::setData(XFW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType)
{
    g_nNumber = nNumber;
    g_nOffset = nOffset;
    g_nType = nType;

    g_listPages.clear();
    g_nPageIndex = 0;
    g_bAddPageEnable = true;

#ifdef QT_DEBUG
    if (options.nImageBase == 0) {
        qDebug("Check ImageBase!!!");
    }
#endif

    setOptions(options);
}

void XFormatWidget::setCwOptions(XFW_DEF::CWOPTIONS cwOptions, bool bReload)
{
    g_cwOptions = cwOptions;

    XFW_DEF::OPTIONS formatOptions = {};

    formatOptions.bIsImage = cwOptions.bIsImage;
    formatOptions.nImageBase = cwOptions.nImageBase;
    formatOptions.fileType = cwOptions.fileType;

    setData(cwOptions.pDevice, formatOptions, 0, 0, 0);

    if (bReload) {
        reloadData(false);
    }
}

XFW_DEF::CWOPTIONS *XFormatWidget::getCwOptions()
{
    return &g_cwOptions;
}

void XFormatWidget::setFileType(XBinary::FT fileType)
{
    g_fileType = fileType;
}

XBinary::FT XFormatWidget::getFileType()
{
    return g_fileType;
}

void XFormatWidget::setMode(XBinary::MODE mode)
{
    g_mode = mode;
}

XBinary::MODE XFormatWidget::getMode()
{
    return g_mode;
}

void XFormatWidget::setEndian(XBinary::ENDIAN endian)
{
    g_endian = endian;
}

XBinary::ENDIAN XFormatWidget::getEndian()
{
    return g_endian;
}

QIODevice *XFormatWidget::getDevice()
{
    return this->g_pDevice;
}

void XFormatWidget::setOptions(XFW_DEF::OPTIONS options)
{
    g_fwOptions = options;
}

XFW_DEF::OPTIONS XFormatWidget::getOptions()
{
    return g_fwOptions;
}

quint32 XFormatWidget::getNumber()
{
    return g_nNumber;
}

qint64 XFormatWidget::getOffset()
{
    return g_nOffset;
}

qint32 XFormatWidget::getType()
{
    return g_nType;
}

QTreeWidgetItem *XFormatWidget::createNewItem(XFW_DEF::TYPE type, XFW_DEF::WIDGETMODE widgetMode, XOptions::ICONTYPE iconType, qint64 nOffset, qint64 nSize,
                                              qint64 nCount, QVariant var1, QVariant var2, XBinary::MODE mode, XBinary::ENDIAN endian, QString sTitle)
{
    QTreeWidgetItem *pResult = new QTreeWidgetItem;

    QString _sTitle = sTitle;

    if (_sTitle == "") {
        _sTitle = getTypeTitle(type, mode, endian);
    }

    pResult->setText(0, _sTitle);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_TYPE, type);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_WIDGETMODE, widgetMode);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_OFFSET, nOffset);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_SIZE, nSize);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_COUNT, nCount);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_VAR1, var1);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_VAR2, var2);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_NAME, sTitle);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_MODE, mode);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_ENDIAN, endian);

    XOptions::adjustTreeWidgetItem(pResult, iconType);

    return pResult;
}

QString XFormatWidget::getTypeTitle(XFW_DEF::TYPE type, XBinary::MODE mode, XBinary::ENDIAN endian)
{
    Q_UNUSED(endian)

    QString sResult;

    if (type == XFW_DEF::TYPE_INFO) {
        sResult = tr("Info");
    } else if (type == XFW_DEF::TYPE_VISUALIZATION) {
        sResult = tr("Visualization");
    } else if (type == XFW_DEF::TYPE_VIRUSTOTAL) {
        sResult = QString("VirusTotal");
    } else if (type == XFW_DEF::TYPE_HEX) {
        sResult = tr("Hex");
    } else if (type == XFW_DEF::TYPE_DISASM) {
        sResult = tr("Disasm");
    } else if (type == XFW_DEF::TYPE_HASH) {
        sResult = tr("Hash");
    } else if (type == XFW_DEF::TYPE_STRINGS) {
        sResult = tr("Strings");
    } else if (type == XFW_DEF::TYPE_SIGNATURES) {
        sResult = tr("Signatures");
    } else if (type == XFW_DEF::TYPE_MEMORYMAP) {
        sResult = tr("Memory map");
    } else if (type == XFW_DEF::TYPE_ENTROPY) {
        sResult = tr("Entropy");
    } else if (type == XFW_DEF::TYPE_NFDSCAN) {
        sResult = QString("Nauz File Detector (NFD)");
    } else if (type == XFW_DEF::TYPE_EXTRACTOR) {
        sResult = tr("Extractor");
    } else if (type == XFW_DEF::TYPE_SEARCH) {
        sResult = tr("Search");
    } else if (type == XFW_DEF::TYPE_DIESCAN) {
        sResult = QString("Detect It Easy (DiE)");
    } else if (type == XFW_DEF::TYPE_YARASCAN) {
        sResult = QString("Yara rules");
    } else if (type == XFW_DEF::TYPE_TOOLS) {
        sResult = tr("Tools");
    } else if (type == XFW_DEF::TYPE_GENERIC_STRINGTABLE_ANSI) {
        sResult = tr("String table");
    } else if (type == XFW_DEF::TYPE_Elf32_Ehdr) {
        sResult = QString("Elf32_Ehdr");
    } else if (type == XFW_DEF::TYPE_Elf64_Ehdr) {
        sResult = QString("Elf64_Ehdr");
    } else if (type == XFW_DEF::TYPE_MSDOS_EXE_file) {
        sResult = QString("EXE_file");
    } else if ((type == XFW_DEF::TYPE_MSDOS_IMAGE_DOS_HEADER) || (type == XFW_DEF::TYPE_NE_IMAGE_DOS_HEADER) || (type == XFW_DEF::TYPE_LE_IMAGE_DOS_HEADER) ||
               (type == XFW_DEF::TYPE_LX_IMAGE_DOS_HEADER) || (type == XFW_DEF::TYPE_PE_IMAGE_DOS_HEADER)) {
        sResult = QString("IMAGE_DOS_HEADER");
    } else if (type == XFW_DEF::TYPE_MACH_mach_header) {
        sResult = QString("mach_header");
    } else if (type == XFW_DEF::TYPE_MACH_mach_header_64) {
        sResult = QString("mach_header_64");
    } else if (type == XFW_DEF::TYPE_MACH_load_command) {
        sResult = QString("load_command");
    } else if (type == XFW_DEF::TYPE_segment_command) {
        sResult = QString("segment_command");
    } else if (type == XFW_DEF::TYPE_segment_command_64) {
        sResult = QString("segment_command_64");
    } else if (type == XFW_DEF::TYPE_dylib_command) {
        sResult = QString("dylib_command");
    } else if (type == XFW_DEF::TYPE_rpath_command) {
        sResult = QString("rpath_command");
    } else if (type == XFW_DEF::TYPE_sub_umbrella_command) {
        sResult = QString("sub_umbrella_command");
    } else if (type == XFW_DEF::TYPE_sub_client_command) {
        sResult = QString("sub_client_command");
    } else if (type == XFW_DEF::TYPE_sub_library_command) {
        sResult = QString("sub_library_command");
    } else if (type == XFW_DEF::TYPE_symtab_command) {
        sResult = QString("symtab_command");
    } else if (type == XFW_DEF::TYPE_dysymtab_command) {
        sResult = QString("dysymtab_command");
    } else if (type == XFW_DEF::TYPE_segment_split_info_command) {
        sResult = QString("segment_split_info_command");
    } else if (type == XFW_DEF::TYPE_atom_info_command) {
        sResult = QString("atom_info_command");
    } else if (type == XFW_DEF::TYPE_function_starts_command) {
        sResult = QString("function_starts_command");
    } else if (type == XFW_DEF::TYPE_dyld_exports_trie_command) {
        sResult = QString("dyld_exports_trie_command");
    } else if (type == XFW_DEF::TYPE_dyld_chained_fixups_command) {
        sResult = QString("dyld_chained_fixups_command");
    } else if (type == XFW_DEF::TYPE_encryption_info_command) {
        sResult = QString("encryption_info_command");
    } else if (type == XFW_DEF::TYPE_encryption_info_command_64) {
        sResult = QString("encryption_info_command_64");
    } else if (type == XFW_DEF::TYPE_routines_command) {
        sResult = QString("routines_command");
    } else if (type == XFW_DEF::TYPE_routines_command_64) {
        sResult = QString("routines_command_64");
    } else if (type == XFW_DEF::TYPE_dyld_info_command) {
        sResult = QString("dyld_info_command");
    } else if (type == XFW_DEF::TYPE_version_min_command) {
        sResult = QString("version_min_command");
    } else if (type == XFW_DEF::TYPE_uuid_command) {
        sResult = QString("uuid_command");
    } else if (type == XFW_DEF::TYPE_build_version_command) {
        sResult = QString("build_version_command");
    } else if (type == XFW_DEF::TYPE_main_command) {
        sResult = QString("main_command");
    } else if (type == XFW_DEF::TYPE_fileset_entry_command) {
        sResult = QString("fileset_entry_command");
    } else if (type == XFW_DEF::TYPE_source_version_command) {
        sResult = QString("source_version_command");
    } else if (type == XFW_DEF::TYPE_dylinker_command) {
        sResult = QString("dylinker_command");
    } else if (type == XFW_DEF::TYPE_data_in_code_command) {
        sResult = QString("data_in_code_command");
    } else if (type == XFW_DEF::TYPE_code_signature_command) {
        sResult = QString("code_signature_command");
    } else if (type == XFW_DEF::TYPE_MACH_nlist) {
        sResult = QString("nlist");
    } else if (type == XFW_DEF::TYPE_MACH_nlist_64) {
        sResult = QString("nlist_64");
    } else if (type == XFW_DEF::TYPE_MACH_dyld_chained_fixups_header) {
        sResult = QString("dyld_chained_fixups_header");
    } else if (type == XFW_DEF::TYPE_MACH_SC_SuperBlob) {
        sResult = QString("__SC_SuperBlob");
    } else if (type == XFW_DEF::TYPE_MACH_CS_BlobIndex) {
        sResult = QString("CS_BlobIndex");
    } else if (type == XFW_DEF::TYPE_MACH_CS_CodeDirectory) {
        sResult = QString("CS_CodeDirectory");
    } else if (type == XFW_DEF::TYPE_MACH_section) {
        sResult = QString("section");
    } else if (type == XFW_DEF::TYPE_MACH_section_64) {
        sResult = QString("section_64");
    } else if (type == XFW_DEF::TYPE_DEX_HEADER) {
        sResult = QString("HEADER");
    }

#ifdef QT_DEBUG
    if (sResult == "") {
        qDebug("Check getTypeTitle!!!");
    }
#endif

    return sResult;
}

QString XFormatWidget::getTypeTitle(const XFW_DEF::CWOPTIONS *pCwOptions)
{
    return getTypeTitle(pCwOptions->_type, pCwOptions->mode, pCwOptions->endian);
}

QList<XFW_DEF::HEADER_RECORD> XFormatWidget::getHeaderRecords(const XFW_DEF::CWOPTIONS *pCwOptions, qint32 nLimit)
{
    QList<XFW_DEF::HEADER_RECORD> listResult;

    const XFW_DEF::HEADER_RECORD *pRecords = 0;
    qint32 nNumberOfRecords = 0;

    if (pCwOptions->_type == XFW_DEF::TYPE_MACH_mach_header) {
        pRecords = XTYPE_MACH::X_mach_header::records32;
        nNumberOfRecords = XTYPE_MACH::X_mach_header::__data_size - 1;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MACH_mach_header_64) {
        pRecords = XTYPE_MACH::X_mach_header::records64;
        nNumberOfRecords = XTYPE_MACH::X_mach_header::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MACH_load_command) {
        pRecords = XTYPE_MACH::X_load_commands::records;
        nNumberOfRecords = XTYPE_MACH::X_load_commands::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_segment_command) {
        pRecords = XTYPE_MACH::X_segment_command::records32;
        nNumberOfRecords = XTYPE_MACH::X_segment_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_segment_command_64) {
        pRecords = XTYPE_MACH::X_segment_command::records64;
        nNumberOfRecords = XTYPE_MACH::X_segment_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_dylib_command) {
        pRecords = XTYPE_MACH::X_dylib_command::records;
        nNumberOfRecords = XTYPE_MACH::X_dylib_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_rpath_command) {
        pRecords = XTYPE_MACH::X_rpath_command::records;
        nNumberOfRecords = XTYPE_MACH::X_rpath_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_sub_umbrella_command) {
        pRecords = XTYPE_MACH::X_sub_umbrella_command::records;
        nNumberOfRecords = XTYPE_MACH::X_sub_umbrella_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_sub_client_command) {
        pRecords = XTYPE_MACH::X_sub_client_command::records;
        nNumberOfRecords = XTYPE_MACH::X_sub_client_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_sub_library_command) {
        pRecords = XTYPE_MACH::X_sub_library_command::records;
        nNumberOfRecords = XTYPE_MACH::X_sub_library_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_symtab_command) {
        pRecords = XTYPE_MACH::X_symtab_command::records;
        nNumberOfRecords = XTYPE_MACH::X_symtab_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_dysymtab_command) {
        pRecords = XTYPE_MACH::X_dysymtab_command::records;
        nNumberOfRecords = XTYPE_MACH::X_dysymtab_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_segment_split_info_command) {
        pRecords = XTYPE_MACH::X_linkedit_data_command::records;
        nNumberOfRecords = XTYPE_MACH::X_linkedit_data_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_atom_info_command) {
        pRecords = XTYPE_MACH::X_linkedit_data_command::records;
        nNumberOfRecords = XTYPE_MACH::X_linkedit_data_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_function_starts_command) {
        pRecords = XTYPE_MACH::X_linkedit_data_command::records;
        nNumberOfRecords = XTYPE_MACH::X_linkedit_data_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_dyld_exports_trie_command) {
        pRecords = XTYPE_MACH::X_linkedit_data_command::records;
        nNumberOfRecords = XTYPE_MACH::X_linkedit_data_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_dyld_chained_fixups_command) {
        pRecords = XTYPE_MACH::X_linkedit_data_command::records;
        nNumberOfRecords = XTYPE_MACH::X_linkedit_data_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_encryption_info_command) {
        pRecords = XTYPE_MACH::X_encryption_info_command::records32;
        nNumberOfRecords = XTYPE_MACH::X_encryption_info_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_encryption_info_command_64) {
        pRecords = XTYPE_MACH::X_encryption_info_command::records64;
        nNumberOfRecords = XTYPE_MACH::X_encryption_info_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_routines_command) {
        pRecords = XTYPE_MACH::X_routines_command::records32;
        nNumberOfRecords = XTYPE_MACH::X_routines_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_routines_command_64) {
        pRecords = XTYPE_MACH::X_routines_command::records64;
        nNumberOfRecords = XTYPE_MACH::X_routines_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_dyld_info_command) {
        pRecords = XTYPE_MACH::X_dyld_info_command::records;
        nNumberOfRecords = XTYPE_MACH::X_dyld_info_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_version_min_command) {
        pRecords = XTYPE_MACH::X_version_min_command::records;
        nNumberOfRecords = XTYPE_MACH::X_version_min_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_uuid_command) {
        pRecords = XTYPE_MACH::X_uuid_command::records;
        nNumberOfRecords = XTYPE_MACH::X_uuid_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_build_version_command) {
        pRecords = XTYPE_MACH::X_build_version_command::records;
        nNumberOfRecords = XTYPE_MACH::X_build_version_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_main_command) {
        pRecords = XTYPE_MACH::X_entry_point_command::records;
        nNumberOfRecords = XTYPE_MACH::X_entry_point_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_fileset_entry_command) {
        pRecords = XTYPE_MACH::X_fileset_entry_command::records;
        nNumberOfRecords = XTYPE_MACH::X_fileset_entry_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_source_version_command) {
        pRecords = XTYPE_MACH::X_source_version_command::records;
        nNumberOfRecords = XTYPE_MACH::X_source_version_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_dylinker_command) {
        pRecords = XTYPE_MACH::X_dylinker_command::records;
        nNumberOfRecords = XTYPE_MACH::X_dylinker_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_data_in_code_command) {
        pRecords = XTYPE_MACH::X_linkedit_data_command::records;
        nNumberOfRecords = XTYPE_MACH::X_linkedit_data_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_code_signature_command) {
        pRecords = XTYPE_MACH::X_linkedit_data_command::records;
        nNumberOfRecords = XTYPE_MACH::X_linkedit_data_command::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MACH_nlist) {
        pRecords = XTYPE_MACH::X_nlist::records32;
        nNumberOfRecords = XTYPE_MACH::X_nlist::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MACH_nlist_64) {
        pRecords = XTYPE_MACH::X_nlist::records64;
        nNumberOfRecords = XTYPE_MACH::X_nlist::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MACH_dyld_chained_fixups_header) {
        pRecords = XTYPE_MACH::X_dyld_chained_fixups_header::records;
        nNumberOfRecords = XTYPE_MACH::X_dyld_chained_fixups_header::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MACH_SC_SuperBlob) {
        pRecords = XTYPE_MACH::X_SC_SuperBlob::records;
        nNumberOfRecords = XTYPE_MACH::X_SC_SuperBlob::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MACH_CS_BlobIndex) {
        pRecords = XTYPE_MACH::X_CS_BlobIndex::records;
        nNumberOfRecords = XTYPE_MACH::X_CS_BlobIndex::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MACH_CS_CodeDirectory) {
        pRecords = XTYPE_MACH::X_CS_CodeDirectory::records;
        nNumberOfRecords = XTYPE_MACH::X_CS_CodeDirectory::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MACH_section) {
        pRecords = XTYPE_MACH::X_section::records;
        nNumberOfRecords = XTYPE_MACH::X_section::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MACH_section_64) {
        pRecords = XTYPE_MACH::X_section_64::records;
        nNumberOfRecords = XTYPE_MACH::X_section_64::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_Elf32_Ehdr) {
        pRecords = XTYPE_ELF::X_Elf_Ehdr::records32;
        nNumberOfRecords = XTYPE_ELF::X_Elf_Ehdr::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_Elf64_Ehdr) {
        pRecords = XTYPE_ELF::X_Elf_Ehdr::records64;
        nNumberOfRecords = XTYPE_ELF::X_Elf_Ehdr::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MSDOS_EXE_file) {
        pRecords = XTYPE_MSDOS::X_Exe_file::records;
        nNumberOfRecords = XTYPE_MSDOS::X_Exe_file::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MSDOS_IMAGE_DOS_HEADER) {
        pRecords = XTYPE_MSDOS::X_IMAGE_DOS_HEADER::records;
        nNumberOfRecords = XTYPE_MSDOS::X_IMAGE_DOS_HEADER::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_DEX_HEADER) {
        pRecords = XTYPE_DEX::X_HEADER::records;
        nNumberOfRecords = XTYPE_DEX::X_HEADER::__data_size;
    }

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        XFW_DEF::HEADER_RECORD record = pRecords[i];

        bool bSuccess = true;

        if (nLimit != -1) {
            bSuccess = (record.nOffset < nLimit);
        }

        if (bSuccess) {
            listResult.append(record);
        }
    }

    return listResult;
}

qint32 XFormatWidget::getHeaderSize(QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords)
{
    qint32 nResult = 0;
    qint32 nNumberOfRecords = pListHeaderRecords->count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        XFW_DEF::HEADER_RECORD record = pListHeaderRecords->at(i);
        nResult += record.nSize;
    }

    return nResult;
}

void XFormatWidget::setValue(QVariant vValue, qint32 nPosition, qint64 nOffset, qint64 nSize)
{
    if (XBinary::saveBackup(XBinary::getBackupDevice(getDevice()))) {
        if (nPosition < listRecWidget.count()) {
            RECWIDGET recWidget = listRecWidget.at(nPosition);

            XBinary binary(getDevice(), getOptions().bIsImage, getOptions().nImageBase);
            if (recWidget.nVType & XFW_DEF::VAL_TYPE_DATA_INT) {
                if (recWidget.nSize == 1) {
                    binary.write_uint8(recWidget.nOffset, vValue.toUInt());
                } else if (recWidget.nSize == 2) {
                    binary.write_uint16(recWidget.nOffset, vValue.toUInt(), (recWidget.endian == XBinary::ENDIAN_BIG));
                } else if (recWidget.nSize == 4) {
                    binary.write_uint32(recWidget.nOffset, vValue.toUInt(), (recWidget.endian == XBinary::ENDIAN_BIG));
                } else if (recWidget.nSize == 8) {
                    binary.write_uint64(recWidget.nOffset, vValue.toULongLong(), (recWidget.endian == XBinary::ENDIAN_BIG));
                }
            } else if (recWidget.nVType & XFW_DEF::VAL_TYPE_DATA_ARRAY) {
                if (recWidget.nVType & XFW_DEF::VAL_TYPE_ANSI) {
                    binary.write_ansiStringFix(recWidget.nOffset, recWidget.nSize, vValue.toString());
                }
            }

            _adjustRecWidget(&recWidget, vValue);

            SV sv = _setValue(vValue, nPosition);
            // if (sv == SV_EDITED) {
            //     reset();
            // } else if (sv == SV_RELOADDATA) {
            //     reset();
            //     reloadData(true);
            // } else if (sv == SV_RELOADALL) {
            //     reset();
            //     reload();
            //     reloadData(false);
            // }

            emit dataChanged(nOffset, nSize);
        }
    } else {
        QMessageBox::critical(XOptions::getMainWidget(this), tr("Error"),
                              tr("Cannot save file") + QString(": %1").arg(XBinary::getBackupFileName(XBinary::getBackupDevice(getDevice()))));
    }
}

void XFormatWidget::adjustGenericHeader(QTableWidget *pTableWidget, const QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords)
{
    qint32 nNumberOfRecords = pListHeaderRecords->count();

    qint32 nValueWidthInSymbols = 2;

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        XFW_DEF::HEADER_RECORD record = pListHeaderRecords->at(i);

        if (record.vtype & XFW_DEF::VAL_TYPE_DATA_INT) {
            nValueWidthInSymbols = qMax(nValueWidthInSymbols, record.nSize * 2);
        }
    }

    qint32 nValueWidth = XOptions::getControlWidth(pTableWidget, nValueWidthInSymbols);
    qint32 nSymbolWidth = XOptions::getCharWidth(pTableWidget);

    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_NAME, QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_OFFSET, QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_SIZE, QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_TYPE, QHeaderView::ResizeToContents);
    pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, nValueWidth);
    pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, nSymbolWidth * 20);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_COMMENT, QHeaderView::Stretch);
}

void XFormatWidget::adjustGenericTable(QTableView *pTableView, const QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords)
{
    // qint32 nSymbolWidth = fm.boundingRect("W").width();

    qint32 nNumberOfRecords = pListHeaderRecords->count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        qint32 nWidth = 200;

        if (pListHeaderRecords->at(i).vtype & XFW_DEF::VAL_TYPE_DATA_INT) {
            if (pListHeaderRecords->at(i).nSize == 1) {
                nWidth = XOptions::getControlWidth(pTableView, 2);
            } else if (pListHeaderRecords->at(i).nSize == 2) {
                nWidth = XOptions::getControlWidth(pTableView, 4);
            } else if (pListHeaderRecords->at(i).nSize == 4) {
                nWidth = XOptions::getControlWidth(pTableView, 8);
            } else if (pListHeaderRecords->at(i).nSize == 8) {
                nWidth = XOptions::getControlWidth(pTableView, 16);
            }
        } else if (pListHeaderRecords->at(i).vtype & XFW_DEF::VAL_TYPE_COUNT) {
            nWidth = XOptions::getControlWidth(pTableView, 4);
        } else if (pListHeaderRecords->at(i).vtype & XFW_DEF::VAL_TYPE_HEX) {
            nWidth = XOptions::getControlWidth(pTableView, 8);
        }

        if (i == (nNumberOfRecords - 1)) {
            if (pListHeaderRecords->at(i).vtype & XFW_DEF::VAL_TYPE_STRING) {
                pTableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
            }
        }

        pTableView->setColumnWidth(i, nWidth);
    }

    // return fm.boundingRect("W").width();  // TODO Check
}

void XFormatWidget::adjustListTable(qint32 nType, QTableWidget *pTableWidget)
{
    Q_UNUSED(nType)
    Q_UNUSED(pTableWidget)
}

QString XFormatWidget::typeIdToString(qint32 nType)
{
    Q_UNUSED(nType)

    return "";
}

void XFormatWidget::setHeaderSelection(QTableWidget *pTableWidget)
{
    qint32 nCurrentRow = pTableWidget->currentRow();

    if (nCurrentRow != -1) {
        qint64 nOffset = pTableWidget->item(nCurrentRow, 0)->data(Qt::UserRole + HEADER_DATA_OFFSET).toULongLong();
        qint64 nSize = pTableWidget->item(nCurrentRow, 0)->data(Qt::UserRole + HEADER_DATA_SIZE).toULongLong();

        emit currentLocationChanged(nOffset, XBinary::LT_OFFSET, nSize);
    }
}

void XFormatWidget::setTableSelection(QTableView *pTableView)
{
    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->model()->index(nRow, 0);

        qint64 nOffset = pTableView->model()->data(index, Qt::UserRole + XFW_DEF::TABLEDATA_DATAOFFSET).toLongLong();
        qint64 nSize = pTableView->model()->data(index, Qt::UserRole + XFW_DEF::TABLEDATA_DATASIZE).toLongLong();

        if (nOffset != -1) {
            emit currentLocationChanged(nOffset, XBinary::LT_OFFSET, nSize);
        }
    }
}

QColor XFormatWidget::getEnabledColor()
{
    return g_colEnabled;
}

QColor XFormatWidget::getDisabledColor()
{
    return g_colDisabled;
}

void XFormatWidget::setItemEnable(QTableWidgetItem *pItem, bool bState)
{
    if (!bState) {
        pItem->setBackground(g_colDisabled);
    }
}

void XFormatWidget::setLineEdit(XLineEditHEX *pLineEdit, qint32 nMaxLength, const QString &sText, qint64 nOffset)
{
    pLineEdit->setMaxLength(nMaxLength);
    pLineEdit->setValue_String(sText);
    pLineEdit->setProperty("OFFSET", nOffset);
}

void XFormatWidget::dumpSection(QTableView *pTableView)
{
    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset = pTableView->model()->data(index, Qt::UserRole + XFW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = pTableView->model()->data(index, Qt::UserRole + XFW_DEF::SECTION_DATA_SIZE).toLongLong();
        QString sName = pTableView->model()->data(index, Qt::UserRole + XFW_DEF::SECTION_DATA_NAME).toString();

        dumpRegion(nOffset, nSize, sName);
    }
}

void XFormatWidget::dumpAll(QTableView *pTableView)
{
    // QString sDirectory = QFileDialog::getExistingDirectory(this, tr("Dump all"), XBinary::getDeviceDirectory(g_pDevice));

    // if (!sDirectory.isEmpty()) {
    //     qint32 nNumberOfRecords = pTableView->model()->rowCount();

    //     if (nNumberOfRecords) {
    //         QList<DumpProcess::RECORD> listRecords;

    //         for (qint32 i = 0; i < nNumberOfRecords; i++) {
    //             QModelIndex index = pTableView->model()->index(i, 0);

    //             DumpProcess::RECORD record = {};

    //             record.nOffset = pTableView->model()->data(index, Qt::UserRole + XFW_DEF::SECTION_DATA_OFFSET).toLongLong();
    //             record.nSize = pTableView->model()->data(index, Qt::UserRole + XFW_DEF::SECTION_DATA_SIZE).toLongLong();
    //             record.sFileName = pTableView->model()->data(index, Qt::UserRole + XFW_DEF::SECTION_DATA_NAME).toString();

    //             record.sFileName = sDirectory + QDir::separator() + QFileInfo(record.sFileName).fileName();

    //             listRecords.append(record);
    //         }

    //         QString sJsonFileName = XBinary::getDeviceFileName(g_pDevice) + ".patch.json";

    //         DialogDumpProcess dd(this);
    //         dd.setGlobal(getShortcuts(), getGlobalOptions());
    //         dd.setData(g_pDevice, listRecords, DumpProcess::DT_DUMP_DEVICE_OFFSET, sJsonFileName);

    //         dd.showDialogDelay();
    //     }
    // }
}

bool XFormatWidget::_setTreeItem(QTreeWidget *pTree, QTreeWidgetItem *pItem, qint32 nID)
{
    bool bResult = false;

    if (pItem->data(0, Qt::UserRole).toInt() == nID) {
        pTree->setCurrentItem(pItem);

        bResult = true;
    } else {
        qint32 nNumberOfChildren = pItem->childCount();

        for (qint32 i = 0; i < nNumberOfChildren; i++) {
            if (_setTreeItem(pTree, pItem->child(i), nID)) {
                bResult = true;
                break;
            }
        }
    }

    return bResult;
}

void XFormatWidget::setTreeItem(QTreeWidget *pTree, qint32 nID)
{
    qint32 nNumberOfItems = pTree->topLevelItemCount();

    for (qint32 i = 0; i < nNumberOfItems; i++) {
        if (_setTreeItem(pTree, pTree->topLevelItem(i), nID)) {
            break;
        }
    }

    XOptions::adjustTreeWidgetSize(pTree, 100);
}

void XFormatWidget::clear()
{
}

void XFormatWidget::cleanup()
{
    qint32 nNumberOfRecords = listRecWidget.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        RECWIDGET recWidget = listRecWidget.at(i);

        if (recWidget.pLineEdit) {
            delete recWidget.pLineEdit;
        }

        if (recWidget.pWidget) {
            delete recWidget.pWidget;
        }

        if (recWidget.pValue) {
            delete recWidget.pValue;
        }

        if (recWidget.pComment) {
            delete recWidget.pComment;
        }
    }

    listRecWidget.clear();
}

void XFormatWidget::reload()
{
}

void XFormatWidget::setReadonly(bool bState)
{
    qint32 nNumberOfRecords = listRecWidget.count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        RECWIDGET recWidget = listRecWidget.at(i);

        if (recWidget.pLineEdit) {
            recWidget.pLineEdit->setReadOnly(bState);
        }

        if (recWidget.pWidget) {
            QList<XComboBoxEx *> listWidgets = recWidget.pWidget->findChildren<XComboBoxEx *>();

            qint32 nNumberOfWidgets = listWidgets.count();

            for (qint32 i = 0; i < nNumberOfWidgets; i++) {
                listWidgets.at(i)->setReadOnly(bState);
            }
        }
    }

    XShortcutsWidget::setReadonly(bState);
}

QList<XFormatWidget::RECWIDGET> *XFormatWidget::getListRecWidgets()
{
    return &listRecWidget;
}

void XFormatWidget::reset()
{
    g_mapInit.clear();
    g_listPages.clear();
}

QString XFormatWidget::getInitStringFromCwOptions(XFW_DEF::CWOPTIONS *pCwOptions)
{
    return _getInitString(pCwOptions->_type, pCwOptions->nDataOffset, pCwOptions->nDataSize, pCwOptions->nDataCount);
}

QString XFormatWidget::_getInitString(XFW_DEF::TYPE _type, qint64 nDataOffset, qint64 nDataSize, qint64 nDataCount)
{
    QString sResult;

    sResult = QString("%1#%2#%3#%4").arg(QString::number((qint32)_type), QString::number(nDataOffset), QString::number(nDataSize), QString::number(nDataCount));

    return sResult;
}

XFW_DEF::TYPE XFormatWidget::_getTypeFromInitString(const QString &sInitString)
{
    QString sString = sInitString.section("#", 0, 0);

    return (XFW_DEF::TYPE)sString.toInt();
}

qint64 XFormatWidget::_getDataOffsetFromInitString(const QString &sInitString)
{
    QString sString = sInitString.section("#", 1, 1);

    return sString.toLongLong();
}

qint64 XFormatWidget::_getDataSizeFromInitString(const QString &sInitString)
{
    QString sString = sInitString.section("#", 2, 2);

    return sString.toLongLong();
}

qint64 XFormatWidget::_getDataCountFromInitString(const QString &sInitString)
{
    QString sString = sInitString.section("#", 3, 3);

    return sString.toLongLong();
}

void XFormatWidget::addInit(const QString &sString)
{
    g_mapInit.insert(sString, sString);
}

bool XFormatWidget::isInitPresent(const QString &sString)
{
    return (g_mapInit.value(sString) == sString);
}

void XFormatWidget::addPage(QTreeWidgetItem *pItem)
{
    if (g_bAddPageEnable) {
        qint32 nNumberOfPages = g_listPages.count();

        for (qint32 i = nNumberOfPages - 1; i > g_nPageIndex; i--) {
            g_listPages.removeAt(i);
        }

        g_listPages.append(pItem);
        g_nPageIndex = g_listPages.count() - 1;
    }
}

void XFormatWidget::setAddPageEnabled(bool bEnable)
{
    g_bAddPageEnable = bEnable;
}

QTreeWidgetItem *XFormatWidget::getPrevPage()
{
    QTreeWidgetItem *pResult = 0;

    if (isPrevPageAvailable()) {
        g_nPageIndex--;
        pResult = g_listPages.at(g_nPageIndex);
    }

    return pResult;
}

QTreeWidgetItem *XFormatWidget::getNextPage()
{
    QTreeWidgetItem *pResult = 0;

    if (isNextPageAvailable()) {
        g_nPageIndex++;
        pResult = g_listPages.at(g_nPageIndex);
    }

    return pResult;
}

bool XFormatWidget::isPrevPageAvailable()
{
    return g_nPageIndex > 0;
}

bool XFormatWidget::isNextPageAvailable()
{
    return g_nPageIndex < (g_listPages.count() - 1);
}

void XFormatWidget::updateRecWidgets(QIODevice *pDevice, QList<RECWIDGET> *pListRecWidget)
{
    XBinary binary(pDevice, getOptions().bIsImage, getOptions().nImageBase);

    qint32 nNumberOfRecords = pListRecWidget->count();

    QList<QVariant> listVariants;

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        RECWIDGET recWidget = pListRecWidget->at(i);

        QVariant varValue = _readVariant(&binary, recWidget.nOffset, recWidget.nSize, recWidget.nVType, (recWidget.endian == XBinary::ENDIAN_BIG));

        _adjustRecWidget(&recWidget, varValue);

        listVariants.append(varValue);
    }

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        if (pListRecWidget->at(i).pWidget) {
            bool bIsSize = false;

            if ((pListRecWidget->at(i).nVType & XFW_DEF::VAL_TYPE_SIZE) && (pListRecWidget->at(i).nSubPosition != -1)) {
                bIsSize = true;
            }

            QList<QToolButton *> listWidgets = pListRecWidget->at(i).pWidget->findChildren<QToolButton *>();

            qint32 nNumberOfWidgets = listWidgets.count();

            for (qint32 j = 0; j < nNumberOfWidgets; j++) {
                qint64 nDelta = listWidgets.at(j)->property("DELTA").toLongLong();
                if (bIsSize) {
                    listWidgets.at(j)->setProperty("LOCATION", listVariants.at(pListRecWidget->at(i).nSubPosition).toULongLong() + nDelta);
                    listWidgets.at(j)->setProperty("SIZE", listVariants.at(i));
                } else {
                    listWidgets.at(j)->setProperty("LOCATION", listVariants.at(i).toULongLong() + nDelta);
                }

                // if (pListRecWidget->at(i).nSubPosition != -1) {
                //     listWidgets.at(j)->setProperty("SUBVALUE", listVariants.at(pListRecWidget->at(i).nSubPosition));
                // }
            }
        }
    }
}

void XFormatWidget::_adjustRecWidget(RECWIDGET *pRecWidget, QVariant varValue)
{
    QString sComment;

    bool bBlockLineEdit = false;
    QList<bool> listBlockComboBoxes;

    if (pRecWidget->pLineEdit) bBlockLineEdit = pRecWidget->pLineEdit->blockSignals(true);

    QList<XComboBoxEx *> listWidgets = pRecWidget->pWidget->findChildren<XComboBoxEx *>();

    qint32 nNumberOfWidgets = listWidgets.count();

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        bool bState = listWidgets.at(i)->blockSignals(true);
        listBlockComboBoxes.append(bState);
    }

    if (pRecWidget->pLineEdit) {
        if (pRecWidget->nVType & XFW_DEF::VAL_TYPE_DATA_INT) {
            if (pRecWidget->nSize == 1) {
                pRecWidget->pLineEdit->setValue_uint8(varValue.toUInt(), XLineEditHEX::_MODE_HEX);
            } else if (pRecWidget->nSize == 2) {
                pRecWidget->pLineEdit->setValue_uint16(varValue.toUInt(), XLineEditHEX::_MODE_HEX);
            } else if (pRecWidget->nSize == 4) {
                pRecWidget->pLineEdit->setValue_uint32(varValue.toUInt(), XLineEditHEX::_MODE_HEX);
            } else if (pRecWidget->nSize == 8) {
                pRecWidget->pLineEdit->setValue_uint64(varValue.toULongLong(), XLineEditHEX::_MODE_HEX);
            }
        } else if (pRecWidget->nVType & XFW_DEF::VAL_TYPE_DATA_ARRAY) {
            if (pRecWidget->nVType & XFW_DEF::VAL_TYPE_ANSI) {
                pRecWidget->pLineEdit->setValue_String(varValue.toString(), pRecWidget->nSize);
            }
        }
    }

    if (pRecWidget->pLineEdit) {
        pRecWidget->pValue->setText(pRecWidget->pLineEdit->text());
    }

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        listWidgets.at(i)->setValue(varValue);
    }

    if (pRecWidget->nVType & XFW_DEF::VAL_TYPE_SIZE) sComment = XBinary::appendText(sComment, XBinary::bytesCountToString(varValue.toULongLong()), ", ");
    if (pRecWidget->nVType & XFW_DEF::VAL_TYPE_COUNT) sComment = XBinary::appendText(sComment, QString("%1").arg(varValue.toULongLong()), ", ");

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        sComment = XBinary::appendText(sComment, listWidgets.at(i)->getDescription(), " | ");
    }

    if (sComment != "") {
        pRecWidget->pComment->setText(sComment);
    }

    if (pRecWidget->pLineEdit) pRecWidget->pLineEdit->blockSignals(bBlockLineEdit);

    for (qint32 i = 0; i < nNumberOfWidgets; i++) {
        listWidgets.at(i)->blockSignals(listBlockComboBoxes.at(i));
    }
}

QVariant XFormatWidget::_readVariant(XBinary *pBinary, qint64 nOffset, qint64 nSize, qint32 vtype, bool bIsBigEndian)
{
    QVariant varResult;

    if (vtype & XFW_DEF::VAL_TYPE_DATA_INT) {
        if (nSize == 1) {
            varResult = pBinary->read_uint8(nOffset);
        } else if (nSize == 2) {
            varResult = pBinary->read_uint16(nOffset, bIsBigEndian);
        } else if (nSize == 4) {
            varResult = pBinary->read_uint32(nOffset, bIsBigEndian);
        } else if (nSize == 8) {
            varResult = pBinary->read_uint64(nOffset, bIsBigEndian);
        }
    } else if (vtype & XFW_DEF::VAL_TYPE_DATA_ARRAY) {
        if (vtype & XFW_DEF::VAL_TYPE_ANSI) {
            varResult = pBinary->read_ansiString(nOffset, nSize);
        }
    }

    return varResult;
}

QStandardItem *XFormatWidget::setItemToModel(QStandardItemModel *pModel, qint32 nRow, qint32 nColumn, const QVariant &var, qint64 nSize, qint32 vtype)
{
    QStandardItem *pResult = new QStandardItem;

    if (vtype & XFW_DEF::VAL_TYPE_DATA_INT) {
        QString sString;

        if (nSize == 1) {
            sString = QString("%1").arg(var.toUInt(), 2, 16, QChar('0'));
        } else if (nSize == 2) {
            sString = QString("%1").arg(var.toUInt(), 4, 16, QChar('0'));
        } else if (nSize == 4) {
            sString = QString("%1").arg(var.toUInt(), 8, 16, QChar('0'));
        } else if (nSize == 8) {
            sString = QString("%1").arg(var.toULongLong(), 16, 16, QChar('0'));
        }
        pResult->setText(sString);
        pResult->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    } else if (vtype & XFW_DEF::VAL_TYPE_COUNT) {
        pResult->setData(var.toULongLong(), Qt::DisplayRole);
        pResult->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    } else if (vtype & XFW_DEF::VAL_TYPE_HEX) {
        QString sString = QString("%1").arg(var.toULongLong(), 8, 16, QChar('0'));
        pResult->setText(sString);
        pResult->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    } else {
        pResult->setText(var.toString());
        pResult->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }

    pModel->setItem(nRow, nColumn, pResult);

    return pResult;
}

QStandardItem *XFormatWidget::setItemToModelData(QStandardItemModel *pModel, qint32 nRow, qint32 nColumn, const QVariant &var, qint64 nSize, qint32 vtype,
                                                 XFW_DEF::TYPE type, qint64 nHeaderOffset, qint64 nHeaderSize, qint64 nDataOffset, qint64 nDataSize, qint64 nDataCount)
{
    QStandardItem *pResult = setItemToModel(pModel, nRow, nColumn, var, nSize, vtype);

    if (pResult) {
        pResult->setData(type, Qt::UserRole + (qint32)(XFW_DEF::TABLEDATA_TYPE));
        pResult->setData(nHeaderOffset, Qt::UserRole + (qint32)(XFW_DEF::TABLEDATA_HEADEROFFSET));
        pResult->setData(nHeaderSize, Qt::UserRole + (qint32)(XFW_DEF::TABLEDATA_HEADERSIZE));
        pResult->setData(nDataOffset, Qt::UserRole + (qint32)(XFW_DEF::TABLEDATA_DATAOFFSET));
        pResult->setData(nDataSize, Qt::UserRole + (qint32)(XFW_DEF::TABLEDATA_DATASIZE));
        pResult->setData(nDataCount, Qt::UserRole + (qint32)(XFW_DEF::TABLEDATA_DATACOUNT));
    }

    return pResult;
}

qint32 XFormatWidget::getColumnWidth(QWidget *pParent, XFormatWidget::CW cw, XBinary::MODE mode)
{
    qint32 nResult = 0;

    qint32 nSymbolWidth = XLineEditHEX::getSymbolWidth(pParent);

    if (cw == CW_UINTMODE) {
        switch (mode) {
            case XBinary::MODE_16: cw = CW_UINT16; break;
            case XBinary::MODE_32: cw = CW_UINT32; break;
            case XBinary::MODE_64: cw = CW_UINT64; break;
            default: cw = CW_UINT32;
        }
    }

    switch (cw) {
        case CW_UINT8: nResult = 2 * nSymbolWidth; break;
        case CW_UINT16: nResult = 4 * nSymbolWidth; break;
        case CW_UINT32: nResult = 8 * nSymbolWidth; break;
        case CW_UINT64: nResult = 14 * nSymbolWidth; break;
        case CW_TYPE: nResult = 8 * nSymbolWidth; break;
        case CW_STRINGSHORT: nResult = 10 * nSymbolWidth; break;
        case CW_STRINGSHORT2: nResult = 15 * nSymbolWidth; break;
        case CW_STRINGMID: nResult = 25 * nSymbolWidth; break;
        case CW_STRINGLONG: nResult = 50 * nSymbolWidth; break;
        default: nResult = 8 * nSymbolWidth; break;
    }

    return nResult;
}

void XFormatWidget::setDisasmInitAddress(XADDR nDisasmInitAddress)
{
    g_nDisamInitAddress = nDisasmInitAddress;
}

XADDR XFormatWidget::getDisasmInitAddress()
{
    return g_nDisamInitAddress;
}

QStandardItemModel *XFormatWidget::getHeaderTableModel(QTableWidget *pTableWidget)
{
    qint32 nNumberOfColumns = pTableWidget->columnCount();
    qint32 nNumberOfRows = pTableWidget->rowCount();

    QStandardItemModel *pResult = new QStandardItemModel(nNumberOfRows, nNumberOfColumns);

    for (qint32 i = 0; i < nNumberOfColumns; i++) {
        pResult->setHeaderData(i, Qt::Horizontal, pTableWidget->horizontalHeaderItem(i)->text());
    }

    for (qint32 i = 0; i < nNumberOfRows; i++) {
        for (qint32 j = 0; j < nNumberOfColumns; j++) {
            QString sText;

            QTableWidgetItem *pTableWidgetItem = pTableWidget->item(i, j);

            if (pTableWidgetItem) {
                sText = pTableWidgetItem->text();
            } else {
                QWidget *pWidget = pTableWidget->cellWidget(i, j);

                XLineEditHEX *pLineEdit = dynamic_cast<XLineEditHEX *>(pWidget);
                XComboBoxEx *pComboBox = dynamic_cast<XComboBoxEx *>(pWidget);

                if (pLineEdit) {
                    sText = pLineEdit->text();
                } else if (pComboBox) {
                    sText = pComboBox->getDescription();
                }
            }

            QStandardItem *pItem = new QStandardItem(sText);

            pResult->setItem(i, j, pItem);
        }
    }

    return pResult;
}

void XFormatWidget::saveHeaderTable(QTableWidget *pTableWidget, const QString &sFileName)
{
    QString _sFileName = QFileDialog::getSaveFileName(this, tr("Save"), sFileName, QString("%1 (*.txt);;%2 (*)").arg(tr("Text files"), tr("All files")));

    if (!_sFileName.isEmpty()) {
        QStandardItemModel *pModel = getHeaderTableModel(pTableWidget);

        if (!XOptions::saveTableModel(pModel, _sFileName)) {
            QMessageBox::critical(XOptions::getMainWidget(this), tr("Error"), QString("%1: %2").arg(tr("Cannot save file"), _sFileName));
        }

        delete pModel;
    }
}

void XFormatWidget::saveModel(QAbstractItemModel *pModel, const QString &sFileName)
{
    QString _sFileName = QFileDialog::getSaveFileName(this, tr("Save"), sFileName, QString("%1 (*.txt);;%2 (*)").arg(tr("Text files"), tr("All files")));

    if (!_sFileName.isEmpty()) {
        if (!XOptions::saveTableModel(pModel, _sFileName)) {
            QMessageBox::critical(XOptions::getMainWidget(this), tr("Error"), QString("%1: %2").arg(tr("Cannot save file"), _sFileName));
        }
    }
}

void XFormatWidget::_addFileType(QTreeWidgetItem *pTreeWidgetItem, QIODevice *pDevice, qint64 nOffset, qint64 nSize, XBinary::FT fileType, bool bIsImage,
                                 XADDR nImageBase)
{
    QIODevice *_pDevice = nullptr;
    SubDevice *pSd = nullptr;

    if ((nOffset == 0) && (pDevice->size() == nSize)) {
        _pDevice = pDevice;
    } else {
        pSd = new SubDevice(pDevice, nOffset, nSize);
        pSd->open(QIODevice::ReadOnly);
        _pDevice = pSd;
    }

    SPSTRUCT spStruct = {};
    spStruct.pTreeWidgetItem = pTreeWidgetItem;
    spStruct.pDevice = pDevice;
    spStruct.nOffset = nOffset;
    spStruct.nSize = nSize;
    spStruct.fileType = fileType;
    spStruct.bIsImage = bIsImage;
    spStruct.nImageBase = nImageBase;

    if ((fileType == XBinary::FT_ELF) || (fileType == XBinary::FT_ELF32) || (fileType == XBinary::FT_ELF64)) {
        XELF elf(_pDevice, bIsImage, nImageBase);

        if (elf.isValid()) {
            spStruct.endian = elf.getEndian();
            spStruct.mode = elf.getMode();
            spStruct.nStructOffset = 0;
            spStruct.nStructCount = 1;
            spStruct.widgetMode = XFW_DEF::WIDGETMODE_HEADER;

            if (spStruct.mode == XBinary::MODE_32) {
                spStruct.type = XFW_DEF::TYPE_Elf32_Ehdr;
                spStruct.fileType = XBinary::FT_ELF32;
            } else if (spStruct.mode == XBinary::MODE_64) {
                spStruct.type = XFW_DEF::TYPE_Elf64_Ehdr;
                spStruct.fileType = XBinary::FT_ELF64;
            }

            _addStruct(spStruct);
        }
    } else if ((fileType == XBinary::FT_MACHO) || (fileType == XBinary::FT_MACHO32) || (fileType == XBinary::FT_MACHO64)) {
        XMACH mach(_pDevice, bIsImage, nImageBase);

        if (mach.isValid()) {
            spStruct.endian = mach.getEndian();
            spStruct.mode = mach.getMode();
            spStruct.nStructOffset = 0;
            spStruct.nStructCount = 1;
            spStruct.widgetMode = XFW_DEF::WIDGETMODE_HEADER;

            if (spStruct.mode == XBinary::MODE_32) {
                spStruct.type = XFW_DEF::TYPE_MACH_mach_header;
                spStruct.fileType = XBinary::FT_MACHO32;
            } else if (spStruct.mode == XBinary::MODE_64) {
                spStruct.type = XFW_DEF::TYPE_MACH_mach_header_64;
                spStruct.fileType = XBinary::FT_MACHO64;
            }

            _addStruct(spStruct);
        }
    } else if (fileType == XBinary::FT_DEX) {
        XDEX dex(_pDevice);

        if (dex.isValid()) {
            spStruct.endian = dex.getEndian();
            spStruct.mode = dex.getMode();
            spStruct.nStructOffset = 0;
            spStruct.nStructCount = 1;
            spStruct.widgetMode = XFW_DEF::WIDGETMODE_HEADER;
            spStruct.type = XFW_DEF::TYPE_DEX_HEADER;
            spStruct.fileType = XBinary::FT_DEX;

            _addStruct(spStruct);
        }
    }

    if (pSd) {
        delete pSd;
    }
}

void XFormatWidget::_addStruct(const SPSTRUCT &spStruct)
{
    SPSTRUCT _spStruct = spStruct;

    XOptions::ICONTYPE iconType = XOptions::ICONTYPE_NONE;

    if (_spStruct.widgetMode == XFW_DEF::WIDGETMODE_HEADER) {
        iconType = XOptions::ICONTYPE_HEADER;
    } else if (_spStruct.widgetMode == XFW_DEF::WIDGETMODE_TABLE) {
        iconType = XOptions::ICONTYPE_TABLE;
    }

    if (_spStruct.nStructSize == 0) {
        if (_spStruct.type == XFW_DEF::TYPE_Elf32_Ehdr) {
            _spStruct.nStructSize = sizeof(XELF_DEF::Elf32_Ehdr);
        } else if (_spStruct.type == XFW_DEF::TYPE_Elf64_Ehdr) {
            _spStruct.nStructSize = sizeof(XELF_DEF::Elf64_Ehdr);
        } else if (_spStruct.type == XFW_DEF::TYPE_MACH_mach_header) {
            _spStruct.nStructSize = sizeof(XMACH_DEF::mach_header);
        } else if (_spStruct.type == XFW_DEF::TYPE_MACH_mach_header_64) {
            _spStruct.nStructSize = sizeof(XMACH_DEF::mach_header_64);
        } else if (_spStruct.type == XFW_DEF::TYPE_MACH_load_command) {
            _spStruct.nStructSize = sizeof(XMACH_DEF::load_command);
        }

        _spStruct.nStructSize *= _spStruct.nStructCount;
    }

    QTreeWidgetItem *pTreeWidgetItem = createNewItem(_spStruct.type, _spStruct.widgetMode, iconType, _spStruct.nOffset + _spStruct.nStructOffset, _spStruct.nStructSize,
                                                     _spStruct.nStructCount, _spStruct.var1, _spStruct.var2, spStruct.mode, spStruct.endian, spStruct.sTitle);

    spStruct.pTreeWidgetItem->addChild(pTreeWidgetItem);

    QIODevice *_pDevice = nullptr;
    SubDevice *pSd = nullptr;

    if ((_spStruct.type > XFW_DEF::TYPE_MACH_START) && (_spStruct.type < XFW_DEF::TYPE_MACH_END)) {
        if ((_spStruct.nOffset == 0) && (_spStruct.pDevice->size() == _spStruct.nSize)) {
            _pDevice = _spStruct.pDevice;
        } else {
            pSd = new SubDevice(_spStruct.pDevice, _spStruct.nOffset, _spStruct.nSize);
            pSd->open(QIODevice::ReadOnly);
            _pDevice = pSd;
        }

        if ((_spStruct.type > XFW_DEF::TYPE_MACH_START) && (_spStruct.type < XFW_DEF::TYPE_MACH_END)) {
            XMACH mach(_pDevice, _spStruct.bIsImage, _spStruct.nImageBase);

            if ((_spStruct.widgetMode == XFW_DEF::WIDGETMODE_HEADER) &&
                ((_spStruct.type == XFW_DEF::TYPE_MACH_mach_header) || (_spStruct.type == XFW_DEF::TYPE_MACH_mach_header_64))) {
                if (mach.isValid()) {
                    qint32 nCommandOffset = mach.getHeaderSize();
                    qint32 nCommandSize = mach.getHeader_sizeofcmds();
                    qint32 nCommandCount = mach.getHeader_ncmds();

                    {
                        SPSTRUCT _spStructRecord = _spStruct;
                        _spStructRecord.pTreeWidgetItem = pTreeWidgetItem;
                        _spStructRecord.nStructOffset = _spStruct.nOffset + nCommandOffset;
                        _spStructRecord.nStructSize = nCommandSize;
                        _spStructRecord.nStructCount = nCommandCount;
                        _spStructRecord.widgetMode = XFW_DEF::WIDGETMODE_TABLE;
                        _spStructRecord.type = XFW_DEF::TYPE_MACH_load_command;

                        _addStruct(_spStructRecord);
                    }
                }
            } else if ((_spStruct.widgetMode == XFW_DEF::WIDGETMODE_TABLE) && (_spStruct.type == XFW_DEF::TYPE_MACH_load_command)) {
                qint32 nCommandOffset = _spStruct.nStructOffset;
                qint32 nCommandSize = _spStruct.nStructSize;
                qint32 nCommandCount = _spStruct.nStructCount;

                QList<XMACH::COMMAND_RECORD> listCommands = mach._getCommandRecords(nCommandOffset, nCommandSize, nCommandCount, mach.is64(), mach.isBigEndian());

                qint32 nNumberOfCommands = listCommands.count();

                for (qint32 i = 0; i < nNumberOfCommands; i++) {
                    SPSTRUCT _spStructRecord = _spStruct;
                    _spStructRecord.pTreeWidgetItem = pTreeWidgetItem;
                    _spStructRecord.nStructOffset = _spStruct.nOffset + listCommands.at(i).nStructOffset;
                    _spStructRecord.nStructSize = listCommands.at(i).nSize;
                    _spStructRecord.nStructCount = 1;
                    _spStructRecord.widgetMode = XFW_DEF::WIDGETMODE_HEADER;
                    _spStructRecord.type = load_commandIdToType(listCommands.at(i).nId);

                    _addStruct(_spStructRecord);
                }
            } else if ((_spStruct.widgetMode == XFW_DEF::WIDGETMODE_HEADER) && (_spStruct.type == XFW_DEF::TYPE_symtab_command)) {
                XMACH_DEF::symtab_command _command = mach._read_symtab_command(_spStruct.nStructOffset);

                if (_command.symoff && _command.nsyms) {
                    SPSTRUCT _spStructRecord = _spStruct;
                    _spStructRecord.pTreeWidgetItem = pTreeWidgetItem;
                    _spStructRecord.nStructOffset = _spStruct.nOffset + _command.symoff;
                    _spStructRecord.nStructSize = 0;
                    _spStructRecord.nStructCount = _command.nsyms;
                    _spStructRecord.widgetMode = XFW_DEF::WIDGETMODE_TABLE;
                    _spStructRecord.var1 = _command.stroff;
                    _spStructRecord.var2 = _command.strsize;

                    if (_spStructRecord.mode == XBinary::MODE_32) {
                        _spStructRecord.type = XFW_DEF::TYPE_MACH_nlist;
                    } else if (_spStructRecord.mode == XBinary::MODE_64) {
                        _spStructRecord.type = XFW_DEF::TYPE_MACH_nlist_64;
                    }

                    _addStruct(_spStructRecord);
                }

                if (_command.stroff && _command.strsize) {
                    SPSTRUCT _spStructRecord = _spStruct;
                    _spStructRecord.pTreeWidgetItem = pTreeWidgetItem;
                    _spStructRecord.nStructOffset = _spStruct.nOffset + _command.stroff;
                    _spStructRecord.nStructSize = _command.strsize;
                    _spStructRecord.nStructCount = 0;
                    _spStructRecord.widgetMode = XFW_DEF::WIDGETMODE_TABLE;
                    _spStructRecord.type = XFW_DEF::TYPE_GENERIC_STRINGTABLE_ANSI;

                    _addStruct(_spStructRecord);
                }
            } else if ((_spStruct.widgetMode == XFW_DEF::WIDGETMODE_HEADER) && (_spStruct.type == XFW_DEF::TYPE_code_signature_command)) {
                XMACH_DEF::linkedit_data_command _command = mach._read_linkedit_data_command(_spStruct.nStructOffset);

                if (_command.dataoff && _command.datasize) {
                    SPSTRUCT _spStructRecord = _spStruct;
                    _spStructRecord.pTreeWidgetItem = pTreeWidgetItem;
                    _spStructRecord.nStructOffset = _spStruct.nOffset + _command.dataoff;
                    _spStructRecord.nStructSize = _command.datasize;
                    _spStructRecord.nStructCount = 0;
                    _spStructRecord.widgetMode = XFW_DEF::WIDGETMODE_HEADER;
                    _spStructRecord.type = XFW_DEF::TYPE_MACH_SC_SuperBlob;
                    _spStructRecord.endian = XBinary::ENDIAN_BIG;  // Important!

                    _addStruct(_spStructRecord);
                }
            } else if ((_spStruct.widgetMode == XFW_DEF::WIDGETMODE_HEADER) && (_spStruct.type == XFW_DEF::TYPE_MACH_SC_SuperBlob)) {
                XMACH_DEF::__SC_SuperBlob _suberBlob = mach._read_SC_SuperBlob(_spStruct.nStructOffset);

                if ((_suberBlob.count > 0) && (_suberBlob.length - sizeof(XMACH_DEF::__SC_SuperBlob) > 0)) {
                    qint64 nOffset = _spStruct.nOffset + _spStruct.nStructOffset + sizeof(XMACH_DEF::__SC_SuperBlob);
                    qint64 nCount = _suberBlob.count;

                    for (qint32 i = 0; i < nCount; i++) {
                        SPSTRUCT _spStructRecord = _spStruct;
                        _spStructRecord.pTreeWidgetItem = pTreeWidgetItem;
                        _spStructRecord.nStructOffset = nOffset + i * sizeof(XMACH_DEF::CS_BlobIndex);
                        _spStructRecord.nStructSize = sizeof(XMACH_DEF::CS_BlobIndex);
                        _spStructRecord.nStructCount = 1;
                        _spStructRecord.widgetMode = XFW_DEF::WIDGETMODE_HEADER;
                        _spStructRecord.type = XFW_DEF::TYPE_MACH_CS_BlobIndex;
                        _spStructRecord.endian = XBinary::ENDIAN_BIG;  // Important!
                        _spStructRecord.var1 = _spStruct.nOffset + _spStruct.nStructOffset;

                        _addStruct(_spStructRecord);
                    }
                }
            } else if ((_spStruct.widgetMode == XFW_DEF::WIDGETMODE_HEADER) && (_spStruct.type == XFW_DEF::TYPE_MACH_CS_BlobIndex)) {
                XMACH_DEF::CS_BlobIndex _blobIndex = mach._read_CS_BlobIndex(_spStruct.nStructOffset);

                if (_blobIndex.type == XMACH_DEF::S_CSSLOT_CODEDIRECTORY) {
                    SPSTRUCT _spStructRecord = _spStruct;
                    _spStructRecord.pTreeWidgetItem = pTreeWidgetItem;
                    _spStructRecord.nStructOffset = _spStruct.nOffset + _blobIndex.offset + _spStruct.var1.toLongLong();
                    _spStructRecord.nStructSize = sizeof(XMACH_DEF::CS_BlobIndex);
                    _spStructRecord.nStructCount = 1;
                    _spStructRecord.widgetMode = XFW_DEF::WIDGETMODE_HEADER;
                    _spStructRecord.type = XFW_DEF::TYPE_MACH_CS_CodeDirectory;
                    _spStructRecord.endian = XBinary::ENDIAN_BIG;  // Important!
                    //_spStructRecord.var1 = _spStruct.nOffset + _spStructRecord.nStructOffset;

                    _addStruct(_spStructRecord);
                }
            } else if ((_spStruct.widgetMode == XFW_DEF::WIDGETMODE_HEADER) && (_spStruct.type == XFW_DEF::TYPE_dyld_chained_fixups_command)) {
                XMACH_DEF::linkedit_data_command _command = mach._read_linkedit_data_command(_spStruct.nStructOffset);

                if (_command.dataoff && _command.datasize) {
                    SPSTRUCT _spStructRecord = _spStruct;
                    _spStructRecord.pTreeWidgetItem = pTreeWidgetItem;
                    _spStructRecord.nStructOffset = _spStruct.nOffset + _command.dataoff;
                    _spStructRecord.nStructSize = _command.datasize;
                    _spStructRecord.nStructCount = 0;
                    _spStructRecord.widgetMode = XFW_DEF::WIDGETMODE_HEADER;
                    _spStructRecord.type = XFW_DEF::TYPE_MACH_dyld_chained_fixups_header;

                    _addStruct(_spStructRecord);
                }
            } else if ((_spStruct.widgetMode == XFW_DEF::WIDGETMODE_HEADER) && (_spStruct.type == XFW_DEF::TYPE_segment_command)) {
                XMACH_DEF::segment_command _command = mach._read_segment_command(_spStruct.nStructOffset);

                if ((_command.nsects > 0) && (_command.cmdsize - sizeof(XMACH_DEF::segment_command) > 0)) {
                    qint64 nOffset = _spStruct.nOffset + _spStruct.nStructOffset + sizeof(XMACH_DEF::segment_command);
                    // qint64 nSize = _command.cmdsize - sizeof(XMACH_DEF::segment_command);
                    qint64 nCount = _command.nsects;

                    for (qint32 i = 0; i < nCount; i++) {
                        SPSTRUCT _spStructRecord = _spStruct;
                        _spStructRecord.pTreeWidgetItem = pTreeWidgetItem;
                        _spStructRecord.nStructOffset = nOffset + i * sizeof(XMACH_DEF::section);
                        _spStructRecord.nStructSize = sizeof(XMACH_DEF::section);
                        _spStructRecord.nStructCount = 1;
                        _spStructRecord.widgetMode = XFW_DEF::WIDGETMODE_HEADER;
                        _spStructRecord.type = XFW_DEF::TYPE_MACH_section;

                        _addStruct(_spStructRecord);
                    }
                }
            } else if ((_spStruct.widgetMode == XFW_DEF::WIDGETMODE_HEADER) && (_spStruct.type == XFW_DEF::TYPE_segment_command_64)) {
                XMACH_DEF::segment_command_64 _command = mach._read_segment_command_64(_spStruct.nStructOffset);

                if ((_command.nsects > 0) && (_command.cmdsize - sizeof(XMACH_DEF::segment_command_64) > 0)) {
                    qint64 nOffset = _spStruct.nOffset + _spStruct.nStructOffset + sizeof(XMACH_DEF::segment_command_64);
                    // qint64 nSize = _command.cmdsize - sizeof(XMACH_DEF::segment_command_64);
                    qint64 nCount = _command.nsects;

                    for (qint32 i = 0; i < nCount; i++) {
                        SPSTRUCT _spStructRecord = _spStruct;
                        _spStructRecord.pTreeWidgetItem = pTreeWidgetItem;
                        _spStructRecord.nStructOffset = nOffset + i * sizeof(XMACH_DEF::section_64);
                        _spStructRecord.nStructSize = sizeof(XMACH_DEF::section_64);
                        _spStructRecord.nStructCount = 1;
                        _spStructRecord.widgetMode = XFW_DEF::WIDGETMODE_HEADER;
                        _spStructRecord.type = XFW_DEF::TYPE_MACH_section_64;

                        _addStruct(_spStructRecord);
                    }
                }
            }
        }
    }

    if (pSd) {
        delete pSd;
    }
}

// void XFormatWidget::_addStruct(QTreeWidgetItem *pTreeWidgetItem, QIODevice *pDevice, qint64 nOffset, qint64 nSize, qint64 nSubOffset, qint32 nCount, XFW_DEF::TYPE
// type, XFW_DEF::WIDGETMODE widgetMode, XBinary::MODE mode, XBinary::ENDIAN endian, QString sTitle)
// {
//     if () {

//     }

//     if ((fileType == XBinary::FT_ELF) || (fileType == XBinary::FT_ELF32) || (fileType == XBinary::FT_ELF64)) {
//         XELF elf(pDevice, bIsImage, nImageBase);

//         if (elf.isValid()) {
//             XBinary::ENDIAN endian = elf.getEndian();
//             XBinary::MODE mode = elf.getMode();
//             bool bIs64 = elf.is64();

//             {
//                 qint64 nDataSize = 0;

//                 if (bIs64) {
//                     nDataSize = sizeof(XELF_DEF::Elf64_Ehdr);
//                 } else {
//                     nDataSize = sizeof(XELF_DEF::Elf32_Ehdr);
//                 }

//                 pTreeWidgetItem->addChild(
//                     createNewItem(XFW_DEF::TYPE_ELF_elf_ehdr, XFW_DEF::WIDGETMODE_HEADER, XOptions::ICONTYPE_HEADER, 0, nDataSize, 0, 0, 0, mode, endian));
//             }
//         }

//     } else if (fileType == XBinary::FT_MSDOS) {
//         XMSDOS msdos(pDevice, bIsImage, nImageBase);

//         if (msdos.isValid()) {
//             XBinary::ENDIAN endian = msdos.getEndian();
//             XBinary::MODE mode = msdos.getMode();

//             {
//                 pTreeWidgetItem->addChild(createNewItem(XFW_DEF::TYPE_MSDOS_EXE_file, XFW_DEF::WIDGETMODE_HEADER, XOptions::ICONTYPE_HEADER, 0,
//                                                            sizeof(XMSDOS_DEF::EXE_file), 0, 0, 0, mode, endian));
//             }
//         }
//     } else if ((fileType == XBinary::FT_PE) || (fileType == XBinary::FT_PE32) || (fileType == XBinary::FT_PE64)) {
//         XPE pe(pDevice, bIsImage, nImageBase);

//         if (pe.isValid()) {
//             XBinary::ENDIAN endian = pe.getEndian();
//             XBinary::MODE mode = pe.getMode();
//             bool bIs64 = pe.is64();

//             {
//                 pTreeWidgetItem->addChild(createNewItem(XFW_DEF::TYPE_MSDOS_IMAGE_DOS_HEADER, XFW_DEF::WIDGETMODE_HEADER,
//                                                            XOptions::ICONTYPE_HEADER, 0, sizeof(XMSDOS_DEF::IMAGE_DOS_HEADEREX), 0, 0, 0, mode, endian));
//             }
//         }
//     }
// }

XFW_DEF::TYPE XFormatWidget::load_commandIdToType(qint32 nCommandId)
{
    XFW_DEF::TYPE result = XFW_DEF::TYPE_MACH_load_command;

    // https://github.com/apple-oss-distributions/dyld/blob/main/mach_o/Header.cpp
    if ((nCommandId == XMACH_DEF::S_LC_ID_DYLIB) || (nCommandId == XMACH_DEF::S_LC_LOAD_DYLIB) || (nCommandId == XMACH_DEF::S_LC_LOAD_WEAK_DYLIB) ||
        (nCommandId == XMACH_DEF::S_LC_REEXPORT_DYLIB) || (nCommandId == XMACH_DEF::S_LC_LOAD_UPWARD_DYLIB)) {
        result = XFW_DEF::TYPE_dylib_command;
    } else if (nCommandId == XMACH_DEF::S_LC_RPATH) {
        result = XFW_DEF::TYPE_rpath_command;
    } else if (nCommandId == XMACH_DEF::S_LC_SUB_UMBRELLA) {
        result = XFW_DEF::TYPE_sub_umbrella_command;
    } else if (nCommandId == XMACH_DEF::S_LC_SUB_CLIENT) {
        result = XFW_DEF::TYPE_sub_client_command;
    } else if (nCommandId == XMACH_DEF::S_LC_SUB_LIBRARY) {
        result = XFW_DEF::TYPE_sub_library_command;
    } else if (nCommandId == XMACH_DEF::S_LC_SYMTAB) {
        result = XFW_DEF::TYPE_symtab_command;
    } else if (nCommandId == XMACH_DEF::S_LC_DYSYMTAB) {
        result = XFW_DEF::TYPE_dysymtab_command;
    } else if (nCommandId == XMACH_DEF::S_LC_SEGMENT_SPLIT_INFO) {
        result = XFW_DEF::TYPE_segment_split_info_command;
    } else if (nCommandId == XMACH_DEF::S_LC_ATOM_INFO) {
        result = XFW_DEF::TYPE_atom_info_command;
    } else if (nCommandId == XMACH_DEF::S_LC_FUNCTION_STARTS) {
        result = XFW_DEF::TYPE_function_starts_command;
    } else if (nCommandId == XMACH_DEF::S_LC_DYLD_EXPORTS_TRIE) {
        result = XFW_DEF::TYPE_dyld_exports_trie_command;
    } else if (nCommandId == XMACH_DEF::S_LC_DYLD_CHAINED_FIXUPS) {
        result = XFW_DEF::TYPE_dyld_chained_fixups_command;
    } else if (nCommandId == XMACH_DEF::S_LC_ENCRYPTION_INFO) {
        result = XFW_DEF::TYPE_encryption_info_command;
    } else if (nCommandId == XMACH_DEF::S_LC_ENCRYPTION_INFO_64) {
        result = XFW_DEF::TYPE_encryption_info_command_64;
    } else if ((nCommandId == XMACH_DEF::S_LC_DYLD_INFO) || (nCommandId == XMACH_DEF::S_LC_DYLD_INFO_ONLY)) {
        result = XFW_DEF::TYPE_dyld_info_command;
    } else if ((nCommandId == XMACH_DEF::S_LC_VERSION_MIN_MACOSX) || (nCommandId == XMACH_DEF::S_LC_VERSION_MIN_IPHONEOS) ||
               (nCommandId == XMACH_DEF::S_LC_VERSION_MIN_TVOS) || (nCommandId == XMACH_DEF::S_LC_VERSION_MIN_WATCHOS)) {
        result = XFW_DEF::TYPE_version_min_command;
    } else if (nCommandId == XMACH_DEF::S_LC_UUID) {
        result = XFW_DEF::TYPE_uuid_command;
    } else if (nCommandId == XMACH_DEF::S_LC_BUILD_VERSION) {
        result = XFW_DEF::TYPE_build_version_command;
    } else if (nCommandId == XMACH_DEF::S_LC_MAIN) {
        result = XFW_DEF::TYPE_main_command;
    } else if (nCommandId == XMACH_DEF::S_LC_SEGMENT) {
        result = XFW_DEF::TYPE_segment_command;
    } else if (nCommandId == XMACH_DEF::S_LC_SEGMENT_64) {
        result = XFW_DEF::TYPE_segment_command_64;
    } else if (nCommandId == XMACH_DEF::S_LC_FILESET_ENTRY) {
        result = XFW_DEF::TYPE_fileset_entry_command;
    } else if (nCommandId == XMACH_DEF::S_LC_SOURCE_VERSION) {
        result = XFW_DEF::TYPE_source_version_command;
    } else if ((nCommandId == XMACH_DEF::S_LC_ID_DYLINKER) || (nCommandId == XMACH_DEF::S_LC_LOAD_DYLINKER)) {
        result = XFW_DEF::TYPE_dylinker_command;
    } else if (nCommandId == XMACH_DEF::S_LC_DATA_IN_CODE) {
        result = XFW_DEF::TYPE_data_in_code_command;
    } else if (nCommandId == XMACH_DEF::S_LC_CODE_SIGNATURE) {
        result = XFW_DEF::TYPE_code_signature_command;
    } else if (nCommandId == XMACH_DEF::S_LC_ROUTINES) {
        result = XFW_DEF::TYPE_routines_command;
    } else if (nCommandId == XMACH_DEF::S_LC_ROUTINES_64) {
        result = XFW_DEF::TYPE_routines_command_64;
    } else {
        result = XFW_DEF::TYPE_MACH_load_command;
    }

    return result;
}

void XFormatWidget::_widgetValueChanged(QVariant vValue)
{
    Q_UNUSED(vValue)
#ifdef QT_DEBUG
    qDebug("TODO _widgetValueChanged");
#endif
}

void XFormatWidget::contextMenuGenericHeaderWidget(const QPoint &pos, QTableWidget *pTableWidget, QList<RECWIDGET> *pListRecWidget, XFW_DEF::CWOPTIONS *pCwOptions)
{
    qint32 nRow = pTableWidget->currentIndex().row();

    if (nRow != -1) {
        QMenu contextMenu(this);

        QList<XShortcuts::MENUITEM> listMenuItems;

        getShortcuts()->_addMenuItem_CopyRow(&listMenuItems, pTableWidget);

        QList<QObject *> listObjects = getShortcuts()->adjustContextMenu(&contextMenu, &listMenuItems);

        contextMenu.exec(pTableWidget->viewport()->mapToGlobal(pos));

        XOptions::deleteQObjectList(&listObjects);
    }
}

void XFormatWidget::contextMenuGenericTableWidget(const QPoint &pos, QTableView *pTableView, QList<RECWIDGET> *pListRecWidget, XFW_DEF::CWOPTIONS *pCwOptions)
{
    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->model()->index(nRow, 0);

        XFW_DEF::TYPE _type = (XFW_DEF::TYPE)(pTableView->model()->data(index, Qt::UserRole + XFW_DEF::TABLEDATA_TYPE).toInt());
        qint64 nDataOffset = (pTableView->model()->data(index, Qt::UserRole + XFW_DEF::TABLEDATA_DATAOFFSET).toLongLong());
        qint64 nDataSize = (pTableView->model()->data(index, Qt::UserRole + XFW_DEF::TABLEDATA_DATASIZE).toLongLong());
        qint64 nDataCount = (pTableView->model()->data(index, Qt::UserRole + XFW_DEF::TABLEDATA_DATACOUNT).toLongLong());

        QString sTypeString = _getInitString(_type, nDataOffset, nDataSize, nDataCount);

        QMenu contextMenu(this);

        QList<XShortcuts::MENUITEM> listMenuItems;

        {
            XShortcuts::MENUITEM menuItem = {};
            menuItem.nShortcutId = XShortcuts::X_ID_TABLE_SHOW;
            menuItem.pRecv = this;
            menuItem.pMethod = SLOT(showTableRecord());
            menuItem.nSubgroups = XShortcuts::GROUPID_NONE;
            menuItem.sPropertyName = "INITSTRING";
            menuItem.varProperty = sTypeString;

            listMenuItems.append(menuItem);
        }

        getShortcuts()->_addMenuItem_CopyRow(&listMenuItems, pTableView);

        QList<QObject *> listObjects = getShortcuts()->adjustContextMenu(&contextMenu, &listMenuItems);

        contextMenu.exec(pTableView->viewport()->mapToGlobal(pos));

        XOptions::deleteQObjectList(&listObjects);
    }
}

void XFormatWidget::tableView_doubleClicked(QTableView *pTableView, const QModelIndex &index)
{
    QModelIndex _index = pTableView->model()->index(index.row(), 0);

    XFW_DEF::TYPE _type = (XFW_DEF::TYPE)(pTableView->model()->data(_index, Qt::UserRole + XFW_DEF::TABLEDATA_TYPE).toInt());
    qint64 nDataOffset = (pTableView->model()->data(_index, Qt::UserRole + XFW_DEF::TABLEDATA_DATAOFFSET).toLongLong());
    qint64 nDataSize = (pTableView->model()->data(_index, Qt::UserRole + XFW_DEF::TABLEDATA_DATASIZE).toLongLong());
    qint64 nDataCount = (pTableView->model()->data(index, Qt::UserRole + XFW_DEF::TABLEDATA_DATACOUNT).toLongLong());

    QString sTypeString = _getInitString(_type, nDataOffset, nDataSize, nDataCount);

    emit showCwWidget(sTypeString, true);
}

void XFormatWidget::_followLocation(quint64 nLocation, qint32 nLocationType, qint64 nSize, qint32 nWidgetType)
{
    Q_UNUSED(nLocation)
    Q_UNUSED(nLocationType)
    Q_UNUSED(nSize)
    Q_UNUSED(nWidgetType)

#ifdef QT_DEBUG
    qDebug("TODO _followLocation");
#endif
}

// void XFormatWidget::resizeToolsWidget(QWidget *pParent,ToolsWidget
// *pToolWidget)
//{
//     qint32 nHeight=pParent->height();
//     qint32 _nMaxHeight=pToolWidget->maximumHeight();
//     pToolWidget->setMaximumHeight(nHeight/4);
//     pToolWidget->setMaximumHeight(_nMaxHeight);
// }

void XFormatWidget::valueChangedSlot(QVariant varValue)
{
    qint32 nPosition = sender()->property("POSITION").toInt();
    qint64 nOffset = sender()->property("OFFSET").toInt();
    qint64 nSize = sender()->property("SIZE").toInt();

    setValue(varValue, nPosition, nOffset, nSize);
}

void XFormatWidget::setEdited(qint64 nDeviceOffset, qint64 nDeviceSize)
{
    Q_UNUSED(nDeviceOffset)
    Q_UNUSED(nDeviceSize)

#ifdef QT_DEBUG
    qDebug("void FormatWidget::setEdited()");
#endif
    reset();
    reloadData(true);

    //    reset();

    //    if(bState)
    //    {
    //        reloadData();
    //    }

    //    emit changed();
}

void XFormatWidget::allReload(qint64 nDeviceOffset, qint64 nDeviceSize)
{
    Q_UNUSED(nDeviceOffset)
    Q_UNUSED(nDeviceSize)
    // TODO save treeview position
    bool bIsReadOnly = isReadonly();

    reset();
    reload();

    reloadData(false);

    setReadonly(bIsReadOnly);
}

// void XFormatWidget::showHex(qint64 nOffset, qint64 nSize)
// {
//     XHexViewWidget::OPTIONS hexOptions = {};

//     hexOptions.nStartAddress = 0;
//     hexOptions.nStartSelectionOffset = nOffset;
//     hexOptions.nSizeOfSelection = nSize;

//     DialogHexView dialogHexView(this);
//     dialogHexView.setGlobal(getShortcuts(), getGlobalOptions());
//     dialogHexView.setData(getDevice(), hexOptions);
//     dialogHexView.setXInfoDB(getXInfoDB());

//     connect(&dialogHexView, SIGNAL(editState(bool)), this, SLOT(setEdited(bool)));

//     dialogHexView.exec();

//     reloadData(true);  // TODO Check
// }

void XFormatWidget::widgetValueChanged(QVariant vValue)
{
    _widgetValueChanged(vValue);
}

void XFormatWidget::dumpRegion(qint64 nOffset, qint64 nSize, const QString &sName)
{
    // QString _sName = sName;

    // if (_sName == "") {
    //     _sName = tr("Dump");
    // }

    // QString sSaveFileName = XBinary::getResultFileName(getDevice(), QString("%1.bin").arg(_sName));
    // QString sFileName = QFileDialog::getSaveFileName(this, tr("Save dump"), sSaveFileName, QString("%1 (*.bin)").arg(tr("Raw data")));

    // if (!sFileName.isEmpty()) {
    //     DialogDumpProcess dd(this);
    //     dd.setGlobal(getShortcuts(), getGlobalOptions());
    //     dd.setData(getDevice(), nOffset, nSize, sFileName, DumpProcess::DT_DUMP_DEVICE_OFFSET);

    //     dd.showDialogDelay();
    // }
}

void XFormatWidget::_reload()
{
    reloadData(true);
}

void XFormatWidget::showTableRecord()
{
    QString sInitString = (sender()->property("INITSTRING").toString());

    emit showCwWidget(sInitString, true);
}

void XFormatWidget::followLocationSlot(quint64 nLocation, qint32 nLocationType, qint64 nSize, qint32 nWidgetType)
{
    _followLocation(nLocation, nLocationType, nSize, nWidgetType);
}

void XFormatWidget::onToolButtonClicked()
{
    qint32 nLocation = sender()->property("LOCATION").toULongLong();
    qint64 nLocationType = sender()->property("LOCATIONTYPE").toInt();
    qint64 nSize = sender()->property("SIZE").toInt();
    qint64 nWidgetType = sender()->property("WIDGETTYPE").toInt();

    emit followLocation(nLocation, nLocationType, nSize, nWidgetType);
}

void XFormatWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

bool XFormatWidget::createHeaderTable(QTableWidget *pTableWidget, const QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords, QList<RECWIDGET> *pListRecWidget,
                                      qint64 nOffset, XBinary::ENDIAN endian, QVariant var1, QVariant var2)
{
    qint32 nNumberOfRecords = pListHeaderRecords->count();

    pTableWidget->clear();

    QStringList slHeader;
    slHeader.append(tr("Name"));
    slHeader.append(tr("Offset"));
    slHeader.append(tr("Size"));
    slHeader.append(tr("Type"));
    slHeader.append(tr("Value"));
    slHeader.append(tr(""));
    slHeader.append(tr("Comment"));

    pTableWidget->setColumnCount(slHeader.count());
    pTableWidget->setRowCount(nNumberOfRecords);

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        RECWIDGET recWidget = {};
        recWidget.endian = endian;
        recWidget.nOffset = nOffset + pListHeaderRecords->at(i).nOffset;
        recWidget.nSize = pListHeaderRecords->at(i).nSize;
        recWidget.nPosition = pListHeaderRecords->at(i).nPosition;
        recWidget.nVType = pListHeaderRecords->at(i).vtype;
        recWidget.nSubPosition = pListHeaderRecords->at(i).nSubPosition;

        QTableWidgetItem *pItemName = new QTableWidgetItem;
        pItemName->setText(pListHeaderRecords->at(i).sName);
        pItemName->setData(Qt::UserRole + HEADER_DATA_OFFSET, nOffset + pListHeaderRecords->at(i).nOffset);
        pItemName->setData(Qt::UserRole + HEADER_DATA_SIZE, pListHeaderRecords->at(i).nSize);
        pTableWidget->setItem(i, HEADER_COLUMN_NAME, pItemName);

        QTableWidgetItem *pItemOffset = new QTableWidgetItem;

        if (pListHeaderRecords->at(i).nOffset != -1) {
            pItemOffset->setText(XBinary::valueToHex((quint16)pListHeaderRecords->at(i).nOffset));
        }

        pItemOffset->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);  // TODO
        pTableWidget->setItem(i, HEADER_COLUMN_OFFSET, pItemOffset);

        QTableWidgetItem *pItemSize = new QTableWidgetItem;

        if (pListHeaderRecords->at(i).nSize != 0) {
            pItemSize->setText(XBinary::valueToHex((quint16)pListHeaderRecords->at(i).nSize));
        }

        pItemSize->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);  // TODO
        pTableWidget->setItem(i, HEADER_COLUMN_SIZE, pItemSize);

        QTableWidgetItem *pItemType = new QTableWidgetItem;
        pItemType->setText(pListHeaderRecords->at(i).sType);
        pTableWidget->setItem(i, HEADER_COLUMN_TYPE, pItemType);

        recWidget.pValue = new QTableWidgetItem;
        pTableWidget->setItem(i, HEADER_COLUMN_VALUE, recWidget.pValue);

        if ((recWidget.nVType & XFW_DEF::VAL_TYPE_DATA_INT) || ((recWidget.nVType & XFW_DEF::VAL_TYPE_DATA_ARRAY) && (recWidget.nVType & XFW_DEF::VAL_TYPE_ANSI))) {
            recWidget.pLineEdit = new XLineEditHEX();
            recWidget.pLineEdit->setProperty("POSITION", recWidget.nPosition);
            recWidget.pLineEdit->setProperty("OFFSET", recWidget.nOffset);
            recWidget.pLineEdit->setProperty("SIZE", recWidget.nSize);

            // if ((pListHeaderRecords->at(i).vtype == FW_DEF::VAL_TYPE_TEXT) || (pListHeaderRecords->at(i).vtype == FW_DEF::VAL_TYPE_UUID)) {
            //     if (pListHeaderRecords->at(i).nSize != -1) {
            //         recWidget.pLineEdit->setMaxLength(pListHeaderRecords->at(i).nSize);
            //     }
            // }

            connect(recWidget.pLineEdit, SIGNAL(valueChanged(QVariant)), this, SLOT(valueChangedSlot(QVariant)));

            pTableWidget->setCellWidget(i, HEADER_COLUMN_VALUE, recWidget.pLineEdit);

            if (pListHeaderRecords->at(i).nSize == 0) {
                recWidget.pLineEdit->setEnabled(false);
            }
        }

        recWidget.pComment = new QTableWidgetItem;

        pTableWidget->setItem(i, HEADER_COLUMN_COMMENT, recWidget.pComment);

        pListRecWidget->append(recWidget);
    }

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        if ((pListRecWidget->at(i).nVType & XFW_DEF::VAL_TYPE_OFFSET) || (pListRecWidget->at(i).nVType & XFW_DEF::VAL_TYPE_ADDRESS) ||
            ((pListRecWidget->at(i).nVType & XFW_DEF::VAL_TYPE_SIZE) && (pListRecWidget->at(i).nSubPosition != -1))) {
            _adjustCellWidget(pListRecWidget, pTableWidget, i, HEADER_COLUMN_INFO);

            XBinary::LT locType = XBinary::LT_UNKNOWN;

            if (pListRecWidget->at(i).nVType & XFW_DEF::VAL_TYPE_OFFSET) {
                locType = XBinary::LT_OFFSET;
            } else if (pListRecWidget->at(i).nVType & XFW_DEF::VAL_TYPE_ADDRESS) {
                locType = XBinary::LT_ADDRESS;
            } else if (pListRecWidget->at(i).nVType & XFW_DEF::VAL_TYPE_SIZE) {
                if (pListRecWidget->at(i).nSubPosition != -1) {
                    qint32 nSubPosition = pListRecWidget->at(i).nSubPosition;
                    if (pListRecWidget->at(nSubPosition).nVType & XFW_DEF::VAL_TYPE_OFFSET) {
                        locType = XBinary::LT_OFFSET;
                    } else if (pListRecWidget->at(nSubPosition).nVType & XFW_DEF::VAL_TYPE_ADDRESS) {
                        locType = XBinary::LT_ADDRESS;
                    }
                }
            }

            if (locType != XBinary::LT_UNKNOWN) {
                QToolButton *pButton = new QToolButton(0);
                pButton->setProperty("LOCATIONTYPE", locType);
                pButton->setProperty("WIDGETTYPE", XOptions::WIDGETTYPE_HEX);
                pButton->setProperty("DELTA", var1.toULongLong());

                if (pListRecWidget->at(i).nVType & XFW_DEF::VAL_TYPE_SIZE) {
                    pButton->setText(tr("Size"));
                    XOptions::adjustToolButton(pButton, XOptions::ICONTYPE_SIZE);
                } else {
                    pButton->setText(tr("Hex"));
                    XOptions::adjustToolButton(pButton, XOptions::ICONTYPE_HEX);
                }

                pButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

                connect(pButton, SIGNAL(clicked(bool)), this, SLOT(onToolButtonClicked()));

                (*pListRecWidget)[i].pWidget->layout()->addWidget(pButton);
            }

            if (pListRecWidget->at(i).nVType & XFW_DEF::VAL_TYPE_CODE) {
                QToolButton *pButton = new QToolButton(0);
                pButton->setText(tr("Disasm"));
                pButton->setProperty("LOCATIONTYPE", locType);
                pButton->setProperty("WIDGETTYPE", XOptions::WIDGETTYPE_DISASM);
                pButton->setProperty("DELTA", var1.toULongLong());
                XOptions::adjustToolButton(pButton, XOptions::ICONTYPE_DISASM);
                pButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

                connect(pButton, SIGNAL(clicked(bool)), this, SLOT(onToolButtonClicked()));

                (*pListRecWidget)[i].pWidget->layout()->addWidget(pButton);
            }
        }
    }

    adjustGenericHeader(pTableWidget, pListHeaderRecords);

    //    XOptions::setTableWidgetHeaderAlignment(pTableWidget, HEADER_COLUMN_NAME, Qt::AlignLeft | Qt::AlignVCenter);
    //    XOptions::setTableWidgetHeaderAlignment(pTableWidget, HEADER_COLUMN_OFFSET, Qt::AlignRight | Qt::AlignVCenter);
    //    XOptions::setTableWidgetHeaderAlignment(pTableWidget, HEADER_COLUMN_TYPE, Qt::AlignLeft | Qt::AlignVCenter);
    //    XOptions::setTableWidgetHeaderAlignment(pTableWidget, HEADER_COLUMN_VALUE, Qt::AlignRight | Qt::AlignVCenter);
    //    XOptions::setTableWidgetHeaderAlignment(pTableWidget, HEADER_COLUMN_INFO, Qt::AlignLeft | Qt::AlignVCenter);
    //    XOptions::setTableWidgetHeaderAlignment(pTableWidget, HEADER_COLUMN_COMMENT, Qt::AlignLeft | Qt::AlignVCenter);

    return true;
}

bool XFormatWidget::createListTable(qint32 nType, QTableWidget *pTableWidget, const XFW_DEF::HEADER_RECORD *pRecords, XLineEditHEX **ppLineEdits, qint32 nNumberOfRecords)
{
    pTableWidget->setColumnCount(2);
    pTableWidget->setRowCount(nNumberOfRecords);

    QStringList slHeader;
    slHeader.append(tr("Name"));
    slHeader.append(tr("Value"));

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        QTableWidgetItem *pItemName = new QTableWidgetItem;
        pItemName->setText(pRecords[i].sName);
        pTableWidget->setItem(i, LIST_COLUMN_NAME, pItemName);

        ppLineEdits[i] = new XLineEditHEX(this);

        ppLineEdits[i]->setProperty("STYPE", nType);
        ppLineEdits[i]->setProperty("NDATA", pRecords[i].nPosition);

        if (pRecords[i].vtype & XFW_DEF::VAL_TYPE_ANSI) {
            ppLineEdits[i]->setAlignment(Qt::AlignLeft);
        }

        if (pRecords[i].nOffset != -1) {
            if (pRecords[i].vtype & XFW_DEF::VAL_TYPE_DATA_ARRAY) {
                if (pRecords[i].nSize != -1) {
                    ppLineEdits[i]->setMaxLength(pRecords[i].nSize);
                }
            }
            connect(ppLineEdits[i], SIGNAL(valueChanged(QVariant)), this, SLOT(valueChangedSlot(QVariant)));
        } else {
            ppLineEdits[i]->setReadOnly(true);
        }

        pTableWidget->setCellWidget(i, LIST_COLUMN_VALUE, ppLineEdits[i]);
    }

    pTableWidget->horizontalHeader()->setSectionResizeMode(LIST_COLUMN_VALUE, QHeaderView::Stretch);

    adjustListTable(nType, pTableWidget);

    return true;
}

void XFormatWidget::addComboBox(QTableWidget *pTableWidget, QList<RECWIDGET> *pListRecWidget, const QMap<quint64, QString> &mapData, qint32 nPosition,
                                XComboBoxEx::CBTYPE cbtype, quint64 nMask)
{
    if (nPosition < pListRecWidget->count()) {
        _adjustCellWidget(pListRecWidget, pTableWidget, nPosition, HEADER_COLUMN_INFO);

        // XComboBoxEx *pComboBox = new XComboBoxEx((*pListRecWidget)[nPosition].pWidget);
        XComboBoxEx *pComboBox = new XComboBoxEx(0);
        pComboBox->setProperty("POSITION", pListRecWidget->at(nPosition).nPosition);
        pComboBox->setProperty("OFFSET", pListRecWidget->at(nPosition).nOffset);
        pComboBox->setProperty("SIZE", pListRecWidget->at(nPosition).nSize);
        pComboBox->setData(mapData, cbtype, nMask, tr("Flags"));
        pComboBox->setReadOnly(isReadonly());

        connect(pComboBox, SIGNAL(valueChanged(QVariant)), this, SLOT(valueChangedSlot(QVariant)));

        (*pListRecWidget)[nPosition].pWidget->layout()->addWidget(pComboBox);
    }
}

void XFormatWidget::_adjustCellWidget(QList<RECWIDGET> *pListRecWidget, QTableWidget *pTableWidget, qint32 nRow, qint32 nColumn)
{
    if ((*pListRecWidget)[nRow].pWidget == 0) {
        (*pListRecWidget)[nRow].pWidget = new QWidget(pTableWidget);

        QHBoxLayout *pLayout = new QHBoxLayout((*pListRecWidget)[nRow].pWidget);
        pLayout->setContentsMargins(0, 0, 0, 0);
        pLayout->setSpacing(0);
        (*pListRecWidget)[nRow].pWidget->setLayout(pLayout);
        pTableWidget->setCellWidget(nRow, nColumn, (*pListRecWidget)[nRow].pWidget);
    }
}

// InvWidget *FormatWidget::createInvWidget(QTableWidget *pTableWidget, qint32 nType, qint32 nData, InvWidget::TYPE widgetType)
// {
//     InvWidget *pResult = new InvWidget(this, widgetType);

//     pResult->setGlobal(getShortcuts(), getGlobalOptions());

//     pResult->setProperty("STYPE", nType);
//     pResult->setProperty("NDATA", nData);

//     connect(pResult, SIGNAL(showHex(qint64, qint64)), this, SLOT(showInHexWindow(qint64, qint64)));
//     connect(pResult, SIGNAL(showDisasm(XADDR)), this, SLOT(showInDisasmWindowAddress(XADDR)));

//     pTableWidget->setCellWidget(nData, HEADER_COLUMN_INFO, pResult);

//     return pResult;
// }

XDateTimeEditX *XFormatWidget::createTimeDateEdit(QTableWidget *pTableWidget, qint32 nType, qint32 nData)
{
    XDateTimeEditX *result = new XDateTimeEditX(this);

    result->setProperty("STYPE", nType);
    result->setProperty("NDATA", nData);

    connect(result, SIGNAL(valueChanged(QVariant)), this, SLOT(widgetValueChanged(QVariant)));  // TODO Check widgetValueChanged

    pTableWidget->setCellWidget(nData, HEADER_COLUMN_INFO, result);

    return result;
}

QPushButton *XFormatWidget::createPushButton(QTableWidget *pTableWidget, qint32 nType, qint32 nData, const QString &sText)
{
    QPushButton *pPushButton = new QPushButton(this);

    pPushButton->setText(sText);
    pPushButton->setProperty("STYPE", nType);
    pPushButton->setProperty("NDATA", nData);

    connect(pPushButton, SIGNAL(clicked()), this, SLOT(widgetAction()));

    pTableWidget->setCellWidget(nData, HEADER_COLUMN_INFO, pPushButton);

    return pPushButton;
}