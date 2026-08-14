/* Copyright (c) 2025-2026 hors<horsicq@gmail.com>
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

#include "xfwidgetadvanced.h"

#include "ui_xfwidgetadvanced.h"

#include <QAbstractItemModel>
#include <QFileDialog>
#include <QtGlobal>

#include "xfmodel.h"
#include "xfwidget_demangle.h"
#include "xfwidget_diescan.h"
#include "xfwidget_disasm.h"
#include "xfwidget_entropy.h"
#include "xfwidget_fileinfo.h"
#include "xfwidget_extractor.h"
#include "xfwidget_hash.h"
#include "xfwidget_header.h"
#include "xfwidget_hex.h"
#include "xfwidget_memorymap.h"
#include "xfwidget_nfdscan.h"
#include "xfwidget_search.h"
#include "xfwidget_signatures.h"
#include "xfwidget_strings.h"
#include "xfwidget_table.h"
#include "xfwidget_tools.h"
#include "xfwidget_virustotal.h"
#include "xfwidget_visualization.h"
#include "xfwidget_yarascan.h"

XFWidgetAdvanced::XFWidgetAdvanced(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::XFWidgetAdvanced)
{
    ui->setupUi(this);

    m_bIsReadonly = false;
    m_inData = {};
    m_options = {};

    XOptions::adjustToolButton(ui->toolButtonReload, XOptions::ICONTYPE_RELOAD);
    ui->toolButtonReload->setToolTip(tr("Reload"));
    ui->toolButtonReload->setAccessibleName(tr("Reload current file"));
    ui->toolButtonReload->setAccessibleDescription(tr("Re-read the current file using the selected interpretation"));
    ui->toolButtonReload->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->comboBoxFileType->setToolTip(tr("File type"));
    ui->comboBoxFileType->setAccessibleName(tr("File type interpretation"));
    ui->comboBoxFileType->setAccessibleDescription(tr("Choose how the file should be parsed"));
    ui->comboBoxFileType->setMinimumWidth(180);

    m_bSplitterInitialized = false;

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 2);
    // A detail page with a wide minimumSizeHint can make QSplitter honor that
    // minimum over setSizes() and collapse the tree to a sliver. Guarantee the
    // tree a modest floor (kept small so it does not inflate the window's own
    // minimum width); seedSplitterSizes() then picks the actual split.
    ui->treeView->setMinimumWidth(140);

    connect(ui->treeView, SIGNAL(headerSelected(XBinary::XFHEADER)), this, SLOT(onHeaderSelected(XBinary::XFHEADER)));
}

XFWidgetAdvanced::~XFWidgetAdvanced()
{
    clearWidgetCache();
    delete ui;
}

void XFWidgetAdvanced::setData(const XBinary::INDATA &inData, const OPTIONS &options)
{
    m_inData = inData;
    m_options = options;

    reload();
    // m_pXBinary = pXBinary;
    // m_mapHeaders.clear();

    // for (qint32 i = 0; i < listHeaders.count(); i++) {
    //     m_mapHeaders.insert(listHeaders.at(i).sTag, listHeaders.at(i));
    // }

    // ui->treeView->setData(pXBinary, listHeaders);

    // if (!listHeaders.isEmpty()) {
    //     ui->tableView->setData(pXBinary, listHeaders.at(0));

    //     QString sStructName;
    //     if (m_pXBinary) {
    //         sStructName = m_pXBinary->structIDToString(listHeaders.at(0).structID);
    //     }

    //     m_sCurrentTag = XBinary::xfHeaderToTag(listHeaders.at(0), sStructName, listHeaders.at(0).sParentTag);
    //     ui->lineEditTag->setText(m_sCurrentTag);

    //     bool bIsTable = (listHeaders.at(0).xfType == XBinary::XFTYPE_TABLE);
    //     ui->toolBar->setVisible(bIsTable);

    //     if (bIsTable) {
    //         ui->tableView->setShowOffset(ui->checkBoxShowOffsets->isChecked());
    //         ui->tableView->setShowPresentation(ui->checkBoxShowPresentation->isChecked());
    //     }
    // }
}

void XFWidgetAdvanced::clear()
{
    clearWidgetCache();
    ui->treeView->clear();
    ui->treeView->clearFilters();
    m_inData = {};
    m_options = {};
}

void XFWidgetAdvanced::reload()
{
    QIODevice *pDevice = XFormats::createDevice(m_inData);

    // createDevice returns nullptr if the file vanished or is locked; the
    // combo-box helpers dereference the device unconditionally.
    if (pDevice) {
        XFormats::setFileTypeComboBox(m_inData.fileType, pDevice, ui->comboBoxFileType);
        XFormats::removeDevice(pDevice, m_inData);
    }

    reloadFileType();
}

void XFWidgetAdvanced::reloadFileType()
{
    XBinary::FT fileType = (XBinary::FT)(ui->comboBoxFileType->currentData().toUInt());

    // Persist the currently viewed type so Reload refreshes it instead of reverting
    // to the auto-detected type (reload() re-selects m_inData.fileType in the combo).
    m_inData.fileType = fileType;

    clearWidgetCache();

    QIODevice *pDevice = XFormats::createDevice(m_inData);

    if (!pDevice) {
        ui->treeView->clear();
        return;
    }

    XBinary *pBinary = XFormats::createClass(fileType, pDevice, m_inData.bIsImage, m_inData.nModuleAddress);

    if (pBinary) {
        QList<XBinary::XFHEADER> listHeaders = pBinary->_getXFHeaders(); // TODO Thread

        XBinary::INDATA inData = m_inData;
        inData.fileType = fileType;
        // setData re-applies the header filters to the fresh model, so the
        // selection below can only land on a row the filter keeps visible
        ui->treeView->setData(inData, listHeaders, true);
        ui->treeView->selectFirstItem();

        delete pBinary;
    }

    XFormats::removeDevice(pDevice, m_inData);
}

void XFWidgetAdvanced::setReadonly(bool bIsReadonly)
{
    m_bIsReadonly = bIsReadonly;
}

QByteArray XFWidgetAdvanced::saveSplitterState() const
{
    return ui->splitter->saveState();
}

void XFWidgetAdvanced::restoreSplitterState(const QByteArray &baState)
{
    if (!baState.isEmpty()) {
        ui->splitter->restoreState(baState);
        // An explicit restore wins - suppress the first-show default below.
        m_bSplitterInitialized = true;
    }
}

void XFWidgetAdvanced::showEvent(QShowEvent *pEvent)
{
    XShortcutsWidget::showEvent(pEvent);
    seedSplitterSizes();
}

void XFWidgetAdvanced::resizeEvent(QResizeEvent *pEvent)
{
    XShortcutsWidget::resizeEvent(pEvent);
    // showEvent may fire before the splitter has its final width; the initial
    // layout resize does have it, so seed here too (guarded to run once).
    seedSplitterSizes();
}

void XFWidgetAdvanced::seedSplitterSizes()
{
    // Stretch factors only distribute EXTRA space; on a fresh widget a wide
    // detail page (e.g. the Info panel) can starve the tree to a sliver. Seed a
    // ~1:2.3 split the first time we have real geometry, unless a saved splitter
    // state was already restored.
    if (m_bSplitterInitialized) {
        return;
    }

    qint32 nTotalWidth = ui->splitter->width();

    if (nTotalWidth > 0) {
        qint32 nTreeWidth = (nTotalWidth * 3) / 10;
        ui->splitter->setSizes(QList<int>() << nTreeWidth << (nTotalWidth - nTreeWidth));
        m_bSplitterInitialized = true;
    }
}

void XFWidgetAdvanced::adjustView()
{
    getGlobalOptions()->adjustTreeView(ui->treeView, XOptions::ID_VIEW_FONT_TREEVIEWS);

    // Re-apply globals to the already-created panels so a runtime Options change (fonts, etc.)
    // is honored immediately instead of only on the next panel (re)creation. The cached widgets
    // are heterogeneous: the XShortcutsWidget-based ones re-propagate via their virtual setGlobal;
    // the three plain-QWidget wrappers carry their own setGlobal (font only).
    for (QWidget *pWidget : m_mapWidgets) {
        if (!pWidget) {
            continue;
        }

        XShortcutsWidget *pShortcutsWidget = qobject_cast<XShortcutsWidget *>(pWidget);
        if (pShortcutsWidget) {
            pShortcutsWidget->setGlobal(getShortcuts(), getGlobalOptions());
            continue;
        }

        XFWidget_Header *pHeader = qobject_cast<XFWidget_Header *>(pWidget);
        if (pHeader) {
            pHeader->setGlobal(getShortcuts(), getGlobalOptions());
            continue;
        }

        XFWidget_Table *pTable = qobject_cast<XFWidget_Table *>(pWidget);
        if (pTable) {
            pTable->setGlobal(getShortcuts(), getGlobalOptions());
            continue;
        }

        XFWidget_Strings *pStrings = qobject_cast<XFWidget_Strings *>(pWidget);
        if (pStrings) {
            pStrings->setGlobal(getShortcuts(), getGlobalOptions());
            continue;
        }
    }
}

void XFWidgetAdvanced::setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions)
{
    XShortcutsWidget::setGlobal(pShortcuts, pXOptions);
}

void XFWidgetAdvanced::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
}

void XFWidgetAdvanced::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

void XFWidgetAdvanced::onHeaderSelected(const XBinary::XFHEADER &xfHeader)
{
    XBinary::FT fileType = (XBinary::FT)(ui->comboBoxFileType->currentData().toUInt());

    QIODevice *pDevice = XFormats::createDevice(m_inData);
    XBinary *pBinary = XFormats::createClass(fileType, pDevice, m_inData.bIsImage, m_inData.nModuleAddress);
    QString sStructName;

    if (pBinary) {
        if (xfHeader.xfType == XBinary::XFTYPE_COMMAND) {
            // Generic command nodes (Strings/Import/Export/Symbols/Resources) use XBinary's own
            // STRUCTID space, not the per-format one the polymorphic call below would resolve to.
            sStructName = pBinary->XBinary::structIDToString(xfHeader.structID);
        } else {
            sStructName = pBinary->structIDToString(xfHeader.structID);
        }
        delete pBinary;
    }

    XFormats::removeDevice(pDevice, m_inData);

    QString sCurrentTag = XBinary::xfHeaderToTag(xfHeader, sStructName, xfHeader.sParentTag);

    XBinary::INDATA inData = m_inData;
    inData.fileType = fileType;

    QString sWidgetKey = getWidgetCacheKey(sCurrentTag, xfHeader);
    QWidget *pWidget = getOrCreateWidget(sWidgetKey, inData, xfHeader);

    bool bIsListCommand = (xfHeader.xfType == XBinary::XFTYPE_COMMAND) &&
                          ((xfHeader.structID == XBinary::STRUCTID_IMPORT) || (xfHeader.structID == XBinary::STRUCTID_EXPORT) ||
                           (xfHeader.structID == XBinary::STRUCTID_SYMBOLS) || (xfHeader.structID == XBinary::STRUCTID_RESOURCES));

    if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_VISUALIZATION)) {
        qobject_cast<XFWidget_Visualization *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_INFO)) {
        qobject_cast<XFWidget_FileInfo *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_DIESCAN)) {
        qobject_cast<XFWidget_DIEScan *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_YARASCAN)) {
        qobject_cast<XFWidget_YaraScan *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_VIRUSTOTALSCAN)) {
        qobject_cast<XFWidget_VirusTotal *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_DEMANGLE)) {
        qobject_cast<XFWidget_Demangle *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_TOOLS)) {
        qobject_cast<XFWidget_Tools *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_HEX)) {
        qobject_cast<XFWidget_Hex *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_DISASM)) {
        qobject_cast<XFWidget_Disasm *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_NFDSCAN)) {
        qobject_cast<XFWidget_NFDScan *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_HASH)) {
        qobject_cast<XFWidget_Hash *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_SIGNATURES)) {
        qobject_cast<XFWidget_Signatures *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_MEMORYMAP)) {
        qobject_cast<XFWidget_MemoryMap *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_ENTROPY)) {
        qobject_cast<XFWidget_Entropy *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_EXTRACTOR)) {
        qobject_cast<XFWidget_Extractor *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_SEARCH)) {
        qobject_cast<XFWidget_Search *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_STRINGS)) {
        qobject_cast<XFWidget_Strings *>(pWidget)->setReadonly(m_bIsReadonly);
    } else if ((xfHeader.xfType == XBinary::XFTYPE_TABLE) || bIsListCommand) {
        qobject_cast<XFWidget_Table *>(pWidget)->setReadonly(m_bIsReadonly);
    } else {
        qobject_cast<XFWidget_Header *>(pWidget)->setReadonly(m_bIsReadonly);
    }

    emit headerSelected(xfHeader);
}

QString XFWidgetAdvanced::getWidgetCacheKey(const QString &sName, const XBinary::XFHEADER &xfHeader) const
{
    return QString("%1|%2|%3|%4|%5|%6|%7")
        .arg(sName)
        .arg((quint32)xfHeader.xfType)
        .arg((quint32)xfHeader.structID)
        .arg(xfHeader.sParentTag)
        .arg((quint64)xfHeader.xLoc.nLocation, 0, 16)
        .arg((quint32)xfHeader.xLoc.locType)
        .arg((quint64)xfHeader.nSize);
}

void XFWidgetAdvanced::clearWidgetCache()
{
    for (QWidget *pWidget : m_mapWidgets) {
        if (!pWidget) {
            continue;
        }

        ui->stackedWidget->removeWidget(pWidget);
        delete pWidget;
    }

    m_mapWidgets.clear();
    m_lruOrder.clear();
}

QWidget *XFWidgetAdvanced::getOrCreateWidget(const QString &sName, const XBinary::INDATA &inData, const XBinary::XFHEADER &xfHeader)
{
    QString sCacheKey = getWidgetCacheKey(sName, xfHeader);

    if (m_mapWidgets.contains(sCacheKey)) {
        m_lruOrder.removeOne(sCacheKey);
        m_lruOrder.append(sCacheKey);
        QWidget *pWidget = m_mapWidgets.value(sCacheKey);
        ui->stackedWidget->setCurrentWidget(pWidget);
        return pWidget;
    }

    if (m_mapWidgets.size() >= 20) {
        const QString sEvict = m_lruOrder.takeFirst();
        QWidget *pEvicted = m_mapWidgets.take(sEvict);

        if (pEvicted) {
            ui->stackedWidget->removeWidget(pEvicted);
            delete pEvicted;
        }
    }

    QWidget *pWidget = nullptr;

    bool bIsListCommand = (xfHeader.xfType == XBinary::XFTYPE_COMMAND) &&
                          ((xfHeader.structID == XBinary::STRUCTID_IMPORT) || (xfHeader.structID == XBinary::STRUCTID_EXPORT) ||
                           (xfHeader.structID == XBinary::STRUCTID_SYMBOLS) || (xfHeader.structID == XBinary::STRUCTID_RESOURCES));

    if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_VISUALIZATION)) {
        XFWidget_Visualization *pVisualization = new XFWidget_Visualization(this);
        pVisualization->setGlobal(getShortcuts(), getGlobalOptions());
        pVisualization->setData(inData);
        pWidget = pVisualization;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_INFO)) {
        XFWidget_FileInfo *pFileInfo = new XFWidget_FileInfo(this);
        pFileInfo->setGlobal(getShortcuts(), getGlobalOptions());
        pFileInfo->setData(inData);
        pWidget = pFileInfo;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_DIESCAN)) {
        XFWidget_DIEScan *pDIEScan = new XFWidget_DIEScan(this);
        pDIEScan->setGlobal(getShortcuts(), getGlobalOptions());
        pDIEScan->setData(inData);
        pWidget = pDIEScan;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_YARASCAN)) {
        XFWidget_YaraScan *pYaraScan = new XFWidget_YaraScan(this);
        pYaraScan->setGlobal(getShortcuts(), getGlobalOptions());
        pYaraScan->setData(inData);
        pWidget = pYaraScan;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_VIRUSTOTALSCAN)) {
        XFWidget_VirusTotal *pVirusTotal = new XFWidget_VirusTotal(this);
        pVirusTotal->setGlobal(getShortcuts(), getGlobalOptions());
        pVirusTotal->setData(inData);
        pWidget = pVirusTotal;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_DEMANGLE)) {
        XFWidget_Demangle *pDemangle = new XFWidget_Demangle(this);
        pDemangle->setGlobal(getShortcuts(), getGlobalOptions());
        pDemangle->setData(inData);
        pWidget = pDemangle;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_TOOLS)) {
        XFWidget_Tools *pTools = new XFWidget_Tools(this);
        pTools->setGlobal(getShortcuts(), getGlobalOptions());
        // Queued: a file mutation reloads the whole view, which deletes this
        // widget - defer so the emitting slot returns first.
        connect(pTools, SIGNAL(dataChanged(qint64, qint64)), this, SLOT(onToolsDataChanged()), Qt::QueuedConnection);
        pTools->setData(inData);
        pWidget = pTools;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_HEX)) {
        XFWidget_Hex *pHex = new XFWidget_Hex(this);
        pHex->setGlobal(getShortcuts(), getGlobalOptions());
        pHex->setData(inData);
        pWidget = pHex;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_DISASM)) {
        XFWidget_Disasm *pDisasm = new XFWidget_Disasm(this);
        pDisasm->setGlobal(getShortcuts(), getGlobalOptions());
        pDisasm->setData(inData);
        pWidget = pDisasm;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_NFDSCAN)) {
        XFWidget_NFDScan *pNFDScan = new XFWidget_NFDScan(this);
        pNFDScan->setGlobal(getShortcuts(), getGlobalOptions());
        pNFDScan->setData(inData);
        pWidget = pNFDScan;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_HASH)) {
        XFWidget_Hash *pHash = new XFWidget_Hash(this);
        pHash->setGlobal(getShortcuts(), getGlobalOptions());
        pHash->setData(inData);
        pWidget = pHash;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_SIGNATURES)) {
        XFWidget_Signatures *pSignatures = new XFWidget_Signatures(this);
        pSignatures->setGlobal(getShortcuts(), getGlobalOptions());
        pSignatures->setData(inData);
        pWidget = pSignatures;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_MEMORYMAP)) {
        XFWidget_MemoryMap *pMemoryMap = new XFWidget_MemoryMap(this);
        pMemoryMap->setGlobal(getShortcuts(), getGlobalOptions());
        pMemoryMap->setData(inData);
        pWidget = pMemoryMap;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_ENTROPY)) {
        XFWidget_Entropy *pEntropy = new XFWidget_Entropy(this);
        pEntropy->setGlobal(getShortcuts(), getGlobalOptions());
        pEntropy->setData(inData);
        pWidget = pEntropy;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_EXTRACTOR)) {
        XFWidget_Extractor *pExtractor = new XFWidget_Extractor(this);
        pExtractor->setGlobal(getShortcuts(), getGlobalOptions());
        pExtractor->setData(inData);
        pWidget = pExtractor;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_SEARCH)) {
        XFWidget_Search *pSearch = new XFWidget_Search(this);
        pSearch->setGlobal(getShortcuts(), getGlobalOptions());
        pSearch->setData(inData);
        pWidget = pSearch;
    } else if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_STRINGS)) {
        XFWidget_Strings *pStrings = new XFWidget_Strings(this);
        pStrings->setGlobal(getShortcuts(), getGlobalOptions());
        pStrings->setData(inData);
        pWidget = pStrings;
    } else if (bIsListCommand) {
        XFWidget_Table *pTable = new XFWidget_Table(this);
        connect(pTable, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)));
        connect(pTable, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)));
        pTable->setGlobal(getShortcuts(), getGlobalOptions());

        QIODevice *pListDevice = XFormats::createDevice(inData);
        XBinary *pListBinary = XFormats::createClass(inData.fileType, pListDevice, inData.bIsImage, inData.nModuleAddress);

        if (pListBinary) {
            if (xfHeader.structID == XBinary::STRUCTID_IMPORT) {
                pTable->setData(inData, pListBinary->getImportStructs());
            } else if (xfHeader.structID == XBinary::STRUCTID_EXPORT) {
                pTable->setData(inData, pListBinary->getExportStructs());
            } else if (xfHeader.structID == XBinary::STRUCTID_SYMBOLS) {
                pTable->setData(inData, pListBinary->getSymbolStructs());
            } else if (xfHeader.structID == XBinary::STRUCTID_RESOURCES) {
                pTable->setData(inData, pListBinary->getResourceStructs());
            }

            delete pListBinary;
        }

        XFormats::removeDevice(pListDevice, inData);

        pWidget = pTable;
    } else if (xfHeader.xfType == XBinary::XFTYPE_TABLE) {
        XFWidget_Table *pTable = new XFWidget_Table(this);
        connect(pTable, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)));
        connect(pTable, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)));
        pTable->setGlobal(getShortcuts(), getGlobalOptions());
        pTable->setData(inData, xfHeader);
        pWidget = pTable;
    } else {
        XFWidget_Header *pHeader = new XFWidget_Header(this);
        connect(pHeader, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)));
        connect(pHeader, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)));
        pHeader->setGlobal(getShortcuts(), getGlobalOptions());
        pHeader->setData(inData, xfHeader);
        pWidget = pHeader;
    }

    m_mapWidgets.insert(sCacheKey, pWidget);
    m_lruOrder.append(sCacheKey);
    ui->stackedWidget->addWidget(pWidget);
    ui->stackedWidget->setCurrentWidget(pWidget);
    return pWidget;
}

void XFWidgetAdvanced::onToolsDataChanged()
{
    // The file was modified by the Tools panel - rebuild everything from disk.
    reload();
}

void XFWidgetAdvanced::on_toolButtonReload_clicked()
{
    reload();
}

void XFWidgetAdvanced::on_comboBoxFileType_currentIndexChanged(int nIndex)
{
    Q_UNUSED(nIndex)

    reloadFileType();
}
