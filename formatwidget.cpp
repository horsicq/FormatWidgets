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
#include "formatwidget.h"

FormatWidget::FormatWidget(QWidget *pParent) : XShortcutsWidget(pParent)
{
    m_pDevice = nullptr;
    g_bIsReadonly = false;
    g_fwOptions = {};
    g_bAddPageEnable = true;
    g_nPageIndex = 0;  // TODO Check
    g_pXInfoDB = nullptr;
    g_nDisamInitAddress = -1;

    g_colDisabled = QWidget::palette().color(QPalette::Window);
    g_colEnabled = QWidget::palette().color(QPalette::BrightText);
}

FormatWidget::FormatWidget(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *pParent) : FormatWidget(pParent)
{
    FormatWidget::setData(pDevice, options, nNumber, nOffset, nType);
}

FormatWidget::~FormatWidget()
{
    if (g_sFileName != "") {
        QFile *pFile = dynamic_cast<QFile *>(m_pDevice);

        if (pFile) {
            pFile->close();
        }
    }
}

void FormatWidget::setXInfoDB(XInfoDB *pXInfoDB)
{
    g_pXInfoDB = pXInfoDB;
}

XInfoDB *FormatWidget::getXInfoDB()
{
    return g_pXInfoDB;
}

void FormatWidget::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobalChildren(this, pShortcuts, pXOptions);
    XShortcutstScrollArea::setGlobalChildren(this, pShortcuts, pXOptions);
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void FormatWidget::adjustView()
{
    getGlobalOptions()->adjustWidget(this, XOptions::ID_VIEW_FONT_CONTROLS);
    XShortcutstScrollArea::adjustViewChildren(this);

    {
        QList<QTreeWidget *> listWidgets = this->findChildren<QTreeWidget *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            QTreeWidget *pChild = dynamic_cast<QTreeWidget *>(listWidgets.at(i));

            if (pChild) {
                getGlobalOptions()->adjustTreeWidget(pChild, XOptions::ID_VIEW_FONT_TREEVIEWS, true);
            }
        }
    }
    {
        QList<QTreeView *> listWidgets = this->findChildren<QTreeView *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            QTreeView *pChild = dynamic_cast<QTreeView *>(listWidgets.at(i));

            if (pChild) {
                getGlobalOptions()->adjustTreeView(pChild, XOptions::ID_VIEW_FONT_TREEVIEWS);
            }
        }
    }
    {
        QList<QTableWidget *> listWidgets = this->findChildren<QTableWidget *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            QTableWidget *pChild = dynamic_cast<QTableWidget *>(listWidgets.at(i));

            if (pChild) {
                getGlobalOptions()->adjustTableWidget(pChild, XOptions::ID_VIEW_FONT_TABLEVIEWS);
            }
        }
    }
    {
        QList<QTableView *> listWidgets = this->findChildren<QTableView *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            QTableView *pChild = dynamic_cast<QTableView *>(listWidgets.at(i));

            if (pChild) {
                getGlobalOptions()->adjustTableView(pChild, XOptions::ID_VIEW_FONT_TABLEVIEWS);
            }
        }
    }
    {
        QList<QTextEdit *> listWidgets = this->findChildren<QTextEdit *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            QTextEdit *pChild = dynamic_cast<QTextEdit *>(listWidgets.at(i));

            if (pChild) {
                getGlobalOptions()->adjustWidget(pChild, XOptions::ID_VIEW_FONT_TEXTEDITS);
            }
        }
    }
    {
        QList<QPlainTextEdit *> listWidgets = this->findChildren<QPlainTextEdit *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            QPlainTextEdit *pChild = dynamic_cast<QPlainTextEdit *>(listWidgets.at(i));

            if (pChild) {
                getGlobalOptions()->adjustWidget(pChild, XOptions::ID_VIEW_FONT_TEXTEDITS);
            }
        }
    }
}

void FormatWidget::setData(QIODevice *pDevice, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType)
{
    m_pDevice = pDevice;
    g_bIsReadonly = !(pDevice->isWritable());

    setData(options, nNumber, nOffset, nType);
}

void FormatWidget::setData(const QString &sFileName, FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType)
{
    g_sFileName = sFileName;

    QFile *pFile = new QFile(sFileName);  // TODO delete !!! or use global

    XBinary::tryToOpen(pFile);

    setData(pFile, options, nNumber, nOffset, nType);
}

void FormatWidget::setData(FW_DEF::OPTIONS options, quint32 nNumber, qint64 nOffset, qint32 nType)
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

void FormatWidget::setFileType(XBinary::FT fileType)
{
    g_fileType = fileType;
}

XBinary::FT FormatWidget::getFileType()
{
    return g_fileType;
}

QIODevice *FormatWidget::getDevice()
{
    return this->m_pDevice;
}

void FormatWidget::setOptions(FW_DEF::OPTIONS options)
{
    g_fwOptions = options;
}

FW_DEF::OPTIONS FormatWidget::getOptions()
{
    return g_fwOptions;
}

quint32 FormatWidget::getNumber()
{
    return g_nNumber;
}

qint64 FormatWidget::getOffset()
{
    return g_nOffset;
}

qint32 FormatWidget::getType()
{
    return g_nType;
}

bool FormatWidget::isReadonly()
{
    return g_bIsReadonly;
}

QTreeWidgetItem *FormatWidget::createNewItem(qint32 nType, const QString &sTitle, XOptions::ICONTYPE iconType, qint64 nOffset, qint64 nSize, qint64 nExtraOffset,
                                             qint64 nExtraSize)
{
    QTreeWidgetItem *pResult = new QTreeWidgetItem;

    pResult->setText(0, sTitle);
    pResult->setData(0, Qt::UserRole + FW_DEF::SECTION_DATA_TYPE, nType);
    pResult->setData(0, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET, nOffset);
    pResult->setData(0, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE, nSize);
    pResult->setData(0, Qt::UserRole + FW_DEF::SECTION_DATA_EXTRAOFFSET, nExtraOffset);
    pResult->setData(0, Qt::UserRole + FW_DEF::SECTION_DATA_EXTRASIZE, nExtraSize);
    pResult->setData(0, Qt::UserRole + FW_DEF::SECTION_DATA_NAME, sTitle);

    XOptions::adjustTreeWidgetItem(pResult, iconType);

    return pResult;
}

void FormatWidget::setValue(QVariant vValue, qint32 nStype, qint32 nNdata, qint32 nVtype, qint32 nPosition, qint64 nOffset)
{
    if (saveBackup()) {
        SV sv = _setValue(vValue, nStype, nNdata, nVtype, nPosition, nOffset);
        if (sv == SV_EDITED) {
            reset();
        } else if (sv == SV_RELOADDATA) {
            reset();
            reloadData(true);
        } else if (sv == SV_RELOADALL) {
            reset();
            reload();
            reloadData(false);
        }

        emit dataChanged(nOffset, 1);  // TODO Check size
    } else {
        QMessageBox::critical(XOptions::getMainWidget(this), tr("Error"),
                              tr("Cannot save file") + QString(": %1").arg(XBinary::getBackupFileName(XBinary::getBackupDevice(getDevice()))));
    }
}

void FormatWidget::adjustHeaderTable(qint32 nType, QTableWidget *pTableWidget)
{
    Q_UNUSED(nType)

    qint32 nSymbolWidth = XLineEditHEX::getSymbolWidth(this);

    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_NAME, QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_OFFSET, QHeaderView::ResizeToContents);
    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_TYPE, QHeaderView::ResizeToContents);
    pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE, nSymbolWidth * 12);
    pTableWidget->setColumnWidth(HEADER_COLUMN_INFO, nSymbolWidth * 20);
}

void FormatWidget::adjustListTable(qint32 nType, QTableWidget *pTableWidget)
{
    Q_UNUSED(nType)
    Q_UNUSED(pTableWidget)
}

QString FormatWidget::typeIdToString(qint32 nType)
{
    Q_UNUSED(nType)

    return "";
}

bool FormatWidget::isEdited()
{
    bool bResult = false;

    bResult = XBinary::isBackupPresent(XBinary::getBackupDevice(getDevice()));

    return bResult;
}

bool FormatWidget::loadHexSubdevice(qint64 nOffset, qint64 nSize, XADDR nAddress, SubDevice **ppSubDevice, ToolsWidget *pToolsWidget, bool bOffset, bool bDisasm,
                                    bool bFollow)
{
    if (*ppSubDevice) {
        (*ppSubDevice)->close();
        delete (*ppSubDevice);

        (*ppSubDevice) = 0;
    }

    if ((nOffset == -1) || (nSize == 0)) {
        nOffset = 0;
        nAddress = 0;
        nSize = 0;
    }

    if (nAddress == (XADDR)-1) {
        nAddress = nOffset;
    }

    (*ppSubDevice) = new SubDevice(getDevice(), nOffset, nSize, this);

    (*ppSubDevice)->open(getDevice()->openMode());

    FW_DEF::OPTIONS hexOptions = getOptions();
    hexOptions.nImageBase = nAddress;
    hexOptions.bOffset = bOffset;

    pToolsWidget->setGlobal(getShortcuts(), getGlobalOptions());
    pToolsWidget->setData((*ppSubDevice), hexOptions, bDisasm, bFollow, getXInfoDB());

    return true;
}

bool FormatWidget::loadHexSubdeviceByTableView(qint32 nRow, qint32 nType, ToolsWidget *pToolsWidget, QTableView *pTableView, SubDevice **ppSubDevice, bool bOffset,
                                               bool bDisasm, bool bFollow)
{
    Q_UNUSED(nType)

    bool bResult = false;

    if (nRow != -1) {
        QModelIndex index = pTableView->model()->index(nRow, 0);

        qint64 nOffset = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();
        XADDR nAddress = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        bResult = loadHexSubdevice(nOffset, nSize, nAddress, ppSubDevice, pToolsWidget, bOffset, bDisasm, bFollow);
    }

    return bResult;
}

bool FormatWidget::setHexSubdeviceByTableView(qint32 nRow, qint32 nType, ToolsWidget *pToolsWidget, QTableView *pTableView)
{
    Q_UNUSED(nType)

    bool bResult = false;

    if (nRow != -1) {
        QModelIndex index = pTableView->model()->index(nRow, 0);

        qint64 nOffset = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();

        pToolsWidget->setSelection(nOffset, nSize, true);
        bResult = true;
    }

    return bResult;
}

void FormatWidget::setHeaderTableSelection(ToolsWidget *pToolWidget, QTableWidget *pTableWidget)
{
    qint32 nCurrentRow = pTableWidget->currentRow();

    if (nCurrentRow != -1) {
        qint32 nOffset = pTableWidget->item(nCurrentRow, 0)->data(Qt::UserRole + HEADER_DATA_OFFSET).toInt();
        qint64 nSize = pTableWidget->item(nCurrentRow, 0)->data(Qt::UserRole + HEADER_DATA_SIZE).toInt();

        if (nOffset != -1) {
            //            qint64
            //            nAddress=pToolWidget->getStartAddress()+nOffset;

            pToolWidget->setSelection(nOffset, nSize, false);
        }
    }
}

QColor FormatWidget::getEnabledColor()
{
    return g_colEnabled;
}

QColor FormatWidget::getDisabledColor()
{
    return g_colDisabled;
}

void FormatWidget::setItemEnable(QTableWidgetItem *pItem, bool bState)
{
    if (!bState) {
        pItem->setBackground(g_colDisabled);
    }
}

void FormatWidget::setLineEdit(XLineEditHEX *pLineEdit, qint32 nMaxLength, const QString &sText, qint64 nOffset)
{
    pLineEdit->setMaxLength(nMaxLength);
    pLineEdit->setValue_String(sText);
    pLineEdit->setProperty("OFFSET", nOffset);
}

void FormatWidget::ajustTableView(qint32 nType, ProcessData *pProcessData, QStandardItemModel **ppModel, XTableView *pTableView, bool bStretchLastSection)
{
    DialogProcessData dialogProcessData(this, pProcessData, getGlobalOptions());
    dialogProcessData.setGlobal(getShortcuts(), getGlobalOptions());
    dialogProcessData.showDialogDelay();

    bool bSort = pTableView->isSortingEnabled();

    if (bSort) {
        pTableView->setSortingEnabled(false);
    }

    pTableView->setCustomModel(*ppModel, true);

    pProcessData->ajustTableView(nType, pTableView);

    if (bSort) {
        pTableView->setSortingEnabled(true);
        pTableView->sortByColumn(0, Qt::AscendingOrder);
    }

    pTableView->horizontalHeader()->setStretchLastSection(bStretchLastSection);
}

void FormatWidget::ajustTreeView(qint32 nType, ProcessData *pProcessData, QStandardItemModel **ppModel, QTreeView *pTreeView)
{
    QAbstractItemModel *pOldModel = pTreeView->model();

    DialogProcessData dialogProcessData(this, pProcessData, getGlobalOptions());
    dialogProcessData.setGlobal(getShortcuts(), getGlobalOptions());
    dialogProcessData.showDialogDelay();

    pTreeView->setModel(*ppModel);

    pProcessData->ajustTreeView(nType, pTreeView);

    deleteOldAbstractModel(&pOldModel);
}

void FormatWidget::ajustDialogModel(ProcessData *pProcessData, QStandardItemModel **ppModel, const QString &sTitle)
{
    DialogProcessData dialogProcessData(this, pProcessData, getGlobalOptions());
    dialogProcessData.setGlobal(getShortcuts(), getGlobalOptions());

    dialogProcessData.showDialogDelay();

    DialogModelInfo dialogModelInfo(this);
    dialogModelInfo.setGlobal(getShortcuts(), getGlobalOptions());

    dialogModelInfo.setData(getDevice(), sTitle, *ppModel);

    dialogModelInfo.exec();
}

void FormatWidget::showSectionHex(QTableView *pTableView)
{
    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();

        followLocationSlot(nOffset, XBinary::LT_OFFSET, nSize, XOptions::WIDGETTYPE_HEX);

        reloadData(true);  // TODO Check

        pTableView->setCurrentIndex(pTableView->model()->index(nRow, 0));
    }
}

void FormatWidget::showSectionEntropy(QTableView *pTableView)
{
    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();

        showEntropy(nOffset, nSize);
    }
}

void FormatWidget::showSectionDisasm(QTableView *pTableView)
{
    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(0);

        XADDR nAddress = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_ADDRESS).toLongLong();

        followLocationSlot(nAddress, XBinary::LT_ADDRESS, 0, XOptions::WIDGETTYPE_DISASM);
    }
}

void FormatWidget::dumpSection(QTableView *pTableView)
{
    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(0);

        qint64 nOffset = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
        qint64 nSize = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();
        QString sName = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_NAME).toString();

        dumpRegion(nOffset, nSize, sName);
    }
}

void FormatWidget::dumpAll(QTableView *pTableView)
{
    QString sDirectory = QFileDialog::getExistingDirectory(this, tr("Dump all"), XBinary::getDeviceDirectory(m_pDevice));

    if (!sDirectory.isEmpty()) {
        qint32 nNumberOfRecords = pTableView->model()->rowCount();

        if (nNumberOfRecords) {
            QList<DumpProcess::RECORD> listRecords;

            for (qint32 i = 0; i < nNumberOfRecords; i++) {
                QModelIndex index = pTableView->model()->index(i, 0);

                DumpProcess::RECORD record = {};

                record.nOffset = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
                record.nSize = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();
                record.sFileName = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_NAME).toString();

                record.sFileName = sDirectory + QDir::separator() + QFileInfo(record.sFileName).fileName();

                listRecords.append(record);
            }

            QString sJsonFileName = XBinary::getDeviceFileName(m_pDevice) + ".patch.json";

            DumpProcess dumpProcess;
            XDialogProcess dd(this, &dumpProcess);
            dd.setGlobal(getShortcuts(), getGlobalOptions());
            dumpProcess.setData(m_pDevice, listRecords, DumpProcess::DT_DUMP_DEVICE_OFFSET, sJsonFileName, dd.getPdStruct());
            dd.start();
            dd.showDialogDelay();
        }
    }
}

qint64 FormatWidget::getTableViewItemSize(QTableView *pTableView)
{
    qint64 nResult = 0;

    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(0);
        nResult = pTableView->model()->data(index, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();
    }

    return nResult;
}

void FormatWidget::showTableViewDemangle(QTableView *pTableView, qint32 nColumn)
{
    qint32 nRow = pTableView->currentIndex().row();

    if (nRow != -1) {
        QModelIndex index = pTableView->selectionModel()->selectedIndexes().at(nColumn);
        QString sString = pTableView->model()->data(index).toString();

        showDemangle(sString);
    }
}

bool FormatWidget::_setTreeItem(QTreeWidget *pTree, QTreeWidgetItem *pItem, qint32 nID)
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

void FormatWidget::setTreeItem(QTreeWidget *pTree, qint32 nID)
{
    qint32 nNumberOfItems = pTree->topLevelItemCount();

    for (qint32 i = 0; i < nNumberOfItems; i++) {
        if (_setTreeItem(pTree, pTree->topLevelItem(i), nID)) {
            break;
        }
    }

    XOptions::adjustTreeWidgetSize(pTree, 100);
}

void FormatWidget::reset()
{
    g_mapInit.clear();
    g_listPages.clear();
}

QString FormatWidget::getInitString(QTreeWidgetItem *pItem)
{
    QString sResult;

    qint32 nType = pItem->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_TYPE).toInt();
    qint64 nDataOffset = pItem->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_OFFSET).toLongLong();
    qint64 nDataSize = pItem->data(0, Qt::UserRole + FW_DEF::SECTION_DATA_SIZE).toLongLong();

    sResult = QString("%1-%2-%3").arg(QString::number(nType), QString::number(nDataOffset), QString::number(nDataSize));

    return sResult;
}

void FormatWidget::addInit(const QString &sString)
{
    g_mapInit.insert(sString.section("-", 0, 0), sString);
}

bool FormatWidget::isInitPresent(const QString &sString)
{
    return (g_mapInit.value(sString.section("-", 0, 0)) == sString);
}

void FormatWidget::addPage(QTreeWidgetItem *pItem)
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

void FormatWidget::setAddPageEnabled(bool bEnable)
{
    g_bAddPageEnable = bEnable;
}

QTreeWidgetItem *FormatWidget::getPrevPage()
{
    QTreeWidgetItem *pResult = 0;

    if (isPrevPageAvailable()) {
        g_nPageIndex--;
        pResult = g_listPages.at(g_nPageIndex);
    }

    return pResult;
}

QTreeWidgetItem *FormatWidget::getNextPage()
{
    QTreeWidgetItem *pResult = 0;

    if (isNextPageAvailable()) {
        g_nPageIndex++;
        pResult = g_listPages.at(g_nPageIndex);
    }

    return pResult;
}

bool FormatWidget::isPrevPageAvailable()
{
    return g_nPageIndex > 0;
}

bool FormatWidget::isNextPageAvailable()
{
    return g_nPageIndex < (g_listPages.count() - 1);
}

void FormatWidget::initWidget()
{
    //    {
    //        QList<XHexViewWidget *>
    //        listWidgets=this->findChildren<XHexViewWidget *>();

    //        qint32 nNumberOfWidgets=listWidgets.count();

    //        for(qint32 i=0;i<nNumberOfWidgets;i++)
    //        {
    //            XHexViewWidget *pChild=dynamic_cast<XHexViewWidget
    //            *>(listWidgets.at(i));

    //            if(pChild)
    //            {
    //                initHexViewWidget(pChild);
    //            }
    //        }
    //    }
    {
        QList<XHexView *> listWidgets = this->findChildren<XHexView *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            XHexView *pChild = dynamic_cast<XHexView *>(listWidgets.at(i));

            if (pChild) {
                initHexView(pChild);
            }
        }
    }
    {
        QList<XMultiDisasmWidget *> listWidgets = this->findChildren<XMultiDisasmWidget *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            XMultiDisasmWidget *pChild = dynamic_cast<XMultiDisasmWidget *>(listWidgets.at(i));

            if (pChild) {
                initMultiDisasmWidget(pChild);
            }
        }
    }
    {
        QList<XMemoryMapWidget *> listWidgets = this->findChildren<XMemoryMapWidget *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            XMemoryMapWidget *pChild = dynamic_cast<XMemoryMapWidget *>(listWidgets.at(i));

            if (pChild) {
                initMemoryMapWidget(pChild);
            }
        }
    }
    {
        QList<SearchSignaturesWidget *> listWidgets = this->findChildren<SearchSignaturesWidget *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            SearchSignaturesWidget *pChild = dynamic_cast<SearchSignaturesWidget *>(listWidgets.at(i));

            if (pChild) {
                initSearchSignaturesWidget(pChild);
            }
        }
    }
    {
        QList<SearchStringsWidget *> listWidgets = this->findChildren<SearchStringsWidget *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            SearchStringsWidget *pChild = dynamic_cast<SearchStringsWidget *>(listWidgets.at(i));

            if (pChild) {
                initSearchStringsWidget(pChild);
            }
        }
    }
    {
        QList<SearchValuesWidget *> listWidgets = this->findChildren<SearchValuesWidget *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            SearchValuesWidget *pChild = dynamic_cast<SearchValuesWidget *>(listWidgets.at(i));

            if (pChild) {
                initSearchValuesWidget(pChild);
            }
        }
    }
    //    {
    //        QList<ToolsWidget *> listWidgets=this->findChildren<ToolsWidget
    //        *>();

    //        qint32 nNumberOfWidgets=listWidgets.count();

    //        for(qint32 i=0;i<nNumberOfWidgets;i++)
    //        {
    //            ToolsWidget *pChild=dynamic_cast<ToolsWidget
    //            *>(listWidgets.at(i));

    //            if(pChild)
    //            {
    //                qDebug("Size: %d",pChild->height());

    //                pChild->resize(pChild->width(),150); // TODO Check

    //                qDebug("Size: %d",pChild->height());
    //            }
    //        }
    //    }
    {
        QList<XExtractorWidget *> listWidgets = this->findChildren<XExtractorWidget *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            XExtractorWidget *pChild = dynamic_cast<XExtractorWidget *>(listWidgets.at(i));

            if (pChild) {
                initExtractorWidget(pChild);
            }
        }
    }
    {
        QList<YARAWidgetAdvanced *> listWidgets = this->findChildren<YARAWidgetAdvanced *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            YARAWidgetAdvanced *pChild = dynamic_cast<YARAWidgetAdvanced *>(listWidgets.at(i));

            if (pChild) {
                initYaraWidget(pChild);
            }
        }
    }
}

void FormatWidget::resetWidget()
{
    {
        QList<XHexViewWidget *> listWidgets = this->findChildren<XHexViewWidget *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            XHexViewWidget *pChild = dynamic_cast<XHexViewWidget *>(listWidgets.at(i));

            if (pChild) {
                pChild->setDevice(nullptr, 0, -1);
            }
        }
    }
    {
        QList<ToolsWidget *> listWidgets = this->findChildren<ToolsWidget *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            ToolsWidget *pChild = dynamic_cast<ToolsWidget *>(listWidgets.at(i));

            if (pChild) {
                pChild->resetWidget();
            }
        }
    }
    {
        QList<XMultiDisasmWidget *> listWidgets = this->findChildren<XMultiDisasmWidget *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            XMultiDisasmWidget *pChild = dynamic_cast<XMultiDisasmWidget *>(listWidgets.at(i));

            if (pChild) {
                XMultiDisasmWidget::OPTIONS options = {};

                pChild->setXInfoDB(nullptr);
                pChild->setData(nullptr, options);
            }
        }
    }
    {
        QList<XMemoryMapWidget *> listWidgets = this->findChildren<XMemoryMapWidget *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            XMemoryMapWidget *pChild = dynamic_cast<XMemoryMapWidget *>(listWidgets.at(i));

            if (pChild) {
                XMemoryMapWidget::OPTIONS options = {};

                pChild->setData(nullptr, options, nullptr);
            }
        }
    }
    {
        QList<XVisualizationWidget *> listWidgets = this->findChildren<XVisualizationWidget *>();

        qint32 nNumberOfWidgets = listWidgets.count();

        for (qint32 i = 0; i < nNumberOfWidgets; i++) {
            XVisualizationWidget *pChild = dynamic_cast<XVisualizationWidget *>(listWidgets.at(i));

            if (pChild) {
                pChild->setData(nullptr, XBinary::FT_UNKNOWN, true);
            }
        }
    }
}

void FormatWidget::initSearchStringsWidget(SearchStringsWidget *pWidget)
{
    connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(setEdited(qint64, qint64)));
    connect(pWidget, SIGNAL(showDemangle(QString)), this, SLOT(showDemangle(QString)));
    connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));
}

void FormatWidget::initSearchSignaturesWidget(SearchSignaturesWidget *pWidget)
{
    connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));
}

void FormatWidget::initSearchValuesWidget(SearchValuesWidget *pWidget)
{
    connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));
}

void FormatWidget::initHexViewWidget(XHexViewWidget *pWidget)
{
    connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(setEdited(qint64, qint64)));
    connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));
}

void FormatWidget::initMultiDisasmWidget(XMultiDisasmWidget *pWidget)
{
    connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(setEdited(qint64, qint64)));
    connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));
}

void FormatWidget::initMemoryMapWidget(XMemoryMapWidget *pWidget)
{
    connect(pWidget, SIGNAL(findValue(quint64, XBinary::ENDIAN)), this, SLOT(findValue(quint64, XBinary::ENDIAN)));
    connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));
}

void FormatWidget::initHexView(XHexView *pWidget)
{
    connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(setEdited(qint64, qint64)));
    connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));
}

void FormatWidget::initDisasmView(XDisasmView *pWidget)
{
    connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(setEdited(qint64, qint64)));
    connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));
}

void FormatWidget::initToolsWidget(ToolsWidget *pWidget)
{
    connect(pWidget, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(setEdited(qint64, qint64)));
    connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));
    connect(pWidget, SIGNAL(showDemangle(QString)), this, SLOT(showDemangle(QString)));

    pWidget->resize(pWidget->width(), 150);  // TODO Check
}

void FormatWidget::initExtractorWidget(XExtractorWidget *pWidget)
{
    connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));
}

void FormatWidget::initYaraWidget(YARAWidgetAdvanced *pWidget)
{
    connect(pWidget, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));
}

qint32 FormatWidget::getColumnWidth(QWidget *pParent, FormatWidget::CW cw, XBinary::MODE mode)
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

void FormatWidget::setDisasmInitAddress(XADDR nDisasmInitAddress)
{
    g_nDisamInitAddress = nDisasmInitAddress;
}

XADDR FormatWidget::getDisasmInitAddress()
{
    return g_nDisamInitAddress;
}

QStandardItemModel *FormatWidget::getHeaderTableModel(QTableWidget *pTableWidget)
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

void FormatWidget::saveHeaderTable(QTableWidget *pTableWidget, const QString &sFileName)
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

void FormatWidget::_showInDisasmWindowAddress(XADDR nAddress)
{
    Q_UNUSED(nAddress)
#ifdef QT_DEBUG
    qDebug("TODO _showInDisasmWindowAddress");
#endif
}

void FormatWidget::_showInDisasmWindowOffset(qint64 nOffset)
{
    Q_UNUSED(nOffset)
#ifdef QT_DEBUG
    qDebug("TODO _showInDisasmWindowOffset");
#endif
}

void FormatWidget::_showInMemoryMapWindowOffset(qint64 nOffset)
{
    Q_UNUSED(nOffset)
#ifdef QT_DEBUG
    qDebug("TODO _showInMemoryMapWindowOffset");
#endif
}

void FormatWidget::_showInHexWindow(qint64 nOffset, qint64 nSize)
{
    Q_UNUSED(nOffset)
    Q_UNUSED(nSize)
#ifdef QT_DEBUG
    qDebug("TODO _showInHexWindow");
#endif
}

void FormatWidget::_findValue(quint64 nValue, XBinary::ENDIAN endian)
{
    Q_UNUSED(nValue)
    Q_UNUSED(endian)
#ifdef QT_DEBUG
    qDebug("TODO _findValue");
#endif
}

void FormatWidget::_widgetValueChanged(QVariant vValue)
{
    Q_UNUSED(vValue)
#ifdef QT_DEBUG
    qDebug("TODO _widgetValueChanged");
#endif
}

// void FormatWidget::resizeToolsWidget(QWidget *pParent,ToolsWidget
// *pToolWidget)
//{
//     qint32 nHeight=pParent->height();
//     qint32 _nMaxHeight=pToolWidget->maximumHeight();
//     pToolWidget->setMaximumHeight(nHeight/4);
//     pToolWidget->setMaximumHeight(_nMaxHeight);
// }

void FormatWidget::valueChangedSlot(QVariant varValue)
{
    XLineEditHEX *pLineEdit = qobject_cast<XLineEditHEX *>(sender());

    qint32 nStype = pLineEdit->property("STYPE").toInt();
    qint32 nNdata = pLineEdit->property("NDATA").toInt();
    qint32 nVtype = pLineEdit->property("VTYPE").toInt();
    qint32 nPosition = pLineEdit->property("POSITION").toInt();
    qint64 nOffset = pLineEdit->property("OFFSET").toLongLong();

    setValue(varValue, nStype, nNdata, nVtype, nPosition, nOffset);
}

void FormatWidget::setEdited(qint64 nDeviceOffset, qint64 nDeviceSize)
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

void FormatWidget::allReload(qint64 nDeviceOffset, qint64 nDeviceSize)
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

void FormatWidget::showHex(qint64 nOffset, qint64 nSize)
{
    XHexViewWidget::OPTIONS hexOptions = {};

    hexOptions.nStartSelectionOffset = nOffset;
    hexOptions.nSizeOfSelection = nSize;

    DialogHexView dialogHexView(this);
    dialogHexView.setGlobal(getShortcuts(), getGlobalOptions());
    dialogHexView.setData(getDevice(), hexOptions);
    dialogHexView.setXInfoDB(getXInfoDB());

    connect(&dialogHexView, SIGNAL(editState(bool)), this, SLOT(setEdited(bool)));

    dialogHexView.exec();

    reloadData(true);  // TODO Check
}

void FormatWidget::followLocationSlot(quint64 nLocation, qint32 nLocationType, qint64 nSize, qint32 nWidgetType)
{
    if (nLocationType == XBinary::LT_OFFSET) {
        if (nWidgetType == XOptions::WIDGETTYPE_DISASM) {
            _showInDisasmWindowOffset(nLocation);
        } else if (nWidgetType == XOptions::WIDGETTYPE_MEMORYMAP) {
            _showInMemoryMapWindowOffset(nLocation);
        } else if (nWidgetType == XOptions::WIDGETTYPE_HEX) {
            _showInHexWindow(nLocation, nSize);
        }
    } else if (nLocationType == XBinary::LT_ADDRESS) {
        _showInDisasmWindowAddress(nLocation);
    }
}

void FormatWidget::widgetValueChanged(QVariant vValue)
{
    _widgetValueChanged(vValue);
}

void FormatWidget::findValue(quint64 nValue, XBinary::ENDIAN endian)
{
    _findValue(nValue, endian);
}

void FormatWidget::showEntropy(qint64 nOffset, qint64 nSize)
{
    DialogEntropy dialogEntropy(this);
    dialogEntropy.setGlobal(getShortcuts(), getGlobalOptions());

    dialogEntropy.setData(getDevice(), nOffset, nSize);

    dialogEntropy.exec();
}

void FormatWidget::dumpRegion(qint64 nOffset, qint64 nSize, const QString &sName)
{
    QString _sName = sName;

    if (_sName == "") {
        _sName = tr("Dump");
    }

    QString sSaveFileName = XBinary::getResultFileName(getDevice(), QString("%1.bin").arg(_sName));
    QString sFileName = QFileDialog::getSaveFileName(this, tr("Save dump"), sSaveFileName, QString("%1 (*.bin)").arg(tr("Raw data")));

    if (!sFileName.isEmpty()) {
        DumpProcess dumpProcess;
        XDialogProcess dd(this, &dumpProcess);
        dd.setGlobal(getShortcuts(), getGlobalOptions());
        dumpProcess.setData(m_pDevice, nOffset, nSize, sFileName, DumpProcess::DT_DUMP_DEVICE_OFFSET, dd.getPdStruct());
        dd.start();
        dd.showDialogDelay();
    }
}

void FormatWidget::showDemangle(const QString &sString)
{
    DialogDemangle dialogDemangle(this, sString);
    dialogDemangle.setGlobal(getShortcuts(), getGlobalOptions());

    dialogDemangle.exec();
}

bool FormatWidget::saveBackup()
{
    bool bResult = true;

    if ((getGlobalOptions()->isSaveBackup()) && (!isEdited())) {
        // Save backup
        bResult = XBinary::saveBackup(XBinary::getBackupDevice(getDevice()));
    }

    return bResult;
}

void FormatWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

bool FormatWidget::createHeaderTable(qint32 nType, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, XLineEditHEX **ppLineEdits, qint32 nNumberOfRecords,
                                     qint32 nPosition, qint64 nOffset)
{
    pTableWidget->setColumnCount(6);
    pTableWidget->setRowCount(nNumberOfRecords);

    QStringList slHeader;
    slHeader.append(tr("Name"));
    slHeader.append(tr("Offset"));
    slHeader.append(tr("Type"));
    slHeader.append(tr("Value"));
    slHeader.append(tr(""));
    slHeader.append(tr(""));

    pTableWidget->setHorizontalHeaderLabels(slHeader);
    pTableWidget->horizontalHeader()->setVisible(true);

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        QTableWidgetItem *pItemName = new QTableWidgetItem;
        pItemName->setText(pRecords[i].sName);
        pItemName->setData(Qt::UserRole + HEADER_DATA_OFFSET, pRecords[i].nOffset);
        pItemName->setData(Qt::UserRole + HEADER_DATA_SIZE, pRecords[i].nSize);
        pTableWidget->setItem(i, HEADER_COLUMN_NAME, pItemName);

        QTableWidgetItem *pItemOffset = new QTableWidgetItem;

        if (pRecords[i].nOffset != -1) {
            pItemOffset->setText(XBinary::valueToHex((quint16)pRecords[i].nOffset));
        }

        pItemOffset->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);  // TODO
        pTableWidget->setItem(i, HEADER_COLUMN_OFFSET, pItemOffset);

        QTableWidgetItem *pItemType = new QTableWidgetItem;
        pItemType->setText(pRecords[i].sType);
        pTableWidget->setItem(i, HEADER_COLUMN_TYPE, pItemType);

        ppLineEdits[i] = new XLineEditHEX(this);
        ppLineEdits[i]->setProperty("STYPE", nType);
        ppLineEdits[i]->setProperty("NDATA", pRecords[i].nData);
        ppLineEdits[i]->setProperty("POSITION", nPosition);
        ppLineEdits[i]->setProperty("OFFSET", nOffset);

        if ((pRecords[i].vtype == FW_DEF::VAL_TYPE_TEXT) || (pRecords[i].vtype == FW_DEF::VAL_TYPE_UUID)) {
            if (pRecords[i].nSize != -1) {
                ppLineEdits[i]->setMaxLength(pRecords[i].nSize);
            }
        }

        connect(ppLineEdits[i], SIGNAL(valueChanged(QVariant)), this, SLOT(valueChangedSlot(QVariant)));

        pTableWidget->setCellWidget(i, HEADER_COLUMN_VALUE, ppLineEdits[i]);

        if (pRecords[i].nSize == 0) {
            ppLineEdits[i]->setEnabled(false);
        }

        pTableWidget->setItem(i, HEADER_COLUMN_COMMENT, new QTableWidgetItem);
    }

    pTableWidget->horizontalHeader()->setSectionResizeMode(HEADER_COLUMN_COMMENT, QHeaderView::Stretch);

    adjustHeaderTable(nType, pTableWidget);

    pTableWidget->resizeColumnToContents(HEADER_COLUMN_NAME);

    //    XOptions::setTableWidgetHeaderAlignment(pTableWidget, HEADER_COLUMN_NAME, Qt::AlignLeft | Qt::AlignVCenter);
    //    XOptions::setTableWidgetHeaderAlignment(pTableWidget, HEADER_COLUMN_OFFSET, Qt::AlignRight | Qt::AlignVCenter);
    //    XOptions::setTableWidgetHeaderAlignment(pTableWidget, HEADER_COLUMN_TYPE, Qt::AlignLeft | Qt::AlignVCenter);
    //    XOptions::setTableWidgetHeaderAlignment(pTableWidget, HEADER_COLUMN_VALUE, Qt::AlignRight | Qt::AlignVCenter);
    //    XOptions::setTableWidgetHeaderAlignment(pTableWidget, HEADER_COLUMN_INFO, Qt::AlignLeft | Qt::AlignVCenter);
    //    XOptions::setTableWidgetHeaderAlignment(pTableWidget, HEADER_COLUMN_COMMENT, Qt::AlignLeft | Qt::AlignVCenter);

    return true;
}

bool FormatWidget::createListTable(qint32 nType, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, XLineEditHEX **ppLineEdits, qint32 nNumberOfRecords)
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
        ppLineEdits[i]->setProperty("NDATA", pRecords[i].nData);

        if (pRecords[i].vtype == FW_DEF::VAL_TYPE_TEXT) {
            ppLineEdits[i]->setAlignment(Qt::AlignLeft);
        }

        if (pRecords[i].nOffset != -1) {
            if ((pRecords[i].vtype == FW_DEF::VAL_TYPE_TEXT) || (pRecords[i].vtype == FW_DEF::VAL_TYPE_UUID)) {
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

void FormatWidget::addComment(QTableWidget *pTableWidget, qint32 nRow, qint32 nColumn, const QString &sComment)
{
    pTableWidget->item(nRow, nColumn)->setText(sComment);
}

void FormatWidget::updateTableRecord(QTableWidget *pTableWidget, qint32 nRow, qint64 nOffset, qint64 nSize)
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

bool FormatWidget::createSectionTable(qint32 nType, QTableWidget *pTableWidget, const FW_DEF::HEADER_RECORD *pRecords, qint32 nNumberOfRecords)
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

void FormatWidget::setLineEditsReadOnly(XLineEditHEX **ppLineEdits, qint32 nCount, bool bState)
{
    for (qint32 i = 0; i < nCount; i++) {
        if (ppLineEdits[i]) {
            ppLineEdits[i]->setReadOnly(bState);
        }
    }
}

void FormatWidget::setComboBoxesReadOnly(XComboBoxEx **ppComboBoxes, qint32 nCount, bool bState)
{
    for (qint32 i = 0; i < nCount; i++) {
        if (ppComboBoxes[i]) {
            ppComboBoxes[i]->setReadOnly(bState);
        }
    }
}

void FormatWidget::setPushButtonReadOnly(QPushButton **ppPushButtons, qint32 nCount, bool bState)
{
    for (qint32 i = 0; i < nCount; i++) {
        if (ppPushButtons[i]) {
            ppPushButtons[i]->setEnabled(!bState);
        }
    }
}

void FormatWidget::setDateTimeEditReadOnly(XDateTimeEditX **ppDateTimeEdits, qint32 nCount, bool bState)
{
    for (qint32 i = 0; i < nCount; i++) {
        if (ppDateTimeEdits[i]) {
            ppDateTimeEdits[i]->setReadOnly(bState);
        }
    }
}

void FormatWidget::_deleteObjects(QObject **ppObjects, qint32 nCount)
{
    nCount = nCount / (sizeof(QObject *));

    for (qint32 i = 0; i < nCount; i++) {
        if (ppObjects[i]) {
            delete (ppObjects[i]);
            ppObjects[i] = 0;
        }
    }
}

void FormatWidget::_deleteSubdevices(SubDevice **ppSubdevices, qint32 nCount)
{
    for (qint32 i = 0; i < nCount; i++) {
        if (ppSubdevices[i]) {
            ppSubdevices[i]->close();

            delete (ppSubdevices[i]);
            ppSubdevices[i] = 0;
        }
    }
}

XComboBoxEx *FormatWidget::createComboBox(QTableWidget *pTableWidget, QMap<quint64, QString> mapData, qint32 nType, qint32 nData, XComboBoxEx::CBTYPE cbtype,
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

InvWidget *FormatWidget::createInvWidget(QTableWidget *pTableWidget, qint32 nType, qint32 nData, InvWidget::TYPE widgetType)
{
    InvWidget *pResult = new InvWidget(this, widgetType);

    pResult->setGlobal(getShortcuts(), getGlobalOptions());

    pResult->setProperty("STYPE", nType);
    pResult->setProperty("NDATA", nData);

    connect(pResult, SIGNAL(followLocation(quint64, qint32, qint64, qint32)), this, SLOT(followLocationSlot(quint64, qint32, qint64, qint32)));

    pTableWidget->setCellWidget(nData, HEADER_COLUMN_INFO, pResult);

    return pResult;
}

XDateTimeEditX *FormatWidget::createTimeDateEdit(QTableWidget *pTableWidget, qint32 nType, qint32 nData)
{
    XDateTimeEditX *result = new XDateTimeEditX(this);

    result->setProperty("STYPE", nType);
    result->setProperty("NDATA", nData);

    connect(result, SIGNAL(valueChanged(QVariant)), this, SLOT(widgetValueChanged(QVariant)));  // TODO Check widgetValueChanged

    pTableWidget->setCellWidget(nData, HEADER_COLUMN_INFO, result);

    return result;
}

QPushButton *FormatWidget::createPushButton(QTableWidget *pTableWidget, qint32 nType, qint32 nData, const QString &sText)
{
    QPushButton *pPushButton = new QPushButton(this);

    pPushButton->setText(sText);
    pPushButton->setProperty("STYPE", nType);
    pPushButton->setProperty("NDATA", nData);

    connect(pPushButton, SIGNAL(clicked()), this, SLOT(widgetAction()));

    pTableWidget->setCellWidget(nData, HEADER_COLUMN_INFO, pPushButton);

    return pPushButton;
}
