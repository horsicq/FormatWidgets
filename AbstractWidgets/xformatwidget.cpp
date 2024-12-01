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

            if (recWidget.pComboBox) {
                getGlobalOptions()->adjustWidget(recWidget.pComboBox, XOptions::ID_VIEW_FONT_TABLEVIEWS);
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

QTreeWidgetItem *XFormatWidget::createNewItem(XFW_DEF::TYPE type, XFW_DEF::WIDGETMODE widgetMode, const QString &sTitle, XOptions::ICONTYPE iconType, qint64 nOffset,
                                              qint64 nSize, QVariant var1, QVariant var2, XBinary::MODE mode, XBinary::ENDIAN endian)
{
    QTreeWidgetItem *pResult = new QTreeWidgetItem;

    pResult->setText(0, sTitle);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_TYPE, type);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_WIDGETMODE, widgetMode);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_OFFSET, nOffset);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_SIZE, nSize);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_VAR1, var1);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_VAR2, var2);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_NAME, sTitle);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_MODE, mode);
    pResult->setData(0, Qt::UserRole + XFW_DEF::WIDGET_DATA_ENDIAN, endian);

    XOptions::adjustTreeWidgetItem(pResult, iconType);

    return pResult;
}

QList<XFW_DEF::HEADER_RECORD> XFormatWidget::getHeaderRecords(const XFW_DEF::CWOPTIONS *pCwOptions)
{
    QList<XFW_DEF::HEADER_RECORD> listResult;

    const XFW_DEF::HEADER_RECORD *pRecords = 0;
    qint32 nNumberOfRecords = 0;

    if (pCwOptions->_type == XFW_DEF::TYPE_MACH_mach_header) {
        if (pCwOptions->mode == XBinary::MODE_64) {
            pRecords = X_mach_header::records64;
            nNumberOfRecords = X_mach_header::__data_size;
        } else if (pCwOptions->mode == XBinary::MODE_32) {
            pRecords = X_mach_header::records32;
            nNumberOfRecords = X_mach_header::__data_size - 1;
        }
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MACH_commands) {
        pRecords = X_mach_commands::records;
        nNumberOfRecords = X_mach_commands::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MACH_segments) {
        if (pCwOptions->mode == XBinary::MODE_64) {
            pRecords = X_mach_segments::records64;
            nNumberOfRecords = X_mach_segments::__data_size;
        } else if (pCwOptions->mode == XBinary::MODE_32) {
            pRecords = X_mach_segments::records32;
            nNumberOfRecords = X_mach_segments::__data_size;
        }
    } else if (pCwOptions->_type == XFW_DEF::TYPE_ELF_elf_ehdr) {
        if (pCwOptions->mode == XBinary::MODE_64) {
            pRecords = X_Elf_Ehdr::records64;
            nNumberOfRecords = X_Elf_Ehdr::__data_size;
        } else if (pCwOptions->mode == XBinary::MODE_32) {
            pRecords = X_Elf_Ehdr::records32;
            nNumberOfRecords = X_Elf_Ehdr::__data_size;
        }
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MSDOS_EXE_file) {
        pRecords = X_Exe_file::records;
        nNumberOfRecords = X_Exe_file::__data_size;
    } else if (pCwOptions->_type == XFW_DEF::TYPE_MSDOS_IMAGE_DOS_HEADER) {
        pRecords = X_IMAGE_DOS_HEADER::records;
        nNumberOfRecords = X_IMAGE_DOS_HEADER::__data_size;
    }

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        XFW_DEF::HEADER_RECORD record = pRecords[i];

        listResult.append(record);
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
            if ((recWidget.vtype == XFW_DEF::VAL_TYPE_DATA) || (recWidget.vtype == XFW_DEF::VAL_TYPE_SIZE) || (recWidget.vtype == XFW_DEF::VAL_TYPE_ADDRESS) ||
                (recWidget.vtype == XFW_DEF::VAL_TYPE_OFFSET)) {
                if (recWidget.nSize == 1) {
                    binary.write_uint8(recWidget.nOffset, vValue.toUInt());
                } else if (recWidget.nSize == 2) {
                    binary.write_uint16(recWidget.nOffset, vValue.toUInt(), (recWidget.endian == XBinary::ENDIAN_BIG));
                } else if (recWidget.nSize == 4) {
                    binary.write_uint32(recWidget.nOffset, vValue.toUInt(), (recWidget.endian == XBinary::ENDIAN_BIG));
                } else if (recWidget.nSize == 8) {
                    binary.write_uint64(recWidget.nOffset, vValue.toULongLong(), (recWidget.endian == XBinary::ENDIAN_BIG));
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
    qint32 nSymbolWidth = XLineEditHEX::getSymbolWidth(pTableWidget);

    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_NAME, QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_OFFSET, QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_SIZE, QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_TYPE, QHeaderView::ResizeToContents);
    pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, nSymbolWidth * 12);
    pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, nSymbolWidth * 20);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_COMMENT, QHeaderView::Stretch);
}

void XFormatWidget::adjustGenericTable(QTableView *pTableView, const QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords)
{
    // QString sFontName = pTableView->font().toString();
    QFontMetrics fm(pTableView->font());

    // qint32 nSymbolWidth = fm.boundingRect("W").width();

    qint32 nNumberOfRecords = pListHeaderRecords->count();

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        qint32 nWidth = 200;

        if ((pListHeaderRecords->at(i).vtype == XFW_DEF::VAL_TYPE_DATA) || (pListHeaderRecords->at(i).vtype == XFW_DEF::VAL_TYPE_SIZE) ||
            (pListHeaderRecords->at(i).vtype == XFW_DEF::VAL_TYPE_ADDRESS) || (pListHeaderRecords->at(i).vtype == XFW_DEF::VAL_TYPE_OFFSET)) {
            if (pListHeaderRecords->at(i).nSize == 1) {
                nWidth = fm.boundingRect(" 00 ").width();
            } else if (pListHeaderRecords->at(i).nSize == 2) {
                nWidth = fm.boundingRect(" 0000 ").width();
            } else if (pListHeaderRecords->at(i).nSize == 4) {
                nWidth = fm.boundingRect(" 00000000 ").width();
            } else if (pListHeaderRecords->at(i).nSize == 8) {
                nWidth = fm.boundingRect(" 0000000000000000 ").width();
            }
        } else if (XFW_DEF::VAL_TYPE_NUMBER) {
            nWidth = fm.boundingRect(" 000").width();
        }

        if (i == (nNumberOfRecords - 1)) {
            if (pListHeaderRecords->at(i).vtype == XFW_DEF::VAL_TYPE_INFO) {
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

void XFormatWidget::showSectionHex(QTableView *pTableView)
{
    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset = pTableView->model()->data(index, Qt::UserRole + XFW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = pTableView->model()->data(index, Qt::UserRole + XFW_DEF::SECTION_DATA_SIZE).toLongLong();

        showInHexWindow(nOffset, nSize);

        reloadData(true);  // TODO Check

        pTableView->setCurrentIndex(pTableView->model()->index(nRow, 0));
    }
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

qint64 XFormatWidget::getTableViewItemSize(QTableView *pTableView)
{
    qint64 nResult = 0;

    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(0);
        nResult = pTableView->model()->data(index, Qt::UserRole + XFW_DEF::SECTION_DATA_SIZE).toLongLong();
    }

    return nResult;
}

void XFormatWidget::showTableViewDemangle(QTableView *pTableView, qint32 nColumn)
{
    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(nColumn);
        QString sString = pTableView->model()->data(index).toString();

        showDemangle(sString);
    }
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

        if (recWidget.pComboBox) {
            delete recWidget.pComboBox;
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

        if (recWidget.pComboBox) {
            recWidget.pComboBox->setReadOnly(bState);
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
    return _getInitString(pCwOptions->_type, pCwOptions->nDataOffset, pCwOptions->nDataSize);
}

QString XFormatWidget::_getInitString(XFW_DEF::TYPE _type, qint64 nDataOffset, qint64 nDataSize)
{
    QString sResult;

    sResult = QString("%1#%2#%3").arg(QString::number((qint32)_type), QString::number(nDataOffset), QString::number(nDataSize));

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

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        RECWIDGET recWidget = pListRecWidget->at(i);

        QVariant varValue = _readVariant(&binary, recWidget.nOffset, recWidget.nSize, recWidget.vtype, (recWidget.endian == XBinary::ENDIAN_BIG));

        _adjustRecWidget(&recWidget, varValue);
    }
}

void XFormatWidget::_adjustRecWidget(RECWIDGET *pRecWidget, QVariant varValue)
{
    QString sComment;

    bool bBlockLineEdit = false;
    bool bBlockComboBox = false;

    if (pRecWidget->pLineEdit) bBlockLineEdit = pRecWidget->pLineEdit->blockSignals(true);
    if (pRecWidget->pComboBox) bBlockComboBox = pRecWidget->pComboBox->blockSignals(true);

    if (pRecWidget->pLineEdit) {
        if ((pRecWidget->vtype == XFW_DEF::VAL_TYPE_DATA) || (pRecWidget->vtype == XFW_DEF::VAL_TYPE_SIZE) || (pRecWidget->vtype == XFW_DEF::VAL_TYPE_ADDRESS) ||
            (pRecWidget->vtype == XFW_DEF::VAL_TYPE_OFFSET)) {
            if (pRecWidget->nSize == 1) {
                pRecWidget->pLineEdit->setValue_uint8(varValue.toUInt(), XLineEditHEX::_MODE_HEX);
            } else if (pRecWidget->nSize == 2) {
                pRecWidget->pLineEdit->setValue_uint16(varValue.toUInt(), XLineEditHEX::_MODE_HEX);
            } else if (pRecWidget->nSize == 4) {
                pRecWidget->pLineEdit->setValue_uint32(varValue.toUInt(), XLineEditHEX::_MODE_HEX);
            } else if (pRecWidget->nSize == 8) {
                pRecWidget->pLineEdit->setValue_uint64(varValue.toULongLong(), XLineEditHEX::_MODE_HEX);
            }
        }
    }

    if (pRecWidget->pComboBox) pRecWidget->pComboBox->setValue(varValue);
    if (pRecWidget->vtype == XFW_DEF::VAL_TYPE_SIZE) sComment = XBinary::bytesCountToString(varValue.toULongLong());

    if (pRecWidget->pComboBox) {
        sComment = pRecWidget->pComboBox->getDescription();
    }

    if (sComment != "") {
        pRecWidget->pComment->setText(sComment);
    }

    if (pRecWidget->pLineEdit) pRecWidget->pLineEdit->blockSignals(bBlockLineEdit);
    if (pRecWidget->pComboBox) pRecWidget->pComboBox->blockSignals(bBlockComboBox);
}

QVariant XFormatWidget::_readVariant(XBinary *pBinary, qint64 nOffset, qint64 nSize, XFW_DEF::VAL_TYPE vtype, bool bIsBigEndian)
{
    QVariant varResult;

    if ((vtype == XFW_DEF::VAL_TYPE_DATA) || (vtype == XFW_DEF::VAL_TYPE_SIZE) || (vtype == XFW_DEF::VAL_TYPE_ADDRESS) || (vtype == XFW_DEF::VAL_TYPE_OFFSET)) {
        if (nSize == 1) {
            varResult = pBinary->read_uint8(nOffset);
        } else if (nSize == 2) {
            varResult = pBinary->read_uint16(nOffset, bIsBigEndian);
        } else if (nSize == 4) {
            varResult = pBinary->read_uint32(nOffset, bIsBigEndian);
        } else if (nSize == 8) {
            varResult = pBinary->read_uint64(nOffset, bIsBigEndian);
        }
    }

    return varResult;
}

QString XFormatWidget::_valueToString(const QVariant &var, qint64 nSize, XFW_DEF::VAL_TYPE vtype)
{
    QString sResult;

    if ((vtype == XFW_DEF::VAL_TYPE_DATA) || (vtype == XFW_DEF::VAL_TYPE_SIZE) || (vtype == XFW_DEF::VAL_TYPE_ADDRESS) || (vtype == XFW_DEF::VAL_TYPE_OFFSET)) {
        if (nSize == 1) {
            sResult = QString("%1").arg(var.toUInt(), 2, 16, QChar('0'));
        } else if (nSize == 2) {
            sResult = QString("%1").arg(var.toUInt(), 4, 16, QChar('0'));
        } else if (nSize == 4) {
            sResult = QString("%1").arg(var.toUInt(), 8, 16, QChar('0'));
        } else if (nSize == 8) {
            sResult = QString("%1").arg(var.toULongLong(), 16, 16, QChar('0'));
        }
    }

    return sResult;
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

void XFormatWidget::_showInDisasmWindowAddress(XADDR nAddress)
{
    Q_UNUSED(nAddress)
#ifdef QT_DEBUG
    qDebug("TODO _showInDisasmWindowAddress");
#endif
}

void XFormatWidget::_showInDisasmWindowOffset(qint64 nOffset)
{
    Q_UNUSED(nOffset)
#ifdef QT_DEBUG
    qDebug("TODO _showInDisasmWindowOffset");
#endif
}

void XFormatWidget::_showInMemoryMapWindowOffset(qint64 nOffset)
{
    Q_UNUSED(nOffset)
#ifdef QT_DEBUG
    qDebug("TODO _showInMemoryMapWindowOffset");
#endif
}

void XFormatWidget::_showInHexWindow(qint64 nOffset, qint64 nSize)
{
    Q_UNUSED(nOffset)
    Q_UNUSED(nSize)
#ifdef QT_DEBUG
    qDebug("TODO _showInHexWindow");
#endif
}

void XFormatWidget::_findValue(quint64 nValue, XBinary::ENDIAN endian)
{
    Q_UNUSED(nValue)
    Q_UNUSED(endian)
#ifdef QT_DEBUG
    qDebug("TODO _findValue");
#endif
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

        QString sTypeString = _getInitString(_type, nDataOffset, nDataSize);

        QMenu contextMenu(this);

        QList<XShortcuts::MENUITEM> listMenuItems;

        {
            XShortcuts::MENUITEM menuItem = {};
            menuItem.nShortcutId = X_ID_TABLE_EDIT;
            menuItem.pRecv = this;
            menuItem.pMethod = SLOT(editRecord());
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

void XFormatWidget::showInDisasmWindowAddress(XADDR nAddress)
{
    _showInDisasmWindowAddress(nAddress);
}

void XFormatWidget::showInDisasmWindowOffset(qint64 nOffset)
{
    _showInDisasmWindowOffset(nOffset);
}

void XFormatWidget::showInMemoryMapWindowOffset(qint64 nOffset)
{
    _showInMemoryMapWindowOffset(nOffset);
}

void XFormatWidget::showInHexWindow(qint64 nOffset, qint64 nSize)
{
    _showInHexWindow(nOffset, nSize);
}

void XFormatWidget::showInHexWindow(qint64 nOffset)
{
    _showInHexWindow(nOffset, 1);
}

void XFormatWidget::widgetValueChanged(QVariant vValue)
{
    _widgetValueChanged(vValue);
}

void XFormatWidget::findValue(quint64 nValue, XBinary::ENDIAN endian)
{
    _findValue(nValue, endian);
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

void XFormatWidget::showDemangle(const QString &sString)
{
    // DialogDemangle dialogDemangle(this, sString);
    // dialogDemangle.setGlobal(getShortcuts(), getGlobalOptions());

    // dialogDemangle.exec();
}

void XFormatWidget::_reload()
{
    reloadData(true);
}

void XFormatWidget::editRecord()
{
    QString sInitString = (sender()->property("INITSTRING").toString());

    emit showCwWidget(sInitString, true);
}

void XFormatWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

bool XFormatWidget::createHeaderTable(QTableWidget *pTableWidget, const QList<XFW_DEF::HEADER_RECORD> *pListHeaderRecords, QList<RECWIDGET> *pListRecWidget,
                                      qint64 nOffset, XBinary::ENDIAN endian)
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
    slHeader.append(tr(""));

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
        recWidget.vtype = pListHeaderRecords->at(i).vtype;

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

        if ((recWidget.vtype == XFW_DEF::VAL_TYPE_DATA) || (recWidget.vtype == XFW_DEF::VAL_TYPE_SIZE) || (recWidget.vtype == XFW_DEF::VAL_TYPE_ADDRESS) ||
            (recWidget.vtype == XFW_DEF::VAL_TYPE_OFFSET)) {
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

        if (pRecords[i].vtype == XFW_DEF::VAL_TYPE_TEXT) {
            ppLineEdits[i]->setAlignment(Qt::AlignLeft);
        }

        if (pRecords[i].nOffset != -1) {
            if ((pRecords[i].vtype == XFW_DEF::VAL_TYPE_TEXT) || (pRecords[i].vtype == XFW_DEF::VAL_TYPE_UUID)) {
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

void XFormatWidget::addComment(QTableWidget *pTableWidget, qint32 nRow, qint32 nColumn, const QString &sComment)
{
    pTableWidget->item(nRow, nColumn)->setText(sComment);
}

void XFormatWidget::updateTableRecord(QTableWidget *pTableWidget, qint32 nRow, qint64 nOffset, qint64 nSize)
{
    pTableWidget->item(nRow, HEADER_COLUMN_NAME)->setData(Qt::UserRole + HEADER_DATA_OFFSET, nOffset);
    pTableWidget->item(nRow, HEADER_COLUMN_NAME)->setData(Qt::UserRole + HEADER_DATA_SIZE, nSize);
    pTableWidget->item(nRow, HEADER_COLUMN_OFFSET)->setText(XBinary::valueToHex((quint16)nOffset));
    pTableWidget->cellWidget(nRow, HEADER_COLUMN_VALUE)->setProperty("OFFSET", nOffset);
}

// bool FormatWidget::createDirectoryTable(int type,QTableWidget
// *pTableWidget,const DIRECTORY_ENTRY_RECORD *pRecords,int nRecordCount)
//{
//     Q_UNUSED(type)

//    pTableWidget->setColumnCount(4);
//    pTableWidget->setRowCount(nRecordCount);

//    qint32 nSymbolWidth=getSymbolWidth();

//    pTableWidget->setColumnWidth(0,nSymbolWidth*3);
//    pTableWidget->setColumnWidth(1,nSymbolWidth*12);
//    pTableWidget->setColumnWidth(2,nSymbolWidth*8);
//    pTableWidget->setColumnWidth(3,nSymbolWidth*8);

//    QStringList slHeader;
//    slHeader.append(tr(""));
//    slHeader.append(tr("Name"));
//    slHeader.append(tr("Address"));
//    slHeader.append(tr("Size"));

//    pTableWidget->setHorizontalHeaderLabels(slHeader);
//    pTableWidget->horizontalHeader()->setVisible(true);

//    for(qint32 i=0;i<nRecordCount;i++)
//    {
//        QTableWidgetItem *newItemNumber=new QTableWidgetItem;
//        newItemNumber->setText(QString("%1").arg(i));
//        pTableWidget->setItem(i,DIRECTORY_COLUMN_NUMBER,newItemNumber);

//        QTableWidgetItem *newItemName=new QTableWidgetItem;
//        newItemName->setText(pRecords[i].pszName);
//        pTableWidget->setItem(i,DIRECTORY_COLUMN_NAME,newItemName);

////        ppLineEdits1[i]=new XLineEditHEX(this);
////        ppLineEdits1[i]->setProperty("STYPE",type);
////        ppLineEdits1[i]->setProperty("NDATA",pRecords[i].nData);
////        ppLineEdits1[i]->setProperty("VTYPE",pRecords[i].vtype[0]);

////
/// connect(ppLineEdits1[i],SIGNAL(valueChanged(quint64)),this,SLOT(hexValueChanged(quint64)));

//// pTableWidget->setCellWidget(i,DIRECTORY_COLUMN_ADDRESS,ppLineEdits1[i]);

////        ppLineEdits2[i]=new XLineEditHEX(this);
////        ppLineEdits2[i]->setProperty("STYPE",type);
////        ppLineEdits2[i]->setProperty("NDATA",pRecords[i].nData);
////        ppLineEdits2[i]->setProperty("VTYPE",pRecords[i].vtype[1]);

////
/// connect(ppLineEdits2[i],SIGNAL(valueChanged(quint64)),this,SLOT(hexValueChanged(quint64)));

//// pTableWidget->setCellWidget(i,DIRECTORY_COLUMN_SIZE,ppLineEdits2[i]);
//    }

//    return true;
//}

bool XFormatWidget::createSectionTable(qint32 nType, QTableWidget *pTableWidget, const XFW_DEF::HEADER_RECORD *pRecords, qint32 nNumberOfRecords)
{
    Q_UNUSED(nType)

    QStringList slHeader;

    pTableWidget->setRowCount(0);

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        slHeader.append(pRecords[i].sName);
    }

    pTableWidget->setColumnCount(nNumberOfRecords);
    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    return true;
}

void XFormatWidget::setLineEditsReadOnly(XLineEditHEX **ppLineEdits, qint32 nCount, bool bState)
{
    for (qint32 i = 0; i < nCount; i++) {
        if (ppLineEdits[i]) {
            ppLineEdits[i]->setReadOnly(bState);
        }
    }
}

void XFormatWidget::setComboBoxesReadOnly(XComboBoxEx **ppComboBoxes, qint32 nCount, bool bState)
{
    for (qint32 i = 0; i < nCount; i++) {
        if (ppComboBoxes[i]) {
            ppComboBoxes[i]->setReadOnly(bState);
        }
    }
}

void XFormatWidget::setPushButtonReadOnly(QPushButton **ppPushButtons, qint32 nCount, bool bState)
{
    for (qint32 i = 0; i < nCount; i++) {
        if (ppPushButtons[i]) {
            ppPushButtons[i]->setEnabled(!bState);
        }
    }
}

void XFormatWidget::setDateTimeEditReadOnly(XDateTimeEditX **ppDateTimeEdits, qint32 nCount, bool bState)
{
    for (qint32 i = 0; i < nCount; i++) {
        if (ppDateTimeEdits[i]) {
            ppDateTimeEdits[i]->setReadOnly(bState);
        }
    }
}

void XFormatWidget::_deleteObjects(QObject **ppObjects, qint32 nCount)
{
    nCount = nCount / (sizeof(QObject *));

    for (qint32 i = 0; i < nCount; i++) {
        if (ppObjects[i]) {
            delete (ppObjects[i]);
            ppObjects[i] = 0;
        }
    }
}

void XFormatWidget::_deleteSubdevices(SubDevice **ppSubdevices, qint32 nCount)
{
    for (qint32 i = 0; i < nCount; i++) {
        if (ppSubdevices[i]) {
            ppSubdevices[i]->close();

            delete (ppSubdevices[i]);
            ppSubdevices[i] = 0;
        }
    }
}

XComboBoxEx *XFormatWidget::createComboBox(QTableWidget *pTableWidget, QMap<quint64, QString> mapData, qint32 nType, qint32 nData, XComboBoxEx::CBTYPE cbtype,
                                           quint64 nMask, qint32 nExtraData)
{
    XComboBoxEx *result = new XComboBoxEx(this);
    result->setData(mapData, cbtype, nMask, tr("Flags"));

    result->setProperty("STYPE", nType);

    if (nExtraData == -1) {
        result->setProperty("NDATA", nData);
    } else {
        result->setProperty("NDATA", nExtraData);
    }

    connect(result, SIGNAL(valueChanged(QVariant)), this, SLOT(widgetValueChanged(QVariant)));  // TODO Check widgetValueChanged

    pTableWidget->setCellWidget(nData, HEADER_COLUMN_INFO, result);

    return result;
}

void XFormatWidget::adjustComboBox(QTableWidget *pTableWidget, QList<RECWIDGET> *pListRecWidget, const QMap<quint64, QString> &mapData, qint32 nPosition,
                                   XComboBoxEx::CBTYPE cbtype, quint64 nMask)
{
    if (nPosition < pListRecWidget->count()) {
        (*pListRecWidget)[nPosition].pComboBox = new XComboBoxEx(pTableWidget);
        pListRecWidget->at(nPosition).pComboBox->setProperty("POSITION", pListRecWidget->at(nPosition).nPosition);
        pListRecWidget->at(nPosition).pComboBox->setProperty("OFFSET", pListRecWidget->at(nPosition).nOffset);
        pListRecWidget->at(nPosition).pComboBox->setProperty("SIZE", pListRecWidget->at(nPosition).nSize);
        pListRecWidget->at(nPosition).pComboBox->setData(mapData, cbtype, nMask, tr("Flags"));

        connect(pListRecWidget->at(nPosition).pComboBox, SIGNAL(valueChanged(QVariant)), this, SLOT(valueChangedSlot(QVariant)));

        pTableWidget->setCellWidget(nPosition, HEADER_COLUMN_INFO, pListRecWidget->at(nPosition).pComboBox);
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
