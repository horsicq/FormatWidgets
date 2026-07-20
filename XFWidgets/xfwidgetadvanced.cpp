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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QRegularExpression>
#else
#include <QRegExp>
#endif

#include "xfmodel.h"
#include "xfwidget_disasm.h"
#include "xfwidget_entropy.h"
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
#include "xfwidget_visualization.h"

XFWidgetAdvanced::XFWidgetAdvanced(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::XFWidgetAdvanced)
{
    ui->setupUi(this);

    m_bIsReadonly = false;

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 2);

    connect(ui->treeView, SIGNAL(headerSelected(XBinary::XFHEADER)), this, SLOT(onHeaderSelected(XBinary::XFHEADER)));
}

XFWidgetAdvanced::~XFWidgetAdvanced()
{
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
    ui->treeView->clear();
}

void XFWidgetAdvanced::reload()
{
    QIODevice *pDevice = XFormats::createDevice(m_inData);
    XFormats::setFileTypeComboBox(m_inData.fileType, pDevice, ui->comboBoxFileType);
    XFormats::removeDevice(pDevice, m_inData);

    reloadFileType();
}

void XFWidgetAdvanced::reloadFileType()
{
    XBinary::FT fileType = (XBinary::FT)(ui->comboBoxFileType->currentData().toUInt());

    QIODevice *pDevice = XFormats::createDevice(m_inData);
    XBinary *pBinary = XFormats::createClass(fileType, pDevice, m_inData.bIsImage, m_inData.nModuleAddress);

    if (pBinary) {
        QList<XBinary::XFHEADER> listHeaders = pBinary->_getXFHeaders(); // TODO Thread

        XBinary::INDATA inData = m_inData;
        inData.fileType = fileType;
        ui->treeView->setData(inData, listHeaders, true);

        delete pBinary;
    }

    XFormats::removeDevice(pDevice, m_inData);
}

void XFWidgetAdvanced::setReadonly(bool bIsReadonly)
{
    m_bIsReadonly = bIsReadonly;
}

void XFWidgetAdvanced::adjustView()
{
    getGlobalOptions()->adjustTreeView(ui->treeView, XOptions::ID_VIEW_FONT_TREEVIEWS);
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

    QWidget *pWidget = getOrCreateWidget(sCurrentTag, inData, xfHeader);

    bool bIsListCommand = (xfHeader.xfType == XBinary::XFTYPE_COMMAND) &&
                          ((xfHeader.structID == XBinary::STRUCTID_IMPORT) || (xfHeader.structID == XBinary::STRUCTID_EXPORT) ||
                           (xfHeader.structID == XBinary::STRUCTID_SYMBOLS) || (xfHeader.structID == XBinary::STRUCTID_RESOURCES));

    if ((xfHeader.xfType == XBinary::XFTYPE_COMMAND) && (xfHeader.structID == XBinary::STRUCTID_VISUALIZATION)) {
        qobject_cast<XFWidget_Visualization *>(pWidget)->setReadonly(m_bIsReadonly);
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

QWidget *XFWidgetAdvanced::getOrCreateWidget(const QString &sName, const XBinary::INDATA &inData, const XBinary::XFHEADER &xfHeader)
{
    if (m_mapWidgets.contains(sName)) {
        m_lruOrder.removeOne(sName);
        m_lruOrder.append(sName);
        QWidget *pWidget = m_mapWidgets.value(sName);
        ui->stackedWidget->setCurrentWidget(pWidget);
        return pWidget;
    }

    if (m_mapWidgets.size() >= 20) {
        const QString sEvict = m_lruOrder.takeFirst();
        QWidget *pEvicted = m_mapWidgets.take(sEvict);
        ui->stackedWidget->removeWidget(pEvicted);
        delete pEvicted;
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
        pStrings->setData(inData);
        pWidget = pStrings;
    } else if (bIsListCommand) {
        XFWidget_Table *pTable = new XFWidget_Table(this);
        connect(pTable, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)));
        connect(pTable, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)));

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
        pTable->setData(inData, xfHeader);
        pWidget = pTable;
    } else {
        XFWidget_Header *pHeader = new XFWidget_Header(this);
        connect(pHeader, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldSelected(qint32, QVariant, XBinary::XFRECORD)));
        connect(pHeader, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)), this, SIGNAL(fieldDoubleClicked(qint32, QVariant, XBinary::XFRECORD)));
        pHeader->setData(inData, xfHeader);
        pWidget = pHeader;
    }

    m_mapWidgets.insert(sName, pWidget);
    m_lruOrder.append(sName);
    ui->stackedWidget->addWidget(pWidget);
    ui->stackedWidget->setCurrentWidget(pWidget);
    return pWidget;
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
